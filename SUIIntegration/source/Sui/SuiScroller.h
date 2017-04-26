#ifndef SUI_SCROLLER_H
#define SUI_SCROLLER_H

#include "SuiRect.h"

class SuiScroller
{
private:
	SuiRect _bg;
	SuiRect _bar;

	bool _enableScroller;
	float _scrollPercent;

	float _viewLength;
	float _totViewLength;

public:
	SuiScroller(float x, float y, float w, float h);
	~SuiScroller();

	void Calc();
	bool IsScrollEnabled();
	void SetViewLength(float viewLength);
	void SetTotalViewLength(float totViewLength);

	float GetX();
	float GetY();
	float GetW();

	bool Contains(float x, float y);
	void SetScrollPercent(float scrollPercent);
	float GetScrollPercent();
	void SetBounds(float x, float y, float w, float h);
	void Move(float dx, float dy);

	void Update();
	void Draw();
};

#endif