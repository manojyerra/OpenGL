#pragma once

class Input
{
private:
	static int currKeyStates[256];
	static int prevKeyStates[256];

	static bool isMouseClicked;
	static bool isMousePressed;
	static bool isMouseReleased;

	static bool isRightMouseClicked;
	static bool isRightMousePressed;
	static bool isRightMouseReleased;

	static float timeCountForKeyPress[256];


public:
	static bool PREV_LEFT_BUTTON_DOWN;
	static bool LEFT_BUTTON_DOWN;

	static bool PREV_RIGHT_BUTTON_DOWN;
	static bool RIGHT_BUTTON_DOWN;

	static bool MOUSE_MOVE;

	static int MouseX;
	static int MouseY;

	static int PrevMouseX;
	static int PrevMouseY;

	static int MouseClickX;
	static int MouseClickY;

	static int MouseReleaseX;
	static int MouseReleaseY;

	static void Init();
	static void Update();

	static bool IsKeyTyped(int key);
	static bool IsKeyPressed(int key);
	static bool IsKeyReleased(int key);

	static bool IsKeyPressedStill(int key, float time=0.15f);
	
	static bool IsMousePressed();
	static bool IsMouseReleased();
	static bool IsMouseClicked();

	static bool IsRightMousePressed();
	static bool IsRightMouseReleased();
	static bool IsRightMouseClicked();

	//static bool IsMouseMoved();
	//static bool IsMouseDragged();
};
