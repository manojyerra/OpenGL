#pragma once

class Input
{
private:
	static float _angle;
	static float _vx;
	static float _vy;
	static float _speed;

public:
	static int MouseX;
	static int MouseY;

	static int PrevMouseX;
	static int PrevMouseY;

	static int MouseClickX;
	static int MouseClickY;

	static int MouseReleaseX;
	static int MouseReleaseY;

	static void Init();
	static void Update(float deltaTime);

	static float GetAngle();
	static float GetSpeed();
	static float GetVelocityOnXAxis();
	static float GetVelocityOnYAxis();

	static bool IsMouseClicked();
	static bool IsMousePressed();
	static bool IsMouseReleased();

	static bool IsRightMouseClicked();
	static bool IsRightMousePressed();
	static bool IsRightMouseReleased();

	static bool IsMouseMoved();
	static bool IsMouseDragged();

	//Keyboard related...
	static bool IsKeyTyped(int key);
	static bool IsKeyPressed(int key);
	static bool IsKeyReleased(int key);
	static bool IsKeyPressedStill(int key, float time);

	//static int IsKeyTypedBetween(int keyFrom, int keyTo);

	//static float GetDistBetweenClickAndRelease();
};
