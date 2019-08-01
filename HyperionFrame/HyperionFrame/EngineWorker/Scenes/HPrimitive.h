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
	// ���弸�ε� Direct3D ��Դ��
	ComPtr<ID3D12Resource>			VB;
	ComPtr<ID3D12Resource>			IB;

	// �� GPU ��Ĭ�϶��д������㻺������Դ��ʹ�����ضѽ��������ݸ��Ƶ����С�
	// �� GPU ʹ����֮ǰ�������ͷ�������Դ��
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

	// ���¼�������ģ�͵�AABB��
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
