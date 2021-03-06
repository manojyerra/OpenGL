#include "SuiSlider.h"
#include "SuiIncludes.h"
#include "SuiFont.h"
#include "SuiEvents.h"
#include "SuiInput.h"

SuiSlider::SuiSlider(string name, float minValue, float maxValue, bool isIntegerValue) : SuiComponent(SuiComponent::SLIDER)
{
	_name = name+": ";
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

	_actionListener = NULL;
	_mouseListener = NULL;
	//_isBgGradient = true;
}

SuiSlider::~SuiSlider()
{
}

void SuiSlider::SetMinValue(float minValue)
{
	_minValue = minValue;
}

void SuiSlider::SetMaxValue(float maxValue)
{
	_maxValue = maxValue;
}

void SuiSlider::Move(float dx, float dy)
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

void SuiSlider::ResetBounds()
{
	float fontSize = SuiFont::GetInstance()->GetFontSize();
	float nameLen = SuiFont::GetInstance()->GetLength(_name, fontSize);

	int strLen = 0;
	if(_isIntegerValue)
	{
		char arr[512];
		sprintf(arr, " %d", (int)_maxValue);
		strLen = strlen(arr);
	}

	float valueBoxLen = strLen * fontSize;
	float sliderLen = _w - (nameLen + valueBoxLen);

	_valueBoxRect.SetBounds(_x + nameLen+sliderLen, _y, valueBoxLen, _h);

	_sliderBgRect.SetBounds(_x + nameLen, _y, sliderLen, _h);
	_sliderBgLineRect.SetBounds(_x + nameLen, _y+(_h/2)-1, sliderLen, 2.0f);

	SetValue(_currValue);
	CalcPointerPos();
}

void SuiSlider::SetValue(double value)
{
	_currValue = value;
	if(_currValue < _minValue)
		_currValue = _minValue;
	else if(_currValue > _maxValue)
		_currValue = _maxValue;

	char arr[512];
	sprintf(arr, " %d", (int)_currValue);
	_valueAsString = string(arr);
}

void SuiSlider::CalcPointerPos()
{
	float totDiff = _maxValue - _minValue;
	float currDiff = _currValue - _minValue;
	float movedInPixels = currDiff * _sliderBgRect.w / totDiff;
	_pointerX = _sliderBgRect.x + movedInPixels;
	_pointerRect.SetBounds(_pointerX - _pointerRect.w/2.0f, _sliderBgRect.y, _pointerRect.w, _sliderBgRect.h);
}

void SuiSlider::SetPointerAt(double posInPercent)
{
	float totVal = _maxValue - _minValue;
	float currVal = _minValue + ( posInPercent * totVal / 100.0f);
	SetValue(currVal);
}

double SuiSlider::GetValue()
{
	if(_isIntegerValue)
		return (int)_currValue;

	return _currValue;
}

void SuiSlider::Draw()
{
	if(_isBgVisible)
		DrawBackground();

	CalcPointerPos();

	SuiFont::GetInstance()->SetColor(0xffffffff);
	SuiFont::GetInstance()->HorBorder(_x, _x+_w);
	float fontSize = SuiFont::GetInstance()->GetFontSize();

	SuiRect::Draw(_x, _y, SuiFont::GetInstance()->GetLength(_name, fontSize), _h, 255,122,17,255, false);

	SuiFont::GetInstance()->Begin();
	SuiFont::GetInstance()->DrawFromLeft(_name, _x+1, _y+_h/2, fontSize);
	SuiFont::GetInstance()->End();

	_valueBoxRect.DrawWithoutBorder();

	SuiFont::GetInstance()->Begin();
	SuiFont::GetInstance()->DrawFromLeft(_valueAsString, _valueBoxRect.x+1, _valueBoxRect.y+_h/2, fontSize);
	SuiFont::GetInstance()->End();

	//_sliderBgRect.DrawWithoutBorder();
	_sliderBgLineRect.DrawWithoutBorder();
	_pointerRect.DrawWithBorder();

	if(_isBorderVisible)
		DrawBorder();
}

SuiEvents SuiSlider::UpdateByInput()
{
	SuiEvents eventsVec;

	if(!Contains((float)SuiInput::MX, (float)SuiInput::MY))
		return eventsVec;

	if(SuiInput::IsMouseClicked() && Contains(SuiInput::MX, SuiInput::MY))
		_pointerActive = true;
	else if(SuiInput::IsMouseReleased())
		_pointerActive = false;

	if(_pointerActive && SuiInput::IsMousePressed() && Contains(SuiInput::MX, SuiInput::MY))
	{
		float prevCurrVal = _currValue;

		_pointerRect.x = SuiInput::MX - _pointerRect.w/2.0f;

		float totDist = _sliderBgRect.w;
		float movedDist = SuiInput::MX - _sliderBgRect.x;
		SetPointerAt( movedDist*100.0f / totDist );

		if(_actionListener)
		{
			if(_isIntegerValue)
			{
				if((int)prevCurrVal != (int)_currValue)
					eventsVec.ACTION_PERFORMED = true;
			}
		}
	}

	return eventsVec;
}
