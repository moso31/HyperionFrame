#pragma once
#include "SceneRenderer.h"

class App
{
public:
	App() = default;
	App(HWND hwnd);
	~App();

	LRESULT MsgProc(HWND hWnd, HUInt msg, WPARAM wParam, LPARAM lParam);

	void Init(HFloat2 windowSize);
	void Run();

	void Update();
	bool Render();

	void SetWindow(HFloat2 size);
	void CreateRenderers();

private:
	HWND m_hwnd;
	std::shared_ptr<DXResource> m_dxResources;
	std::shared_ptr<SceneRenderer> m_sceneRenderer;
};
