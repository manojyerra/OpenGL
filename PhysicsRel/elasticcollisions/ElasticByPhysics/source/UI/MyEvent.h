#pragma once

#include "../CommonLib/Button.h"

class MyEvent
{
private:

public:
	Button* com;
	bool JustNowOn;
	bool IsOn;

	MyEvent()
	{
		com = NULL;
		JustNowOn = false;
		IsOn = false;
	}
	~MyEvent(){}
};
