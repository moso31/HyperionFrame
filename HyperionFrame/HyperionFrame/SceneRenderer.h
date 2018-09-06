#pragma once
#include "Box.h"

class SceneRenderer
{
public:
	SceneRenderer();
	SceneRenderer(const std::shared_ptr<DXResource>& dxResource);
	~SceneRenderer();

	void CreateSceneResources();
	void LoadSceneAssets();
	void WindowSizeChanged();

	void Update();
	bool Render();

private:
	std::shared_ptr<DXResource> m_dxResources;

	ComPtr<ID3D12GraphicsCommandList>					m_commandList;
	ComPtr<ID3D12RootSignature>							m_rootSignature;
	ComPtr<ID3D12PipelineState>							m_pipelineState;
	vector<byte>										m_vertexShader;
	vector<byte>										m_pixelShader;

	ComPtr<ID3D12DescriptorHeap>						m_cbvHeap;

	D3D12_RECT											m_scissorRect;

	Box* m_test_box;
	Box* m_test_box_2;
	Camera* m_test_mainCamera;
};

