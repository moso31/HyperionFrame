#pragma once
#include "SceneRenderer.h"

class App
{
public:
	App();
	App(HWND hwnd);
	~App();

	void Init(XMFLOAT2 windowSize);
	void Run();

	void Update();
	bool Render();

	void SetWindow(XMFLOAT2 size);
	void CreateRenderers();

	void OnLButtonClicked(XMINT2 screenXY);
	void OnKeyDown(WPARAM wParam);

private:
	HWND m_hwnd;
	std::shared_ptr<DXResource> m_dxResources;
	std::shared_ptr<SceneRenderer> m_sceneRenderer;

	std::shared_ptr<HEvent> m_pEventKeyDown;
};
