#include "Input.h"
#include "math.h"
#include <windows.h>


int Input::MX = 0;
int Input::MY = 0;

int Input::PrevMX = 0;
int Input::PrevMY = 0;

int Input::MouseClickX = 0;
int Input::MouseClickY = 0;

int Input::MouseReleaseX = 0;
int Input::MouseReleaseY = 0;

bool Input::MOUSE_MOVE = false;
int Input::WIN_MOVE_X = 0;
int Input::WIN_MOVE_Y = 0;

bool Input::PREV_LEFT_BUTTON_DOWN = false;
bool Input::LEFT_BUTTON_DOWN = false;

bool Input::PREV_RIGHT_BUTTON_DOWN = false;
bool Input::RIGHT_BUTTON_DOWN = false;

bool Input::PREV_MIDDLE_BUTTON_DOWN = false;
bool Input::MIDDLE_BUTTON_DOWN = false;

int Input::SCROLL_STATE = Input::SCROLL_NONE; 

bool Input::isMouseClicked = false;
bool Input::isMousePressed = false;
bool Input::isMouseReleased = false;
bool Input::isMouseDoubleClicked = false;

bool Input::isRightMouseClicked = false;
bool Input::isRightMousePressed = false;
bool Input::isRightMouseReleased = false;
bool Input::isRightMouseDoubleClicked = false;

bool Input::isMiddleMouseClicked = false;
bool Input::isMiddleMousePressed = false;
bool Input::isMiddleMouseReleased = false;
bool Input::isMiddleMouseDoubleClicked = false;

float Input::clickTimeCount = 0;
float Input::rightClickTimeCount = 0;
float Input::middleClickTimeCount = 0;
float Input::DCTime = 0.2;

int Input::currKeyStates[] = {0};
int Input::prevKeyStates[] = {0};

float Input::timeCountForKeyPress[] = {0};

void Input::Init()
{
	for(int i=0;i<256;i++) currKeyStates[i] = 0;
	for(int i=0;i<256;i++) prevKeyStates[i] = 0;
	for(int i=0;i<256;i++) timeCountForKeyPress[i] = 0;
}

void Input::Update(float deltaTime)
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

	isMouseClicked = false;
	isMousePressed = false;
	isMouseReleased = false;
	isMouseDoubleClicked = false;

	if(PREV_LEFT_BUTTON_DOWN == false && LEFT_BUTTON_DOWN == true)		isMouseClicked = true;
	else if(PREV_LEFT_BUTTON_DOWN == true && LEFT_BUTTON_DOWN == true)	isMousePressed = true;
	else if(PREV_LEFT_BUTTON_DOWN == true && LEFT_BUTTON_DOWN == false)	isMouseReleased = true;

	PREV_LEFT_BUTTON_DOWN = LEFT_BUTTON_DOWN;


	isRightMouseClicked = false;
	isRightMousePressed = false;
	isRightMouseReleased = false;
	isRightMouseDoubleClicked = false;

	if(PREV_RIGHT_BUTTON_DOWN == false && RIGHT_BUTTON_DOWN == true)		isRightMouseClicked = true;
	else if(PREV_RIGHT_BUTTON_DOWN == true && RIGHT_BUTTON_DOWN == true)	isRightMousePressed = true;
	else if(PREV_RIGHT_BUTTON_DOWN == true && RIGHT_BUTTON_DOWN == false)	isRightMouseReleased = true;

	PREV_RIGHT_BUTTON_DOWN = RIGHT_BUTTON_DOWN;


	isMiddleMouseClicked = false;
	isMiddleMousePressed = false;
	isMiddleMouseReleased = false;
	isMiddleMouseDoubleClicked = false;

	if(PREV_MIDDLE_BUTTON_DOWN == false && MIDDLE_BUTTON_DOWN == true)		isMiddleMouseClicked = true;
	else if(PREV_MIDDLE_BUTTON_DOWN == true && MIDDLE_BUTTON_DOWN == true)	isMiddleMousePressed = true;
	else if(PREV_MIDDLE_BUTTON_DOWN == true && MIDDLE_BUTTON_DOWN == false)	isMiddleMouseReleased = true;

	PREV_MIDDLE_BUTTON_DOWN = MIDDLE_BUTTON_DOWN;

	POINT pos;
	GetCursorPos(&pos);

	PrevMX = MX;
	PrevMY = MY;

	MX = pos.x - WIN_MOVE_X;
	MY = pos.y - WIN_MOVE_Y;

	if(IsMouseClicked() || IsRightMouseClicked() || IsMiddleMouseClicked())
	{
		if(clickTimeCount < DCTime && isMouseClicked)				{ isMouseDoubleClicked = true;		}
		if(rightClickTimeCount < DCTime && isRightMouseClicked)		{ isRightMouseDoubleClicked = true; }
		if(middleClickTimeCount < DCTime && isMiddleMouseClicked)	{ isMiddleMouseDoubleClicked = true;}

		clickTimeCount = 0;
		rightClickTimeCount = 0;
		middleClickTimeCount = 0;

		MouseClickX = MX;
	    MouseClickY = MY;

		PrevMX = MX;
		PrevMY = MY;
	}
	else if(IsMouseReleased() || IsRightMouseReleased() || IsMiddleMouseReleased())
	{
		MouseReleaseX = MX;
	    MouseReleaseY = MY;

		PrevMX = MX;
		PrevMY = MY;
	}

	clickTimeCount += deltaTime;
	rightClickTimeCount += deltaTime;
	middleClickTimeCount += deltaTime;
}

bool Input::IsKeyTyped(int key)		{ return (bool)(!(prevKeyStates[key]&0x80)  && (currKeyStates[key]&0x80) ); }
bool Input::IsKeyReleased(int key)	{ return (bool)((prevKeyStates[key]&0x80)  && !(currKeyStates[key]&0x80) );}
bool Input::IsKeyPressed(int key)	{ return (bool)( (currKeyStates[key]&0x80) && true ); }
bool Input::IsKeyPressedStill(int key, float time) { return (IsKeyPressed(key) && timeCountForKeyPress[key] > time); }

bool Input::IsMousePressed()			{ return isMousePressed;	}
bool Input::IsMouseReleased()		{ return isMouseReleased;	}
bool Input::IsMouseClicked()			{ return (isMouseClicked && !isMouseDoubleClicked); }
bool Input::IsMouseDragged()			{ return (IsMouseMoved() && IsMousePressed()); }
bool Input::IsMouseDoubleClicked()	{ return isMouseDoubleClicked;		}

bool Input::IsRightMousePressed()	{ return isRightMousePressed; }
bool Input::IsRightMouseReleased()	{ return isRightMouseReleased; }
bool Input::IsRightMouseClicked()	{ return (isRightMouseClicked && !isRightMouseDoubleClicked); }
bool Input::IsRightMouseDragged()	{ return (IsMouseMoved() && IsRightMousePressed()); }
bool Input::IsRightMouseDoubleClicked()	{ return isRightMouseDoubleClicked;		}

bool Input::IsMiddleMousePressed()	{ return isMiddleMousePressed; }
bool Input::IsMiddleMouseReleased()	{ return isMiddleMouseReleased; }
bool Input::IsMiddleMouseClicked()	{ return (isMiddleMouseClicked && !isMiddleMouseDoubleClicked); }
bool Input::IsMiddleMouseDragged()	{ return (IsMouseMoved() && IsMiddleMousePressed()); }
bool Input::IsMiddleMouseDoubleClicked()	{ return isMiddleMouseDoubleClicked;		}

bool Input::IsScrollUp()		{ return (SCROLL_STATE == SCROLL_UP);	}
bool Input::IsScrollDown()	{ return (SCROLL_STATE == SCROLL_DOWN);	}

bool Input::IsMouseMoved()
{
	return (PrevMX != MX || PrevMY != MY);
}

float Input::GetDragDist()
{
	if(IsMouseDragged() || IsRightMouseDragged() || IsMiddleMouseDragged())
	{
		float dx = MX - MouseClickX;
		float dy = MY - MouseClickY;

		return sqrt( dx*dx + dy*dy );
	}

	return 0;
}

float Input::GetAngle()
{
	float dx = Input::MX - Input::PrevMX;
	float dy = Input::MY - Input::PrevMY;

	float piVal = 3.14159265;
	float moveAngle = atan2( dy, dx ) * 180.0f / piVal;
	moveAngle = (int)moveAngle % 360 + (moveAngle-(int)moveAngle);
	if(moveAngle < 0)
		moveAngle = 360 + moveAngle;

	return moveAngle;
}