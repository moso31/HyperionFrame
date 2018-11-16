#pragma once
#include "HMath.h"

struct HBVHInfoAABB
{
	int index;
	AABB data;
};

struct HBVHTreeNodeAABB
{
	HBVHTreeNodeAABB* child[2];
	int index, offset;
	AABB aabb;
};

class HBVHTree
{
public:
	bool BuildTreesWithScene(HScene* scene);
	void BuildRecursive(HBVHTreeNodeAABB* node, vector<HBVHInfoAABB>::iterator itBegin, vector<HBVHInfoAABB>::iterator itEnd);

	bool Intersect(Ray& worldRay, int * out_hitShapeIndex);

private:
	void RecursiveIntersect(Ray& worldRay, HBVHTreeNodeAABB* node, float* out_dist, int* out_hitShapeIndex);

private:
	vector<HBVHInfoAABB> m_boundInfo;
	HBVHTreeNodeAABB* node;
};