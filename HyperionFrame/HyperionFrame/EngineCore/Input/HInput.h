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

	bool KeyDown(int key);
	bool Key(int key);
	bool KeyUp(int key);
	bool MouseDown(int key);
	bool MousePressing(int key);
	bool MouseUp(int key);
	XMINT2 MouseMove();
	XMINT2 MousePosition();	// ��ȡ����ڵ�ǰ�ͻ��˵�λ�ã����Ͻ�0,0��
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

	XMINT2 m_mouseMove;
};
