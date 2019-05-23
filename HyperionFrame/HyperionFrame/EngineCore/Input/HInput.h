#pragma once
#include "header.h"
#include "HInstance.h"

#define HBII HInput::GetInstance()

#define MK_LEFT		0
#define MK_MID		1
#define MK_RIGHT	2

class HInput : public HInstance<HInput>
{
public:
	HInput();
	~HInput();

	bool KeyDown(HInt key);
	bool Key(HInt key);
	bool KeyUp(HInt key);
	bool MouseDown(HInt key);
	bool MousePressing(HInt key);
	bool MouseUp(HInt key);
	HInt2 MouseMove();
	HInt2 MousePosition();	// 获取鼠标在当前客户端的位置（左上角0,0）
	void RestoreData();

	void Update();
	void UpdateRawInput(LPARAM lParam);

	void PrintMouseState();

	void Release();

private:
	bool m_keyState[256];
	bool m_keyActivite[256];

	bool m_mouseState[256];
	bool m_mouseActivite[256];

	HInt2 m_mouseMove;
};

