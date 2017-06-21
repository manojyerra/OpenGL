#include "Input.h"
#include "math.h"
#include "Cam.h"
#include "../Platform/Platform.h"


int Input::MouseX = 0;
int Input::MouseY = 0;

int Input::PrevMouseX = 0;
int Input::PrevMouseY = 0;

int Input::MouseClickX = 0;
int Input::MouseClickY = 0;

int Input::MouseReleaseX = 0;
int Input::MouseReleaseY = 0;

float Input::_angle = 0.0f;
float Input::_vx = 0.0f;
float Input::_vy = 0.0f;
float Input::_speed = 0.0f;

void Input::Init()
{
}

void Input::Update(float deltaTime)
{
	Platform::UpdateMouse();
	Platform::UpdateKeyBoard(deltaTime);

	PrevMouseX = MouseX;
	PrevMouseY = MouseY;

	//Finding rect coordinate in the world which is displayed on the screen
	float windowWidth = (float)Platform::GetWindowWidth();
	float windowHeight = (float)Platform::GetWindowHeight();

	float x = 0;
	float y = 0;
	float hByW = windowHeight/windowWidth;

	x += Cam::X;
	y += Cam::Y;

	x -= Cam::Z;
	y -= Cam::Z*hByW;

	float newW = windowWidth + (2*Cam::Z);
	float newH = windowHeight + (2*Cam::Z*hByW);

	///////////////////////////////////////////////////////////////////////

	float mX = (float)Platform::MouseX();
	float mY = (float)Platform::MouseY();

	float nx = newW*mX/windowWidth;
	float ny = newH*mY/windowHeight;

	MouseX = (int)(x + nx);
	MouseY = (int)(y + ny);


	//MouseX = (float)Platform::MouseX();
	//MouseY = (float)Platform::MouseY();

	if(IsMouseClicked())
	{
		MouseClickX = MouseX;
	    MouseClickY = MouseY;

		PrevMouseX = MouseX;
		PrevMouseY = MouseY;
	}
	else if(IsMouseReleased())
	{
		MouseReleaseX = MouseX;
		MouseReleaseY = MouseY;
	}
}

float Input::GetAngle(){ return _angle; }
float Input::GetSpeed(){ return _speed; }
float Input::GetVelocityOnXAxis() { return _vx; }
float Input::GetVelocityOnYAxis() { return _vy; }

bool Input::IsMouseClicked() { return Platform::IsMouseClicked();}
bool Input::IsMousePressed() { return Platform::IsMousePressed(); }
bool Input::IsMouseReleased(){ return Platform::IsMouseReleased();}

bool Input::IsRightMouseClicked() { return Platform::IsRightMouseClicked();}
bool Input::IsRightMousePressed() { return Platform::IsRightMousePressed(); }
bool Input::IsRightMouseReleased(){ return Platform::IsRightMouseReleased();}

bool Input::IsMouseMoved()	{ return (PrevMouseX != MouseX || PrevMouseY != MouseY); }
bool Input::IsMouseDragged(){ return (IsMouseMoved() && IsMousePressed()); }

bool Input::IsKeyTyped(int key)						{	return Platform::IsKeyTyped(key);		}
bool Input::IsKeyPressed(int key)					{	return Platform::IsKeyPressed(key);		}
bool Input::IsKeyReleased(int key)					{	return Platform::IsKeyReleased(key);	}
bool Input::IsKeyPressedStill(int key, float time)	{	return Platform::IsKeyPressedStill(key, time);	}

//bool Input::IsMousePressed(int button) { return( IsMouseClicked() == false && (s3ePointerGetState((s3ePointerButton)button) & S3E_POINTER_STATE_DOWN) > 0);}
//bool Input::IsMouseReleased(int button){ return((s3ePointerGetState((s3ePointerButton)button) & S3E_POINTER_STATE_RELEASED) > 0);}
//bool Input::IsMouseClicked(int button) { return((s3ePointerGetState((s3ePointerButton)button) & S3E_POINTER_STATE_PRESSED) > 0);}
//
//bool Input::IsKeyTyped(int key)		{ return ((s3eKeyboardGetState(s3eKey(key)) & S3E_KEY_STATE_PRESSED) > 0); }
//bool Input::IsKeyPressed(int key)	{ return ((s3eKeyboardGetState(s3eKey(key)) & S3E_KEY_STATE_DOWN) > 0);	}
//bool Input::IsKeyReleased(int key)	{ return ((s3eKeyboardGetState(s3eKey(key)) & S3E_KEY_STATE_RELEASED) > 0);}
//int Input::IsKeyTypedBetween(int keyFrom, int keyTo)
//{
//	for(int i=keyFrom; i<=keyTo; i++)
//		if(IsKeyTyped((s3eKey)i))
//			return i;
//	return -1;
//}
//
//float Input::GetDistBetweenClickAndRelease()
//{
//	 return ((float) sqrt((MouseReleaseX-MouseClickX)*(MouseReleaseX-MouseClickX)+ (MouseReleaseY-MouseClickY)*(MouseReleaseY-MouseClickY)));
//}