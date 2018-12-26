#ifdef _WINDOWS
#include <windows.h>
#endif

#include "Input.h"
#include "../GameInfo.h"

int Input::MouseX = 0;
int Input::MouseY = 0;

int Input::PrevMouseX = 0;
int Input::PrevMouseY = 0;

int Input::MouseClickX = 0;
int Input::MouseClickY = 0;

int Input::MouseReleaseX = 0;
int Input::MouseReleaseY = 0;

bool Input::MOUSE_MOVE = false;

bool Input::PREV_LEFT_BUTTON_DOWN = false;
bool Input::LEFT_BUTTON_DOWN = false;

bool Input::PREV_RIGHT_BUTTON_DOWN = false;
bool Input::RIGHT_BUTTON_DOWN = false;

bool Input::isMouseClicked = false;
bool Input::isMousePressed = false;
bool Input::isMouseReleased = false;

bool Input::isRightMouseClicked = false;
bool Input::isRightMousePressed = false;
bool Input::isRightMouseReleased = false;


int Input::currKeyStates[] = {0};
int Input::prevKeyStates[] = {0};

float Input::timeCountForKeyPress[] = {0};

void Input::Init()
{
	for(int i=0;i<256;i++) currKeyStates[i] = 0;
	for(int i=0;i<256;i++) prevKeyStates[i] = 0;
	for(int i=0;i<256;i++) timeCountForKeyPress[i] = 0;
}

void Input::Update()
{
	for(int i=0;i<256;i++)	prevKeyStates[i] = currKeyStates[i];
	for(int i=0;i<256;i++)	currKeyStates[i] =  GetKeyState(i);

	for(int i=0;i<256;i++)
	{
		if(!(currKeyStates[i]&0x80))
		{
			timeCountForKeyPress[i] = 0.0f;
		}
		else
		{
			timeCountForKeyPress[i] += GameInfo::deltaTime;
		}
	}


	isMouseClicked = false;
	isMousePressed = false;
	isMouseReleased = false;

	if(PREV_LEFT_BUTTON_DOWN == false && LEFT_BUTTON_DOWN == true)		isMouseClicked = true;
	else if(PREV_LEFT_BUTTON_DOWN == true && LEFT_BUTTON_DOWN == true)	isMousePressed = true;
	else if(PREV_LEFT_BUTTON_DOWN == true && LEFT_BUTTON_DOWN == false)	isMouseReleased = true;

	PREV_LEFT_BUTTON_DOWN = LEFT_BUTTON_DOWN;



	isRightMouseClicked = false;
	isRightMousePressed = false;
	isRightMouseReleased = false;

	if(PREV_RIGHT_BUTTON_DOWN == false && RIGHT_BUTTON_DOWN == true)		isRightMouseClicked = true;
	else if(PREV_RIGHT_BUTTON_DOWN == true && RIGHT_BUTTON_DOWN == true)	isRightMousePressed = true;
	else if(PREV_RIGHT_BUTTON_DOWN == true && RIGHT_BUTTON_DOWN == false)	isRightMouseReleased = true;

	PREV_RIGHT_BUTTON_DOWN = RIGHT_BUTTON_DOWN;



	PrevMouseX = MouseX;
	PrevMouseY = MouseY;

	POINT pos;
	GetCursorPos(&pos);

	MouseX = pos.x;
	MouseY = pos.y-30;

	float CODE_WIDTH = 1250.0f;
	float CODE_HEIGHT = 950.0f;

	float TARGET_WIDTH = 921.0f;
	float TARGET_HEIGHT = 700.0f;

	MouseX = MouseX * CODE_WIDTH / TARGET_WIDTH;
	MouseY = MouseY * CODE_HEIGHT / TARGET_HEIGHT;

	if(IsMouseClicked())
	{
		MouseClickX = MouseX;
	    MouseClickY = MouseY;
	}
	else if(IsMouseReleased())
	{
		MouseReleaseX = MouseX;
	    MouseReleaseY = MouseY;
	}
}

bool Input::IsKeyTyped(int key)		{ return (	!(prevKeyStates[key]&0x80)  && (currKeyStates[key]&0x80) ); }
bool Input::IsKeyPressed(int key)	{ return (currKeyStates[key]&0x80); }
bool Input::IsKeyReleased(int key)	{ return (	(prevKeyStates[key]&0x80)  && !(currKeyStates[key]&0x80) ); }

bool Input::IsKeyPressedStill(int key, float time)
{
	return (IsKeyPressed(key) && timeCountForKeyPress[key] > time);
}

bool Input::IsMousePressed() { return isMousePressed; }
bool Input::IsMouseReleased(){ return isMouseReleased; }
bool Input::IsMouseClicked() { return isMouseClicked; }

bool Input::IsRightMousePressed() { return isRightMousePressed; }
bool Input::IsRightMouseReleased(){ return isRightMouseReleased; }
bool Input::IsRightMouseClicked() { return isRightMouseClicked; }

//bool Input::IsMouseMoved()	{ return (PrevMouseX != MouseX || PrevMouseY != MouseY); }
//bool Input::IsMouseDragged()	{ return (IsMouseMoved() && IsMousePressed()); }