#pragma once
#include "DXResource.h"
#include "DirectXHelper.h"
#include "ShaderStructures.h"

#include "Transform.h"

struct ShapeBuffer
{
	// 立体几何的 Direct3D 资源。
	ComPtr<ID3D12Resource>			VB;
	ComPtr<ID3D12Resource>			IB;

	// 在 GPU 的默认堆中创建顶点缓冲区资源并使用上载堆将顶点数据复制到其中。
	// 在 GPU 使用完之前，不得释放上载资源。
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
	HSHAPE_SPHERE,
	HSHAPE_MESH,
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

	// 重新计算整个模型的AABB。
	AABB GetAABBWorld();

	virtual void Update(UINT8 * destination) = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	UINT GetFaceCount();
	Triangle GetFace(UINT faceIndex);
	TriangleUV GetUVs(UINT faceIndex);

	shared_ptr<HMaterial> GetMaterial() { return m_material; }
	void SetMaterial(shared_ptr<HMaterial> mat);

	virtual bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, float* out_tHit) = 0;
	virtual bool IntersectP(Ray worldRay, float* out_t0, float* out_t1) = 0;

	void GenerateShapeBuffer(ComPtr<ID3D12GraphicsCommandList> pCommandList, ShapeBuffer * pShapeBuffer);
	void SetShapeBuffer(ShapeBuffer* pShapeBuffer);

protected:
	shared_ptr<DXResource>		m_dxResources;

	eShapeType					m_type;

	vector<VertexPNT>			m_vertices;
	vector<USHORT>				m_indices;
	AABB						m_aabb;
	shared_ptr<HMaterial>		m_material;

	CBufferMeshData				m_cbMeshData;
	ShapeBuffer*				m_pShapeBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW		m_indexBufferView;
};
