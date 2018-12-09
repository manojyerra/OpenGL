#pragma once

#include <vector>
#include "FileReader.h"

#define KEYS_CLEAR          0x0C
#define KEYS_RETURN         0x0D

#define KEYS_SHIFT          0x10
#define KEYS_CONTROL        0x11
#define KEYS_MENU           0x12
#define KEYS_PAUSE          0x13
#define KEYS_CAPITAL        0x14

#define KEYS_KANA           0x15
#define KEYS_HANGEUL        0x15 
#define KEYS_HANGUL         0x15
#define KEYS_JUNJA          0x17
#define KEYS_FINAL          0x18
#define KEYS_HANJA          0x19
#define KEYS_KANJI          0x19

#define KEYS_ESCAPE         0x1B

#define KEYS_CONVERT        0x1C
#define KEYS_NONCONVERT     0x1D
#define KEYS_ACCEPT         0x1E
#define KEYS_MODECHANGE     0x1F

#define KEYS_SPACE          0x20
#define KEYS_PRIOR          0x21
#define KEYS_NEXT           0x22
#define KEYS_END            0x23
#define KEYS_HOME           0x24
#define KEYS_LEFT           0x25
#define KEYS_UP             0x26
#define KEYS_RIGHT          0x27
#define KEYS_DOWN           0x28
#define KEYS_SELECT         0x29
#define KEYS_PRINT          0x2A
#define KEYS_EXECUTE        0x2B
#define KEYS_SNAPSHOT       0x2C
#define KEYS_INSERT         0x2D
#define KEYS_DELETE         0x2E
#define KEYS_HELP           0x2F

#define KEYS_NUMPAD0        0x60
#define KEYS_NUMPAD1        0x61
#define KEYS_NUMPAD2        0x62
#define KEYS_NUMPAD3        0x63
#define KEYS_NUMPAD4        0x64
#define KEYS_NUMPAD5        0x65
#define KEYS_NUMPAD6        0x66
#define KEYS_NUMPAD7        0x67
#define KEYS_NUMPAD8        0x68
#define KEYS_NUMPAD9        0x69
#define KEYS_MULTIPLY       0x6A
#define KEYS_ADD            0x6B
#define KEYS_SEPARATOR      0x6C
#define KEYS_SUBTRACT       0x6D
#define KEYS_DECIMAL        0x6E
#define KEYS_DIVIDE         0x6F
#define KEYS_F1             0x70
#define KEYS_F2             0x71
#define KEYS_F3             0x72
#define KEYS_F4             0x73
#define KEYS_F5             0x74
#define KEYS_F6             0x75
#define KEYS_F7             0x76
#define KEYS_F8             0x77
#define KEYS_F9             0x78
#define KEYS_F10            0x79
#define KEYS_F11            0x7A
#define KEYS_F12            0x7B

class TouchInput
{
public:
	static const int LEFT_MOUSE_DOWN_TRUE = 1;
	static const int LEFT_MOUSE_DOWN_FALSE = 2;
	static const int RIGHT_MOUSE_DOWN_TRUE = 3;
	static const int RIGHT_MOUSE_DOWN_FALSE = 4;

	int x;
	int y;
	bool leftMouseDown;
	bool rightMouseDown;

	TouchInput(int x, int y, int mouseState);
	~TouchInput();
};

class Input
{
private:
	static int currKeyStates[256];
	static int prevKeyStates[256];
	static float keyPressTime[256];

	static bool LEFT_MOUSE_DOWN;
	static bool PREV_LEFT_MOUSE_DOWN;
	static bool STORE_LEFT_MOUSE_DOWN;

	static bool RIGHT_MOUSE_DOWN;
	static bool PREV_RIGHT_MOUSE_DOWN;
	static bool STORE_RIGHT_MOUSE_DOWN;

	static std::vector<TouchInput>* touchVec;

	static float timeBetweenClickAndRelease;
	static bool StoreBackKeyPressed;
	static bool BackKeyPressed;

	static int StoreAccMeterMovedDir;
	static int AccMeterMovedDir;

public:
	static int MouseX;
	static int MouseY;

	static int PrevMouseX;
	static int PrevMouseY;

	static int MouseClickX;
	static int MouseClickY;

	static int MouseReleaseX;
	static int MouseReleaseY;

	static int StoreX;
	static int StoreY;

	static void Update();

	static void onTouch(int x, int y, int eventID);

	static bool IsKeyTyped(int key);
	static bool IsKeyPressed(int key);
	static bool IsKeyReleased(int key);

	static bool IsKeyPressedStill(int key, float timeLimit = 0.5);

	static bool IsMouseClicked();
	static bool IsMousePressed();
	static bool IsMouseReleased();

	static bool IsRightMouseClicked();
	static bool IsRightMousePressed();
	static bool IsRightMouseReleased();

	static bool IsBackPressed();
	static void SetBackKeyPressed();
	
	static void SetAccMeterMoved(int dir);
	static int GetAccMeterDir();
	static int GetDirection();
	static int GetDirOnPress();
	static int GetSecondaryDir();

	static float GetDistBetweenClickAndRelease();
	static float GetTimeBetweenClickAndRelease();
	
	static float GetDistFromClick();
	static void ClearTouches();
};