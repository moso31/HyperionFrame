#pragma once
#include "Camera.h"

class Box : public Shape
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // ������������С���붼�� 256 �ֽڵ���������

	Box();
	Box(const shared_ptr<DXResource>& dxResources, Camera* camera);
	~Box();

	void Init();

	void Update();
	void Render();

private:
	Camera* m_camera;
	shared_ptr<DXResource> m_dxResources;

	// ���弸�ε� Direct3D ��Դ��
	ComPtr<ID3D12GraphicsCommandList>					m_commandList;
	ComPtr<ID3D12RootSignature>							m_rootSignature;
	ComPtr<ID3D12PipelineState>							m_pipelineState;
	ComPtr<ID3D12DescriptorHeap>						m_cbvHeap;
	ComPtr<ID3D12Resource>								m_vertexBuffer;
	ComPtr<ID3D12Resource>								m_indexBuffer;
	ComPtr<ID3D12Resource>								m_constantBuffer;

	UINT8*												m_mappedConstantBuffer;
	UINT												m_cbvDescriptorSize;
	vector<byte>										m_vertexShader;
	vector<byte>										m_pixelShader;
	D3D12_VERTEX_BUFFER_VIEW							m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW								m_indexBufferView;
};
