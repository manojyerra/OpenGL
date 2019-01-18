#ifndef SUIMouseListener_H
#define SUIMouseListener_H

#include "SUIMouseEvent.h"

class SUIMouseListener
{
public:
	virtual void mouseClicked(SUIMouseEvent mouseEvent) = 0;
	virtual void mousePressed(SUIMouseEvent mouseEvent) = 0;
	virtual void mouseReleased(SUIMouseEvent mouseEvent) = 0;
	//virtual void mouseEntered(UIEvent event) = 0;
	//virtual void mouseExited(UIEvent event) = 0;
};

#endif