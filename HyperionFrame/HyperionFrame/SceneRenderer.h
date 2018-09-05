#pragma once
#include "Box.h"

class SceneRenderer
{
public:
	SceneRenderer();
	SceneRenderer(const std::shared_ptr<DXResource>& dxResource);
	~SceneRenderer();

	void CreateDeviceDependentResources();
	void WindowSizeChanged();

	void Update();
	bool Render();

private:
	std::shared_ptr<DXResource> m_dxResources;

	// 立体几何的 Direct3D 资源。
	//ComPtr<ID3D12PipelineState>							m_pipelineState;
	ComPtr<ID3D12DescriptorHeap>						m_cbvHeap;

	D3D12_RECT											m_scissorRect;

	Box* m_test_box;
	Box* m_test_box_2;
	Camera* m_test_mainCamera;
};

