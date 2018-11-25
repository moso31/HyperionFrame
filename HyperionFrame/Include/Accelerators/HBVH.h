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

	// HBVHTree�ṹû�п����жϷ���IntersectP����ΪЧ�ʼ�����Intersect��࣬û�б�Ҫ��дһ����
	// ���������жϵ�ʱ�������¼out_hitShapeIndex���������ò������ÿ�ֵ��
	bool Intersect(Ray& worldRay, const HScene& scene, int * out_hitShapeIndex = nullptr);

private:
	// �ݹ����жϷ�����
	// out_dist���������洢��ǰ�ݹ�״̬�µ����Ž⣬�����Զ����Ϊ�ա�
	// ��out_hitShapeIndex���������жϵ�ʱ����Ҫ��¼����˿���Ϊ�ա�
	void RecursiveIntersect(Ray& worldRay, const HScene& scene, HBVHTreeNodeAABB* node, float* out_dist, int* out_hitShapeIndex = nullptr);

private:
	vector<HBVHInfoAABB> m_boundInfo;
	HBVHTreeNodeAABB* node;
};