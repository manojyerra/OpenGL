#ifndef ACTION_LISTENER
#define ACTION_LISTENER

#include "SuiActionEvent.h"

class SuiActionListener
{
public:
	virtual void actionPerformed(SuiActionEvent suiActionEvent) = 0;
};

#endif