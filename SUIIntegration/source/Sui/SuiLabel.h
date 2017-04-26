#ifndef LABEL_H
#define LABEL_H

#include "SuiComponent.h"
#include "SuiMouseListener.h"

class SuiLabel : public SuiComponent
{
private:
	SuiMouseListener* _mouseListener;
	unsigned int _fontColor;

public:
	SuiLabel(string name, int nameAlignment);
	~SuiLabel();

	SuiEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds(){}
	void Draw(); 
	void SetFontColor(unsigned int fontColor);

	//Listeners...

	void AddMouseListener(SuiMouseListener* mouseListener)			{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()									{ _mouseListener = NULL;			}
	SuiMouseListener* GetMouseListener()							{ return _mouseListener;			}

};

#endif