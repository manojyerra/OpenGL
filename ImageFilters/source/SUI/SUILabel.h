#ifndef SUILabel_H
#define SUILabel_H

#include "SUIComponent.h"
#include "SUIMouseListener.h"

class SUILabel : public SUIComponent
{
private:
	SUIMouseListener* _mouseListener;
	unsigned int _fontColor;

public:
	SUILabel(string name, int nameAlignment);
	~SUILabel();

	SUIEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds(){}
	void Draw(); 
	void SetFontColor(unsigned int fontColor);

	//Listeners...

	void AddMouseListener(SUIMouseListener* mouseListener)			{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()									{ _mouseListener = NULL;			}
	SUIMouseListener* GetMouseListener()							{ return _mouseListener;			}

};

#endif
