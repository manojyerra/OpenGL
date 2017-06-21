#include "Platform.h"


#ifdef  PLATFORM_WIN32

MSG	msg;
int WINDOW_WIDTH = 1200;
int WINDOW_HEIGHT = 600;

int WINDOW_X = 0;
int WINDOW_Y = 0;

HINSTANCE Platform::hInstance = 0;
HWND Platform::hWnd = 0;
HDC Platform::hDC = 0;
bool Platform::_quit = false;

int Platform::MousePosX = 0;
int Platform::MousePosY = 0;

int Platform::PrevMousePosX = 0;
int Platform::PrevMousePosY = 0;

bool Platform::PREV_LEFT_BUTTON_DOWN = false;
bool Platform::LEFT_BUTTON_DOWN = false;

bool Platform::PREV_RIGHT_BUTTON_DOWN = false;
bool Platform::RIGHT_BUTTON_DOWN = false;

bool Platform::isMouseClicked = false;
bool Platform::isMousePressed = false;
bool Platform::isMouseReleased = false;

bool Platform::isRightMouseClicked = false;
bool Platform::isRightMousePressed = false;
bool Platform::isRightMouseReleased = false;

int Platform::currKeyStates[] = {0};
int Platform::prevKeyStates[] = {0};
float Platform::timeCountForKeyPress[] = {0};


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
	//int height, width;

	// dispatch messages
	switch (uMsg)
	{	
	case WM_CREATE:			// window creation
		hDC = GetDC(hWnd);
		SetupPixelFormat(hDC);
		//SetupPalette();
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		break;

	case WM_DESTROY:			// window destroy
	case WM_QUIT:
	case WM_CLOSE:					// windows is closing
		// deselect rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		// send WM_QUIT to message queue
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		WINDOW_WIDTH = LOWORD(lParam);
		WINDOW_HEIGHT = HIWORD(lParam);
		break;

	case WM_MOVE:
		RECT rChild;
		GetWindowRect(hWnd, &rChild);
		WINDOW_X = rChild.left;
		WINDOW_Y = rChild.top;
		break;


	case WM_ACTIVATEAPP:		// activate app
		break;

	case WM_PAINT:				// paint
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:		// left mouse button
		Platform::LEFT_BUTTON_DOWN = true;
		break;
	case WM_RBUTTONDOWN:		// right mouse button
		Platform::RIGHT_BUTTON_DOWN = true;
		break;
	case WM_MOUSEMOVE:			// mouse movement
		break;
	case WM_LBUTTONUP:			// left button release
		Platform::LEFT_BUTTON_DOWN = false;
		break;
	case WM_RBUTTONUP:			// right button release
		Platform::RIGHT_BUTTON_DOWN = false;
		break;
	case WM_KEYUP:
		break;
	case WM_KEYDOWN:
		int fwKeys;
		LPARAM keyData;
		fwKeys = (int)wParam;    // virtual-key code 
		keyData = lParam;          // key data 

		switch(fwKeys)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void Platform::Init()
{
	WNDCLASSEX windowClass;		// window class
	//HWND	   hwnd;			// window handle
	//MSG		   msg;				// message
	DWORD	   dwExStyle;		// Window Extended Style
	DWORD	   dwStyle;			// Window Style
	RECT	   windowRect;

	windowRect.left=(long)0;
	windowRect.right=(long)WINDOW_WIDTH;
	windowRect.top=(long)0;
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
	windowClass.lpszClassName	= "GLClass";
	windowClass.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);		// windows logo small icon

	// register the windows class
	if (!RegisterClassEx(&windowClass))
		return;

	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;					// Windows Style

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// class registered, so now create our window
	hWnd = CreateWindowEx(NULL,									// extended style
		"GLClass",							// class name
		"My Window",						// app name
		dwStyle | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,								// x,y coordinate
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top, // width, height
		NULL,								// handle to parent
		NULL,								// handle to menu
		hInstance,							// application instance
		NULL);								// no extra params

	hDC = GetDC(hWnd);

	if(!hWnd)
		return;

	ShowWindow(hWnd, SW_SHOW);			// display the window
	UpdateWindow(hWnd);					// update the window

	for(int i=0;i<256;i++) currKeyStates[i] = 0;
	for(int i=0;i<256;i++) prevKeyStates[i] = 0;
	for(int i=0;i<256;i++) timeCountForKeyPress[i] = 0;
}

void Platform::Swap_Buffers()									
{
	SwapBuffers(hDC);
}

unsigned long Platform::TimerGetMs()							{	return GetTickCount();	}
int Platform::GetWindowWidth()									{	return WINDOW_WIDTH;	}
int Platform::GetWindowHeight()									{	return WINDOW_HEIGHT;	}
int Platform::SecureStorageGet(void* data, unsigned int size)	{	return RESULT_ERROR;	}
int Platform::SecureStoragePut(void* data, unsigned int size)	{	return RESULT_ERROR;	}
int Platform::DebugAssertShow(const char* string)
{
	MessageBox(NULL, string, NULL, MB_OK);
	return CONTINUE;
}

//**************************** Start of Mouse Input ********************************
void Platform::UpdateMouse()
{
	isMouseClicked = false;
	isMousePressed = false;
	isMouseReleased = false;

	isRightMouseClicked = false;
	isRightMousePressed = false;
	isRightMouseReleased = false;

	if(PREV_LEFT_BUTTON_DOWN == false && LEFT_BUTTON_DOWN == true)		
		isMouseClicked = true;
	else if(PREV_LEFT_BUTTON_DOWN == true && LEFT_BUTTON_DOWN == true)
		isMousePressed = true;
	else if(PREV_LEFT_BUTTON_DOWN == true && LEFT_BUTTON_DOWN == false)
		isMouseReleased = true;

	PREV_LEFT_BUTTON_DOWN = LEFT_BUTTON_DOWN;


	if(PREV_RIGHT_BUTTON_DOWN == false && RIGHT_BUTTON_DOWN == true)		
		isRightMouseClicked = true;
	else if(PREV_RIGHT_BUTTON_DOWN == true && RIGHT_BUTTON_DOWN == true)
		isRightMousePressed = true;
	else if(PREV_RIGHT_BUTTON_DOWN == true && RIGHT_BUTTON_DOWN == false)
		isRightMouseReleased = true;

	PREV_RIGHT_BUTTON_DOWN = RIGHT_BUTTON_DOWN;


	POINT pos;
	GetCursorPos(&pos);

	PrevMousePosX = MouseX();
	PrevMousePosY = MouseY();

	MousePosX = pos.x-9;
	MousePosY = pos.y-30;
}

int Platform::MouseX()			{return MousePosX-WINDOW_X;}
int Platform::MouseY()			{return MousePosY-WINDOW_Y;}

bool Platform::IsMousePressed() { return isMousePressed; }
bool Platform::IsMouseReleased(){ return isMouseReleased; }
bool Platform::IsMouseClicked() { return isMouseClicked; }

bool Platform::IsRightMousePressed() { return isRightMousePressed; }
bool Platform::IsRightMouseReleased(){ return isRightMouseReleased; }
bool Platform::IsRightMouseClicked() { return isRightMouseClicked; }

void Platform::UpdateKeyBoard(float deltaTime)
{
	for(int i=0;i<256;i++)	prevKeyStates[i] = currKeyStates[i];
	for(int i=0;i<256;i++)	currKeyStates[i] =  GetKeyState(i);

	for(int i=0;i<256;i++)
	{
		if(!(currKeyStates[i]&0x80))
			timeCountForKeyPress[i] = 0.0f;
		else
			timeCountForKeyPress[i] += deltaTime;
	}
}

bool Platform::IsKeyTyped(int key)		{ return (	!(prevKeyStates[key]&0x80)  && (currKeyStates[key]&0x80) ); }
bool Platform::IsKeyReleased(int key)	{ return (	(prevKeyStates[key]&0x80)  && !(currKeyStates[key]&0x80) ); }
bool Platform::IsKeyPressed(int key)
{
	return (bool)(currKeyStates[key]&0x80);
}

bool Platform::IsKeyPressedStill(int key, float time)
{
	return (Platform::IsKeyPressed(key) && timeCountForKeyPress[key] > time);
}
//*************************** End of Mouse Input ***********************************


void Platform::SetWindowTitle(string title)
{
	SetWindowText( hWnd, title.c_str());
}

//******************************* Device Specific **********************************
void Platform::DeviceYield(int milliSeconds)	
{
	while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!GetMessage (&msg, NULL, 0, 0))
		{
			_quit = true;
			break;
		}

		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
}
bool Platform::DeviceCheckQuitRequest()			{ return _quit; }
void Platform::DeviceExit(){}
//******************************* Upto here **********************************

void Platform::Terminate(){}

#endif



