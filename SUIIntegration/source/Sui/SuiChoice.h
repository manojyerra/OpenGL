#ifndef SUI_CHOICE_H
#define SUI_CHOICE_H

#include "SuiComponent.h"
#include "SuiActionListener.h"
#include "SuiMouseListener.h"
#include "SuiList.h"

#include <string>
using namespace std;

class SuiChoice : public SuiComponent
{
private:
	string _selectedStr;
	SuiList* _suiList;
	bool _isListActive;
	bool _actionPerformed;

	SuiMouseListener* _mouseListener;
	SuiActionListener* _actionListener;

public:
	SuiChoice(int elementsToShow);
	~SuiChoice();

	void SetSelect(int index);
	void SetSelect(string itemName);
	int GetSelectedIndex();
	string GetSelectedStr();
	string GetItem(int index);

	void Move(float dx, float dy);
	void ResetBounds();
	void Add(string name);
	SuiEvents UpdateByInput();
	void Draw();

	bool IsDialogMode();
	void UpdateDialog();
	void DrawDialog();

	bool Contains(float x, float y);

	//Listeners...

	void AddMouseListener(SuiMouseListener* mouseListener)			{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()										{ _mouseListener = NULL;			}
	SuiMouseListener* GetMouseListener()							{ return _mouseListener;			}

	void AddActionListener(SuiActionListener* actionListener)		{ _actionListener = actionListener; }
	void RemoveActionListener()										{ _actionListener = NULL;			}
	SuiActionListener* GetActionListener()							{ return _actionListener;			}
};

#endif