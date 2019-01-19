#ifndef SUIButton_H
#define SUIButton_H

#include "SUIComponent.h"
#include "SUIActionListener.h"
#include "SUIMouseListener.h"

namespace SUI
{
	class SUIButton : public SUIComponent
	{
	private:
		SUIActionListener* _actionListener;
		SUIMouseListener* _mouseListener;

	public:
		SUIButton(string name);
		SUIButton(string name, int nameAlignment);
		SUIButton(string name, SUIActionListener* actionListener);

		~SUIButton();

		void InitGlobas();
		SUIEvents UpdateByInput();
		void Move(float dx, float dy);
		void ResetBounds();
		void Draw();

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