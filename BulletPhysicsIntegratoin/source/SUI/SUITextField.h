#ifndef SUITextField_H
#define SUITextField_H

#include "SUIComponent.h"
#include "SUIActionListener.h"
#include "SUIMouseListener.h"

class SUITextField : public SUIComponent
{
private:
	SUIActionListener* _actionListener;
	SUIMouseListener* _mouseListener;

	void Init(string name, int nameAlignment, SUIActionListener* actionListener);
public:
	SUITextField(string name);
	SUITextField(string name, int nameAlignment);
	SUITextField(string name, SUIActionListener* actionListener);
	SUITextField(string name, int nameAlignment, SUIActionListener* actionListener);

	~SUITextField();

	SUIEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds();
	void Draw();

	//Listeners...
	void AddMouseListener(SUIMouseListener* mouseListener)		{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()									{ _mouseListener = NULL;			}
	SUIMouseListener* GetMouseListener()						{ return _mouseListener;			}

	void AddActionListener(SUIActionListener* actionListener)	{ _actionListener = actionListener; }
	void RemoveActionListener()									{ _actionListener = NULL;			}
	SUIActionListener* GetActionListener()						{ return _actionListener;			}
};

#endif
