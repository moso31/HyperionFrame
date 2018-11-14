#include "HBVH.h"

bool HBVHTree::BuildTreesWithScene(HScene * scene)
{
	node = new HBVHTreeNodeAABB();

	int count = 0;
	for (vector<Shape*>::iterator it = scene->shapes.begin(); it != scene->shapes.end(); it++)
	{
		HBVHInfoAABB info;
		info.data = (*it)->GetAABB();
		info.index = count++;
		m_boundInfo.push_back(info);
		node->aabb.Merge(info.data);

		//auto currShapeNode = new HBVHTreeNodeTriangle();
		//for (int i = 0; i < (*it)->GetFaceCount(); i++)
		//{
		//	HBVHInfoTriangle triInfo;
		//	Triangle tri = (*it)->GetFace(i);

		//	triInfo.index = i;
		//	for (int j = 0; j < 3; j++)
		//	{
		//		triInfo.aabb.Merge(tri.p[j]);
		//		triInfo.data.p[j] = tri.p[j];
		//	}

		//	BuildRecursive(currShapeNode);
		//}

		//shapesNode.push_back(currShapeNode);
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
		}
		else
		{
			// interior
			BuildRecursive(node, itBegin, split);
			BuildRecursive(node, split, itEnd);
		}
	}
}

//void BuildRecursive(HBVHTreeNodeTriangle* node, vector<HBVHInfoTriangle>::iterator itBegin, vector<HBVHInfoTriangle>::iterator itEnd) 
//{
//	int dim = node->aabb.GetMaximumExtent();
//	float midPos = XMVectorGetByIndex(XMLoadFloat3(&node->aabb.GetCenter()), dim);
//	auto split = partition(m_boundInfo.begin(), m_boundInfo.end(), [dim, midPos](HBVHInfoAABB& info)
//	{
//		auto boundPos = XMVectorGetByIndex(XMLoadFloat3(&info.data.GetCenter()), dim);
//		return boundPos < midPos;
//	});
//
//	if (split == itBegin || split == itEnd)
//	{
//		// bad result, so leaf
//		node->child[0] = node->child[1] = nullptr;
//	}
//	else
//	{
//		// interior
//		BuildRecursive(node, itBegin, split);
//		BuildRecursive(node, split, itEnd);
//	}
//}