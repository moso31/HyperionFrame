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
	bool isLeaf;
	int index, offset;
	AABB aabb;
};

struct HBVHBucketInfo
{
	AABB aabb;
	int count = 0;
};

enum HBVHBuildMode
{
	SplitPosition,
	SplitCount,
	SAH
};

class HBVHTree
{
public:
	bool BuildTreesWithScene(HScene* scene);
	void BuildRecursive(HBVHTreeNodeAABB* node, const vector<HBVHInfoAABB>::iterator &itBegin, const vector<HBVHInfoAABB>::iterator &itEnd, HBVHBuildMode buildMode = SAH);

	// HBVHTree结构没有快速判断方法IntersectP，因为效率几乎和Intersect差不多，没有必要重写一个。
	// 不过快速判断的时候无需记录out_hitShapeIndex，因此允许该参数设置空值。
	bool Intersect(Ray& worldRay, const HScene& scene, int * out_hitShapeIndex = nullptr);

private:
	// 递归求交判断方法。
	// out_dist参数用来存储当前递归状态下的最优解，因此永远不能为空。
	// 但out_hitShapeIndex在做快速判断的时候不需要记录，因此可以为空。
	void RecursiveIntersect(Ray& worldRay, const HScene& scene, HBVHTreeNodeAABB* node, float* out_dist, int* out_hitShapeIndex = nullptr);

private:
	vector<HBVHInfoAABB> m_boundInfo;
	HBVHTreeNodeAABB* node;
};