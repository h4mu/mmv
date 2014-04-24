#include <windows.h>

static const UINT IDT_TIMER = 1;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow)
{
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	char appTitle[] = "MMV";

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appTitle;

	if (!RegisterClass(&wc))
		return 0;

	hwnd = CreateWindow(appTitle, appTitle,
		WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
		CW_USEDEFAULT, CW_USEDEFAULT, 100, 100,
		NULL, NULL, hInst, NULL);

	if (!hwnd)
		return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC dc;
			RECT r;
			GetClientRect(hwnd, &r);
			dc = BeginPaint(hwnd, &ps);
			DrawText(dc, "Mouse Mover", -1, &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hwnd, &ps);
			break;

		case WM_CREATE:
			ShowWindow(hwnd, SW_MAXIMIZE);
			SetTimer(hwnd, IDT_TIMER, 10000, NULL);
			break;

		case WM_TIMER:
			POINT pt;
			RECT rc;
			GetClientRect(hwnd, &rc);
			pt.x = rc.left + (rand() % (rc.right - rc.left));
			pt.y = rc.top + (rand() % (rc.top - rc.bottom));
			ClientToScreen(hwnd, &pt);
			SetCursorPos(pt.x, pt.y);
			break;

		case WM_DESTROY:
			KillTimer(hwnd, IDT_TIMER);
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}
