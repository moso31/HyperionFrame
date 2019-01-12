#include "HPrimitive.h"

HPrimitive::HPrimitive()
{
}

HPrimitive::HPrimitive(const shared_ptr<DXResource>& dxResources) :
	m_dxResources(dxResources)
{
}

HPrimitive::~HPrimitive()
{
}

AABB HPrimitive::GetAABB()
{
	return m_aabb;
}

AABB HPrimitive::GetAABBWorld()
{
	XMVECTOR maxV = XMLoadFloat3(&m_aabb.GetVecMax());
	XMVECTOR minV = XMLoadFloat3(&m_aabb.GetVecMin());
	XMMATRIX mxObject2World = XMLoadFloat4x4(&worldMatrix);
	XMFLOAT3 max, min;
	XMStoreFloat3(&max, XMVector3TransformCoord(maxV, mxObject2World));
	XMStoreFloat3(&min, XMVector3TransformCoord(minV, mxObject2World));
	return AABB(min, max);
}

void HPrimitive::GeneratePrimitiveBuffer(ComPtr<ID3D12GraphicsCommandList> pCommandList, PrimitiveBuffer * pShapeBuffer)
{
	if (m_vertices.empty() || m_indices.empty())
	{
		printf("[WARNING] 顶点/索引数据为空，无法创建缓存。\n");
		return;
	}

	auto d3dDevice = m_dxResources->GetD3DDevice();

	const UINT vertexBufferSize = UINT(sizeof(VertexPNT) * m_vertices.size());

	CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&defaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&pShapeBuffer->VB)));

	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&pShapeBuffer->VBUpload)));

	DX::NAME_D3D12_OBJECT(pShapeBuffer->VB);
	DX::NAME_D3D12_OBJECT(pShapeBuffer->VBUpload);

	// 将顶点缓冲区上载到 GPU。
	{
		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = reinterpret_cast<BYTE*>(m_vertices.data());
		vertexData.RowPitch = vertexBufferSize;
		vertexData.SlicePitch = vertexData.RowPitch;

		UpdateSubresources(pCommandList.Get(), pShapeBuffer->VB.Get(), pShapeBuffer->VBUpload.Get(), 0, 0, 1, &vertexData);

		CD3DX12_RESOURCE_BARRIER vertexBufferResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(pShapeBuffer->VB.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		pCommandList->ResourceBarrier(1, &vertexBufferResourceBarrier);
	}

	const UINT indexBufferSize = UINT(sizeof(unsigned short) * m_indices.size());

	CD3DX12_RESOURCE_DESC indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
	DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&defaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&pShapeBuffer->IB)));

	DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&indexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&pShapeBuffer->IBUpload)));

	DX::NAME_D3D12_OBJECT(pShapeBuffer->IB);
	DX::NAME_D3D12_OBJECT(pShapeBuffer->IBUpload);

	// 将索引缓冲区上载到 GPU。
	{
		D3D12_SUBRESOURCE_DATA indexData = {};
		indexData.pData = reinterpret_cast<BYTE*>(m_indices.data());
		indexData.RowPitch = indexBufferSize;
		indexData.SlicePitch = indexData.RowPitch;

		UpdateSubresources(pCommandList.Get(), pShapeBuffer->IB.Get(), pShapeBuffer->IBUpload.Get(), 0, 0, 1, &indexData);

		CD3DX12_RESOURCE_BARRIER indexBufferResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(pShapeBuffer->IB.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
		pCommandList->ResourceBarrier(1, &indexBufferResourceBarrier);
	}

	// 创建顶点/索引缓冲区视图。
	m_vertexBufferView.BufferLocation = pShapeBuffer->VB->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(VertexPNT);
	m_vertexBufferView.SizeInBytes = (UINT)(sizeof(VertexPNT) * m_vertices.size());

	m_indexBufferView.BufferLocation = pShapeBuffer->IB->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = (UINT)(sizeof(USHORT) * m_indices.size());
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;

	m_pShapeBuffer = pShapeBuffer;
}

void HPrimitive::SetPrimitiveBuffer(PrimitiveBuffer * pShapeBuffer)
{
	// 创建顶点/索引缓冲区视图。
	m_vertexBufferView.BufferLocation = pShapeBuffer->VB->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(VertexPNT);
	m_vertexBufferView.SizeInBytes = (UINT)(sizeof(VertexPNT) * m_vertices.size());

	m_indexBufferView.BufferLocation = pShapeBuffer->IB->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = (UINT)(sizeof(USHORT) * m_indices.size());
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;

	m_pShapeBuffer = pShapeBuffer;
}
