#ifndef SUIMouseEvent_H
#define SUIMouseEvent_H

class SUIMouseEvent
{
	void* _com;
	float _x;
	float _y;
	int _buttonID;

public:
	static const int LEFT_BUTTON = 1;
	static const int RIGHT_BUTTON = 2;
	static const int MIDDLE_BUTTON = 3;

	SUIMouseEvent(void* com, float x, float y, int buttonID)
	{
		_com = com;
		_x = x;
		_y = y;
		_buttonID = buttonID;
	}

	void* GetComponent()		{	return _com;		}
	float GetX()				{	return _x;			}
	float GetY()				{	return _y;			}
	int GetButtonID()			{	return _buttonID;	}
};

#endif
