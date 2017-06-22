#ifndef SUIInput_H
#define SUIInput_H

#include <vector>
using namespace std;

class SUIInput
{
private:
	static const int NUM_KEYS = 256;

	static short currKeyStates[NUM_KEYS];
	static short prevKeyStates[NUM_KEYS];
	static float timeCountForKeyPress[NUM_KEYS];

	static bool isMouseClicked;
	static bool isMousePressed;
	static bool isMouseReleased;
	static bool isMouseDoubleClicked;

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

	//Begin : Scroll related code
	static int SCROLL_STATE; 
	static int SCROLL_STATE_STORE;
	static const int SCROLL_NONE = 0;
	static const int SCROLL_UP = 1;
	static const int SCROLL_DOWN = 2;
	//End : Scroll related code

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
	static int GetReleasedKey();

	static bool IsKeyPressedStill(int key, float time=0.15f);
	
	static bool IsMousePressed();
	static bool IsMouseReleased();
	static bool IsMouseClicked();
	static bool IsMouseDragged();
	static bool IsMouseDoubleClicked();

	static bool IsMouseMoved();

	//Begin : Scroll related code
	static bool IsScrollUp();
	static bool IsScrollDown();
	static bool IsScrolled();
	static void SetScrollState(int scrollState);
	//End : Scroll related code
};

#endif
