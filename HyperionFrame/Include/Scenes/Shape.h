#pragma once
#include "Camera.h"

class Shape : public Transform
{
public:
	Shape();
	Shape(const shared_ptr<DXResource>& dxResources, Camera* camera);
	virtual ~Shape();

	AABB GetAABB();

	// ���¼�������ģ�͵�AABB��
	AABB GenerateAABB();

	virtual void Update(UINT8 * destination) = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	UINT GetFaceCount();
	void GetFace(UINT faceIndex, XMFLOAT3 out_face[3]);
	void GetUVs(UINT faceIndex, XMFLOAT2 out_uv[3]);

	HMaterial* GetMaterial() { return m_material; }
	void SetMaterial(HMaterial* mat) { m_material = mat; }

	virtual void Intersect(Ray worldRay, SurfaceInteraction* out_isect) = 0;
	virtual bool IntersectP(Ray worldRay) = 0;

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

	vector<VertexPCT>				m_vertices;
	vector<USHORT>					m_indices;

	HMaterial* m_material;
};
