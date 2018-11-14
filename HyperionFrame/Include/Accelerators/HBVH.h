#pragma once
#include "HScene.h"

struct HBVHInfoAABB
{
	int index;
	AABB data;
};

//struct HBVHInfoTriangle
//{
//	int index;
//	AABB aabb;
//	Triangle data;
//};

struct HBVHTreeNodeAABB
{
	HBVHTreeNodeAABB* child[2];
	AABB aabb;
};

//struct HBVHTreeNodeTriangle
//{
//	HBVHTreeNodeTriangle* child[2];
//	AABB aabb;
//	Triangle triangle;
//};

class HBVHTree
{
public:
	bool BuildTreesWithScene(HScene* scene);
	void BuildRecursive(HBVHTreeNodeAABB* node, vector<HBVHInfoAABB>::iterator itBegin, vector<HBVHInfoAABB>::iterator itEnd);
	//void BuildRecursive(HBVHTreeNodeTriangle* node, vector<HBVHInfoTriangle>::iterator itBegin, vector<HBVHInfoTriangle>::iterator itEnd);

private:
	vector<HBVHInfoAABB> m_boundInfo;
	HBVHTreeNodeAABB* node;
	//vector<vector<HBVHInfoTriangle>> m_shapesBoundInfo;
	//vector<HBVHTreeNodeTriangle*> shapesNode;
};