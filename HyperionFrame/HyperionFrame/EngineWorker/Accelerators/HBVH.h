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

	// ���ݳ�����Ϣ�����ɹ���BVH������Ҫ����Ϣ��
	void BuildTreesWithScene(HBVHSplitMode mode);

	// BVH��ײ��⡣
	// ���������������ߣ����SurfaceInteraction�Ͷ����������hitIndex��
	void Intersect(const Ray& worldRay, SurfaceInteraction* si, HInt* out_hitIndex, HFloat tMax = FLT_MAX);

private:
	void BuildTree(HBVHTreeNode* node, HInt stIndex, HInt edIndex, HBVHSplitMode mode);
	void RecursiveIntersect(HBVHTreeNode* node, const Ray& worldRay, SurfaceInteraction* si, HFloat* out_tResult, HInt* out_hitIndex);

	// ����HLBVH����Ҫ�õ���treelet��һ�ι���һ����
	HBVHTreeNode* BuildTreelet(HInt stIndex, HInt edIndex, HInt bitIndex);

	// �����ϲ�������
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
