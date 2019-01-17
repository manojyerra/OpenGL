#include "SuiScroller.h"
#include "SuiIncludes.h"
#include "SuiInput.h"

SuiScroller::SuiScroller(float x, float y, float w, float h)
{
	_bg.SetBounds(x,y,w,h);
	_bar.SetBounds(_bg.x, _bg.y, _bg.w, 0);

	_bg.SetColor(255, 0, 100, 255);
	_bg.SetBorderColor(0,0,255,128);

	_bar.SetColor(0, 0, 255, 85);

	_viewLength = _bg.h;
	_totViewLength = 0;
	_scrollPercent = 0;
	_enableScroller = false;
}

SuiScroller::~SuiScroller()
{
}

float SuiScroller::GetX() { return _bg.x; }
float SuiScroller::GetY() { return _bg.y; }
float SuiScroller::GetW() { return _bg.w; }

bool SuiScroller::Contains(float x, float y)
{
	return _bg.Contains(x, y);
}

void SuiScroller::Calc()
{
	if(_viewLength <= 0.01 || _totViewLength <= 0.01 || _viewLength >= _totViewLength)
	{
		_enableScroller = false;
		return;
	}

	float totMovable = _bg.h - _bar.h;

	//if(totMovable > 0)
	{
		_enableScroller = true;
		_bar.h = _bg.h*_viewLength / _totViewLength;
		_bar.y = _bg.y + (_scrollPercent * totMovable) / 100;
	}
}

void SuiScroller::SetViewLength(float viewLength)
{
	_viewLength = viewLength;
}

void SuiScroller::SetTotalViewLength(float totViewLength)
{
	_totViewLength = totViewLength;
}

void SuiScroller::SetBounds(float x, float y, float w, float h)
{
	_bg.SetBounds(x,y,w,h);
	_bar.SetBounds(_bg.x, _bar.y, _bg.w, _bar.h);
}

bool SuiScroller::IsScrollEnabled()
{
	return _enableScroller;
}

float SuiScroller::GetScrollPercent()
{
	return _scrollPercent;
}

void SuiScroller::SetScrollPercent(float scrollPercent)
{
	_scrollPercent = scrollPercent;
}

void SuiScroller::Move(float dx, float dy)
{
	_bg.Move(dx, dy);
	_bar.Move(dx, dy);
}

void SuiScroller::Update()
{
	if(_enableScroller)
	{
		if(SuiInput::IsMouseClicked() || SuiInput::IsMousePressed())
		{
			float mx = _bg.x + _bg.w/2.0f;
			float my = (float)SuiInput::MY;

			if(_bg.Contains(mx,my))
			{
				if(_bar.Contains(mx,my))
					_bar.y += SuiInput::MY - SuiInput::PrevMY;
				else
					_bar.y = SuiInput::MY - _bar.h/2.0f;

				if(_bar.y < _bg.y)
					_bar.y = _bg.y;
				else if(_bar.y+_bar.h > _bg.y+_bg.h)
					_bar.y = _bg.y+_bg.h-_bar.h;

				float totMovable = _bg.h - _bar.h;
				float moved = _bar.y - _bg.y;
				if(totMovable == 0)
					_scrollPercent = 0;
				else
					_scrollPercent = moved * 100.0f / totMovable;
			}
		}
	}
}

void SuiScroller::Draw()
{
	if(_enableScroller)
	{
		_bg.DrawWithoutBorder();

		if(_bar.y+_bar.h > _bg.y+_bg.h)	// Just an adjustment ( need to find problem and fix it in proper way.)
		{
			float prevH = _bar.h;
			_bar.h = _bar.h - ( (_bar.y+_bar.h) - (_bg.y+_bg.h) );
			_bar.DrawWithoutBorder();
			_bar.h = prevH;
		}
		else
			_bar.DrawWithoutBorder();

		glLineWidth(1.0f);
		_bg.DrawBorder();
	}
}