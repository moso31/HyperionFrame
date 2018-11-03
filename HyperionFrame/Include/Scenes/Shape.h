#pragma once
#include "Camera.h"

class Shape : public Transform
{
public:
	Shape();
	Shape(const shared_ptr<DXResource>& dxResources, Camera* camera);
	virtual ~Shape();

	AABB GetAABB();

	// 重新计算整个模型的AABB。
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

	// 立体几何的 Direct3D 资源。
	ComPtr<ID3D12Resource>			m_vertexBuffer;
	ComPtr<ID3D12Resource>			m_indexBuffer;

	// 在 GPU 的默认堆中创建顶点缓冲区资源并使用上载堆将顶点数据复制到其中。
	// 在 GPU 使用完之前，不得释放上载资源。
	ComPtr<ID3D12Resource>			m_vertexBufferUpload;
	ComPtr<ID3D12Resource>			m_indexBufferUpload;

	D3D12_VERTEX_BUFFER_VIEW		m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW			m_indexBufferView;

	vector<VertexPCT>				m_vertices;
	vector<USHORT>					m_indices;

	HMaterial* m_material;
};
