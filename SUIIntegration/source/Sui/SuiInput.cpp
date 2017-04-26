#include "SuiInput.h"
#include "SuiIncludes.h"
#include "math.h"

int SuiInput::state = ALLOW_INPUT;

int SuiInput::MX_ON_WIN = 0;
int SuiInput::MY_ON_WIN = 0;

int SuiInput::MX = 0;
int SuiInput::MY = 0;

int SuiInput::PrevMX = 0;
int SuiInput::PrevMY = 0;

int SuiInput::MouseClickX = 0;
int SuiInput::MouseClickY = 0;

int SuiInput::MouseReleaseX = 0;
int SuiInput::MouseReleaseY = 0;

bool SuiInput::MOUSE_MOVE = false;
int SuiInput::WIN_MOVE_X = 0;
int SuiInput::WIN_MOVE_Y = 0;

bool SuiInput::PREV_LEFT_BUTTON_DOWN = false;
bool SuiInput::LEFT_BUTTON_DOWN = false;

bool SuiInput::PREV_RIGHT_BUTTON_DOWN = false;
bool SuiInput::RIGHT_BUTTON_DOWN = false;

bool SuiInput::PREV_MIDDLE_BUTTON_DOWN = false;
bool SuiInput::MIDDLE_BUTTON_DOWN = false;

int SuiInput::SCROLL_STATE = SuiInput::SCROLL_NONE; 

bool SuiInput::isMouseClicked = false;
bool SuiInput::isMousePressed = false;
bool SuiInput::isMouseReleased = false;
bool SuiInput::isMouseDoubleClicked = false;

bool SuiInput::isRightMouseClicked = false;
bool SuiInput::isRightMousePressed = false;
bool SuiInput::isRightMouseReleased = false;
bool SuiInput::isRightMouseDoubleClicked = false;

bool SuiInput::isMiddleMouseClicked = false;
bool SuiInput::isMiddleMousePressed = false;
bool SuiInput::isMiddleMouseReleased = false;
bool SuiInput::isMiddleMouseDoubleClicked = false;

float SuiInput::clickTimeCount = 0;
float SuiInput::rightClickTimeCount = 0;
float SuiInput::middleClickTimeCount = 0;
float SuiInput::DCTime = 0.2;

int SuiInput::currKeyStates[] = {0};
int SuiInput::prevKeyStates[] = {0};

float SuiInput::timeCountForKeyPress[] = {0};

void SuiInput::Init()
{
	for(int i=0;i<256;i++) currKeyStates[i] = 0;
	for(int i=0;i<256;i++) prevKeyStates[i] = 0;
	for(int i=0;i<256;i++) timeCountForKeyPress[i] = 0;

	state = ALLOW_INPUT;
}

void SuiInput::Update(float deltaTime)
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

void SuiInput::SetInputState(int stateVal)
{
	state = stateVal;
}

bool SuiInput::IsAllEventsFired(vector<int> vec)
{
	if(vec.size()%2 != 0)
		return false;

	bool allFalse = true;
	for(int i=0;i<vec.size();i+=2)
	{
		if(vec[i] != 0 && vec[i+1] != 0)
		{
			allFalse = false;
			break;
		}
	}

	if(allFalse)
		return false;

	for(int i=0;i<vec.size();i+=2)
	{
		if(vec[i] == 0 || vec[i+1] == 0)
			continue;
		if(IsEventFired(vec[i], vec[i+1]) == false)
			return false;
	}

	return true;
}

bool SuiInput::IsEventFired(int keyID, int eventID)
{
	if(keyID > 0 && eventID > 0)
	{
		if(keyID <= 4)
		{
			if(keyID == MOUSE_LEFT)
			{
				if(eventID == MOUSE_CLICK)				return IsMouseClicked();
				else if(eventID == MOUSE_PRESS)			return IsMousePressed();
				else if(eventID == MOUSE_RELEASE)		return IsMouseReleased();
				else if(eventID == MOUSE_DRAG)			return IsMouseDragged();
				else if(eventID == MOUSE_DOUBLE_CLICK)	return IsMouseDoubleClicked();
			}
			else if(keyID == MOUSE_MIDDLE)
			{
				if(eventID == MOUSE_CLICK)				return IsMiddleMouseClicked();
				else if(eventID == MOUSE_PRESS)			return IsMiddleMousePressed();
				else if(eventID == MOUSE_RELEASE)		return IsMiddleMouseReleased();
				else if(eventID == MOUSE_DRAG)			return IsMiddleMouseDragged();
				else if(eventID == MOUSE_DOUBLE_CLICK)	return IsMouseDoubleClicked();
			}
			else if(keyID == MOUSE_RIGHT)
			{
				if(eventID == MOUSE_CLICK)				return IsRightMouseClicked();
				else if(eventID == MOUSE_PRESS)			return IsRightMousePressed();
				else if(eventID == MOUSE_RELEASE)		return IsRightMouseReleased();
				else if(eventID == MOUSE_DRAG)			return IsRightMouseDragged();
				else if(eventID == MOUSE_DOUBLE_CLICK)	return IsRightMouseDoubleClicked();
			}
			else if(keyID == MOUSE_WHEEL)
			{
				if(eventID == MOUSE_WHEEL_UP)			return IsScrollUp();
				else if(eventID == MOUSE_WHEEL_DOWN)	return IsScrollDown();
			}
		}
		else
		{
			if(eventID == KEY_TYPE)				return IsKeyTyped(keyID);
			else if(eventID == KEY_PRESS)		return IsKeyPressed(keyID);
			else if(eventID == KEY_RELEASE)		return IsKeyReleased(keyID);
		}
	}

	return false;
}

bool SuiInput::IsKeyTyped(int key)		{ if(state == NO_INPUT) return false; return (bool)(!(prevKeyStates[key]&0x80)  && (currKeyStates[key]&0x80) ); }
bool SuiInput::IsKeyReleased(int key)	{ if(state == NO_INPUT) return false; return (bool)((prevKeyStates[key]&0x80)  && !(currKeyStates[key]&0x80) );}
bool SuiInput::IsKeyPressed(int key)	{ if(state == NO_INPUT) return false; return (bool)( (currKeyStates[key]&0x80) && true ); }
bool SuiInput::IsKeyPressedStill(int key, float time) { if(state == NO_INPUT) return false;	return (IsKeyPressed(key) && timeCountForKeyPress[key] > time); }

bool SuiInput::IsMousePressed()			{ if(state == NO_INPUT) return false; return isMousePressed;	}
bool SuiInput::IsMouseReleased()		{ if(state == NO_INPUT) return false; return isMouseReleased;	}
bool SuiInput::IsMouseClicked()			{ if(state == NO_INPUT) return false; return (isMouseClicked && !isMouseDoubleClicked); }
bool SuiInput::IsMouseDragged()			{ if(state == NO_INPUT) return false; return (IsMouseMoved() && IsMousePressed()); }
bool SuiInput::IsMouseDoubleClicked()	{ if(state == NO_INPUT) return false; return isMouseDoubleClicked;		}

bool SuiInput::IsRightMousePressed()	{ if(state == NO_INPUT) return false; return isRightMousePressed; }
bool SuiInput::IsRightMouseReleased()	{ if(state == NO_INPUT) return false; return isRightMouseReleased; }
bool SuiInput::IsRightMouseClicked()	{ if(state == NO_INPUT) return false; return (isRightMouseClicked && !isRightMouseDoubleClicked); }
bool SuiInput::IsRightMouseDragged()	{ if(state == NO_INPUT) return false; return (IsMouseMoved() && IsRightMousePressed()); }
bool SuiInput::IsRightMouseDoubleClicked()	{ if(state == NO_INPUT) return false; return isRightMouseDoubleClicked;		}

bool SuiInput::IsMiddleMousePressed()	{ if(state == NO_INPUT) return false; return isMiddleMousePressed; }
bool SuiInput::IsMiddleMouseReleased()	{ if(state == NO_INPUT) return false; return isMiddleMouseReleased; }
bool SuiInput::IsMiddleMouseClicked()	{ if(state == NO_INPUT) return false; return (isMiddleMouseClicked && !isMiddleMouseDoubleClicked); }
bool SuiInput::IsMiddleMouseDragged()	{ if(state == NO_INPUT) return false; return (IsMouseMoved() && IsMiddleMousePressed()); }
bool SuiInput::IsMiddleMouseDoubleClicked()	{ if(state == NO_INPUT) return false; return isMiddleMouseDoubleClicked;		}

bool SuiInput::IsScrollUp()		{ if(state == NO_INPUT) return false; return (SCROLL_STATE == SCROLL_UP);	}
bool SuiInput::IsScrollDown()	{ if(state == NO_INPUT) return false; return (SCROLL_STATE == SCROLL_DOWN);	}

bool SuiInput::IsMouseMoved()
{
	if(state == NO_INPUT) return false;
	return (PrevMX != MX || PrevMY != MY);
}

float SuiInput::GetDragDist()
{
	if(state == NO_INPUT) return 0;

	if(IsMouseDragged() || IsRightMouseDragged() || IsMiddleMouseDragged())
	{
		float dx = MX - MouseClickX;
		float dy = MY - MouseClickY;

		return sqrt( dx*dx + dy*dy );
	}

	return 0;
}

float SuiInput::GetAngle()
{
	float dx = SuiInput::MX - SuiInput::PrevMX;
	float dy = SuiInput::MY - SuiInput::PrevMY;

	float piVal = 3.14159265;
	float moveAngle = atan2( dy, dx ) * 180.0f / piVal;
	moveAngle = (int)moveAngle % 360 + (moveAngle-(int)moveAngle);
	if(moveAngle < 0)
		moveAngle = 360 + moveAngle;

	return moveAngle;
}