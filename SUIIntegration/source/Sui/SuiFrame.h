#ifndef FRAME_H
#define FRAME_H

#include "SuiComponent.h"
#include "SuiScroller.h"
#include "SuiRect.h"

#include <vector>
using namespace std;

class SuiFrame : public SuiComponent
{
private:
	int _alignment;
	vector<SuiComponent*> _elementVec;

	SuiRect _titleBarRect;
	SuiRect _dragRect;

	SuiRect _minRect;
	SuiRect _maxRect;
	SuiRect _closeRect;

	bool _removeCloseOption;

	bool _isMinimized;
	bool _isMaximized;
	SuiRect _beforeMaxRect;
	SuiRect _miniIconRect;

	bool _focusGain;



	//Scroller realted...
	SuiScroller* _scroller;
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
	SuiFrame(float x, float y, float w, float h, int alignment);
	~SuiFrame();

	void SetFocusOn();
	void SetFocusOnFromManager(bool focus);

	bool IsMinimized();
	bool IsMaximized();

	void SetMinimized(bool val);
	void SetMaximized(bool val);
	void SetRemoveCloseOption(bool remove);

	SuiEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds();
	bool IsResetBoundsCalled();

	void SetBounds(float x, float y, float w, float h);
	SuiComponent* getComponentAt(float x, float y);
	SuiComponent* getComponent(int index);
	int GetNumComponents();

	void Add(SuiComponent* com);
	void Draw();
};

#endif