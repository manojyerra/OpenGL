#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "DefinesAndIncludes.h"
#include "Looper.h"
#include "Input.h"

HWND hWnd = NULL;
HDC hDC = NULL;
HGLRC hRC = NULL;

int colorBPP = 32;
int depthBPP = 24;
int hasAlphaBuffer = 0;
int windowW = 1300;
int windowH = 600;


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if(message == WM_DESTROY)
	{
		PostQuitMessage(0);
	}
	else if(message == WM_MOVE)
	{
		Input::WIN_MOVE_X = LOWORD(lParam);
		Input::WIN_MOVE_Y = HIWORD(lParam);
	}
	else if(message == WM_MOUSEWHEEL)
	{
		if(GET_WHEEL_DELTA_WPARAM(wParam) > 0)		Input::SCROLL_STATE = Input::SCROLL_UP;
		else if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)	Input::SCROLL_STATE = Input::SCROLL_DOWN;
	}
	else if(message == WM_LBUTTONDOWN)	{	Input::LEFT_BUTTON_DOWN = true;		}
	else if(message == WM_LBUTTONUP)	{	Input::LEFT_BUTTON_DOWN = false;		}
	else if(message == WM_RBUTTONDOWN)	{	Input::RIGHT_BUTTON_DOWN = true;		}
	else if(message == WM_RBUTTONUP)	{	Input::RIGHT_BUTTON_DOWN = false;	}
	else if(message == WM_MBUTTONUP)	{	Input::MIDDLE_BUTTON_DOWN = false;	}
	else if(message == WM_MBUTTONDOWN)	{	Input::MIDDLE_BUTTON_DOWN = true;	}
	else if(message == WM_MOUSEMOVE)	{	Input::MOUSE_MOVE = true;			}
	else if(message == WM_SIZING)
	{
		RECT &rc = *(LPRECT) lParam;
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);
		rc = windowRect;
	}

    return DefWindowProc (hWnd, message, wParam, lParam);	// Handle any messages the switch statement didn't
}

void ShowFPSOnWindowTitleBar(int fps)
{
	char arr[1024];
	sprintf(arr, "FPS : %d", fps);
	SetWindowText(hWnd, arr);
}

void DestroyWindow()
{
	if(hWnd != 0)											// Does The Window Have A Handle?
	{	
		if(hDC != 0)										// Does The Window Have A Device Context?
		{
			ReleaseDC(hWnd, hDC);							// Release The Device Context
			hDC = 0;										// Zero The Device Context
		}
		DestroyWindow(hWnd);								// Destroy The Window
		hWnd = 0;											// Zero The Window Handle
	}
}

void DeleteContext()
{
	if(hWnd != 0)											// Does The Window Have A Handle?
	{	
		if(hDC != 0)										// Does The Window Have A Device Context?
		{
			wglMakeCurrent(hDC, 0);							// Set The Current Active Rendering Context To Zero
			if(hRC != 0)									// Does The Window Have A Rendering Context?
			{
				wglDeleteContext(hRC);						// Release The Rendering Context
				hRC = 0;									// Zero The Rendering Context
			}
		}
	}
}

bool RegisterWindowClass(HINSTANCE hInstance, const char* className)						// Register A Window Class For This Application.
{																		// TRUE If Successful

	WNDCLASSEX wc;	// this struct holds information for the window class
	ZeroMemory(&wc, sizeof(WNDCLASSEX));		// clear out the window class for use

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = className;

	if (RegisterClassEx (&wc) == 0)							// Did Registering The Class Fail?
	{
		MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;													// Return False (Failure)
	}

	return true;
}

bool CreateWindowWithoutRC(HINSTANCE hInstance)
{
	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		colorBPP,														// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		hasAlphaBuffer,													// Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		depthBPP,														// Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style
	DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
	RECT windowRect = {0, 0, windowW, windowH};
	
	AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);

	hWnd = CreateWindowEx (windowExtendedStyle,									// Extended Style
								   (const char*)"WindowClass1",					// Class Name
								   (const char*)"Title here...",				// Window Title
								   windowStyle,									// Window Style
								   0, 0,										// Window X,Y Position
								   windowRect.right - windowRect.left,			// Window Width
								   windowRect.bottom - windowRect.top,			// Window Height
								   NULL,										// Desktop Is Window's Parent
								   0,											// No Menu
								   hInstance,									// Pass The Window Instance
								   NULL);

	int pixelFormat = 0;

	if(hWnd == 0)										{ DestroyWindow(); return false; }
	if((hDC = GetDC(hWnd)) == 0)						{ DestroyWindow(); return false; }
	if((pixelFormat = ChoosePixelFormat(hDC,&pfd)) == 0){ DestroyWindow(); return false; }
	if(SetPixelFormat(hDC, pixelFormat, &pfd) == 0)		{ DestroyWindow(); return false; }

	return true;
}

bool SetupWindow(HINSTANCE hInstance)
{
	CreateWindowWithoutRC(hInstance);

	if(hRC == NULL || (hRC && wglMakeCurrent(hDC, hRC) == FALSE))
	{
		DeleteContext();

		if((hRC = wglCreateContext(hDC)) == 0)				{ DestroyWindow(); return FALSE; }
		if(wglMakeCurrent(hDC, hRC) == FALSE)				{ DestroyWindow(); return FALSE; }
	}

	ShowWindow(hWnd, SW_NORMAL);
	SetForegroundWindow(hWnd);                      // Slightly Higher Priority
	SetFocus(hWnd);                                 // Sets Keyboard Focus To The Window
	UpdateWindow(hWnd);

	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(!RegisterWindowClass(hInstance, "WindowClass1"))
		return 0;

	SetupWindow(hInstance);

    MSG msg;
	int framesCount = 0;
	float deltaTime = 0;
	unsigned long prevTickCount = GetTickCount();
	unsigned long startTime = GetTickCount();
	
	Looper* looper = new Looper(windowW, windowH);
	
    while(true)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		if(msg.message == WM_QUIT)
            break;

		long tickCount = GetTickCount();
		float deltaTime = (tickCount-prevTickCount) == 0 ? 1.0f/1000.0f : (float)(tickCount-prevTickCount) / 1000.0f;
		prevTickCount = tickCount;

		Input::Update(deltaTime);

		looper->Draw(deltaTime);

		int errorID = glGetError();
		if(errorID != GL_NO_ERROR)
			writeConsole("Error in GL %d", errorID);

		if(SwapBuffers(hDC) == FALSE)
			writeConsole("Error in SwapBuffers...");

		if(GetTickCount() - startTime >= 500)
		{
			ShowFPSOnWindowTitleBar(framesCount * 2);
			framesCount = 0;
			startTime = GetTickCount();
		}

		framesCount++;

		Sleep(30);
    }

	if(looper)
	{
		delete looper;
		looper = NULL;
	}

	DeleteContext();
	DestroyWindow();

    return msg.wParam;
}
