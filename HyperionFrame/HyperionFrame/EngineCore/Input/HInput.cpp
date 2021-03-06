#include "HInput.h"
#include "global.h"

HInput::HInput()
{
	m_mouseMove = HInt2(0, 0);
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

	printf("Raw Input 输入设备注册中...\n");
	if (!RegisterRawInputDevices(rid, 2, sizeof(rid[0])))
	{
		printf("失败：设备注册失败。\n");
		return;
	}
	printf("完成。\n");
}

HInput::~HInput()
{
}

bool HInput::KeyDown(HInt key)
{
	return m_keyState[key] && m_keyActivite[key];
}

bool HInput::Key(HInt key)
{
	return m_keyState[key];
}

bool HInput::KeyUp(HInt key)
{
	return !m_keyState[key] && m_keyActivite[key];
}

bool HInput::MouseDown(HInt key)
{
	return m_mouseState[key] && m_mouseActivite[key];
}

bool HInput::MousePressing(HInt key)
{
	return m_mouseState[key];
}

bool HInput::MouseUp(HInt key)
{
	return !m_mouseState[key] && m_mouseActivite[key];
}

HInt2 HInput::MouseMove()
{
	return m_mouseMove;
}

HInt2 HInput::MousePosition()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(g_hWnd, &p);
	return HInt2(p.x, p.y);
}

void HInput::RestoreData()
{
	m_mouseMove = HInt2(0, 0);
	::ZeroMemory(m_keyActivite, sizeof(m_keyActivite));
	::ZeroMemory(m_mouseActivite, sizeof(m_mouseActivite));
}

void HInput::Update()
{
}

#define REGISTER_MKMOUSESTATE(RI_MOUSE_CODE, MK_CODE, bValue) if ((raw->data.mouse.usButtonFlags & RI_MOUSE_CODE) != 0) m_mouseKey[MK_CODE] = bValue;
void HInput::UpdateRawInput(LPARAM lParam)
{
	HUInt dwSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == nullptr)
		return;

	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		printf("注意：GetRawInputData返回的数据值和预计的长度不匹配！这可能会造成未知错误\n");

	HEventArg eArg;
	HInt2 cursor = MousePosition();
	eArg.X = cursor.x;
	eArg.Y = cursor.y;

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		//LOGMSG(raw->data.keyboard.MakeCode, raw->data.keyboard.Flags, raw->data.keyboard.Reserved, raw->data.keyboard.ExtraInformation, raw->data.keyboard.Message, raw->data.keyboard.VKey);
		eArg.VKey = raw->data.keyboard.VKey;
		bool bIsPressing = (raw->data.keyboard.Flags & 1) == 0;

		m_keyState[eArg.VKey] = bIsPressing;
		m_keyActivite[eArg.VKey] = true;

		if (bIsPressing) HKeyDownEvent::GetInstance()->OnNotify(eArg);
		else HKeyUpEvent::GetInstance()->OnNotify(eArg);
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		//LOGMSG(raw->data.mouse.usFlags, raw->data.mouse.ulButtons, raw->data.mouse.usButtonFlags, raw->data.mouse.usButtonData, raw->data.mouse.ulRawButtons, raw->data.mouse.lLastX, raw->data.mouse.lLastY, raw->data.mouse.ulExtraInformation);

		eArg.VMouse = raw->data.mouse.usButtonFlags;
		eArg.VWheel = raw->data.mouse.usButtonData;
		HInt iMousePressing = eArg.VMouse;
		HInt count = 0;

		bool bIsPressing = false;
		while (iMousePressing)
		{
			m_mouseState[count] = (iMousePressing & 3) == 1;
			m_mouseActivite[count] = iMousePressing & 3;

			if (m_mouseState[count]) 
				bIsPressing = true;
			iMousePressing >>= 2;
			count++;
		}

		eArg.LastX = raw->data.mouse.lLastX;
		eArg.LastY = raw->data.mouse.lLastY;
		m_mouseMove.x = eArg.LastX;
		m_mouseMove.y = eArg.LastY;

		if (bIsPressing) HMouseDownEvent::GetInstance()->OnNotify(eArg);
		else HMouseUpEvent::GetInstance()->OnNotify(eArg);

		if (eArg.LastX || eArg.LastY)
			HMouseMoveEvent::GetInstance()->OnNotify(eArg);
	}

	//PrintMouseState();

	delete[] lpb;
}

void HInput::PrintMouseState()
{
	bool flag = false;
	for (HInt i = 0; i < 5; i++)
	{
		if (m_mouseActivite[i])
			flag = true;
	}
	if (!flag)
		return;

	printf("state: ");
	for (HInt i = 0; i < 5; i++) printf("%d ", m_mouseState[i]);
	printf("activite: ");
	for (HInt i = 0; i < 5; i++) printf("%d ", m_mouseActivite[i]);
	printf("\n");
}

void HInput::Release()
{
}