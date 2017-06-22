#include "Input.h"
#include "math.h"
#include "DefinesAndIncludes.h"

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
int Input::SCROLL_STATE_STORE = Input::SCROLL_NONE;

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
float Input::DCTime = 0.2f;

int Input::currKeyStates[] = {0};
int Input::prevKeyStates[] = {0};

float Input::timeCountForKeyPress[] = {0};

bool Input::enable = true;

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

	SCROLL_STATE = SCROLL_STATE_STORE;
	SCROLL_STATE_STORE = SCROLL_NONE;

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

void Input::SetEnable(bool enableInput)
{
	enable = enableInput;
}

bool Input::IsKeyTyped(int key)		{ return enable && (bool)(!(prevKeyStates[key]&0x80)  && (currKeyStates[key]&0x80) );		}
bool Input::IsKeyReleased(int key)	{ return enable && (bool)((prevKeyStates[key]&0x80)  && !(currKeyStates[key]&0x80) );		}
bool Input::IsKeyPressed(int key)	{ return enable && (bool)( (currKeyStates[key]&0x80) && true );								}
bool Input::IsKeyPressedStill(int key, float time) { return enable && (IsKeyPressed(key) && timeCountForKeyPress[key] > time);	}

bool Input::IsAnyKeyReleased(int* keys, int size)
{
	for(int i=0; i<size; i++)
	{
		if(IsKeyReleased(keys[i]))
			return true;
	}

	return false;
}

bool Input::IsMousePressed()		{ return enable && isMousePressed;								}
bool Input::IsMouseReleased()		{ return enable && isMouseReleased;								}
bool Input::IsMouseClicked()		{ return enable && (isMouseClicked && !isMouseDoubleClicked);	}
bool Input::IsMouseDragged()		{ return enable && (IsMouseMoved() && IsMousePressed());		}
bool Input::IsMouseDoubleClicked()	{ return enable && isMouseDoubleClicked;						}

bool Input::IsRightMousePressed()		{ return enable && isRightMousePressed;									}
bool Input::IsRightMouseReleased()		{ return enable && isRightMouseReleased;								}
bool Input::IsRightMouseClicked()		{ return enable && (isRightMouseClicked && !isRightMouseDoubleClicked); }
bool Input::IsRightMouseDragged()		{ return enable && (IsMouseMoved() && IsRightMousePressed());			}
bool Input::IsRightMouseDoubleClicked()	{ return enable && isRightMouseDoubleClicked;							}

bool Input::IsMiddleMousePressed()			{ return enable && isMiddleMousePressed;									}
bool Input::IsMiddleMouseReleased()			{ return enable && isMiddleMouseReleased;									}
bool Input::IsMiddleMouseClicked()			{ return enable && (isMiddleMouseClicked && !isMiddleMouseDoubleClicked);	}
bool Input::IsMiddleMouseDragged()			{ return enable && (IsMouseMoved() && IsMiddleMousePressed());				}
bool Input::IsMiddleMouseDoubleClicked()	{ return enable && isMiddleMouseDoubleClicked;								}

bool Input::IsScrollUp()					{ return enable && (SCROLL_STATE == SCROLL_UP);		}
bool Input::IsScrollDown()					{ return enable && (SCROLL_STATE == SCROLL_DOWN);	}
void Input::SetScrollState(int scrollState)	{ SCROLL_STATE_STORE = scrollState;					}

bool Input::IsMouseMoved()
{
	return enable && (PrevMX != MX || PrevMY != MY);
}

float Input::GetDragDist()
{
	if(IsMouseDragged() || IsRightMouseDragged() || IsMiddleMouseDragged())
	{
		float dx = (float)(MX - MouseClickX);
		float dy = (float)(MY - MouseClickY);

		return sqrt( dx*dx + dy*dy );
	}

	return 0;
}

float Input::GetAngle()
{
	float dx = (float)(Input::MX - Input::PrevMX);
	float dy = (float)(Input::MY - Input::PrevMY);

	float piVal = 3.14159265f;
	float moveAngle = atan2( dy, dx ) * 180.0f / piVal;
	moveAngle = (int)moveAngle % 360 + (moveAngle-(int)moveAngle);
	if(moveAngle < 0)
		moveAngle = 360 + moveAngle;

	return moveAngle;
}
