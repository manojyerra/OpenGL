#ifdef _PLATFORM_WIN32

//#define ENABLE_VLD
#ifdef ENABLE_VLD
#include "vld.h"
#endif

#include "Platform.h"
#include "Includes.h"
#include "Win32Window.h"

#define WIN_SCREEN_W 1024
#define WIN_SCREEN_H 600

Win32Window* win32Window = NULL;

void UpdateMouseButtons();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	#ifdef ENABLE_VLD
	VLDEnable();
	#endif

	win32Window = new Win32Window();
	win32Window->CreateWin32Window(hInstance, "classFirst", 0,0,WIN_SCREEN_W,WIN_SCREEN_H);

	Platform::GetPlatform()->onCreate();
	Platform::GetPlatform()->onSizeChange(WIN_SCREEN_W, WIN_SCREEN_H);
	Platform::GetPlatform()->SetDataFolderPath("../../assets");
	Platform::GetPlatform()->SetSaveFolderPath("./");

	int startTime = GetTickCount();
	bool isActive = true;

	while(!win32Window->DeviceCheckQuitRequest())
	{
		win32Window->DeviceYield(20);

		UpdateMouseButtons();

		int deltaTimeInMillis = (int)(GetTickCount() - startTime);
    	startTime = GetTickCount();

		if(isActive != Win32Window::isActive)
		{
			isActive = Win32Window::isActive;

			if(isActive)	Platform::GetPlatform()->onResume();
			else			Platform::GetPlatform()->onPause();
		}

		Platform::GetPlatform()->Draw(deltaTimeInMillis);
		win32Window->Swap_Buffers();
	}

	Platform::GetPlatform()->onExit();
	Platform::GetPlatform()->DeletePlatform();

	if(win32Window)
	{
		delete win32Window;
		win32Window = NULL;
	}

	#ifdef ENABLE_VLD
	VLDReportLeaks();
	_CrtDumpMemoryLeaks();
	#endif

	return 0;
}


bool prevLeftMouseDown = false;
bool prevRightMouseDown = false;


void UpdateMouseButtons()
{
	POINT pos;
	GetCursorPos(&pos);

	float mx = pos.x - 8 - win32Window->WINDOW_X;
	float my = pos.y - 30 - win32Window->WINDOW_Y;


	// Begin : LeftMouse
	if (prevLeftMouseDown != win32Window->LEFT_MOUSE_DOWN || win32Window->LEFT_MOUSE_DOWN)
	{
		if (win32Window->LEFT_MOUSE_DOWN)
			Platform::GetPlatform()->onTouch(mx, my, 1);
		else
			Platform::GetPlatform()->onTouch(mx, my, 2);
	}

	prevLeftMouseDown = win32Window->LEFT_MOUSE_DOWN;
	// End : LeftMouse


	// Begin : RightMouse
	if (prevRightMouseDown != win32Window->RIGHT_MOUSE_DOWN || win32Window->RIGHT_MOUSE_DOWN)
	{
		if (win32Window->RIGHT_MOUSE_DOWN)
			Platform::GetPlatform()->onTouch(mx, my, 3);
		else
			Platform::GetPlatform()->onTouch(mx, my, 4);
	}

	prevRightMouseDown = win32Window->RIGHT_MOUSE_DOWN;
	// End : RightMouse
}

#endif
