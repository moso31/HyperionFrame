#pragma once
#include "HMath.h"

enum HBVHSplitMode
{
	SplitPosition,
	SplitCount,
	SAH,
	HLBVH
};

struct HBVHPrimitiveInfo
{
	AABB aabb;
	int index;
};

struct HBVHBucketInfo
{
	AABB aabb;
	int nPrimitive = 0;
};

struct HBVHTreeNode
{
	HBVHTreeNode* child[2];
	AABB aabb;
	int index;
	int offset;
};

struct HBVHMortonPrimitiveInfo
{
	int mortonCode;
	int index;
	AABB aabb;
};

struct HBVHTreeletInfo
{
	int startIndex, int endIndex;
};

class HBVHTree
{
public:
	HBVHTree(HScene* scene);
	~HBVHTree() {}

	// 根据场景信息，生成构建BVH树所需要的信息。
	void BuildTreesWithScene(HBVHSplitMode mode);

	// BVH碰撞检测。
	// 给定世界坐标射线，输出SurfaceInteraction和对象的索引号hitIndex，
	void Intersect(const Ray& worldRay, SurfaceInteraction* si, int* out_hitIndex);

private:
	void BuildTree(HBVHTreeNode* node, int stIndex, int edIndex, HBVHSplitMode mode);
	void RecursiveIntersect(HBVHTreeNode* node, const Ray& worldRay, SurfaceInteraction* si, float* out_tResult, int* out_hitIndex);

private:
	const int SPLIT_COST = 4;

	HBVHTreeNode* root;
	HScene* m_scene;
	vector<HBVHPrimitiveInfo> m_primitiveInfo;
	vector<HBVHMortonPrimitiveInfo> m_mortonPrimitiveInfo;
};
