#pragma once
#include "Shape.h"

// ������ĵ���������¼��
// �����ǰ�����������������洢���������У��������������������������Ѿ����ڣ���Ͷ�Ӧ�����干��ͬһbuffer��
// �˲��������Ż�������buffer��������
struct CommonFeatureParams
{
	bool IsSame(const CommonFeatureParams &other) const;

	// ��������͡�
	ePrimitiveType type;
	// �������������������Ϊ�ա�
	vector<int> params;	
	// �����buffer���ݡ�
	PrimitiveBuffer bufferData;
};

class HSceneManager
{
public:
	HSceneManager();
	HSceneManager(std::shared_ptr<DXResource> dxResources, ComPtr<ID3D12GraphicsCommandList> pCommandList);
	~HSceneManager();

	shared_ptr<Box>			CreateBox(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
	shared_ptr<Sphere>		CreateSphere(float radius, int segmentHorizontal, int segmentVertical);
	shared_ptr<HMesh>		CreateMesh(string filepath);
	shared_ptr<HSegment>	CreateSegment(XMFLOAT3 point1, XMFLOAT3 point2);

	int GetCommonFeatureTableCount();

private:
	std::shared_ptr<DXResource>			m_dxResources;
	ComPtr<ID3D12GraphicsCommandList>	m_pCommandList;

	// �������洢������ÿ�������������
	// ������ͬ�������ͨ�ò���GPU���ݣ��Ӷ���Լ�Դ濪����
	vector<CommonFeatureParams> m_commonFeatureTable;
};
