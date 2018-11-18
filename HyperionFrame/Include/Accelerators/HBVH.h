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

	// HBVHTree�ṹû�п����жϷ���IntersectP����ΪЧ�ʼ�����Intersect��࣬û�б�Ҫ��дһ����
	// ���������жϵ�ʱ�������¼out_hitShapeIndex���������ò������ÿ�ֵ��
	bool Intersect(Ray& worldRay, int * out_hitShapeIndex = nullptr);

private:
	// �ݹ����жϷ�����
	// out_dist���������洢��ǰ�ݹ�״̬�µ����Ž⣬�����Զ����Ϊ�ա�
	// ��out_hitShapeIndex���������жϵ�ʱ����Ҫ��¼����˿���Ϊ�ա�
	void RecursiveIntersect(Ray& worldRay, HBVHTreeNodeAABB* node, float* out_dist, int* out_hitShapeIndex = nullptr);

private:
	vector<HBVHInfoAABB> m_boundInfo;
	HBVHTreeNodeAABB* node;
};