#pragma once
#include "Shape.h"

// 特征表的单个特征纪录。
// 如果当前物体具有新特征，则存储到特征表中，如果新物体的特征在特征表中已经存在，则和对应的物体共用同一buffer。
// 此操作用于优化并减少buffer的数量。
struct CommonFeatureParams
{
	bool IsSame(const CommonFeatureParams &other) const;

	// 物体的类型。
	ePrimitiveType type;
	// 物体的特征参数。可能为空。
	vector<int> params;	
	// 物体的buffer数据。
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

	// 特征表，存储场景中每种物体的特征。
	// 特征相同的物体会通用部分GPU数据，从而节约显存开销。
	vector<CommonFeatureParams> m_commonFeatureTable;
};
