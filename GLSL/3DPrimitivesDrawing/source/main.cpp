#include "Macros.h"
#include <GL/freeglut.h>
#include "Input.h"
#include "GameLoop.h"
//#include <vld.h>

int CreateGlutWindow(char* title, int x, int y, int w, int h);
void MouseInput(int button, int updown, int x, int y);
void MouseWheel(int button, int dir, int x, int y);
void Display();
void CloseWindow();

float _sw = 1300.0f;
float _sh = 600.0f;
int _frameCount = 0;
unsigned long _startTime = 0;
bool _once = true;

GameLoop* _gameLoop;


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	CreateGlutWindow("First Window", 0,0,_sw,_sh);
	glutMouseFunc(MouseInput);
	glutMouseWheelFunc(MouseWheel);
	glutCloseFunc(CloseWindow);
	glewInit();
	glutDisplayFunc(Display);

	_gameLoop = new GameLoop(_sw, _sh);

	glutMainLoop();

	return 0;
}

int CreateGlutWindow(char* title, int x, int y, int w, int h)
{
	glutInitWindowPosition(x,y);

	//DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style
	//DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
	//RECT windowRect = {x, y, w, h};
	//
	//AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);

	glutInitWindowSize(w,h);
	glutCreateWindow(title);

 // 	HWND hThisWnd = FindWindow( "FREEGLUT", title );
	//if( hThisWnd )
	//{
	//	LONG lStyle = GetWindowLong( hThisWnd, GWL_STYLE );
	//	SetWindowLong( hThisWnd, GWL_STYLE, 0);
	//	ShowWindow(hThisWnd, SW_SHOW);
	//}

	_sh += 22;

	return 1;
}

void MouseInput(int button, int updown, int x, int y)
{
	if(button == 0)
	{
		Input::LEFT_BUTTON_DOWN = (updown == 0);
	}
	else if(button == 1)
	{
		Input::MIDDLE_BUTTON_DOWN = (updown == 0);
	}
	else if(button == 2)
	{
		Input::RIGHT_BUTTON_DOWN = (updown == 0);
	}
}

void MouseWheel(int button, int dir, int x, int y)
{
	Input::SCROLL_STATE_STORE = (dir > 0) ? Input::SCROLL_UP : Input::SCROLL_DOWN;
}

void CloseWindow()
{
	if (_gameLoop)
	{
		delete _gameLoop;
		_gameLoop = NULL;
	}

	//VLDReportLeaks();
}

void Display()
{
	if(_once)
	{
		_startTime = GetTickCount();
		_once = false;
	}

	Input::Update(1.0f/30.0f);
	_gameLoop->Update(1.0f / 30.0f);
	_gameLoop->Draw();

	glutSwapBuffers();
	glutPostRedisplay();

	_frameCount++;

	if(_frameCount == 120)
	{
		_frameCount = 0;

		unsigned long timeTaken = GetTickCount() - _startTime;
		//printf("\nTime taken %d", (int)timeTaken);
		_startTime = GetTickCount();
	}
}




