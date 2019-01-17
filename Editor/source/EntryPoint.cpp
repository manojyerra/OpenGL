#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "vld.h"
#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>

#include "AllDemos.h"
#include "Sui/Sui.h"
#include "Sui/SuiInput.h"
#include "AppInfo.h"
#include "IO/File/FileReader.h"

HWND hWnd = NULL;
HDC hDC = NULL;
HGLRC hRC = NULL;

AppInfo* appInfo = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	

	if(message == WM_DESTROY)
	{
		PostQuitMessage(0);
	}
	else if(message == WM_MOVE)
	{
		SuiInput::WIN_MOVE_X = LOWORD(lParam);
		SuiInput::WIN_MOVE_Y = HIWORD(lParam);
	}
	else if(message == WM_MOUSEWHEEL)
	{
		if(GET_WHEEL_DELTA_WPARAM(wParam) > 0)		SuiInput::SCROLL_STATE = SuiInput::SCROLL_UP;
		else if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)	SuiInput::SCROLL_STATE = SuiInput::SCROLL_DOWN;
	}
	else if(message == WM_LBUTTONDOWN)	{	SuiInput::LEFT_BUTTON_DOWN = true;		}
	else if(message == WM_LBUTTONUP)	{	SuiInput::LEFT_BUTTON_DOWN = false;		}
	else if(message == WM_RBUTTONDOWN)	{	SuiInput::RIGHT_BUTTON_DOWN = true;		}
	else if(message == WM_RBUTTONUP)	{	SuiInput::RIGHT_BUTTON_DOWN = false;	}
	else if(message == WM_MBUTTONUP)	{	SuiInput::MIDDLE_BUTTON_DOWN = false;	}
	else if(message == WM_MBUTTONDOWN)	{	SuiInput::MIDDLE_BUTTON_DOWN = true;	}
	else if(message == WM_MOUSEMOVE)	{	SuiInput::MOUSE_MOVE = true;			}
	else if(message == WM_SIZING)
	{
		RECT &rc = *(LPRECT) lParam;
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);
		rc = windowRect;
	}

    return DefWindowProc (hWnd, message, wParam, lParam);	// Handle any messages the switch statement didn't
}

void EnableVerticalSync(int enableVerticalSync)
{
    // WGL_EXT_swap_control.
    typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(GLint);

    static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
        reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(
        wglGetProcAddress("wglSwapIntervalEXT"));

    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(enableVerticalSync);
}

void SetInfoOnTitle(HWND hwnd)
{
	int lanchT = appInfo->_appLaunchEndTime - appInfo->_appLaunchStartTime;
	int texCompress = appInfo->_textureCompressionSupport;
	int s3tc = appInfo->_textureCompression_s3tc_Support;

	char arr[1024];
	
	sprintf(arr, "Editor, FPS : %d, LanchTime : %d millis, VBO : %d, texCompress = %d, s3tc = %d  ", 
				appInfo->fps, lanchT, appInfo->_vboSupport, texCompress, s3tc);

	SetWindowText(hwnd, arr);
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

bool ChangeScreenResolution (int width, int height, int bitsPerPixel)	// Change The Screen Resolution
{
	DEVMODE dmScreenSettings;											// Device Mode
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth		= width;						// Select Screen Width
	dmScreenSettings.dmPelsHeight		= height;						// Select Screen Height
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Select Bits Per Pixel
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if(ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		MessageBox (HWND_DESKTOP, "Failed In ChangeDisplaySetting!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
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

bool CreateWindowWithoutRC(HINSTANCE hInstance, AppInfo* appInfo)
{
	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		appInfo->colorBPP,												// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		appInfo->hasAlphaBuffer,										// Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		appInfo->depthBPP,												// Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style
	DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
	RECT windowRect = {0, 0, appInfo->currWindowW, appInfo->currWindowH};
	
	if(appInfo->isFullScreen)
	{
		windowStyle = WS_POPUP;

		if(appInfo->changeDisplay)
		{
			if(ChangeScreenResolution(appInfo->currWindowW, appInfo->currWindowH, appInfo->colorBPP) == false)
			{
				//Display change is failed, so switching back to window mode.
				appInfo->isFullScreen = false;
				windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
				AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
			}
		}
	}
	else
	{
		AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	}

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

bool DeleteAndCreateWindow(HINSTANCE hInstance, AppInfo* appInfo)
{
	if(hWnd)
	{
		//DeleteContext();
		DestroyWindow();
		if(appInfo->isFullScreen == false)
			ChangeDisplaySettings(NULL,0); // Switch Back To Desktop Resolution
	}

	if(appInfo->isFullScreen && appInfo->changeDisplay == false)
	{
		appInfo->currWindowW = appInfo->monitorW;
		appInfo->currWindowH = appInfo->monitorH;
	}
	else
	{
		appInfo->currWindowW = appInfo->baseWindowW;
		appInfo->currWindowH = appInfo->baseWindowH;
	}

	CreateWindowWithoutRC(hInstance, appInfo);

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
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//void* mem = malloc(1000);

	appInfo = AppInfo::GetInstance();
	appInfo->_appLaunchStartTime = GetTickCount();

	FileReader fr("data/WindowInfo.txt");

	//Window info...
	appInfo->colorBPP = fr.GetTagIntData("colorBPP");
	appInfo->depthBPP = fr.GetTagIntData("depthBPP");
	appInfo->hasAlphaBuffer = fr.GetTagIntData("hasAlphaBuffer");
	appInfo->baseWindowW = fr.GetTagIntData("baseWindowW");
	appInfo->baseWindowH = fr.GetTagIntData("baseWindowH");
	appInfo->currWindowW = appInfo->baseWindowW;
	appInfo->currWindowH = appInfo->baseWindowH;
	appInfo->monitorW = GetSystemMetrics(SM_CXSCREEN);
	appInfo->monitorH = GetSystemMetrics(SM_CYSCREEN);
	appInfo->isFullScreen = fr.GetTagIntData("isFullScreen");
	appInfo->changeDisplay = fr.GetTagIntData("changeDisplay");
	appInfo->maintainRatio = fr.GetTagIntData("maintainRatio");

	if(!RegisterWindowClass(hInstance, "WindowClass1"))
		return 0;

	DeleteAndCreateWindow(hInstance, appInfo);

    MSG msg;
	int fps = 0;
	unsigned long startTime = GetTickCount();

	SuiSetup(appInfo->baseWindowW, appInfo->baseWindowH);
	
	AllDemos* _allDemos = new AllDemos(appInfo->baseWindowW, appInfo->baseWindowH);
	SuiInput::Init();
	SuiManager::GetInstance();

    while(true)
    {
		bool justNowWindowDeleted = false;

		if(SuiInput::IsKeyReleased(VK_ESCAPE) && appInfo->isFullScreen)
		{
			appInfo->isFullScreen = false;
			DeleteAndCreateWindow(hInstance, appInfo);
			justNowWindowDeleted = true;
		}
		else if(SuiInput::IsKeyPressed(VK_MENU) && SuiInput::IsKeyReleased(VK_RETURN))
		{
			appInfo->isFullScreen = !appInfo->isFullScreen;
			DeleteAndCreateWindow(hInstance, appInfo);
			justNowWindowDeleted = true;
		}

        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		if(msg.message == WM_QUIT && justNowWindowDeleted == false)
            break;

		if(appInfo->isFullScreen && appInfo->changeDisplay == false)
		{
			float newWidth = appInfo->monitorW;
			float newHeight = appInfo->baseWindowH * newWidth/ appInfo->baseWindowW;

			if(newHeight > appInfo->monitorH)
			{
				newHeight = appInfo->monitorH;
				newWidth = appInfo->baseWindowW * newHeight/ appInfo->baseWindowH;
			}

			float startX = (appInfo->monitorW - newWidth) / 2.0f;
			float startY = (appInfo->monitorH - newHeight) / 2.0f;

			appInfo->viewX = startX;
			appInfo->viewY = startY;
			appInfo->viewW = newWidth;
			appInfo->viewH = newHeight;

			glViewport(appInfo->viewX,appInfo->viewY,appInfo->viewW,appInfo->viewH);
		}
		else
		{
			appInfo->viewX = 0;
			appInfo->viewY = 0;
			appInfo->viewW = appInfo->currWindowW;
			appInfo->viewH = appInfo->currWindowH;

			glViewport(0,0,appInfo->currWindowW, appInfo->currWindowH);
		}

		float deltaTime = 1.0f / (float)appInfo->fps;
		SuiInput::Update(deltaTime);

		if(SuiManager::GetInstance()->Contains(SuiInput::MX, SuiInput::MY))
			SuiInput::SetInputState(SuiInput::NO_INPUT);

		_allDemos->Update(deltaTime);
		_allDemos->Draw(deltaTime);

		SuiInput::SetInputState(SuiInput::ALLOW_INPUT);
		SuiRun();
		SuiInput::SCROLL_STATE = SuiInput::SCROLL_NONE;

		int errorID = glGetError();
		if(errorID != GL_NO_ERROR)
			printf("Error in GL %d", errorID);

		if(SwapBuffers(hDC) == FALSE)
			printf("Error in SwapBuffers...");

		if(appInfo->_appLanched == false)
		{
			appInfo->_appLaunchEndTime = GetTickCount();
			appInfo->_appLanched = true;
		}

		fps++;
		if(GetTickCount() - startTime >= 500)
		{
			appInfo->fps = fps*2;
			SetInfoOnTitle(hWnd);
			startTime = GetTickCount();
			fps = 0;
		}
    }

	if(_allDemos)
	{
		delete _allDemos;
		_allDemos = NULL;
	}

	SuiQuit();
	DeleteContext();
	DestroyWindow();

    return msg.wParam;
}



/*

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library

//#include "vld.h"
#include "AllDemos.h"
#include "Sui/Sui.h"
#include "Sui/SuiInput.h"
#include "AppInfo.h"

typedef struct {									// Structure For Keyboard Stuff
	BOOL keyDown [256];								// Holds TRUE / FALSE For Each Key
} Keys;												// Keys

typedef struct {									// Contains Information Vital To Applications
	HINSTANCE		hInstance;						// Application Instance
	const char*		className;						// Application ClassName
} Application;										// Application

typedef struct {									// Window Creation Info
	Application*		application;				// Application Structure
	char*				title;						// Window Title
	int					width;						// Width
	int					height;						// Height
	int					bitsPerPixel;				// Bits Per Pixel
	BOOL				isFullScreen;				// FullScreen?
} GL_WindowInit;									// GL_WindowInit

typedef struct {									// Contains Information Vital To A Window
	Keys*				keys;						// Key Structure
	HWND				hWnd;						// Window Handle
	HDC					hDC;						// Device Context
	HGLRC				hRC;						// Rendering Context
	GL_WindowInit		init;						// Window Init
	BOOL				isVisible;					// Window Visible?
	DWORD				lastTickCount;				// Tick Counter
} GL_Window;										// GL_Window



#define WM_TOGGLEFULLSCREEN (WM_USER+1)									// Application Define Message For Toggling

static BOOL g_isProgramLooping;											// Window Creation Loop, For FullScreen/Windowed Toggle																		// Between Fullscreen / Windowed Mode
static BOOL g_createFullScreen;											// If TRUE, Then Create Fullscreen

void TerminateApplication (GL_Window* window)							// Terminate The Application
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
	g_isProgramLooping = FALSE;											// Stop Looping Of The Program
}

void ToggleFullscreen (GL_Window* window)								// Toggle Fullscreen/Windowed
{
	PostMessage (window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				// Send A WM_TOGGLEFULLSCREEN Message
}

void ReshapeGL (int width, int height)									// Reshape The Window When It's Moved Or Resized
{
	AppInfo::GetInstance()->windowW = width;
	AppInfo::GetInstance()->windowH = height;
}

BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel)	// Change The Screen Resolution
{
	DEVMODE dmScreenSettings;											// Device Mode
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth		= width;						// Select Screen Width
	dmScreenSettings.dmPelsHeight		= height;						// Select Screen Height
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Select Bits Per Pixel
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;													// Display Change Failed, Return False
	}
	return TRUE;														// Display Change Was Successful, Return True
}

BOOL CreateWindowGL (GL_Window* window)									// This Code Creates Our OpenGL Window
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style

	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		window->init.bitsPerPixel,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		1,																// Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		16,																// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	RECT windowRect = {0, 0, window->init.width, window->init.height};	// Define Our Window Coordinates

	if(ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel))
	{
		ShowCursor (TRUE);											// Turn Off The Cursor
		windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
		windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
	}
	else															// Otherwise (If Fullscreen Mode Was Successful)
	{
		MessageBox (HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
		window->init.isFullScreen = FALSE;							// Set isFullscreen To False (Windowed Mode)
	}																// (Top Window Covering Everything Else)

	window->hWnd = CreateWindowEx (windowExtendedStyle,					// Extended Style
								   window->init.application->className,	// Class Name
								   window->init.title,					// Window Title
								   windowStyle,							// Window Style
								   0, 0,								// Window X,Y Position
								   windowRect.right - windowRect.left,	// Window Width
								   windowRect.bottom - windowRect.top,	// Window Height
								   HWND_DESKTOP,						// Desktop Is Window's Parent
								   0,									// No Menu
								   window->init.application->hInstance, // Pass The Window Instance
								   window);

	GLuint PixelFormat = 0;
	window->hDC = GetDC (window->hWnd);
	PixelFormat=ChoosePixelFormat(window->hDC,&pfd);
	SetPixelFormat (window->hDC, PixelFormat, &pfd);
	window->hRC = wglCreateContext (window->hDC);
	wglMakeCurrent (window->hDC, window->hRC);
	ShowWindow (window->hWnd, SW_NORMAL);
	return TRUE;
}

BOOL DestroyWindowGL (GL_Window* window)								// Destroy The OpenGL Window & Release Resources
{
	if (window->hWnd != 0)												// Does The Window Have A Handle?
	{	
		if (window->hDC != 0)											// Does The Window Have A Device Context?
		{
			wglMakeCurrent (window->hDC, 0);							// Set The Current Active Rendering Context To Zero
			if (window->hRC != 0)										// Does The Window Have A Rendering Context?
			{
				wglDeleteContext (window->hRC);							// Release The Rendering Context
				window->hRC = 0;										// Zero The Rendering Context

			}
			ReleaseDC (window->hWnd, window->hDC);						// Release The Device Context
			window->hDC = 0;											// Zero The Device Context
		}
		DestroyWindow (window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
	}

	if (window->init.isFullScreen)										// Is Window In Fullscreen Mode
	{
		ChangeDisplaySettings (NULL,0);									// Switch Back To Desktop Resolution
		ShowCursor (TRUE);												// Show The Cursor
	}	
	return TRUE;														// Return True
}

// Process Window Message Callbacks
LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Get The Window Context
	GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));

	float wheelVal = 0;

	switch (uMsg)														// Evaluate Window Message
	{
		case WM_SYSCOMMAND:												// Intercept System Commands
		{
			switch (wParam)												// Check System Calls
			{
				case SC_SCREENSAVE:										// Screensaver Trying To Start?
				case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
				return 0;												// Prevent From Happening
			}
			break;														// Exit
		}
		return 0;														// Return


		case WM_CREATE:													// Window Creation
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// Store Window Structure Pointer
			window = (GL_Window*)(creation->lpCreateParams);
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;														// Return

		case WM_CLOSE:													// Closing The Window
			TerminateApplication(window);								// Terminate The Application
		return 0;														// Return															// Break

		case WM_MOVE:
			{
				SuiInput::WIN_MOVE_X = LOWORD(lParam);
				SuiInput::WIN_MOVE_Y = HIWORD(lParam);		// retrieve width and height
			}
			break;
		case WM_LBUTTONDOWN:
			SuiInput::LEFT_BUTTON_DOWN = true;
			break;
		case WM_LBUTTONUP:
			SuiInput::LEFT_BUTTON_DOWN = false;
			break;

		case WM_RBUTTONDOWN:
			SuiInput::RIGHT_BUTTON_DOWN = true;
			break;
		case WM_RBUTTONUP:
			SuiInput::RIGHT_BUTTON_DOWN = false;
			break;

		case WM_MBUTTONUP:
			SuiInput::MIDDLE_BUTTON_DOWN = false;
			break;
		case WM_MBUTTONDOWN:
			SuiInput::MIDDLE_BUTTON_DOWN = true;
			break;

		case WM_MOUSEMOVE:
			SuiInput::MOUSE_MOVE = true;
			break;

		case WM_MOUSEWHEEL:
			{
			wheelVal = GET_WHEEL_DELTA_WPARAM(wParam);

			if(wheelVal > 0)
				SuiInput::SCROLL_STATE = SuiInput::SCROLL_UP;
			else if(wheelVal < 0)
				SuiInput::SCROLL_STATE = SuiInput::SCROLL_DOWN;
			}
			break;

		case WM_KEYDOWN:												// Update Keyboard Buffers For Keys Pressed
			if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
			{
				window->keys->keyDown [wParam] = TRUE;					// Set The Selected Key (wParam) To True
				return 0;												// Return
			}
			break;															// Break

		case WM_KEYUP:													// Update Keyboard Buffers For Keys Released
			if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
			{
				window->keys->keyDown[wParam] = FALSE;					// Set The Selected Key (wParam) To False
				return 0;												// Return
			}
			break;															// Break
	}

	return DefWindowProc (hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
}

BOOL RegisterWindowClass (Application* application)						// Register A Window Class For This Application.
{																		// TRUE If Successful
	// Register A Window Class
	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// Make Sure Memory Is Cleared
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// WindowProc Handles Messages
	windowClass.hInstance		= application->hInstance;				// Set The Instance
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// Class Background Brush Color
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName	= application->className;				// Sets The Applications Classname
	if (RegisterClassEx (&windowClass) == 0)							// Did Registering The Class Fail?
	{
		// NOTE: Failure, Should Never Happen
		MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// Return False (Failure)
	}
	return TRUE;														// Return True (Success)
}

void CheckFullScreen(GL_Window* window)
{
	if(SuiInput::IsKeyReleased(VK_ESCAPE))
	{
		if(g_createFullScreen)
		{
			g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
			PostMessage(window->hWnd, WM_QUIT, 0, 0);
		}
	}
	else if(SuiInput::IsKeyPressed(VK_MENU) && SuiInput::IsKeyReleased(VK_RETURN))
	{
		g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
		PostMessage(window->hWnd, WM_QUIT, 0, 0);
	}
}


void SetFPS(HWND hwnd, int fps)
{
	unsigned char uniArr[128];
	memset(uniArr, 0, 128);

	char arr[64];
	sprintf(arr, "Editor, FPS : %d", fps);

	for(int i=0;i<16;i++)
		uniArr[2*i] = arr[i];

	SetWindowTextW(hwnd, (const WCHAR*)uniArr);
}

// Program Entry (WinMain)
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application			application;									// Application Structure
	GL_Window			window;											// Window Structure
	Keys				keys;											// Key Structure
	BOOL				isMessagePumpActive;							// Message Pump Active?
	MSG					msg;											// Window Message Structure
	DWORD				tickCount;										// Used For The Tick Counter

	// Fill Out Application Data
	application.className = "OpenGL";									// Application Class Name
	application.hInstance = hInstance;									// Application Instance

	// Fill Out Window
	ZeroMemory (&window, sizeof (GL_Window));							// Make Sure Memory Is Zeroed
	window.keys					= &keys;								// Window Key Structure
	window.init.application		= &application;							// Window Application
	window.init.title			= "Lesson 46: NeHe & MainRoach's FSAA Tutorial)";	// Window Title
	window.init.width			= 800;									// Window Width
	window.init.height			= 600;									// Window Height
	window.init.bitsPerPixel	= 32;									// Bits Per Pixel
	window.init.isFullScreen	= TRUE;									// Fullscreen? (Set To TRUE)

	ZeroMemory (&keys, sizeof (Keys));									// Zero keys Structure

	// Register A Class For Our Window To Use
	if (RegisterWindowClass (&application) == FALSE)					// Did Registering A Class Fail?
	{
		// Failure
		MessageBox (HWND_DESKTOP, "Error Registering Window Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// Terminate Application
	}

	g_isProgramLooping = TRUE;											// Program Looping Is Set To TRUE
	g_createFullScreen = window.init.isFullScreen;						// g_createFullScreen Is Set To User Default
	
	while (g_isProgramLooping)											// Loop Until WM_QUIT Is Received
	{
		// Create A Window
		window.init.isFullScreen = g_createFullScreen;					// Set Init Param Of Window Creation To Fullscreen?
		if (CreateWindowGL (&window) == TRUE)							// Was Window Creation Successful?
		{
			AppInfo* appInfo = AppInfo::GetInstance();
			AppInfo::GetInstance()->windowW = window.init.width;
			AppInfo::GetInstance()->windowH = window.init.height;

			SuiSetup(window.init.width, window.init.height, window.init.width, window.init.height);
			
			AllDemos* _allDemos = new AllDemos(window.init.width, window.init.height);
			SuiInput::Init();
			SuiManager::GetInstance();

			int fps = 0;
			float timeCount = 0;

			isMessagePumpActive = TRUE;								// Set isMessagePumpActive To TRUE
			while (isMessagePumpActive == TRUE)						// While The Message Pump Is Active
			{
				unsigned int startTime = GetTickCount();

				// Success Creating Window.  Check For Window Messages
				if (PeekMessage (&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
				{
					// Check For WM_QUIT Message
					if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
						DispatchMessage (&msg);						// If Not, Dispatch The Message
					else											// Otherwise (If Message Is WM_QUIT)
						isMessagePumpActive = FALSE;				// Terminate The Message Pump
				}
				else												// If There Are No Messages
				{
					if (window.isVisible == FALSE)					// If Window Is Not Visible
					{
						WaitMessage ();								// Application Is Minimized Wait For A Message
					}
					else											// If Window Is Visible
					{
						float deltaTime = 1.0f / AppInfo::GetInstance()->fps;

						SuiInput::Update(deltaTime);
						CheckFullScreen(&window);
						SetFPS(window.hWnd, AppInfo::GetInstance()->fps);

						if(SuiManager::GetInstance()->Contains(SuiInput::MX, SuiInput::MY))
							SuiInput::SetInputState(SuiInput::NO_INPUT);

						_allDemos->Update(deltaTime);
						_allDemos->Draw(deltaTime);

						SuiInput::SetInputState(SuiInput::ALLOW_INPUT);

						SuiRun();

						if(AppInfo::GetInstance()->_appLanched == false)
						{
							AppInfo::GetInstance()->_appLaunchEndTime = GetTickCount();
							AppInfo::GetInstance()->_appLanched = true;
						}

						SuiInput::SCROLL_STATE = SuiInput::SCROLL_NONE;

						fps++;

						timeCount += GetTickCount() - startTime;
						if(timeCount >= 1000)
						{
							AppInfo::GetInstance()->fps = fps;
							timeCount = 0;
							fps = 0;
						}

						SwapBuffers (window.hDC);					// Swap Buffers (Double Buffering)
					}
				}
			}														// Loop While isMessagePumpActive == TRUE

			if(_allDemos)
				delete _allDemos;

			SuiQuit();

			DestroyWindowGL (&window);									// Destroy The Active Window
		}
		else															// If Window Creation Failed
		{
			// Error Creating Window
			MessageBox (HWND_DESKTOP, "Error Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									// Terminate The Loop
		}
	}																	// While (isProgramLooping)

	UnregisterClass (application.className, application.hInstance);		// UnRegister Window Class
	return 0;
}																		// End Of WinMain()
*/