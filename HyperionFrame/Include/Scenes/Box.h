#pragma once
#include "Camera.h"

class Box : public Shape
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // 常量缓冲区大小必须都是 256 字节的整数倍。

	Box();
	Box(const shared_ptr<DXResource>& dxResources, Camera* camera);
	~Box();

	void Init();

	void Update();
	void Render();

private:
	Camera* m_camera;
	shared_ptr<DXResource> m_dxResources;

	// 立体几何的 Direct3D 资源。
	ComPtr<ID3D12DescriptorHeap>						m_cbvHeap;
	ComPtr<ID3D12Resource>								m_vertexBuffer;
	ComPtr<ID3D12Resource>								m_indexBuffer;
	ComPtr<ID3D12Resource>								m_constantBuffer;

	UINT8*												m_mappedConstantBuffer;
	UINT												m_cbvDescriptorSize;
	D3D12_VERTEX_BUFFER_VIEW							m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW								m_indexBufferView;
};
