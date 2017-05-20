#ifndef SUIRadioButton_H
#define SUIRadioButton_H

#include "SUIComponent.h"
#include "SUIActionListener.h"
#include "SUIMouseListener.h"
#include "SUICheckBox.h"

class SUIRadioButton : public SUIComponent
{
private:
	int _contentAlignment;
	vector<SUICheckBox*> _elementVec;
	SUIActionListener* _actionListener;
	SUIMouseListener* _mouseListener;
	int _selectedIndex;

public:
	SUIRadioButton(int contentAlignment);
	~SUIRadioButton();

	void SetShortCut(SUIKeyCombi keyCombi);
	void SetSelect(int index);
	int GetSelectedIndex();
	void Move(float dx, float dy);
	void ResetBounds();
	void AddCheckBox(SUICheckBox* checkBox);
	SUIEvents UpdateByInput();
	void Draw();

	//Listeners...

	void AddMouseListener(SUIMouseListener* mouseListener)			{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()										{ _mouseListener = NULL;			}
	SUIMouseListener* GetMouseListener()							{ return _mouseListener;			}

	void AddActionListener(SUIActionListener* actionListener)		{ _actionListener = actionListener; }
	void RemoveActionListener()										{ _actionListener = NULL;			}
	SUIActionListener* GetActionListener()							{ return _actionListener;			}
};

#endif
