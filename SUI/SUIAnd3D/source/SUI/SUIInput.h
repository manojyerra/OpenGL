#ifndef SUIInput_H
#define SUIInput_H

#include <vector>
using namespace std;

class SUIInput
{
private:
	static int currKeyStates[256];
	static int prevKeyStates[256];

	static bool isMouseClicked;
	static bool isMousePressed;
	static bool isMouseReleased;
	static bool isMouseDoubleClicked;

	static float timeCountForKeyPress[256];

	static bool PREV_LEFT_BUTTON_DOWN;

	static float clickTimeCount;
	static float doubleClickTime;

public:

	static bool LEFT_BUTTON_DOWN;

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

	static int MX;
	static int MY;

	static int PrevMX;
	static int PrevMY;

	static int MouseClickX;
	static int MouseClickY;

	static int MouseReleaseX;
	static int MouseReleaseY;


	static void Init();
	static bool Update(float mouseX, float mouseY, bool down, float deltaTime);

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

	static bool IsMouseMoved();
};

#endif
