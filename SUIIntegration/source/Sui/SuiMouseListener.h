#ifndef MOUSE_LISTENER
#define MOUSE_LISTENER

#include "SuiMouseEvent.h"

class SuiMouseListener
{
public:
	virtual void mouseClicked(SuiMouseEvent mouseEvent) = 0;
	virtual void mousePressed(SuiMouseEvent mouseEvent) = 0;
	virtual void mouseReleased(SuiMouseEvent mouseEvent) = 0;
	//virtual void mouseEntered(UIEvent event) = 0;
	//virtual void mouseExited(UIEvent event) = 0;
};

#endif