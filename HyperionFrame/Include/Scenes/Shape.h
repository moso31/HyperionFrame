#pragma once
#include "DXResource.h"
#include "DirectXHelper.h"
#include "ShaderStructures.h"

#include "Transform.h"

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

class Shape : public Transform
{
public:
	Shape();
	Shape(const shared_ptr<DXResource>& dxResources);
	virtual ~Shape();

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

private:
	virtual void _initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

protected:
	Camera*		m_camera;
	AABB		m_aabb;

	shared_ptr<DXResource> m_dxResources;

	// ���弸�ε� Direct3D ��Դ��
	ComPtr<ID3D12Resource>			m_vertexBuffer;
	ComPtr<ID3D12Resource>			m_indexBuffer;

	// �� GPU ��Ĭ�϶��д������㻺������Դ��ʹ�����ضѽ��������ݸ��Ƶ����С�
	// �� GPU ʹ����֮ǰ�������ͷ�������Դ��
	ComPtr<ID3D12Resource>			m_vertexBufferUpload;
	ComPtr<ID3D12Resource>			m_indexBufferUpload;

	D3D12_VERTEX_BUFFER_VIEW		m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW			m_indexBufferView;

	vector<VertexPNT>				m_vertices;
	vector<USHORT>					m_indices;

	HMaterial* m_material;
	CBufferVertData m_cbDataMaterial;
};
