#pragma once
#include "HPrimitive.h"

class HSceneManager
{
public:
	HSceneManager();
	HSceneManager(std::shared_ptr<DXResource> dxResources, const ComPtr<ID3D12DescriptorHeap>& pCbvHeap, const ComPtr<ID3D12GraphicsCommandList>& pCommandList);
	~HSceneManager();

	shared_ptr<Box>			CreateBox(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
	shared_ptr<Sphere>		CreateSphere(float radius, int segmentHorizontal, int segmentVertical);
	shared_ptr<HMesh>		CreateMesh(string filepath);
	shared_ptr<HSegment>	CreateSegment(XMFLOAT3 point1, XMFLOAT3 point2);

private:
	std::shared_ptr<DXResource>			m_dxResources;
	ComPtr<ID3D12GraphicsCommandList>	m_pCommandList;
};
