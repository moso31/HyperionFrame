#include "App.h"
#include "global.h"
#include "HBInput.h"

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DWORD x = LOWORD(lParam);
	DWORD y = HIWORD(lParam);

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
		g_app->OnLButtonClicked(XMINT2((int)x, (int)y));
		break;
	case WM_LBUTTONUP:
		g_test = false;
		break;
	case WM_MOUSEMOVE:
		g_pos = { (int)x, (int)y };
		break;
	case WM_MOUSEACTIVATE:
		g_pos = { (int)x, (int)y };
		break;
	case WM_KEYDOWN:
	{
		g_app->OnKeyDown(wParam);
		break;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool InitWindow()
{
	AllocConsole();

	FILE* fp = 0;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	printf("����ִ��WIN32 API ���ڳ�ʼ��...");
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = ::MsgProc; //ָ���ص�����
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInstance;
	wc.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Hello baby.";
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		printf("���ڳ�ʼ��ʧ�ܣ�ע��WIN32 API��������Ч��\n");
		MessageBox(0, "RegisterClass Failed.", 0, 0);
		return false;
	}

	SetProcessDPIAware();
	HDC screen = GetDC(NULL);
	int screenWidth = GetDeviceCaps(screen, HORZRES);
	int screenHeight = GetDeviceCaps(screen, VERTRES);
	ReleaseDC(NULL, screen);

	g_windowSize = XMFLOAT2(screenWidth * WINDOW_RATIO, screenHeight * WINDOW_RATIO);
	//g_windowSize = XMFLOAT2(1366, 768);

	RECT R = { 0, 0, (int)g_windowSize.x, (int)g_windowSize.y };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int clientW = R.right - R.left;
	int clientH = R.bottom - R.top;

	g_hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, 100, 100, clientW, clientH, NULL, NULL, g_hInstance, NULL);
	
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	printf("��ɡ�\n");
	return true;
}

bool InitApp()
{
	g_app = new App(g_hWnd);
	g_app->Init(g_windowSize);
	return false;
}

void RunApp()
{
	g_app->Run();
}

int Run()
{
	MSG msg = { 0 };
	::ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		HBII->Update();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (g_test)
			{
				g_app->OnLButtonClicked(g_pos);
				g_test = false;
			}

			RunApp();

			HBII->RestoreData(); // ���һ�����λ��
		}
	}

	return static_cast<int>(msg.wParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	InitWindow();
	InitApp();

	Run();
	return 0;
}
