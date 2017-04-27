#include "SuiInput.h"
#include "SuiIncludes.h"
#include "math.h"

bool SuiInput::PREV_LEFT_BUTTON_DOWN = false;
bool SuiInput::LEFT_BUTTON_DOWN = false;

int SuiInput::MX = 0;
int SuiInput::MY = 0;

int SuiInput::PrevMX = 0;
int SuiInput::PrevMY = 0;

int SuiInput::MouseClickX = 0;
int SuiInput::MouseClickY = 0;

int SuiInput::MouseReleaseX = 0;
int SuiInput::MouseReleaseY = 0;

bool SuiInput::isMouseClicked = false;
bool SuiInput::isMousePressed = false;
bool SuiInput::isMouseReleased = false;
bool SuiInput::isMouseDoubleClicked = false;

float SuiInput::clickTimeCount = 0;
float SuiInput::doubleClickTime = 0.2;

int SuiInput::currKeyStates[] = {0};
int SuiInput::prevKeyStates[] = {0};

float SuiInput::timeCountForKeyPress[] = {0};

void SuiInput::Init()
{
	for(int i=0;i<256;i++) currKeyStates[i] = 0;
	for(int i=0;i<256;i++) prevKeyStates[i] = 0;
	for(int i=0;i<256;i++) timeCountForKeyPress[i] = 0;
}

void SuiInput::Update(float mouseX, float mouseY, bool down, float deltaTime)
{
	LEFT_BUTTON_DOWN = down;

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

	//POINT pos;
	//GetCursorPos(&pos);

	PrevMX = MX;
	PrevMY = MY;

	MX = mouseX;
	MY = mouseY;

	if(IsMouseClicked())
	{
		if(clickTimeCount < doubleClickTime && isMouseClicked)
			isMouseDoubleClicked = true;

		clickTimeCount = 0;

		MouseClickX = MX;
	    MouseClickY = MY;

		PrevMX = MX;
		PrevMY = MY;
	}
	else if(IsMouseReleased())
	{
		MouseReleaseX = MX;
	    MouseReleaseY = MY;

		PrevMX = MX;
		PrevMY = MY;
	}

	clickTimeCount += deltaTime;
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

bool SuiInput::IsKeyTyped(int key)		{ return (bool)(!(prevKeyStates[key]&0x80)  && (currKeyStates[key]&0x80) ); }
bool SuiInput::IsKeyReleased(int key)	{ return (bool)((prevKeyStates[key]&0x80)  && !(currKeyStates[key]&0x80) );}
bool SuiInput::IsKeyPressed(int key)	{ return (bool)( (currKeyStates[key]&0x80) && true ); }
bool SuiInput::IsKeyPressedStill(int key, float time) { return (IsKeyPressed(key) && timeCountForKeyPress[key] > time); }

bool SuiInput::IsMousePressed()			{ return isMousePressed;	}
bool SuiInput::IsMouseReleased()		{ return isMouseReleased;	}
bool SuiInput::IsMouseClicked()			{ return (isMouseClicked && !isMouseDoubleClicked); }
bool SuiInput::IsMouseDragged()			{ return (IsMouseMoved() && IsMousePressed()); }
bool SuiInput::IsMouseDoubleClicked()	{ return isMouseDoubleClicked;		}

bool SuiInput::IsMouseMoved()
{
	return (PrevMX != MX || PrevMY != MY);
}