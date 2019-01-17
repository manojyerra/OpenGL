#pragma once

#include <vector>
using namespace std;

class SuiInput
{
private:
	static int currKeyStates[256];
	static int prevKeyStates[256];

	static bool isMouseClicked;
	static bool isMousePressed;
	static bool isMouseReleased;
	static bool isMouseDoubleClicked;

	static bool isRightMouseClicked;
	static bool isRightMousePressed;
	static bool isRightMouseReleased;
	static bool isRightMouseDoubleClicked;

	static bool isMiddleMouseClicked;
	static bool isMiddleMousePressed;
	static bool isMiddleMouseReleased;
	static bool isMiddleMouseDoubleClicked;

	static float timeCountForKeyPress[256];

	static bool PREV_LEFT_BUTTON_DOWN;
	static bool PREV_RIGHT_BUTTON_DOWN;
	static bool PREV_MIDDLE_BUTTON_DOWN;

	static int state;

	static float clickTimeCount;
	static float rightClickTimeCount;
	static float middleClickTimeCount;

	static float DCTime;

public:

	//Events...
	static const int EVENT_NONE = 0;
	static const int KEY_TYPE = 1;
	static const int KEY_PRESS = 2;
	static const int KEY_RELEASE = 3;
	static const int MOUSE_CLICK = 4;
	static const int MOUSE_PRESS = 5;
	static const int MOUSE_RELEASE = 6;
	static const int MOUSE_DRAG = 7;
	static const int MOUSE_DOUBLE_CLICK = 8;
	static const int MOUSE_WHEEL_UP = 9;
	static const int MOUSE_WHEEL_DOWN = 10;

	//Mouse ID...
	static const int MOUSE_NONE = 0;
	static const int MOUSE_LEFT = 1;
	static const int MOUSE_MIDDLE = 2;
	static const int MOUSE_RIGHT = 3;
	static const int MOUSE_WHEEL = 4;

	static const int NO_INPUT = 0;
	static const int ALLOW_INPUT = 1;

	static bool LEFT_BUTTON_DOWN;
	static bool RIGHT_BUTTON_DOWN;
	static bool MIDDLE_BUTTON_DOWN;

	static int WIN_MOVE_X;
	static int WIN_MOVE_Y;

	static int SCROLL_STATE; 
	static const int SCROLL_NONE = 0;
	static const int SCROLL_UP = 1;
	static const int SCROLL_DOWN = 2;

	static int MX_ON_WIN;
	static int MY_ON_WIN;

	static int MX;
	static int MY;

	static int PrevMX;
	static int PrevMY;

	static int MouseClickX;
	static int MouseClickY;

	static int MouseReleaseX;
	static int MouseReleaseY;

	static bool MOUSE_MOVE;


	static void Init();
	static void Update(float deltaTime);
	static void SetInputState(int stateVal);

	static bool IsEventFired(int keyID, int eventID);
	static bool IsAllEventsFired(vector<int> vec);

	static bool IsKeyTyped(int key);
	static bool IsKeyPressed(int key);
	static bool IsKeyReleased(int key);

	static bool IsKeyPressedStill(int key, float time=0.15f);
	
	static bool IsMousePressed();
	static bool IsMouseReleased();
	static bool IsMouseClicked();
	static bool IsMouseDragged();
	static bool IsMouseDoubleClicked();

	static bool IsRightMousePressed();
	static bool IsRightMouseReleased();
	static bool IsRightMouseClicked();
	static bool IsRightMouseDragged();
	static bool IsRightMouseDoubleClicked();

	static bool IsMiddleMousePressed();
	static bool IsMiddleMouseReleased();
	static bool IsMiddleMouseClicked();
	static bool IsMiddleMouseDragged();
	static bool IsMiddleMouseDoubleClicked();

	static bool IsScrollUp();
	static bool IsScrollDown();

	static bool IsMouseMoved();


	static float GetDragDist();
	static float GetAngle();
};
