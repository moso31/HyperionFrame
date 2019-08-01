#pragma once
#include "DXResource.h"
#include "ShaderStructures.h"

#include "Transform.h"

enum eRenderType
{
	None,
	Shape,
	Line
};

enum ePrimitiveType
{
	NONE,

	// Shapes
	BOX,
	SPHERE,
	MESH,

	// Lines
	SEGMENT
};

struct PrimitiveBuffer
{
	// 立体几何的 Direct3D 资源。
	ComPtr<ID3D12Resource>			VB;
	ComPtr<ID3D12Resource>			IB;

	// 在 GPU 的默认堆中创建顶点缓冲区资源并使用上载堆将顶点数据复制到其中。
	// 在 GPU 使用完之前，不得释放上载资源。
	ComPtr<ID3D12Resource>			VBUpload;
	ComPtr<ID3D12Resource>			IBUpload;
};

class HPrimitive : public Transform
{
public:
	HPrimitive() = default;
	HPrimitive(const shared_ptr<DXResource>& dxResources);
	virtual ~HPrimitive();

	ePrimitiveType GetType() { return m_type; }
	void SetType(ePrimitiveType type) { m_type = type; }
	eRenderType GetRenderType() { return m_renderType; }
	void SetType(eRenderType type) { m_renderType = type; }

	AABB GetAABB();

	// 重新计算整个模型的AABB。
	AABB GetAABBWorld();

	virtual void Update() = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;


	virtual HUInt GetAlignedConstantBufferSize() = 0;
	void GeneratePrimitiveBuffer(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	virtual void SetPrimitiveBuffer(const PrimitiveBuffer& primitiveBuffer);

protected:
	shared_ptr<DXResource>		m_dxResources;

	ePrimitiveType				m_type;
	eRenderType					m_renderType;

	vector<VertexPNT>			m_vertices;
	vector<HUShort>				m_indices;
	AABB						m_aabb;

	PrimitiveBuffer				m_primitiveBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW		m_indexBufferView;
};
