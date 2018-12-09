#ifndef SUICheckBox_H
#define SUICheckBox_H

#include "SUIComponent.h"
#include "SUIMouseListener.h"
#include "SUIActionListener.h"

class SUICheckBox : public SUIComponent
{
private:
	int	_boxType;
	bool _isSelected;
	SUIActionListener* _actionListener;
	SUIMouseListener* _mouseListener;

	void DrawRect(float x, float y, float w, float h);
	void DrawCircle(float cx, float cy, float r);

	void Init(string name, int nameAlignment, SUIActionListener* actionListener);

public:
	static int const BOX_SQUARE = 1;
	static int const BOX_CIRCLE = 2;

	SUICheckBox(string name, int nameAlignment);
	SUICheckBox(string name, SUIActionListener* actionListener);
	SUICheckBox(string name, int nameAlignment, SUIActionListener* actionListener);

	~SUICheckBox();

	void SetShortCut(SUIKeyCombi keysCombi);
	SUIEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds();

	bool IsSelected();
	void SetSelect(bool select);
	void SetBoxType(int boxType);
	void Draw(); 

	//Listeners...
	void AddActionListener(SUIActionListener* actionListener)	{ _actionListener = actionListener; }
	void RemoveActionListener()									{ _actionListener = NULL;			}
	SUIActionListener* GetActionListener()						{ return _actionListener;			}
};

#endif
