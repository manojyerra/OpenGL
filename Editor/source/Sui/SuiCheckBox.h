#ifndef SUI_CHECKBOX_H
#define SUI_CHECKBOX_H

#include "SuiComponent.h"
#include "SuiMouseListener.h"
#include "SuiActionListener.h"

class SuiCheckBox : public SuiComponent
{
private:
	int	_boxType;
	bool _isSelected;
	SuiActionListener* _actionListener;
	SuiMouseListener* _mouseListener;

	void DrawRect(float x, float y, float w, float h);
	void DrawCircle(float cx, float cy, float r);

public:
	static int const BOX_SQUARE = 1;
	static int const BOX_CIRCLE = 2;

	SuiCheckBox(string name, int nameAlignment);
	~SuiCheckBox();

	SuiEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds();

	bool IsSelected();
	void SetSelect(bool select);
	void SetBoxType(int boxType);
	void Draw(); 

	//Listeners...
	void AddActionListener(SuiActionListener* actionListener)	{ _actionListener = actionListener; }
	void RemoveActionListener()									{ _actionListener = NULL;			}
	SuiActionListener* GetActionListener()						{ return _actionListener;			}
};

#endif
