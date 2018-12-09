#include "Input.h"
#include "math.h"
#include "Includes.h"
#include "GameInfo.h"
#include "Dir.h"

int Input::StoreX = 0;
int Input::StoreY = 0;

int Input::MouseX = 0;
int Input::MouseY = 0;

int Input::PrevMouseX = 0;
int Input::PrevMouseY = 0;

int Input::MouseClickX = 0;
int Input::MouseClickY = 0;

int Input::MouseReleaseX = 0;
int Input::MouseReleaseY = 0;

bool Input::LEFT_MOUSE_DOWN = false;
bool Input::PREV_LEFT_MOUSE_DOWN = false;
bool Input::STORE_LEFT_MOUSE_DOWN = false;

bool Input::RIGHT_MOUSE_DOWN = false;
bool Input::PREV_RIGHT_MOUSE_DOWN = false;
bool Input::STORE_RIGHT_MOUSE_DOWN = false;

int Input::currKeyStates[] = {0};
int Input::prevKeyStates[] = {0};
float Input::keyPressTime[] = {0};

bool Input::StoreBackKeyPressed = false;
bool Input::BackKeyPressed = false;

int Input::StoreAccMeterMovedDir = Dir::NONE;
int Input::AccMeterMovedDir = Dir::NONE;

float Input::timeBetweenClickAndRelease = 0;

std::vector<TouchInput>* Input::touchVec = NULL;

void Input::Update()
{
#ifdef _PLATFORM_WIN32
	for (int i = 0; i < 256; i++)	prevKeyStates[i] = currKeyStates[i];
	for (int i = 0; i < 256; i++)	currKeyStates[i] = GetKeyState(i);

	for (int i = 0; i < 256; i++)
	{
		if (IsKeyPressed(i))
		{
			keyPressTime[i] += GameInfo::deltaTime;
		}
		else
		{
			keyPressTime[i] = 0;
		}
	}

#endif

	if(touchVec == NULL)
	{
		touchVec = new vector<TouchInput>();
	}

	if(touchVec->size() > 0)
	{
		StoreX = touchVec->at(0).x;
		StoreY = touchVec->at(0).y;

		STORE_LEFT_MOUSE_DOWN = touchVec->at(0).leftMouseDown;
		STORE_RIGHT_MOUSE_DOWN = touchVec->at(0).rightMouseDown;

		touchVec->erase(touchVec->begin()+0);
	}

	BackKeyPressed = false;
	AccMeterMovedDir = Dir::NONE;

	PrevMouseX = MouseX;
	PrevMouseY = MouseY;

	GameInfo* in = GameInfo::GetInstance();

	MouseX = (StoreX - in->drawX) * in->CodedW() / in->drawW;
	MouseY = (StoreY - in->drawY) * in->CodedW() / in->drawW;

	PREV_LEFT_MOUSE_DOWN = LEFT_MOUSE_DOWN;
	LEFT_MOUSE_DOWN = STORE_LEFT_MOUSE_DOWN;

	PREV_RIGHT_MOUSE_DOWN = RIGHT_MOUSE_DOWN;
	RIGHT_MOUSE_DOWN = STORE_RIGHT_MOUSE_DOWN;


	if(IsMouseClicked())
	{
		PrevMouseX = MouseX;
		PrevMouseY = MouseY;

		MouseClickX = MouseX;
	    MouseClickY = MouseY;

		timeBetweenClickAndRelease = 0;
	}
	else if(IsMouseReleased())
	{
		MouseReleaseX = MouseX;
		MouseReleaseY = MouseY;
	}


	if(IsMousePressed())
		timeBetweenClickAndRelease += GameInfo::deltaTime;

	BackKeyPressed = StoreBackKeyPressed;
	StoreBackKeyPressed = false;

	AccMeterMovedDir = StoreAccMeterMovedDir;
	StoreAccMeterMovedDir = Dir::NONE;
}

void Input::onTouch(int x, int y, int eventID)
{
	if(touchVec)
	{
		if(eventID == 1)
		{
			touchVec->push_back(TouchInput(x,y,TouchInput::LEFT_MOUSE_DOWN_TRUE));
		}
		else if(eventID == 2)
		{
			touchVec->push_back(TouchInput(x,y, TouchInput::LEFT_MOUSE_DOWN_FALSE));
		}
		else if (eventID == 3)
		{
			touchVec->push_back(TouchInput(x, y, TouchInput::RIGHT_MOUSE_DOWN_TRUE));
		}
		else if (eventID == 4)
		{
			touchVec->push_back(TouchInput(x, y, TouchInput::RIGHT_MOUSE_DOWN_FALSE));
		}
	}
}

bool Input::IsMouseClicked() { return (LEFT_MOUSE_DOWN && !PREV_LEFT_MOUSE_DOWN); }
bool Input::IsMousePressed() { return (LEFT_MOUSE_DOWN); }
bool Input::IsMouseReleased(){ return (!LEFT_MOUSE_DOWN && PREV_LEFT_MOUSE_DOWN); }

bool Input::IsRightMouseClicked() { return (RIGHT_MOUSE_DOWN && !PREV_RIGHT_MOUSE_DOWN); }
bool Input::IsRightMousePressed() { return (RIGHT_MOUSE_DOWN); }
bool Input::IsRightMouseReleased() { return (!RIGHT_MOUSE_DOWN && PREV_RIGHT_MOUSE_DOWN); }


bool Input::IsBackPressed()
{
	return BackKeyPressed;
}

void Input::SetBackKeyPressed()
{
	StoreBackKeyPressed = true;
}

void Input::SetAccMeterMoved(int dir)
{
	StoreAccMeterMovedDir = dir;
}

int Input::GetAccMeterDir()
{
	return AccMeterMovedDir;
}

float Input::GetDistBetweenClickAndRelease()
{
	float dx = (MouseClickX - MouseReleaseX);
	float dy = (MouseClickY - MouseReleaseY);

	return sqrt( dx*dx + dy*dy);
}

float Input::GetDistFromClick()
{
	float dx = (MouseClickX - MouseX);
	float dy = (MouseClickY - MouseY);

	return sqrt( dx*dx + dy*dy);
}

int Input::GetDirection()
{
	if(IsMouseReleased())
	{
		float dx = (MouseClickX - MouseReleaseX);
		float dy = (MouseClickY - MouseReleaseY);

		float angle = atan2(dy, dx) * 180.0f / 3.14159265;

		angle = (int)angle % 360;

		if(angle < 0)
			angle = 360+angle;

		if((angle > 315 && angle <=360) || (angle > 0 && angle <= 45))
			return Dir::LEFT;

		else if(angle > 45 && angle <= 135)
			return Dir::UP;

		else if(angle > 135 && angle <= 225)
			return Dir::RIGHT;

		else if(angle > 225 && angle <= 315)
			return Dir::DOWN;
	}

	return Dir::NONE;
}

int Input::GetDirOnPress()
{
	if(IsMousePressed())
	{
		float dx = (MouseClickX - MouseX);
		float dy = (MouseClickY - MouseY);

		if(sqrt(dx*dx + dy*dy) < 20)
			return Dir::NONE;

		float angle = atan2(dy, dx) * 180.0f / 3.14159265;

		angle = (int)angle % 360;

		if(angle < 0)
			angle = 360+angle;

		if((angle > 315 && angle <=360) || (angle > 0 && angle <= 45))
			return Dir::LEFT;

		else if(angle > 45 && angle <= 135)
			return Dir::UP;

		else if(angle > 135 && angle <= 225)
			return Dir::RIGHT;

		else if(angle > 225 && angle <= 315)
			return Dir::DOWN;
	}

	return Dir::NONE;
}

int Input::GetSecondaryDir()
{
	int dir = GetDirection();

	if(dir != Dir::NONE)
	{
		float dx = (MouseClickX - MouseReleaseX);
		float dy = (MouseClickY - MouseReleaseY);

		float angle = atan2(dy, dx) * 180.0f / 3.14159265;
			
		angle = (int)angle % 360;

		if(angle < 0)
			angle = 360+angle;

		if(dir == Dir::LEFT || dir == Dir::RIGHT)
		{
			if(angle > 0 && angle <= 180)
				dir = Dir::UP;
			else if(angle > 180 && angle <= 360)
				dir = Dir::DOWN;
		}
		else if(dir == Dir::UP || dir == Dir::DOWN)
		{
			if((angle > 270 && angle <=360) || (angle > 0 && angle <= 90))
				dir = Dir::LEFT;
			else if(angle > 90 && angle <= 270)
				dir = Dir::RIGHT;
		}
	}

	return dir;
}

float Input::GetTimeBetweenClickAndRelease()
{
	return timeBetweenClickAndRelease;
}

bool Input::IsKeyTyped(int key)		{ return (bool)(!(prevKeyStates[key]&0x80)  && (currKeyStates[key]&0x80) ); }
bool Input::IsKeyReleased(int key)	{ return (bool)((prevKeyStates[key]&0x80)  && !(currKeyStates[key]&0x80) );}
bool Input::IsKeyPressed(int key)	{ return (bool)(currKeyStates[key]&0x80); }

bool Input::IsKeyPressedStill(int key, float timeLimit)
{
	return (keyPressTime[key] > timeLimit);
}

void Input::ClearTouches()
{
	touchVec->clear();
	delete touchVec;
	touchVec = NULL;
}



TouchInput::TouchInput(int x, int y, int mouseState)
{
	this->x = x;
	this->y = y;
	this->leftMouseDown = false;
	this->rightMouseDown = false;

	if(mouseState == LEFT_MOUSE_DOWN_TRUE)
		this->leftMouseDown = true;
	else if (mouseState == LEFT_MOUSE_DOWN_FALSE)
		this->leftMouseDown = false;
	else if (mouseState == RIGHT_MOUSE_DOWN_TRUE)
		this->rightMouseDown = true;
	else if (mouseState == RIGHT_MOUSE_DOWN_FALSE)
		this->rightMouseDown = false;
}

TouchInput::~TouchInput()
{
}

