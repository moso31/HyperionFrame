#include "App.h"
#include "global.h"
#include "HInput.h"

App::App(HWND hwnd)
{
	m_hwnd = hwnd;
	m_dxResources = std::make_shared<DXResource>(hwnd);
}

App::~App()
{
}

LRESULT App::MsgProc(HWND hWnd, HUInt msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_INPUT:
		HBII->UpdateRawInput(lParam);

		if (HBII->KeyDown(VK_ESCAPE))
			PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void App::Init(HFloat2 windowSize)
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

void App::SetWindow(HFloat2 size)
{
	m_dxResources->SetWindow(size);
}

void App::CreateRenderers()
{
	m_sceneRenderer = std::make_shared<SceneRenderer>(m_dxResources);
	m_sceneRenderer->WindowSizeChanged();
}
