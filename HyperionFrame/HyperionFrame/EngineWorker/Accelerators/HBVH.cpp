#include "HBVH.h"
#include "HScene.h"

inline HInt LeftShift3(HInt x) {
	if (x == (1 << 10)) --x;
	x = (x | (x << 16)) & 0x30000ff;
	x = (x | (x << 8)) & 0x300f00f;
	x = (x | (x << 4)) & 0x30c30c3;
	x = (x | (x << 2)) & 0x9249249;
	return x;
}

inline HInt EncodeMorton3(const HInt3 &v) {
	return (LeftShift3(v.z) << 2) | (LeftShift3(v.y) << 1) | LeftShift3(v.x);
}

HBVHTree::HBVHTree(const shared_ptr<HScene>& scene)
{
	m_scene = scene;
}

void HBVHTree::BuildTreesWithScene(HBVHSplitMode mode)
{
	printf("生成BMP位图...\n");

	if (!m_scene)
	{
		printf("ERROR: Can not create BVH trees. scene has pointed to nullptr.\n");
		return;
	}

	auto time_st = GetTickCount64();

	m_mode_temp = mode;

	root = new HBVHTreeNode();
	HInt count = 0;	// 场景中的primitive总数
	HInt skipCount = 0;	// 跳过的primitive总数（诸如line之类不可能相交计算的primitive都会被跳过）。
	if (mode != HLBVH)
	{
		//m_primitiveInfo.reserve(m_scene->primitives.size());
		for (auto it = m_scene->primitives.begin(); it < m_scene->primitives.end(); it++)
		{
			if ((*it)->GetRenderType() == eRenderType::Shape)
			{
				HBVHPrimitiveInfo primitiveInfo;
				primitiveInfo.index = count++;
				primitiveInfo.aabb = (*it)->GetAABBWorld();
				m_primitiveInfo.push_back(primitiveInfo);
			}
			else
			{
				count++;
				skipCount++;
			}
		}

		BuildTree(root, 0, count - skipCount, mode);
	}
	else
	{
		//m_mortonPrimitiveInfo.reserve(m_scene->primitives.size());
		for (auto it = m_scene->primitives.begin(); it < m_scene->primitives.end(); it++)
		{
			if ((*it)->GetRenderType() == eRenderType::Shape)
			{
				HBVHMortonPrimitiveInfo primitiveInfo;
				primitiveInfo.index = count++;
				primitiveInfo.aabb = (*it)->GetAABBWorld();
				HFloat3 fRelativePosition = m_scene->GetAABB().Offset(primitiveInfo.aabb.GetCenter());
				HInt mortonScale = 1 << 10;
				HInt3 iRelativePositionScaled = { (HInt)(fRelativePosition.x * mortonScale), (HInt)(fRelativePosition.y * mortonScale), (HInt)(fRelativePosition.z * mortonScale) };
				primitiveInfo.mortonCode = EncodeMorton3(iRelativePositionScaled);
				m_mortonPrimitiveInfo.push_back(primitiveInfo);
			}
		}

		sort(m_mortonPrimitiveInfo.begin(), m_mortonPrimitiveInfo.end(), [](const HBVHMortonPrimitiveInfo& a, const HBVHMortonPrimitiveInfo& b) {
			return a.mortonCode < b.mortonCode;
		});

		HInt mask = 0x3FFC0000;
		HInt start = 0;
		HInt lastMaskPos = m_mortonPrimitiveInfo[0].mortonCode & mask;
		HInt nPrimitiveLimit = 1000;
		for (HInt i = 1; i < count; i++)
		{
			HInt currentMaskPos = m_mortonPrimitiveInfo[i].mortonCode & mask;
			HInt nPrimitive = i - start;
			if (currentMaskPos != lastMaskPos || nPrimitive > nPrimitiveLimit)
			{
				HBVHTreeletInfo treelet;
				treelet.startIndex = start;
				treelet.nPrimitive = nPrimitive;
				m_treeletInfo.push_back(treelet);

				lastMaskPos = currentMaskPos;
				start = i;
			}
		}
		HBVHTreeletInfo treelet;
		treelet.startIndex = start;
		treelet.nPrimitive = count - start;
		m_treeletInfo.push_back(treelet);

		// 多线程构建所有treelet
		for (HInt i = 0; i < m_treeletInfo.size(); i++)
		{
			m_treeletInfo[i].node = BuildTreelet(m_treeletInfo[i].startIndex, m_treeletInfo[i].startIndex + m_treeletInfo[i].nPrimitive, 29 - 12);
		}

		// 所有treelet构建完毕后构建上层总树。
		BuildUpperTree(root, 0, (HInt)m_treeletInfo.size());
	}

	auto time_ed = GetTickCount64();
	printf("BVH done. 用时：%.3f 秒\n", (HFloat)(time_ed - time_st) / 1000.0f);
}

void HBVHTree::Intersect(const Ray & worldRay, SurfaceInteraction* si, HInt* out_hitIndex, HFloat tMax)
{
	HFloat tResult = tMax;
	RecursiveIntersect(root, worldRay, si, &tResult, out_hitIndex);
}

void HBVHTree::BuildTree(HBVHTreeNode * node, HInt stIndex, HInt edIndex, HBVHSplitMode mode)
{
	//	递归建树
	//	如果node下只有一个节点就构建子树。
	//	如果当前node下，遍历所有节点的开销少于进一步分割的开销，也构建子树。
	//	如果分割以后分不开也构建子树。
	//	其他情况下构建中间树。

	for (HInt i = stIndex; i < edIndex; i++)
	{
		node->aabb.Merge(m_primitiveInfo[i].aabb);
	}
	node->index = stIndex;
	node->offset = edIndex - stIndex;

	if (edIndex - stIndex == 1)
	{
		// isLeaf
		node->child[0] = nullptr;
		node->child[1] = nullptr;
		return;
	}

	if (edIndex - stIndex < SPLIT_COST)
	{
		// isLeaf
		node->child[0] = nullptr;
		node->child[1] = nullptr;
		return;
	}

	vector<HBVHPrimitiveInfo>::iterator itSplit;

	AABB centroidAABB;
	for (HInt i = stIndex; i < edIndex; i++)
	{
		centroidAABB.Merge(m_primitiveInfo[i].aabb.GetCenter());
	}
	HInt dim = centroidAABB.GetMaximumExtent();
	HFloat startPos = centroidAABB.max[dim];
	HFloat endPos = centroidAABB.min[dim];
	if (startPos == endPos)
	{
		// is leaf
		node->child[0] = nullptr;
		node->child[1] = nullptr;
		return;
	}

	switch (mode)
	{
	case SplitPosition:
	{
		HFloat midPos = centroidAABB.GetCenter()[dim];
		itSplit = partition(m_primitiveInfo.begin() + stIndex, m_primitiveInfo.begin() + edIndex, [dim, midPos](HBVHPrimitiveInfo& info)
		{
			HFloat boundPos = info.aabb.GetCenter()[dim];
			return boundPos < midPos;
		});
		break;
	}
	case SplitCount:
	{
		HInt midIndex = (stIndex + edIndex) / 2;
		itSplit = m_primitiveInfo.begin() + midIndex;
		nth_element(m_primitiveInfo.begin() + stIndex, itSplit, m_primitiveInfo.begin() + edIndex, [dim](HBVHPrimitiveInfo& a, HBVHPrimitiveInfo& b) {
			HFloat aPos = a.aabb.GetCenter()[dim];
			HFloat bPos = b.aabb.GetCenter()[dim];
			return aPos < bPos;
		});
		break;
	}
	case SAH:
	{
		// SAH 建树。
		// 将当前节点沿dim方向分成12份。然后从中间的11种分割方式里取一个最优秀的。
		// 可以通过将子节点表面积比例的方法计入权重的方式，来判断哪个最优秀。

		const HInt nBucket = 12;
		HBVHBucketInfo bucket[nBucket];
		for (auto it = m_primitiveInfo.begin() + stIndex; it != m_primitiveInfo.begin() + edIndex; it++)
		{
			HInt bucketPos = (HInt)(nBucket * centroidAABB.Offset(it->aabb.GetCenter())[dim]);
			bucketPos = Clamp(bucketPos, 0, nBucket - 1);
			bucket[bucketPos].aabb.Merge(it->aabb);
			bucket[bucketPos].nPrimitive++;
		}

		HFloat s = node->aabb.GetSurfaceArea();
		HFloat cost[nBucket - 1];
		for (HInt i = 0; i < nBucket - 1; i++)
		{
			AABB abLeft, abRight;
			HInt nA = 0;
			HInt nB = 0;
			for (HInt j = 0; j < i + 1; j++)
			{
				abLeft.Merge(bucket[j].aabb);
				nA += bucket[j].nPrimitive;
			}

			for (HInt j = i + 1; j < nBucket; j++)
			{
				abRight.Merge(bucket[j].aabb);
				nB += bucket[j].nPrimitive;
			}

			HFloat sA = abLeft.GetSurfaceArea();
			HFloat sB = abRight.GetSurfaceArea();
			HFloat costValue = 1.0f + (sA * nA + sB * nB) / s;
			cost[i] = isnan(costValue) ? FLT_MAX : costValue;
		}

		// 从11个分割方案中取最优秀的。
		HFloat minCost = cost[0];
		HInt minCostBucket = 0;
		for (HInt i = 1; i < nBucket - 1; i++)
		{
			if (minCost > cost[i])
			{
				minCost = cost[i];
				minCostBucket = i;
			}
		}

		// 如果最优秀的方案的花费值依然比当前node的图元数量还多，那还不如直接遍历创建图元。
		if (minCost < node->offset * 10)
		{
			itSplit = partition(m_primitiveInfo.begin() + stIndex, m_primitiveInfo.begin() + edIndex, [=](HBVHPrimitiveInfo& info)
			{
				HInt bucketPos = (HInt)(nBucket * centroidAABB.Offset(info.aabb.GetCenter())[dim]);
				return bucketPos <= minCostBucket;
			});
		}
		else
		{
			// isLeaf
			node->child[0] = nullptr;
			node->child[1] = nullptr;
			return;
		}

		break;
	}
	default:
		break;
	}

	HInt splitPos = (HInt)(itSplit - m_primitiveInfo.begin());
	if (splitPos == stIndex || splitPos == edIndex)
	{
		// isLeaf
		node->child[0] = nullptr;
		node->child[1] = nullptr;
		return;
	}

	// isInterior
	node->child[0] = new HBVHTreeNode();
	node->child[1] = new HBVHTreeNode();
	BuildTree(node->child[0], stIndex, splitPos, mode);
	BuildTree(node->child[1], splitPos, edIndex, mode);
}

void HBVHTree::RecursiveIntersect(HBVHTreeNode * node, const Ray & worldRay, SurfaceInteraction* si, HFloat* out_tResult, HInt* out_hitIndex)
{
	HFloat t0, t1;
	if (node->aabb.IntersectP(worldRay, &t0, &t1))
	{
		HFloat tNodeHit = t0;
		if (tNodeHit < 1e-5f) tNodeHit = t1;
		if (tNodeHit < 1e-5f)
			return;

		if (node->child[0] == nullptr && node->child[1] == nullptr)
		{
			if (m_mode_temp == HLBVH)
			{
				// leaf
				for (HInt i = node->index; i < node->index + node->offset; i++)
				{
					for (HInt j = 0; j < m_treeletInfo[i].nPrimitive; j++)
					{
						HInt idx = m_treeletInfo[i].startIndex + j;

						if (m_scene->primitives[m_mortonPrimitiveInfo[idx].index]->GetRenderType() == eRenderType::Shape)
						{
							//auto str = m_scene->primitives[m_mortonPrimitiveInfo[idx].index]->GetName();

							if (m_mortonPrimitiveInfo[idx].aabb.IntersectP(worldRay, &t0, &t1))
							{
								EFloat tHit;
								SurfaceInteraction temp_si;
								shared_ptr<HShape> pCurrentShape = dynamic_pointer_cast<HShape>(m_scene->primitives[m_mortonPrimitiveInfo[idx].index]);
								if (pCurrentShape->Intersect(worldRay, &temp_si, &tHit))
								{
									if (*out_tResult > tHit.v)
									{
										*out_tResult = tHit.v;
										*si = temp_si;
										*out_hitIndex = m_mortonPrimitiveInfo[idx].index;
									}
								}
							}
						}
					}
				}
			}
			else
			{
				// leaf
				for (HInt i = node->index; i < node->index + node->offset; i++)
				{
					auto str = m_scene->primitives[m_primitiveInfo[i].index]->GetName();

					if (m_scene->primitives[m_primitiveInfo[i].index]->GetRenderType() == eRenderType::Shape)
					{
						if (m_primitiveInfo[i].aabb.IntersectP(worldRay, &t0, &t1))
						{
							EFloat tHit;
							SurfaceInteraction temp_si;
							shared_ptr<HShape> pCurrentShape = dynamic_pointer_cast<HShape>(m_scene->primitives[m_primitiveInfo[i].index]);
							auto t = pCurrentShape->GetAABBWorld();
							if (pCurrentShape->Intersect(worldRay, &temp_si, &tHit))
							{
								if (*out_tResult > tHit.v)
								{
									*out_tResult = tHit.v;
									*si = temp_si;
									*out_hitIndex = m_primitiveInfo[i].index;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			// interior
			RecursiveIntersect(node->child[0], worldRay, si, out_tResult, out_hitIndex);
			RecursiveIntersect(node->child[1], worldRay, si, out_tResult, out_hitIndex);
		}
	}
}

HBVHTreeNode* HBVHTree::BuildTreelet(HInt stIndex, HInt edIndex, HInt bitIndex)
{
	// 如果已经没有递归位分割，直接创建一个包括stIndex到edIndex所有primitive的子节点，并停止分割
	// 一般分到这么细（十亿分之一）基本上不会剩下什么重复的东西了，对性能的影响不大。
	if (bitIndex == -1)
	{
		HBVHTreeNode* result = new HBVHTreeNode();
		for (HInt i = stIndex; i < edIndex; i++)
		{
			result->aabb.Merge(m_scene->primitives[m_mortonPrimitiveInfo[i].index]->GetAABBWorld());
		}
		result->index = stIndex;
		result->offset = edIndex - stIndex;
		result->child[0] = result->child[1] = nullptr;

		return result;
	}

	// 否则，计算中间位
	HInt startMorton = m_mortonPrimitiveInfo[stIndex].mortonCode & bitIndex;
	HInt splitIndex = stIndex;
	for (HInt i = stIndex; i < edIndex; i++)
	{
		if ((m_mortonPrimitiveInfo[i].mortonCode & bitIndex) != startMorton)
		{
			splitIndex = i;
		}
	}

	// 如果中间位没能分割出东西来，就交由下一级分割
	if (splitIndex == stIndex || splitIndex == edIndex)
	{
		return BuildTreelet(stIndex, edIndex, bitIndex - 1);
	}

	// 能分割出东西，就创建出当前节点，并递归创建两个子树
	HBVHTreeNode* result = new HBVHTreeNode();
	for (HInt i = stIndex; i < edIndex; i++)
	{
		result->aabb.Merge(m_scene->primitives[m_mortonPrimitiveInfo[i].index]->GetAABBWorld());
	}
	result->index = stIndex;
	result->offset = edIndex - stIndex;
	result->child[0] = BuildTreelet(stIndex, splitIndex, bitIndex - 1);
	result->child[1] = BuildTreelet(splitIndex, edIndex, bitIndex - 1);

	return result;
}

void HBVHTree::BuildUpperTree(HBVHTreeNode* node, HInt stIndex, HInt edIndex)
{
	for (HInt i = stIndex; i < edIndex; i++)
	{
		node->aabb.Merge(m_treeletInfo[i].node->aabb);
	}

	node->index = stIndex;
	node->offset = edIndex - stIndex;
	if (edIndex - stIndex == 1)
	{
		node = m_treeletInfo[stIndex].node;
		return;
	}

	AABB centroidAABB;
	for (HInt i = stIndex; i < edIndex; i++)
	{
		centroidAABB.Merge(m_treeletInfo[i].node->aabb.GetCenter());
	}

	vector<HBVHTreeletInfo>::iterator itSplit;
	HInt dim = centroidAABB.GetMaximumExtent();
	const HInt nBucket = 12;
	HBVHBucketInfo bucket[nBucket];
	for (auto it = m_treeletInfo.begin() + stIndex; it != m_treeletInfo.begin() + edIndex; it++)
	{
		HInt bucketPos = (HInt)(nBucket * centroidAABB.Offset(it->node->aabb.GetCenter())[dim]);
		bucketPos = Clamp(bucketPos, 0, nBucket - 1);
		bucket[bucketPos].aabb.Merge(it->node->aabb);
		bucket[bucketPos].nPrimitive += it->nPrimitive;
	}

	HFloat s = node->aabb.GetSurfaceArea();
	HFloat cost[nBucket - 1];
	for (HInt i = 0; i < nBucket - 1; i++)
	{
		AABB abLeft, abRight;
		HInt nA = 0;
		HInt nB = 0;
		for (HInt j = 0; j < i + 1; j++)
		{
			abLeft.Merge(bucket[j].aabb);
			nA += bucket[j].nPrimitive;
		}

		for (HInt j = i + 1; j < nBucket; j++)
		{
			abRight.Merge(bucket[j].aabb);
			nB += bucket[j].nPrimitive;
		}

		HFloat sA = abLeft.GetSurfaceArea();
		HFloat sB = abRight.GetSurfaceArea();
		HFloat costValue = 1.0f + (sA * nA + sB * nB) / s;
		cost[i] = isnan(costValue) ? FLT_MAX : costValue;
	}

	// 从11个分割方案中取最优秀的。
	HFloat minCost = cost[0];
	HInt minCostBucket = 0;
	for (HInt i = 1; i < nBucket - 1; i++)
	{
		if (minCost > cost[i])
		{
			minCost = cost[i];
			minCostBucket = i;
		}
	}

	// 和普通SAH不同，HLBVH的上层建树最小单位是一个高位大区。
	// 因此不存在子节点的情况（treelet已经建好了子节点），直接构建中间节点即可。
	itSplit = partition(m_treeletInfo.begin() + stIndex, m_treeletInfo.begin() + edIndex, [=](HBVHTreeletInfo& info)
	{
		HInt bucketPos = (HInt)(nBucket * centroidAABB.Offset(info.node->aabb.GetCenter())[dim]);
		return bucketPos <= minCostBucket;
	});

	HInt splitIndex = (HInt)(itSplit - m_treeletInfo.begin());
	node->child[0] = new HBVHTreeNode();
	node->child[1] = new HBVHTreeNode();
	BuildUpperTree(node->child[0], stIndex, splitIndex);
	BuildUpperTree(node->child[1], splitIndex, edIndex);
}
