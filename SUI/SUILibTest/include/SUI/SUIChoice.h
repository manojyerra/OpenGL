#ifndef SUIChoice_H
#define SUIChoice_H

#include "SUIComponent.h"
#include "SUIActionListener.h"
#include "SUIMouseListener.h"
#include "SUIList.h"

#include <string>
using namespace std;

namespace SUI
{
	class SUIChoice : public SUIComponent
	{
	private:
		string _selectedStr;
		SUIList* _SUIList;
		bool _isListActive;
		bool _actionPerformed;

		SUIMouseListener* _mouseListener;
		SUIActionListener* _actionListener;

	public:
		SUIChoice(int elementsToShow);
		~SUIChoice();

		void SetSelect(int index);
		void SetSelect(string itemName);
		int GetSelectedIndex();
		string GetSelectedStr();
		string GetItem(int index);

		void Move(float dx, float dy);
		void ResetBounds();
		void Add(string name);
		SUIEvents UpdateByInput();
		void Draw();

		bool IsDialogMode();
		void UpdateDialog();
		void DrawDialog();

		bool Contains(float x, float y);

		//Listeners...

		void AddMouseListener(SUIMouseListener* mouseListener) { _mouseListener = mouseListener; }
		void RemoveMouseListener() { _mouseListener = NULL; }
		SUIMouseListener* GetMouseListener() { return _mouseListener; }

		void AddActionListener(SUIActionListener* actionListener) { _actionListener = actionListener; }
		void RemoveActionListener() { _actionListener = NULL; }
		SUIActionListener* GetActionListener() { return _actionListener; }
	};
}

#endif