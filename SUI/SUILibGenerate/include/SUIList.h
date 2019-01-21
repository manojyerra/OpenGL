#ifndef SUIList_H
#define SUIList_H

#include "SUIComponent.h"
#include "SUIActionListener.h"
#include "SUIMouseListener.h"
#include "SUILabel.h"
#include "SUIScroller.h"

namespace SUI
{
	class SUIList : public SUIComponent
	{
	private:
		int _contentAlignment;
		vector<SUILabel*> _elementsVec;
		SUIActionListener* _actionListener;
		SUIMouseListener* _mouseListener;

		int _selectedIndex;
		int _elementsToShow;
		bool _isDisableFrameScissor;

		unsigned char _selectedColor[4];
		unsigned char _nonSelectedColor[4];

		void MoveElements(float dx, float dy);
		void ResetBounds(float x, float y, float w);

		//Scroller realted...
		SUIScroller* _scroller;
		float _totViewHeight;
		float _scrollAmount;
		bool _scrollActive;
		static const int SLIDER_WIDTH = 15;
		void Scroll(float percent);
		//////////////////////

	public:
		SUIList(int elementsToShow);
		~SUIList();

		void SetSelectColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		void SetNonSelectColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		void SetSelect(int index);
		int GetSelectedIndex();
		string GetSelectedStr();
		void Move(float dx, float dy);
		void ResetBounds();
		void Add(string name);
		SUIEvents UpdateByInput();
		void Draw();

		string GetItem(int index);
		int Size();
		bool Contains(float x, float y);

		void DisableScissor();

		//Listeners...

		void AddActionListener(SUIActionListener* actionListener) { _actionListener = actionListener; }
		void RemoveActionListener() { _actionListener = NULL; }
		SUIActionListener* GetActionListener() { return _actionListener; }
	};
}

#endif