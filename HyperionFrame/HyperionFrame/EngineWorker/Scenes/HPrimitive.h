#pragma once
#include "DXResource.h"
#include "DirectXHelper.h"
#include "ShaderStructures.h"

#include "Transform.h"

enum ePrimitiveType
{
	NONE,

	// Shapes
	BOX,
	SPHERE,
	MESH,

	// Lines
	//LINE
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
	HPrimitive();
	HPrimitive(const shared_ptr<DXResource>& dxResources);
	virtual ~HPrimitive();

	ePrimitiveType GetType() { return m_type; }
	void SetType(ePrimitiveType type) { m_type = type; }

	AABB GetAABB();

	// ���¼�������ģ�͵�AABB��
	AABB GetAABBWorld();

	virtual void Update(UINT8 * destination) = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	void GeneratePrimitiveBuffer(ComPtr<ID3D12GraphicsCommandList> pCommandList, PrimitiveBuffer * pShapeBuffer);
	void SetPrimitiveBuffer(PrimitiveBuffer* pShapeBuffer);

protected:
	shared_ptr<DXResource>		m_dxResources;

	ePrimitiveType				m_type;

	vector<VertexPNT>			m_vertices;
	vector<USHORT>				m_indices;
	AABB						m_aabb;

	PrimitiveBuffer*			m_pShapeBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW		m_indexBufferView;
};
