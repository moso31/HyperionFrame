#pragma once
#include "HEngineMath.h"

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
	HInt index;
};

struct HBVHBucketInfo
{
	AABB aabb;
	HInt nPrimitive = 0;
};

struct HBVHTreeNode
{
	HBVHTreeNode* child[2];
	AABB aabb;
	HInt index;
	HInt offset;
};

struct HBVHMortonPrimitiveInfo
{
	HInt mortonCode;
	HInt index;
	AABB aabb;
};

struct HBVHTreeletInfo
{
	HInt startIndex;
	HInt nPrimitive;
	HBVHTreeNode* node;
};

class HBVHTree
{
public:
	HBVHTree(const shared_ptr<HScene>& scene);
	~HBVHTree() {}

	// 根据场景信息，生成构建BVH树所需要的信息。
	void BuildTreesWithScene(HBVHSplitMode mode);

	// BVH碰撞检测。
	// 给定世界坐标射线，输出SurfaceInteraction和对象的索引号hitIndex，
	void Intersect(const Ray& worldRay, SurfaceInteraction* si, HInt* out_hitIndex, HFloat tMax = FLT_MAX);

private:
	void BuildTree(HBVHTreeNode* node, HInt stIndex, HInt edIndex, HBVHSplitMode mode);
	void RecursiveIntersect(HBVHTreeNode* node, const Ray& worldRay, SurfaceInteraction* si, HFloat* out_tResult, HInt* out_hitIndex);

	// 构建HLBVH所需要用到的treelet。一次构建一个。
	HBVHTreeNode* BuildTreelet(HInt stIndex, HInt edIndex, HInt bitIndex);

	// 构建上层总树。
	void BuildUpperTree(HBVHTreeNode* node, HInt stIndex, HInt edIndex);

private:
	const HInt SPLIT_COST = 4;

	HBVHTreeNode* root;
	shared_ptr<HScene> m_scene;
	vector<HBVHPrimitiveInfo> m_primitiveInfo;
	vector<HBVHMortonPrimitiveInfo> m_mortonPrimitiveInfo;
	vector<HBVHTreeletInfo> m_treeletInfo;

	HBVHSplitMode m_mode_temp;
};
