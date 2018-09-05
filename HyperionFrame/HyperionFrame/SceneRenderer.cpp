#include "SceneRenderer.h"

SceneRenderer::SceneRenderer()
{
}

SceneRenderer::SceneRenderer(const std::shared_ptr<DXResource>& dxResource)
{
	m_dxResources = dxResource;

	m_test_mainCamera = new Camera(dxResource);
	m_test_mainCamera->Init();

	m_test_box = new Box(dxResource, m_test_mainCamera);
	m_test_box->Init();

	m_test_box_2 = new Box(dxResource, m_test_mainCamera);
	m_test_box_2->Init();

	m_test_box_2->SetTranslation(10.0f, 0.0f, -10.0f);
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::CreateDeviceDependentResources()
{
}

void SceneRenderer::WindowSizeChanged()
{
	m_test_mainCamera->OnResize();
}

void SceneRenderer::Update()
{
	static float x = 0;
	x += 0.01f;
	m_test_box->SetRotation(0.0f, x, 0.0f);
	m_test_box->Update();
	m_test_box_2->SetRotation(0.0f, 0.0f, x);
	m_test_box_2->Update();
}

bool SceneRenderer::Render()
{
	ThrowIfFailed(m_dxResources->GetCommandAllocator()->Reset());
	m_test_box->Render();
	m_test_box_2->Render();
	return true;
}
