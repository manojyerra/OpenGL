#include "SUISlider.h"
#include "SUIIncludes.h"
#include "SUIFont.h"
#include "SUIEvents.h"
#include "SUIInput.h"

SUISlider::SUISlider(string name, float minValue, float maxValue, bool isIntegerValue, SUIActionListener* actionListener) : SUIComponent(SUIComponent::SLIDER)
{
	Init(name, minValue, maxValue, isIntegerValue, actionListener);
}

SUISlider::SUISlider(string name, float minValue, float maxValue, bool isIntegerValue) : SUIComponent(SUIComponent::SLIDER)
{
	Init(name, minValue, maxValue, isIntegerValue, NULL);
}

void SUISlider::Init(string name, float minValue, float maxValue, bool isIntegerValue, SUIActionListener* actionListener)
{
	_name = name;
	_minValue = minValue;
	_maxValue = maxValue;
	_valueBoxRect.SetBounds(0,0,0,0);
	_valueBoxRect.SetColor(255,122,17,255);

	_sliderBgRect.SetBounds(0,0,0,0);
	_sliderBgRect.SetColor(255,3,98,255);

	_sliderBgLineRect.SetBounds(0,0,0,0);
	_sliderBgLineRect.SetColor(100,100,100,255);

	_pointerRect.SetBounds(0,0,12,0);
	_pointerRect.SetColor(130,130,255,255);

	_pointerX = 0;
	_pointerActive = false;

	SetValue((_minValue + _maxValue)/2.0f);

	_isIntegerValue = isIntegerValue;
	_valueAsString = "";
	
	_h = 22;

	_isBgVisible = false;
	_isBorderVisible = true;

	_bgR = 171;
	_bgG = 171;
	_bgB = 171;
	_bgA = 255;

	_borderR = 64;
	_borderG = 64;
	_borderB = 64;
	_borderA = 255;

	_actionListener = actionListener;
	_mouseListener = NULL;
	//_isBgGradient = true;
}

SUISlider::~SUISlider()
{
}

void SUISlider::SetMinValue(float minValue)
{
	_minValue = minValue;
}

void SUISlider::SetMaxValue(float maxValue)
{
	_maxValue = maxValue;
}

void SUISlider::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;

	_valueBoxRect.Move(dx, dy);
	_valueBoxRect.Move(dx, dy);

	_sliderBgRect.Move(dx, dy);
	_sliderBgRect.Move(dx, dy);

	_sliderBgLineRect.Move(dx, dy);
	_sliderBgLineRect.Move(dx, dy);

	_pointerRect.Move(dx, dy);
}

void SUISlider::ResetBounds()
{
	float fontSize = SUIFont::GetInstance()->GetFontSize();
	float nameLen = SUIFont::GetInstance()->GetLength(_name, fontSize);

	int strLen = 0;
	if(_isIntegerValue)
	{
		char arr[512];
		sprintf(arr, " %d", (int)_maxValue);
		strLen = strlen(arr);
	}

	float valueBoxLen = 0;//strLen * fontSize;
	float sliderLen = _w - (nameLen + valueBoxLen);

	_valueBoxRect.SetBounds(_x + nameLen+sliderLen, _y, valueBoxLen, _h);

	_sliderBgRect.SetBounds(_x + nameLen, _y, sliderLen, _h);
	_sliderBgLineRect.SetBounds(_x + nameLen, _y+(_h/2)-1, sliderLen, 2.0f);

	SetValue(_currValue);
	CalcPointerPos();
}

void SUISlider::SetValue(double value)
{
	_currValue = value;
	if(_currValue < _minValue)
		_currValue = _minValue;
	else if(_currValue > _maxValue)
		_currValue = _maxValue;

	char arr[512];
	
	if(_isIntegerValue)
		sprintf(arr, "%d", (int)_currValue);
	else
		sprintf(arr, "%.2f", (float)_currValue);

	_valueAsString = string(arr);
}

void SUISlider::CalcPointerPos()
{
	float totDiff = (float)(_maxValue - _minValue);
	float currDiff = (float)(_currValue - _minValue);
	float movedInPixels = currDiff * _sliderBgRect.w / totDiff;
	_pointerX = _sliderBgRect.x + movedInPixels;
	_pointerRect.SetBounds(_pointerX - _pointerRect.w/2.0f, _sliderBgRect.y, _pointerRect.w, _sliderBgRect.h);
}

void SUISlider::SetPointerAt(double posInPercent)
{
	float totVal = (float)(_maxValue - _minValue);
	float currVal = (float)(_minValue + ( posInPercent * totVal / 100.0f));
	SetValue((double)currVal);
}

double SUISlider::GetValue()
{
	if(_isIntegerValue)
		return (int)_currValue;

	return _currValue;
}

void SUISlider::Draw()
{
	if(_isBgVisible)
		DrawBackground();

	CalcPointerPos();

	SUIFont::GetInstance()->SetColor(0xffffffff);
	SUIFont::GetInstance()->HorBorder(_x, _x+_w);
	float fontSize = SUIFont::GetInstance()->GetFontSize();

	SUIRect::Draw(_x, _y, SUIFont::GetInstance()->GetLength(_name, fontSize), _h, 128,122,128,255, false);

	SUIFont::GetInstance()->Begin();
	SUIFont::GetInstance()->DrawFromLeft(_name, _x+1, _y+_h/2, fontSize);
	SUIFont::GetInstance()->End();

	_valueBoxRect.DrawWithoutBorder();

	SUIFont::GetInstance()->Begin();
	SUIFont::GetInstance()->DrawFromRight(_valueAsString, _valueBoxRect.x-1, _valueBoxRect.y+_h/2, fontSize);
	SUIFont::GetInstance()->End();

	//_sliderBgRect.DrawWithoutBorder();
	_sliderBgLineRect.DrawWithoutBorder();
	_pointerRect.DrawWithBorder();

	if(_isBorderVisible)
		DrawBorder();
}

SUIEvents SUISlider::UpdateByInput()
{
	SUIEvents eventsVec;

	if(!Contains((float)SUIInput::MX, (float)SUIInput::MY))
		return eventsVec;

	if(SUIInput::IsMouseClicked() && Contains((float)SUIInput::MX, (float)SUIInput::MY))
		_pointerActive = true;
	else if(SUIInput::IsMouseReleased())
		_pointerActive = false;

	if(_pointerActive && SUIInput::IsMousePressed() && Contains((float)SUIInput::MX, (float)SUIInput::MY))
	{
		float prevCurrVal = (float)_currValue;

		_pointerRect.x = SUIInput::MX - _pointerRect.w/2.0f;

		float totDist = _sliderBgRect.w;
		float movedDist = SUIInput::MX - _sliderBgRect.x;
		SetPointerAt( movedDist*100.0f / totDist );

		if(_actionListener)
		{
			if(_isIntegerValue)
			{
				if((int)prevCurrVal != (int)_currValue)
					eventsVec.ACTION_PERFORMED = true;
			}
			else
			{
				if((float)prevCurrVal != (float)_currValue)
					eventsVec.ACTION_PERFORMED = true;
			}

		}
	}

	return eventsVec;
}
