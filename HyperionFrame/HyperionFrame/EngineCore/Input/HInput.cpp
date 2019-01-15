#include "HInput.h"
#include "global.h"

HInput::HInput()
{
	m_mouseMove = XMINT2(0, 0);
	::ZeroMemory(m_keyState, sizeof(m_keyState));
	::ZeroMemory(m_keyActivite, sizeof(m_keyActivite));
	::ZeroMemory(m_mouseState, sizeof(m_mouseState));
	::ZeroMemory(m_mouseActivite, sizeof(m_mouseActivite));
	
	RAWINPUTDEVICE rid[2];
	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x02;
	rid[0].dwFlags = 0; // RIDEV_NOLEGACY;
	rid[0].hwndTarget = 0;
	
	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x06;
	rid[1].dwFlags = 0; // RIDEV_NOLEGACY;
	rid[1].hwndTarget = 0;

	printf("Raw Input �����豸ע����...\n");
	if (!RegisterRawInputDevices(rid, 2, sizeof(rid[0])))
	{
		printf("ʧ�ܣ��豸ע��ʧ�ܡ�\n");
		return;
	}
	printf("��ɡ�\n");
}

HInput::~HInput()
{
}

bool HInput::KeyDown(int key)
{
	return m_keyState[key] && m_keyActivite[key];
}

bool HInput::Key(int key)
{
	return m_keyState[key];
}

bool HInput::KeyUp(int key)
{
	return !m_keyState[key] && m_keyActivite[key];
}

bool HInput::MouseDown(int key)
{
	return m_mouseState[key] && m_mouseActivite[key];
}

bool HInput::MousePressing(int key)
{
	return m_mouseState[key];
}

bool HInput::MouseUp(int key)
{
	return !m_mouseState[key] && m_mouseActivite[key];
}

XMINT2 HInput::MouseMove()
{
	return m_mouseMove;
}

XMINT2 HInput::MousePosition()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(g_hWnd, &p);
	return XMINT2(p.x, p.y);
}

void HInput::RestoreData()
{
	m_mouseMove = XMINT2(0, 0);
	::ZeroMemory(m_keyActivite, sizeof(m_keyActivite));
	::ZeroMemory(m_mouseActivite, sizeof(m_mouseActivite));
}

void HInput::Update()
{
}

#define REGISTER_MKMOUSESTATE(RI_MOUSE_CODE, MK_CODE, bValue) if ((raw->data.mouse.usButtonFlags & RI_MOUSE_CODE) != 0) m_mouseKey[MK_CODE] = bValue;
void HInput::UpdateRawInput(LPARAM lParam)
{
	UINT dwSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == nullptr)
		return;

	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		printf("ע�⣺GetRawInputData���ص�����ֵ��Ԥ�Ƶĳ��Ȳ�ƥ�䣡����ܻ����δ֪����\n");

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		//LOGMSG(raw->data.keyboard.MakeCode, raw->data.keyboard.Flags, raw->data.keyboard.Reserved, raw->data.keyboard.ExtraInformation, raw->data.keyboard.Message, raw->data.keyboard.VKey);
		USHORT	iKeyCode = raw->data.keyboard.VKey;
		bool	bIsKeyPressing = (raw->data.keyboard.Flags & 1) == 0;

		m_keyState[iKeyCode] = bIsKeyPressing;
		m_keyActivite[iKeyCode] = true;
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		//LOGMSG(raw->data.mouse.usFlags, raw->data.mouse.ulButtons, raw->data.mouse.usButtonFlags, raw->data.mouse.usButtonData, raw->data.mouse.ulRawButtons, raw->data.mouse.lLastX, raw->data.mouse.lLastY, raw->data.mouse.ulExtraInformation);

		int iMousePressing = raw->data.mouse.usButtonFlags;
		int count = 0;

		while (iMousePressing)
		{
			m_mouseState[count] = (iMousePressing & 3) == 1;
			m_mouseActivite[count] = iMousePressing & 3;
			iMousePressing >>= 2;
			count++;
		}

		m_mouseMove.x = raw->data.mouse.lLastX;
		m_mouseMove.y = raw->data.mouse.lLastY;
	}

	//PrintMouseState();

	delete[] lpb;
}

void HInput::PrintMouseState()
{
	bool flag = false;
	for (int i = 0; i < 5; i++)
	{
		if (m_mouseActivite[i])
			flag = true;
	}
	if (!flag)
		return;

	printf("state: ");
	for (int i = 0; i < 5; i++) printf("%d ", m_mouseState[i]);
	printf("activite: ");
	for (int i = 0; i < 5; i++) printf("%d ", m_mouseActivite[i]);
	printf("\n");
}

void HInput::Release()
{
}