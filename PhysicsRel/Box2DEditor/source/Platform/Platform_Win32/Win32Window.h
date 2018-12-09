#ifdef _PLATFORM_WIN32

#pragma once
#include "Includes.h"
#include <string>
using namespace std;

class Win32Window
{
private:

public:
	HDC hDC1;
	static HWND hWnd1;
	static HINSTANCE hInstance;
	bool Quit;

	static int WINDOW_X;
	static int WINDOW_Y;
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static bool isScrolledUp;
	static bool isScrolledDown;

	static bool LEFT_MOUSE_DOWN;
	static bool RIGHT_MOUSE_DOWN;

	static bool isActive;

	Win32Window();
	~Win32Window();

	void CreateWin32Window(HINSTANCE hInstance, const char* className, int windowX, int windowY, int windowW, int windowH);
	void Swap_Buffers();									
	int GetScreenWidth();
	int GetScreenHeight();
	void SetWindowTitle(string title);
	void DeviceYield(int milliSeconds);	
	bool DeviceCheckQuitRequest();
	void DeviceExit();
	void Terminate();
};

#endif
