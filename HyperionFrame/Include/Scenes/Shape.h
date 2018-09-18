#pragma once
#include "Camera.h"

struct HTriangle
{
	union 
	{
		struct
		{
			XMFLOAT3 point0, point1, point2;
		};
		struct
		{
			XMFLOAT3 p[3];
		};
	};
};

class Shape : public Transform
{
public:
	Shape();
	Shape(const shared_ptr<DXResource>& dxResources, Camera* camera);
	~Shape();

	AABB GetAABB();

	// 重新计算整个模型的AABB。
	AABB GenerateAABB();

	virtual void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;
	virtual void Update(UINT8 * destination) = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	UINT GetFaceCount();
	HTriangle GetFace(UINT faceIndex);

private:
	// 初始化缓存
	virtual void _initBufferData(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	// 初始化数据（AABB、长宽高、半径等）
	virtual void _initParameters() = 0;

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

	vector<VertexPositionColor>		m_vertices;
	vector<USHORT>					m_indices;
};
