#ifndef SUI_LIST_H
#define SUI_LIST_H

#include "SuiComponent.h"
#include "SuiActionListener.h"
#include "SuiMouseListener.h"
#include "SuiLabel.h"
#include "SuiScroller.h"

class SuiList : public SuiComponent
{
private:
	int _contentAlignment;
	vector<SuiLabel*> _elementsVec;
	SuiActionListener* _actionListener;
	SuiMouseListener* _mouseListener;

	int _selectedIndex;
	int _elementsToShow;
	bool _isDisableFrameScissor;

	unsigned char _selectedColor[4];
	unsigned char _nonSelectedColor[4];

	void MoveElements(float dx, float dy);
	void ResetBounds(float x, float y, float w);

	//Scroller realted...
	SuiScroller* _scroller;
	float _totViewHeight;
	float _scrollAmount;
	bool _scrollActive;
	static const int SLIDER_WIDTH = 15;
	void Scroll(float percent);
	//////////////////////

public:
	SuiList(int elementsToShow);
	~SuiList();

	void SetSelectColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetNonSelectColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetSelect(int index);
	int GetSelectedIndex();
	string GetSelectedStr();
	void Move(float dx, float dy);
	void ResetBounds();
	void Add(string name);
	SuiEvents UpdateByInput();
	void Draw();

	string GetItem(int index);
	int Size();
	bool Contains(float x, float y);

	void DisableScissor();

	//Listeners...

	void AddActionListener(SuiActionListener* actionListener)		{ _actionListener = actionListener; }
	void RemoveActionListener()										{ _actionListener = NULL;			}
	SuiActionListener* GetActionListener()							{ return _actionListener;			}
};

#endif