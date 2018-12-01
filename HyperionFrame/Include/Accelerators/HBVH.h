#pragma once
#include "HMath.h"

enum HBVHSplitMode
{
	SplitPosition,
	SplitCount,
	SAH
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

class HBVHTree
{
public:
	HBVHTree(HScene* scene);
	~HBVHTree() {}

	// ���ݳ�����Ϣ�����ɹ���BVH������Ҫ����Ϣ��
	void BuildTreesWithScene();

	// BVH��ײ��⡣
	// ���������������ߣ����SurfaceInteraction�Ͷ����������hitIndex��
	void Intersect(const Ray& worldRay, SurfaceInteraction* si, int* out_hitIndex);

private:
	void RecursiveBuild(HBVHTreeNode* node, int stIndex, int edIndex, HBVHSplitMode mode = SplitPosition);
	void RecursiveIntersect(HBVHTreeNode* node, const Ray& worldRay, SurfaceInteraction* si, float* out_tResult, int* out_hitIndex);

private:
	const int SPLIT_COST = 4;

	HBVHTreeNode* root;
	HScene* m_scene;
	vector<HBVHPrimitiveInfo> m_primitiveInfo;
};
