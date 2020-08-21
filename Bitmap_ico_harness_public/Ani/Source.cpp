/*
Fuzzing harness to fuzz Bitmap ico file using winafl
Author: hardik shah
Email: hardik05@gmail.com
web: http://hardik05.wordpress.com
*/

#include <Windows.h>
#include <stdio.h>

void* hWnd;
bool  visible = true;
bool  running = true;
LRESULT CALLBACK WndProcInternal(HWND hWnd, UINT uMsg, WPARAM    wParam, LPARAM  lParam);
HCURSOR cursor = 0;

int FuzzMe(LPCWSTR filename)
{

	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, filename,IMAGE_BITMAP,0, 0,	LR_LOADFROMFILE);
	BITMAP bmp;
	HICON m_hCursor;

	::GetObject(hBitmap, sizeof(BITMAP), &bmp);

	HBITMAP hMask = ::CreateCompatibleBitmap(::GetDC(NULL),
		bmp.bmWidth, bmp.bmHeight);


	ICONINFO ii = { 0 };
	ii.fIcon = FALSE;
	ii.hbmColor = hBitmap;
	ii.hbmMask = hMask;
	ii.xHotspot = 0;
	ii.yHotspot = 0;

	m_hCursor = ::CreateIconIndirect(&ii);
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s <path to ani file>\n", argv[0]);
		return 1;
	}
	
	//Setup configuration
	const int Width = 640, Height = 480;
	const int Method = 1;

	//Setup window class 
	WNDCLASS wcd;
	wcd.style = CS_PARENTDC | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcd.lpfnWndProc = (WNDPROC)WndProcInternal;
	wcd.cbClsExtra = 0;
	wcd.cbWndExtra = 0;
	wcd.hInstance = GetModuleHandle(NULL);
	wcd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcd.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wcd.lpszMenuName = NULL;
	wcd.lpszClassName = TEXT("AnimatedIcon");

	//Register the window class
	if (!RegisterClass(&wcd))
	{
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		FatalExit(-1);
	}

	//Create a window
	if (!(hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("AnimatedIcon"), TEXT("AnimatedIcon"),
		WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU,
		0, 0, Width, Height, NULL, NULL, NULL, NULL)))
	{
		MessageBoxA(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		FatalExit(-1);
	}

	LPWSTR *szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	if (szArgList == NULL)
	{
		return 1;
	}
	// InitializeGdiPlus

	FuzzMe(szArgList[1]);

	LocalFree(szArgList);
	
	while (running)
	{
		MSG wmsg;
		if (PeekMessage(&wmsg, (HWND)hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&wmsg);
			DispatchMessage(&wmsg);
		}
	}
	return 0;
}

LRESULT CALLBACK WndProcInternal(HWND hWnd, UINT uMsg, WPARAM    wParam, LPARAM  lParam)
{
	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(1);
		running = false;
	}

	return (long)DefWindowProc((HWND)hWnd, uMsg, (WPARAM)wParam, (LPARAM)lParam);
}