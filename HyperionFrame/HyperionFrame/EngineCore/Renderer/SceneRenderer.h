#pragma once
#include "HScene.h"

class SceneRenderer
{
public:
	SceneRenderer();
	SceneRenderer(const std::shared_ptr<DXResource>& dxResource);
	~SceneRenderer();

	void CreateSceneResources();
	void WindowSizeChanged();

	void Update();
	bool Render();

private:
	std::shared_ptr<DXResource> m_dxResources;

	ComPtr<ID3D12GraphicsCommandList>					m_commandList;
	ComPtr<ID3D12RootSignature>							m_rootSignature;

	// 记录场景的所有渲染管线状态对象。
	map<string, ComPtr<ID3D12PipelineState>>			m_PSOs;

	vector<byte>										m_vertexShader;
	vector<byte>										m_pixelShader;

	D3D12_RECT											m_scissorRect;

	std::shared_ptr<HScene> m_test_scene;
};

