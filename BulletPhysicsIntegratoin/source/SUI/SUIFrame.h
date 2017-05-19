#ifndef SUIFrame_H
#define SUIFrame_H

#include "SUIComponent.h"
#include "SUIScroller.h"
#include "SUIRect.h"

#include <vector>
using namespace std;

class SUIFrame : public SUIComponent
{
private:
	int _alignment;
	vector<SUIComponent*> _elementVec;

	SUIRect _titleBarRect;
	SUIRect _dragRect;

	SUIRect _minRect;
	SUIRect _maxRect;
	SUIRect _closeRect;

	bool _removeCloseOption;

	bool _isMinimized;
	bool _isMaximized;
	SUIRect _beforeMaxRect;
	SUIRect _miniIconRect;

	bool _focusGain;



	//Scroller realted...
	SUIScroller* _scroller;
	float _totViewHeight;
	float _scrollAmount;
	void Scroll(float percent);
	static const int SLIDER_WIDTH = 15;

	int _activeBar;
	float _dxOnActive;
	float _dyOnActive;
	bool _resetBoundsCalled;

	void ResetBounds(float x, float y, float w);
	void MoveElements(float dx, float dy);
	void DrawDragger();
	void DrawTitleBar();
	bool IsMinMaxCloseContains(float x, float y);
	void SetBoundsToMinMaxClose();
	void RectifyFrameBounds();
	bool TitleBarContains(float x, float y);
	void DrawMinMaxClose();

	static const int NONE = 0;
	static const int TITLE_BAR = 1;
	static const int SCROLL_BAR = 2;
	static const int DRAGGER = 3;

	static const int TITLE_BAR_HEIGHT = 22;

public:
	SUIFrame(float x, float y, float w, float h, int alignment);
	~SUIFrame();

	void SetFocusOn();
	void SetFocusOnFromManager(bool focus);

	bool IsMinimized();
	bool IsMaximized();

	void SetMinimized(bool val);
	void SetMaximized(bool val);
	void SetRemoveCloseOption(bool remove);

	SUIEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds();
	bool IsResetBoundsCalled();

	void SetBounds(float x, float y, float w, float h);
	SUIComponent* getComponentAt(float x, float y);
	SUIComponent* getComponent(unsigned int index);
	int GetNumComponents();

	void Add(SUIComponent* com);
	void Delete(SUIComponent* com);
	void Draw();
};

#endif
