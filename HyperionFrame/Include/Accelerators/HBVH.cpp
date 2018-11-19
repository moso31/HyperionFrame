#include "HBVH.h"
#include "HScene.h"

bool HBVHTree::BuildTreesWithScene(HScene * scene)
{
	node = new HBVHTreeNodeAABB();

	int count = 0;
	for (vector<Shape*>::iterator it = scene->shapes.begin(); it != scene->shapes.end(); it++)
	{
		HBVHInfoAABB info;
		info.data = (*it)->GetAABBWorld();
		info.index = count++;
		m_boundInfo.push_back(info);
		node->aabb.Merge(info.data);
	}

	BuildRecursive(node, m_boundInfo.begin(), m_boundInfo.end());
	return false;
}

void HBVHTree::BuildRecursive(HBVHTreeNodeAABB* node, const vector<HBVHInfoAABB>::iterator &itBegin, const vector<HBVHInfoAABB>::iterator &itEnd)
{
	node->aabb = AABB();
	for (auto it = itBegin; it != itEnd; it++)
		node->aabb.Merge(it->data);
	node->index = (int)(itBegin - m_boundInfo.begin());
	node->offset = (int)(itEnd - itBegin);

	if (itBegin + 1 == itEnd)
	{
		// leaf
		node->child[0] = node->child[1] = nullptr;
		node->isLeaf = true;
	}
	else
	{
		int dim = node->aabb.GetMaximumExtent();
		float midPos = XMVectorGetByIndex(XMLoadFloat3(&node->aabb.GetCenter()), dim);
		auto split = partition(itBegin, itEnd, [dim, midPos](HBVHInfoAABB& info)
		{
			auto boundPos = XMVectorGetByIndex(XMLoadFloat3(&info.data.GetCenter()), dim);
			return boundPos < midPos;
		});

		vector<XMFLOAT3> testVec;
		for (auto it = itBegin; it != itEnd; it++)
		{
			testVec.push_back(it->data.GetCenter());
		}

		if (split == itBegin || split == itEnd)
		{
			// bad result, so leaf
			node->child[0] = node->child[1] = nullptr;
			node->isLeaf = true;
		}
		else
		{
			// interior
			node->child[0] = new HBVHTreeNodeAABB();
			node->child[1] = new HBVHTreeNodeAABB();
			BuildRecursive(node->child[0], itBegin, split);
			BuildRecursive(node->child[1], split, itEnd);
			node->isLeaf = false;
		}
	}
}

bool HBVHTree::Intersect(Ray& worldRay, const HScene& scene, int * out_hitShapeIndex) 
{
	float dist = FLT_MAX;
	RecursiveIntersect(worldRay, scene, node, &dist, out_hitShapeIndex);
	return dist != FLT_MAX;
}

// BVH 递归求交检测
// 对BVH树进行前序搜索，发现合适的leaf节点就记录下来，并不断迭代求出最小的记录值。
// 当发现到合适的leaf值时，先对AABB进行一次intersectP的简易判断，通过简易判断后，再进行精确判断。
// 算法只会记录并更新精确判断的值，直到求出最小解。
void HBVHTree::RecursiveIntersect(Ray & worldRay, const HScene& scene, HBVHTreeNodeAABB * node, float * out_dist, int * out_hitShapeIndex)
{
	if (node->isLeaf)
	{
		// is leaf
		float t0, t1;
		for (int i = node->index; i < node->index + node->offset; i++)
		{
			auto str = scene.shapes[m_boundInfo[i].index]->GetName();
			if (m_boundInfo[i].data.IntersectP(worldRay, &t0, &t1))
			{
				if ((*out_dist > t0 && t0 > H_EPSILON) || (*out_dist > t1 && t1 > H_EPSILON))
				{
					if (scene.shapes[m_boundInfo[i].index]->Intersect(worldRay, &SurfaceInteraction()))
					{
						*out_dist = t0 > H_EPSILON ? t0 : t1;
						if (out_hitShapeIndex)
							*out_hitShapeIndex = m_boundInfo[i].index;
					}
				}
			}
		}
	}
	else
	{
		RecursiveIntersect(worldRay, scene, node->child[0], out_dist, out_hitShapeIndex);
		RecursiveIntersect(worldRay, scene, node->child[1], out_dist, out_hitShapeIndex);
	}
}

