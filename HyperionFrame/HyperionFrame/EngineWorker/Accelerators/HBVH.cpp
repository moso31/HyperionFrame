#include "HBVH.h"
#include "HScene.h"

inline int LeftShift3(int x) {
	if (x == (1 << 10)) --x;
	x = (x | (x << 16)) & 0x30000ff;
	x = (x | (x << 8)) & 0x300f00f;
	x = (x | (x << 4)) & 0x30c30c3;
	x = (x | (x << 2)) & 0x9249249;
	return x;
}

inline int EncodeMorton3(const HInt3 &v) {
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

	auto time_st = GetTickCount();

	m_mode_temp = mode;

	root = new HBVHTreeNode();
	int count = 0;	// 场景中的primitive总数
	int skipCount = 0;	// 跳过的primitive总数（诸如line之类不可能相交计算的primitive都会被跳过）。
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
				int mortonScale = 1 << 10;
				HInt3 iRelativePositionScaled = { (int)(fRelativePosition.x * mortonScale), (int)(fRelativePosition.y * mortonScale), (int)(fRelativePosition.z * mortonScale) };
				primitiveInfo.mortonCode = EncodeMorton3(iRelativePositionScaled);
				m_mortonPrimitiveInfo.push_back(primitiveInfo);
			}
		}

		sort(m_mortonPrimitiveInfo.begin(), m_mortonPrimitiveInfo.end(), [](const HBVHMortonPrimitiveInfo& a, const HBVHMortonPrimitiveInfo& b) {
			return a.mortonCode < b.mortonCode;
		});

		int mask = 0x3FFC0000;
		int start = 0;
		int lastMaskPos = m_mortonPrimitiveInfo[0].mortonCode & mask;
		int nPrimitiveLimit = 1000;
		for (int i = 1; i < count; i++)
		{
			int currentMaskPos = m_mortonPrimitiveInfo[i].mortonCode & mask;
			int nPrimitive = i - start;
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
		for (int i = 0; i < m_treeletInfo.size(); i++)
		{
			m_treeletInfo[i].node = BuildTreelet(m_treeletInfo[i].startIndex, m_treeletInfo[i].startIndex + m_treeletInfo[i].nPrimitive, 29 - 12);
		}

		// 所有treelet构建完毕后构建上层总树。
		BuildUpperTree(root, 0, (int)m_treeletInfo.size());
	}

	auto time_ed = GetTickCount();
	printf("BVH done. 用时：%.3f 秒\n", (float)(time_ed - time_st) / 1000.0f);
}

void HBVHTree::Intersect(const Ray & worldRay, SurfaceInteraction* si, int* out_hitIndex, float tMax)
{
	float tResult = tMax;
	RecursiveIntersect(root, worldRay, si, &tResult, out_hitIndex);
}

void HBVHTree::BuildTree(HBVHTreeNode * node, int stIndex, int edIndex, HBVHSplitMode mode)
{
	//	递归建树
	//	如果node下只有一个节点就构建子树。
	//	如果当前node下，遍历所有节点的开销少于进一步分割的开销，也构建子树。
	//	如果分割以后分不开也构建子树。
	//	其他情况下构建中间树。

	for (int i = stIndex; i < edIndex; i++)
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
	for (int i = stIndex; i < edIndex; i++)
	{
		centroidAABB.Merge(m_primitiveInfo[i].aabb.GetCenter());
	}
	int dim = centroidAABB.GetMaximumExtent();
	float startPos = XMVectorGetByIndex(XMLoadFloat3(&centroidAABB.GetVecMax()), dim);
	float endPos = XMVectorGetByIndex(XMLoadFloat3(&centroidAABB.GetVecMin()), dim);
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
		float midPos = XMVectorGetByIndex(XMLoadFloat3(&centroidAABB.GetCenter()), dim);
		itSplit = partition(m_primitiveInfo.begin() + stIndex, m_primitiveInfo.begin() + edIndex, [dim, midPos](HBVHPrimitiveInfo& info)
		{
			auto boundPos = XMVectorGetByIndex(XMLoadFloat3(&info.aabb.GetCenter()), dim);
			return boundPos < midPos;
		});
		break;
	}
	case SplitCount:
	{
		int midIndex = (stIndex + edIndex) / 2;
		itSplit = m_primitiveInfo.begin() + midIndex;
		nth_element(m_primitiveInfo.begin() + stIndex, itSplit, m_primitiveInfo.begin() + edIndex, [dim](HBVHPrimitiveInfo& a, HBVHPrimitiveInfo& b) {
			auto aPos = XMVectorGetByIndex(XMLoadFloat3(&a.aabb.GetCenter()), dim);
			auto bPos = XMVectorGetByIndex(XMLoadFloat3(&b.aabb.GetCenter()), dim);
			return aPos < bPos;
		});
		break;
	}
	case SAH:
	{
		// SAH 建树。
		// 将当前节点沿dim方向分成12份。然后从中间的11种分割方式里取一个最优秀的。
		// 可以通过将子节点表面积比例的方法计入权重的方式，来判断哪个最优秀。

		const int nBucket = 12;
		HBVHBucketInfo bucket[nBucket];
		for (auto it = m_primitiveInfo.begin() + stIndex; it != m_primitiveInfo.begin() + edIndex; it++)
		{
			int bucketPos = (int)(nBucket * XMVectorGetByIndex(XMLoadFloat3(&centroidAABB.Offset(it->aabb.GetCenter())), dim));
			bucketPos = Clamp(bucketPos, 0, nBucket - 1);
			bucket[bucketPos].aabb.Merge(it->aabb);
			bucket[bucketPos].nPrimitive++;
		}

		float s = node->aabb.GetSurfaceArea();
		float cost[nBucket - 1];
		for (int i = 0; i < nBucket - 1; i++)
		{
			AABB abLeft, abRight;
			int nA = 0;
			int nB = 0;
			for (int j = 0; j < i + 1; j++)
			{
				abLeft.Merge(bucket[j].aabb);
				nA += bucket[j].nPrimitive;
			}

			for (int j = i + 1; j < nBucket; j++)
			{
				abRight.Merge(bucket[j].aabb);
				nB += bucket[j].nPrimitive;
			}

			float sA = abLeft.GetSurfaceArea();
			float sB = abRight.GetSurfaceArea();
			float costValue = 1.0f + (sA * nA + sB * nB) / s;
			cost[i] = isnan(costValue) ? FLT_MAX : costValue;
		}

		// 从11个分割方案中取最优秀的。
		float minCost = cost[0];
		int minCostBucket = 0;
		for (int i = 1; i < nBucket - 1; i++)
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
				int bucketPos = (int)(nBucket * XMVectorGetByIndex(XMLoadFloat3(&centroidAABB.Offset(info.aabb.GetCenter())), dim));
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

	int splitPos = (int)(itSplit - m_primitiveInfo.begin());
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

void HBVHTree::RecursiveIntersect(HBVHTreeNode * node, const Ray & worldRay, SurfaceInteraction* si, float* out_tResult, int* out_hitIndex)
{
	float t0, t1;
	if (node->aabb.IntersectP(worldRay, &t0, &t1))
	{
		float tNodeHit = t0;
		if (tNodeHit < 1e-5f) tNodeHit = t1;
		if (tNodeHit < 1e-5f)
			return;

		if (node->child[0] == nullptr && node->child[1] == nullptr)
		{
			if (m_mode_temp == HLBVH)
			{
				// leaf
				for (int i = node->index; i < node->index + node->offset; i++)
				{
					for (int j = 0; j < m_treeletInfo[i].nPrimitive; j++)
					{
						int idx = m_treeletInfo[i].startIndex + j;

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
				for (int i = node->index; i < node->index + node->offset; i++)
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

HBVHTreeNode* HBVHTree::BuildTreelet(int stIndex, int edIndex, int bitIndex)
{
	// 如果已经没有递归位分割，直接创建一个包括stIndex到edIndex所有primitive的子节点，并停止分割
	// 一般分到这么细（十亿分之一）基本上不会剩下什么重复的东西了，对性能的影响不大。
	if (bitIndex == -1)
	{
		HBVHTreeNode* result = new HBVHTreeNode();
		for (int i = stIndex; i < edIndex; i++)
		{
			result->aabb.Merge(m_scene->primitives[m_mortonPrimitiveInfo[i].index]->GetAABBWorld());
		}
		result->index = stIndex;
		result->offset = edIndex - stIndex;
		result->child[0] = result->child[1] = nullptr;

		return result;
	}

	// 否则，计算中间位
	int startMorton = m_mortonPrimitiveInfo[stIndex].mortonCode & bitIndex;
	int splitIndex = stIndex;
	for (int i = stIndex; i < edIndex; i++)
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
	for (int i = stIndex; i < edIndex; i++)
	{
		result->aabb.Merge(m_scene->primitives[m_mortonPrimitiveInfo[i].index]->GetAABBWorld());
	}
	result->index = stIndex;
	result->offset = edIndex - stIndex;
	result->child[0] = BuildTreelet(stIndex, splitIndex, bitIndex - 1);
	result->child[1] = BuildTreelet(splitIndex, edIndex, bitIndex - 1);

	return result;
}

void HBVHTree::BuildUpperTree(HBVHTreeNode* node, int stIndex, int edIndex)
{
	for (int i = stIndex; i < edIndex; i++)
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
	for (int i = stIndex; i < edIndex; i++)
	{
		centroidAABB.Merge(m_treeletInfo[i].node->aabb.GetCenter());
	}

	vector<HBVHTreeletInfo>::iterator itSplit;
	int dim = centroidAABB.GetMaximumExtent();
	const int nBucket = 12;
	HBVHBucketInfo bucket[nBucket];
	for (auto it = m_treeletInfo.begin() + stIndex; it != m_treeletInfo.begin() + edIndex; it++)
	{
		int bucketPos = (int)(nBucket * XMVectorGetByIndex(XMLoadFloat3(&centroidAABB.Offset(it->node->aabb.GetCenter())), dim));
		bucketPos = Clamp(bucketPos, 0, nBucket - 1);
		bucket[bucketPos].aabb.Merge(it->node->aabb);
		bucket[bucketPos].nPrimitive += it->nPrimitive;
	}

	float s = node->aabb.GetSurfaceArea();
	float cost[nBucket - 1];
	for (int i = 0; i < nBucket - 1; i++)
	{
		AABB abLeft, abRight;
		int nA = 0;
		int nB = 0;
		for (int j = 0; j < i + 1; j++)
		{
			abLeft.Merge(bucket[j].aabb);
			nA += bucket[j].nPrimitive;
		}

		for (int j = i + 1; j < nBucket; j++)
		{
			abRight.Merge(bucket[j].aabb);
			nB += bucket[j].nPrimitive;
		}

		float sA = abLeft.GetSurfaceArea();
		float sB = abRight.GetSurfaceArea();
		float costValue = 1.0f + (sA * nA + sB * nB) / s;
		cost[i] = isnan(costValue) ? FLT_MAX : costValue;
	}

	// 从11个分割方案中取最优秀的。
	float minCost = cost[0];
	int minCostBucket = 0;
	for (int i = 1; i < nBucket - 1; i++)
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
		int bucketPos = (int)(nBucket * XMVectorGetByIndex(XMLoadFloat3(&centroidAABB.Offset(info.node->aabb.GetCenter())), dim));
		return bucketPos <= minCostBucket;
	});

	int splitIndex = (int)(itSplit - m_treeletInfo.begin());
	node->child[0] = new HBVHTreeNode();
	node->child[1] = new HBVHTreeNode();
	BuildUpperTree(node->child[0], stIndex, splitIndex);
	BuildUpperTree(node->child[1], splitIndex, edIndex);
}
