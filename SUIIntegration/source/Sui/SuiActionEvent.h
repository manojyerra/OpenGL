#ifndef UIACTION_EVENT_H
#define UIACTION_EVENT_H

class SuiActionEvent
{
	void* _com;

public:
	SuiActionEvent(void* com)
	{
		_com = com;
	}

	void* GetComponent()		{	return _com;		}
};

#endif