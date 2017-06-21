#ifdef _PLATFORM_WIN32

#include "Win32Window.h"
#include "Platform.h"

HWND Win32Window::hWnd1 = NULL;
HINSTANCE Win32Window::hInstance = NULL;

int Win32Window::WINDOW_X = 0;
int Win32Window::WINDOW_Y = 0;
int Win32Window::WINDOW_WIDTH = 0;
int Win32Window::WINDOW_HEIGHT = 0;

bool Win32Window::isScrolledUp = false;
bool Win32Window::isScrolledDown = false;

bool Win32Window::LEFT_MOUSE_DOWN = false;
bool Win32Window::RIGHT_MOUSE_DOWN = false;

bool Win32Window::isActive = true;

MSG	msg;

void SetupPixelFormat(HDC hDC)
{
	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd =
	{	
		sizeof(PIXELFORMATDESCRIPTOR),	// size
			1,							// version
			PFD_SUPPORT_OPENGL |		// OpenGL window
			PFD_DRAW_TO_WINDOW |		// render to window
			PFD_DOUBLEBUFFER,			// support double-buffering
			PFD_TYPE_RGBA,				// color type
			32,							// prefered color depth
			0, 0, 0, 0, 0, 0,			// color bits (ignored)
			0,							// no alpha buffer
			0,							// alpha bits (ignored)
			0,							// no accumulation buffer
			0, 0, 0, 0,					// accum bits (ignored)
			16,							// depth buffer
			0,							// no stencil buffer
			0,							// no auxiliary buffers
			PFD_MAIN_PLANE,				// main layer
			0,							// reserved
			0, 0, 0,					// no layer, visible, damage masks
	};

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hDC;
	static HGLRC hRC;
	float wheelVal;

	//WM_DESTROY
	//WM_QUIT

	if(uMsg == WM_CREATE)							// window creation
	{
		hDC = GetDC(hWnd);
		SetupPixelFormat(hDC);
		//SetupPalette();
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
	}
	else if(uMsg == WM_CLOSE)								// windows is closing
	{
		// deselect rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		// send WM_QUIT to message queue
		PostQuitMessage(0);
	}
	else if(uMsg == WM_SIZE)
	{
		Win32Window::WINDOW_WIDTH = LOWORD(lParam);
		Win32Window::WINDOW_HEIGHT = HIWORD(lParam);
	}
	else if(uMsg == WM_MOVE)
	{
		RECT rChild;
		GetWindowRect(hWnd, &rChild);
		Win32Window::WINDOW_X = rChild.left;
		Win32Window::WINDOW_Y = rChild.top;
	}
	else if(uMsg == WM_SYSCOMMAND)
	{
		int val = (wParam & 0xFFF0);
		if(val == SC_MINIMIZE)
			Win32Window::isActive = false;
		else if(val == 61728)
			Win32Window::isActive = true;		
	}
	else if(uMsg == WM_PAINT)								// paint
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	else if(uMsg == WM_LBUTTONDOWN)							// left mouse button
	{
		Win32Window::LEFT_MOUSE_DOWN = true;
	}
	else if(uMsg == WM_LBUTTONUP)							// left button release
	{
		Win32Window::LEFT_MOUSE_DOWN = false;
	}
	else if(uMsg == WM_RBUTTONDOWN)							// right mouse button
	{
		Win32Window::RIGHT_MOUSE_DOWN = true;
	}
	else if(uMsg == WM_RBUTTONUP)							// right button release
	{
		Win32Window::RIGHT_MOUSE_DOWN = false;
	}
	else if(uMsg == WM_MOUSEMOVE)							// mouse movement
	{
	}
	else if(uMsg == WM_MOUSEWHEEL)
	{
		wheelVal = GET_WHEEL_DELTA_WPARAM(wParam);

		if(wheelVal > 0)
			Win32Window::isScrolledUp = true;
		else if(wheelVal < 0)
			Win32Window::isScrolledDown = true;
	}
	else if(uMsg == WM_KEYUP)
	{
	}
	else if(uMsg == WM_KEYDOWN)
	{
		int fwKeys;
		LPARAM keyData;
		fwKeys = (int)wParam;		// virtual-key code 
		keyData = lParam;			// key data 

		//if(fwKeys == VK_ESCAPE)
		//	PostQuitMessage(0);
		//else 
			if(fwKeys == VK_BACK)
				Platform::GetPlatform()->SetBackKeyPressed();
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Win32Window::Win32Window()
{
	Quit = false;
}

Win32Window::~Win32Window()
{
}

void Win32Window::CreateWin32Window(HINSTANCE hInstance, const char* className, int windowX, int windowY, int windowW, int windowH)
{
	this->hInstance = hInstance;

	WNDCLASSEX windowClass;		// window class
	DWORD	   dwExStyle;		// Window Extended Style
	DWORD	   dwStyle;			// Window Style
	RECT	   windowRect;

	WINDOW_X = windowX;
	WINDOW_Y = windowY;
	WINDOW_WIDTH = windowW;
	WINDOW_HEIGHT = windowH;

	windowRect.left=(long)WINDOW_X;
	windowRect.right=(long)WINDOW_WIDTH;
	windowRect.top=(long)WINDOW_Y;
	windowRect.bottom=(long)WINDOW_HEIGHT;

	// fill out the window class structure
	windowClass.cbSize			= sizeof(WNDCLASSEX);
	windowClass.style			= CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc		= MainWindowProc;
	windowClass.cbClsExtra		= 0;
	windowClass.cbWndExtra		= 0;
	windowClass.hInstance		= hInstance;
	windowClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);	// default icon
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);		// default arrow
	windowClass.hbrBackground	= NULL;								// don't need background
	windowClass.lpszMenuName	= NULL;								// no menu
	windowClass.lpszClassName	= className;
	windowClass.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);		// windows logo small icon

	// register the windows class
	if (!RegisterClassEx(&windowClass))
		return;

	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;					// Windows Style

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// class registered, so now create our window
	hWnd1 = CreateWindowEx(NULL,									// extended style
		className,							// class name
		"My Window",						// app name ( as well as window title )
		dwStyle | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,								// x,y coordinate
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top, // width, height
		NULL,								// handle to parent
		NULL,								// handle to menu
		hInstance,							// application instance
		NULL);								// no extra params

	hDC1 = GetDC(hWnd1);

	if(!hWnd1)
		return;

	ShowWindow(hWnd1, SW_SHOW);			// display the window
	UpdateWindow(hWnd1);				// update the window
}


void Win32Window::Swap_Buffers()									
{
	SwapBuffers(hDC1);
}

int Win32Window::GetScreenWidth()
{
	return WINDOW_WIDTH;
}

int Win32Window::GetScreenHeight()
{
	return WINDOW_HEIGHT;
}

void Win32Window::SetWindowTitle(string title)
{
	SetWindowText( hWnd1, title.c_str());
}

void Win32Window::DeviceYield(int milliSeconds)	
{
	Sleep(milliSeconds);

	while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!GetMessage (&msg, NULL, 0, 0))
		{
			Quit = true;
			break;
		}

		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
}

bool Win32Window::DeviceCheckQuitRequest()
{
	return Quit;
}

void Win32Window::DeviceExit()
{
}

void Win32Window::Terminate()
{
}

#endif


/*
	// Creates textbox for input
	HWND hwnd_ed_1 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "Text here",
	WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
	100, 100, 100, 24,	// x, y, w, h
	Win32Window::hWnd1, NULL,
	Win32Window::hInstance, NULL);

	SetWindowText(hwnd_ed_1, "0.60");

	char szInput[MAX_PATH];
	GetWindowText(hwnd_ed_1, szInput, MAX_PATH);
*/