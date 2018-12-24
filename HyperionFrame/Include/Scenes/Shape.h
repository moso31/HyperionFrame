#pragma once
#include "DXResource.h"
#include "DirectXHelper.h"
#include "ShaderStructures.h"

#include "Transform.h"

struct ShapeBuffer
{
	// ���弸�ε� Direct3D ��Դ��
	ComPtr<ID3D12Resource>			VB;
	ComPtr<ID3D12Resource>			IB;

	// �� GPU ��Ĭ�϶��д������㻺������Դ��ʹ�����ضѽ��������ݸ��Ƶ����С�
	// �� GPU ʹ����֮ǰ�������ͷ�������Դ��
	ComPtr<ID3D12Resource>			VBUpload;
	ComPtr<ID3D12Resource>			IBUpload;
};

struct Triangle
{
	union
	{
		struct
		{
			XMFLOAT3 A, B, C;
		};
		XMFLOAT3 p[3];
	};
};

struct TriangleUV
{
	union
	{
		struct
		{
			XMFLOAT2 A, B, C;
		};
		XMFLOAT2 p[3];
	};
};

enum eShapeType
{
	HSHAPE_NONE,
	HSHAPE_BOX,
	HSHAPE_SPHERE
};

class Shape : public Transform
{
public:
	Shape();
	Shape(const shared_ptr<DXResource>& dxResources);
	virtual ~Shape();

	eShapeType GetType() { return m_type; }
	void SetType(eShapeType type) { m_type = type; }

	AABB GetAABB();

	// ���¼�������ģ�͵�AABB��
	AABB GetAABBWorld();

	virtual void Update(UINT8 * destination) = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	UINT GetFaceCount();
	Triangle GetFace(UINT faceIndex);
	TriangleUV GetUVs(UINT faceIndex);

	HMaterial* GetMaterial() { return m_material; }
	void SetMaterial(HMaterial* mat);

	virtual bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, float* out_tHit) = 0;
	virtual bool IntersectP(Ray worldRay, float* out_t0, float* out_t1) = 0;

	void GenerateShapeBuffer(ComPtr<ID3D12GraphicsCommandList> pCommandList, ShapeBuffer * pShapeBuffer);
	void SetShapeBuffer(ShapeBuffer* pShapeBuffer);

protected:
	eShapeType	m_type;

	Camera*		m_camera;
	AABB		m_aabb;

	shared_ptr<DXResource> m_dxResources;

	ShapeBuffer*					m_pShapeBuffer;
	D3D12_VERTEX_BUFFER_VIEW		m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW			m_indexBufferView;

	vector<VertexPNT>				m_vertices;
	vector<USHORT>					m_indices;

	HMaterial* m_material;
	CBufferVertData m_cbDataMaterial;
};
