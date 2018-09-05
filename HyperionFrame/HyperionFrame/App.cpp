#include "App.h"


App::App()
{
}

App::App(HWND hwnd)
{
	m_hwnd = hwnd;
	m_dxResources = std::make_shared<DXResource>(hwnd);
}

App::~App()
{
}

void App::Init(XMFLOAT2 windowSize)
{
	SetWindow(windowSize);
	CreateRenderers();
}

void App::Run()
{
	auto commandQueue = m_dxResources->GetCommandQueue();
	PIXBeginEvent(commandQueue, 0, L"Update");
	{
		Update();
	}
	PIXEndEvent(commandQueue);

	PIXBeginEvent(commandQueue, 0, L"Render");
	{
		if (Render())
		{
			m_dxResources->Present();
		}
	}
	PIXEndEvent(commandQueue);
}

void App::Update()
{
	m_sceneRenderer->Update();
}

bool App::Render()
{
	return m_sceneRenderer->Render();
}

void App::SetWindow(XMFLOAT2 size)
{
	m_dxResources->SetWindow(size);
}

void App::CreateRenderers()
{
	m_sceneRenderer = std::make_shared<SceneRenderer>(m_dxResources);
	m_sceneRenderer->WindowSizeChanged();
}
