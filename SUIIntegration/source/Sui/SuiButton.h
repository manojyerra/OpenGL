#ifndef MYBUTTON_H
#define MYBUTTON_H

#include "SuiComponent.h"
#include "SuiActionListener.h"
#include "SuiMouseListener.h"

class SuiButton : public SuiComponent
{
private:
	SuiActionListener* _actionListener;
	SuiMouseListener* _mouseListener;

public:
	SuiButton(string name);
	SuiButton(string name, int nameAlignment);
	SuiButton(string name, SuiActionListener* actionListener);

	~SuiButton();

	void InitGlobas();
	SuiEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds();
	void Draw();

	//Listeners...
	void AddMouseListener(SuiMouseListener* mouseListener)		{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()									{ _mouseListener = NULL;			}
	SuiMouseListener* GetMouseListener()						{ return _mouseListener;			}

	void AddActionListener(SuiActionListener* actionListener)	{ _actionListener = actionListener; }
	void RemoveActionListener()									{ _actionListener = NULL;			}
	SuiActionListener* GetActionListener()						{ return _actionListener;			}
};

#endif