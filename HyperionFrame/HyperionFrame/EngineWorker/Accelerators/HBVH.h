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
	int startIndex;
	int nPrimitive;
	HBVHTreeNode* node;
};

class HBVHTree
{
public:
	HBVHTree(HScene* scene);
	~HBVHTree() {}

	// ���ݳ�����Ϣ�����ɹ���BVH������Ҫ����Ϣ��
	void BuildTreesWithScene(HBVHSplitMode mode);

	// BVH��ײ��⡣
	// ���������������ߣ����SurfaceInteraction�Ͷ����������hitIndex��
	void Intersect(const Ray& worldRay, SurfaceInteraction* si, int* out_hitIndex);

private:
	void BuildTree(HBVHTreeNode* node, int stIndex, int edIndex, HBVHSplitMode mode);
	void RecursiveIntersect(HBVHTreeNode* node, const Ray& worldRay, SurfaceInteraction* si, float* out_tResult, int* out_hitIndex);

	// ����HLBVH����Ҫ�õ���treelet��һ�ι���һ����
	HBVHTreeNode* BuildTreelet(int stIndex, int edIndex, int bitIndex);

	// �����ϲ�������
	void BuildUpperTree(HBVHTreeNode* node, int stIndex, int edIndex);

private:
	const int SPLIT_COST = 4;

	HBVHTreeNode* root;
	HScene* m_scene;
	vector<HBVHPrimitiveInfo> m_primitiveInfo;
	vector<HBVHMortonPrimitiveInfo> m_mortonPrimitiveInfo;
	vector<HBVHTreeletInfo> m_treeletInfo;

	HBVHSplitMode m_mode_temp;
};