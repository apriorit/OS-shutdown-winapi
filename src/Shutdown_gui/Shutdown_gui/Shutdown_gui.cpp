#include <windows.h>
#include <tchar.h>
#include "FileLog.h"
#include <string>

std::wstring g_className(L"ShutdownWin");
FileLog g_logger;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_POWERBROADCAST:
		{
			if (wParam == PBT_APMSUSPEND)
				g_logger.WriteToLog("Computer is suspending");
			break;
		}
	case WM_QUERYENDSESSION:
		{
			if (lParam == 0)
			{
				g_logger.WriteToLog("Computer is shutting down");
				ShutdownBlockReasonCreate(hwnd, _T("Please, don't kill me"));
			}
			if ((lParam & ENDSESSION_LOGOFF) == ENDSESSION_LOGOFF)
			{
				g_logger.WriteToLog("User is logging off");
			}
			break;
		}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	if (!g_logger.Open("test.txt"))
	{
		MessageBox(NULL, L"Cannot open log file", L"Error",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	g_logger.WriteToLog("--------------------------------------");

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	DWORD dwLevel = 0;
	DWORD dwFlags = 0;
	if (GetProcessShutdownParameters(&dwLevel, &dwFlags))
		SetProcessShutdownParameters(dwLevel, SHUTDOWN_NORETRY);

	//Step 1: Registering the Window Class
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_className.c_str();
	wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed", L"Error",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_className.c_str(),
		L"Shutdown GUI test",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed", L"Error",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}