#pragma once
#include "HScene.h"

class SceneRenderer
{
public:
	// 常量缓冲区大小必须都是 256 字节的整数倍。
	static const UINT c_alignedConstantBufferSize = (
		(sizeof(ModelViewProjectionConstantBuffer) + 255) + 
		(sizeof(CBufferMeshData) + 255)
		) & ~255;
	SceneRenderer();
	SceneRenderer(const std::shared_ptr<DXResource>& dxResource, const std::shared_ptr<HInput>& input);
	~SceneRenderer();

	void CreateSceneResources();
	void WindowSizeChanged();

	void Update();
	bool Render();

	void OnLButtonClicked(XMINT2 screenXY);
	void OnKeyDown(WPARAM wParam);

private:
	std::shared_ptr<DXResource> m_dxResources;
	std::shared_ptr<HInput> m_input;

	ComPtr<ID3D12GraphicsCommandList>					m_commandList;
	ComPtr<ID3D12RootSignature>							m_rootSignature;
	ComPtr<ID3D12PipelineState>							m_pipelineState;
	vector<byte>										m_vertexShader;
	vector<byte>										m_pixelShader;

	ComPtr<ID3D12DescriptorHeap>						m_cbvHeap;
	UINT												m_cbvDescriptorSize;
	ComPtr<ID3D12Resource>								m_constantBuffer;
	UINT8*												m_mappedConstantBuffer;

	D3D12_RECT											m_scissorRect;

	HScene* m_test_scene;
};

