#include "SUIInput.h"
#include "SUIIncludes.h"
#include "math.h"
#include "SUIManager.h"

bool SUIInput::PREV_LEFT_BUTTON_DOWN = false;
bool SUIInput::LEFT_BUTTON_DOWN = false;

//Begin : Scroll related code.
int SUIInput::SCROLL_STATE = SCROLL_NONE;
int SUIInput::SCROLL_STATE_STORE = SCROLL_NONE;
//End : Scroll related code.

int SUIInput::MX = 0;
int SUIInput::MY = 0;

int SUIInput::PrevMX = 0;
int SUIInput::PrevMY = 0;

int SUIInput::MouseClickX = 0;
int SUIInput::MouseClickY = 0;

int SUIInput::MouseReleaseX = 0;
int SUIInput::MouseReleaseY = 0;

bool SUIInput::isMouseClicked = false;
bool SUIInput::isMousePressed = false;
bool SUIInput::isMouseReleased = false;
bool SUIInput::isMouseDoubleClicked = false;

float SUIInput::clickTimeCount = 0;
float SUIInput::doubleClickTime = 0.2f;

int SUIInput::currKeyStates[] = {0};
int SUIInput::prevKeyStates[] = {0};

float SUIInput::timeCountForKeyPress[] = {0};

void SUIInput::Init()
{
	for(int i=0;i<256;i++) currKeyStates[i] = 0;
	for(int i=0;i<256;i++) prevKeyStates[i] = 0;
	for(int i=0;i<256;i++) timeCountForKeyPress[i] = 0;
}

bool SUIInput::Update(float mouseX, float mouseY, bool down, float deltaTime)
{
	PREV_LEFT_BUTTON_DOWN = LEFT_BUTTON_DOWN;
	LEFT_BUTTON_DOWN = down;

	SCROLL_STATE = SCROLL_STATE_STORE;
	SCROLL_STATE_STORE = SCROLL_NONE;

	isMouseClicked = false;
	isMousePressed = false;
	isMouseReleased = false;
	isMouseDoubleClicked = false;


	for(int i=0;i<256;i++)	prevKeyStates[i] = currKeyStates[i];
	for(int i=0;i<256;i++)	currKeyStates[i] =  GetKeyState(i);

	for(int i=0;i<256;i++)
	{
		if(!(currKeyStates[i]&0x80))
			timeCountForKeyPress[i] = 0.0f;
		else
			timeCountForKeyPress[i] += deltaTime;
	}

	if(PREV_LEFT_BUTTON_DOWN == false && LEFT_BUTTON_DOWN == true)		isMouseClicked = true;
	else if(PREV_LEFT_BUTTON_DOWN == true && LEFT_BUTTON_DOWN == true)	
		isMousePressed = true;
	else if(PREV_LEFT_BUTTON_DOWN == true && LEFT_BUTTON_DOWN == false)	isMouseReleased = true;

	PrevMX = MX;
	PrevMY = MY;

	MX = (int)mouseX;
	MY = (int)mouseY;

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

	bool returnVal = SUIManager::GetInstance()->Contains((float)MX, (float)MY);

	if(!returnVal)
	{
		PREV_LEFT_BUTTON_DOWN = false;
		LEFT_BUTTON_DOWN = false;

		isMouseClicked = false;
		isMousePressed = false;
		isMouseReleased = false;
		isMouseDoubleClicked = false;
	}

	return returnVal;
}

bool SUIInput::IsAllEventsFired(vector<int> vec)
{
	if(vec.size()%2 != 0)
		return false;

	bool allFalse = true;

	for(unsigned int i=0;i<vec.size();i+=2)
	{
		if(vec[i] != 0 && vec[i+1] != 0)
		{
			allFalse = false;
			break;
		}
	}

	if(allFalse)
		return false;

	for(unsigned int i=0;i<vec.size();i+=2)
	{
		if(vec[i] == 0 || vec[i+1] == 0)
			continue;
		if(IsEventFired(vec[i], vec[i+1]) == false)
			return false;
	}

	return true;
}

bool SUIInput::IsEventFired(int keyID, int eventID)
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

bool SUIInput::IsKeyTyped(int key)		{ return (bool)(!(prevKeyStates[key]&0x80)  && (currKeyStates[key]&0x80) ); }
bool SUIInput::IsKeyReleased(int key)	{ return (bool)((prevKeyStates[key]&0x80)  && !(currKeyStates[key]&0x80) );}
bool SUIInput::IsKeyPressed(int key)	{ return (bool)( (currKeyStates[key]&0x80) && true ); }
bool SUIInput::IsKeyPressedStill(int key, float time) { return (IsKeyPressed(key) && timeCountForKeyPress[key] > time); }

bool SUIInput::IsMousePressed()			{ return isMousePressed;	}
bool SUIInput::IsMouseReleased()		{ return isMouseReleased;	}
bool SUIInput::IsMouseClicked()			{ return (isMouseClicked && !isMouseDoubleClicked); }
bool SUIInput::IsMouseDragged()			{ return (IsMouseMoved() && IsMousePressed()); }
bool SUIInput::IsMouseDoubleClicked()	{ return isMouseDoubleClicked;		}

bool SUIInput::IsMouseMoved()
{
	return (PrevMX != MX || PrevMY != MY);
}

//Begin : Scroll related code.
bool SUIInput::IsScrollUp()						{ return SCROLL_STATE == SCROLL_UP;		}
bool SUIInput::IsScrollDown()					{ return SCROLL_STATE == SCROLL_DOWN;	}
bool SUIInput::IsScrolled()						{ return IsScrollDown() || IsScrollUp();}
void SUIInput::SetScrollState(int scrollState)	
{ 
	SCROLL_STATE_STORE = scrollState;
}
//End : Scroll related code.