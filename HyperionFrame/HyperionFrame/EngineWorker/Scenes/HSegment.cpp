#include "HSegment.h"
#include "PipelineManager.h"

HSegment::HSegment()
{
}

HSegment::HSegment(const shared_ptr<DXResource>& dxResources) :
	HLine(dxResources)
{
	m_type = ePrimitiveType::SEGMENT;
	m_renderType = eRenderType::Line;
}

HSegment::~HSegment()
{
}

void HSegment::InitParameters(HFloat3 point1, HFloat3 point2)
{
	m_vertices =
	{
		{ point1, HFloat3(), HFloat2() },
		{ point2, HFloat3(), HFloat2() },
	};

	m_indices =
	{
		0,  1
	};

	m_aabb = AABB(point1);
	m_aabb.Merge(point2);
}

void HSegment::Update()
{
	// 准备将更新的模型矩阵传递到着色器。
	PipelineManager::s_constantBufferData.model;

	memcpy(m_mappedConstantBuffer, &PipelineManager::s_constantBufferData, sizeof(PipelineManager::s_constantBufferData));
	//memcpy(destination + 256, &m_cbMeshData, sizeof(m_cbMeshData));
}

void HSegment::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	pCommandList->IASetIndexBuffer(&m_indexBufferView);
	pCommandList->DrawIndexedInstanced(2, 1, 0, 0, 0);
}
