#ifndef SUIActionListener_H
#define SUIActionListener_H

#include "SUIActionEvent.h"

class SUIActionListener
{
public:
	virtual void actionPerformed(SUIActionEvent SUIActionEvent) = 0;
};

#endif
