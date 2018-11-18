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

void HBVHTree::BuildRecursive(HBVHTreeNodeAABB* node, vector<HBVHInfoAABB>::iterator itBegin, vector<HBVHInfoAABB>::iterator itEnd)
{
	if (itBegin + 1 == itEnd)
	{
		// leaf
		node->child[0] = node->child[1] = nullptr;
		node->index = (int)(itBegin - m_boundInfo.begin());
		node->offset = (int)(itEnd - itBegin);
		node->aabb = (*itBegin).data;
	}
	else
	{
		int dim = node->aabb.GetMaximumExtent();
		float midPos = XMVectorGetByIndex(XMLoadFloat3(&node->aabb.GetCenter()), dim);
		auto split = partition(m_boundInfo.begin(), m_boundInfo.end(), [dim, midPos](HBVHInfoAABB& info)
		{
			auto boundPos = XMVectorGetByIndex(XMLoadFloat3(&info.data.GetCenter()), dim);
			return boundPos < midPos;
		});

		if (split == itBegin || split == itEnd)
		{
			// bad result, so leaf
			node->child[0] = node->child[1] = nullptr;
			node->index = (int)(itBegin - m_boundInfo.begin());
			node->offset = (int)(itEnd - itBegin);
		}
		else
		{
			// interior
			node->child[0] = new HBVHTreeNodeAABB();
			node->child[1] = new HBVHTreeNodeAABB();
			BuildRecursive(node->child[0], itBegin, split);
			BuildRecursive(node->child[1], split, itEnd);
			node->index = -1;
			node->offset = -1;
		}
	}
}

bool HBVHTree::Intersect(Ray& worldRay, int * out_hitShapeIndex) 
{
	float dist = FLT_MAX;
	RecursiveIntersect(worldRay, node, &dist, out_hitShapeIndex);
	return dist != FLT_MAX;
}

void HBVHTree::RecursiveIntersect(Ray & worldRay, HBVHTreeNodeAABB * node, float * out_dist, int * out_hitShapeIndex)
{
	if (node->index != -1)
	{
		float t0, t1;
		for (int i = node->index; i < node->index + node->offset; i++)
		{
			if (m_boundInfo[i].data.IntersectP(worldRay, &t0, &t1))
			{
				if (*out_dist > t0 && t0 > 0.0f)
				{
					*out_dist = t0;
					if (out_hitShapeIndex)
						*out_hitShapeIndex = m_boundInfo[i].index;
				}
			}
		}
	}
	else
	{
		RecursiveIntersect(worldRay, node->child[0], out_dist, out_hitShapeIndex);
		RecursiveIntersect(worldRay, node->child[1], out_dist, out_hitShapeIndex);
	}
}

