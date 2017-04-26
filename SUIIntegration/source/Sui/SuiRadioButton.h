#ifndef RADIO_BUTTON_H
#define RADIO_BUTTON_H

#include "SuiComponent.h"
#include "SuiActionListener.h"
#include "SuiMouseListener.h"
#include "SuiCheckBox.h"

class SuiRadioButton : public SuiComponent
{
private:
	int _contentAlignment;
	vector<SuiCheckBox*> _elementVec;
	SuiActionListener* _actionListener;
	SuiMouseListener* _mouseListener;
	int _selectedIndex;

public:
	SuiRadioButton(int contentAlignment);
	~SuiRadioButton();

	void SetSelect(int index);
	int GetSelectedIndex();
	void Move(float dx, float dy);
	void ResetBounds();
	void AddCheckBox(SuiCheckBox* checkBox);
	SuiEvents UpdateByInput();
	void Draw();

	//Listeners...

	void AddMouseListener(SuiMouseListener* mouseListener)			{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()										{ _mouseListener = NULL;			}
	SuiMouseListener* GetMouseListener()							{ return _mouseListener;			}

	void AddActionListener(SuiActionListener* actionListener)		{ _actionListener = actionListener; }
	void RemoveActionListener()										{ _actionListener = NULL;			}
	SuiActionListener* GetActionListener()							{ return _actionListener;			}
};

#endif