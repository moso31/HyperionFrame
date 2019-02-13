#include "App.h"
#include "global.h"
#include "HInput.h"

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

LRESULT App::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HEventArg eArg;
	eArg.X = (UINT)LOWORD(lParam);
	eArg.Y = (UINT)HIWORD(lParam);

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
	case WM_LBUTTONDOWN:
	{
		HMouseDownEvent::GetInstance()->OnNotify(eArg);
		break;
	}
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_MOUSEACTIVATE:
		break;
	case WM_KEYDOWN:
	{
		HKeyDownEvent::GetInstance()->OnNotify(eArg);
		break;
	}
	case WM_KEYUP:
	{
		//HKeyUpEvent::GetInstance()->OnNotify(eArg);
		break;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
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
