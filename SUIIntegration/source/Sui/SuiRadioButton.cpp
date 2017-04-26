#include "SuiRadioButton.h"
#include "SuiIncludes.h"
#include "SuiInput.h"
#include "SuiEvents.h"

SuiRadioButton::SuiRadioButton(int contentAlignment) : SuiComponent(RADIO_BUTTON)
{
	_contentAlignment = contentAlignment;
	_elementVec.clear();

	_isBorderVisible = true;
	_isBgVisible = true;
	_selectedIndex = -1;

	_bgR = 170;
	_bgG = 170;
	_bgB = 170;
	_bgA = 255;

	_borderR = 64;
	_borderG = 64;
	_borderB = 64;
	_borderA = 255;

	_actionListener = NULL;
	_mouseListener = NULL;
}

SuiRadioButton::~SuiRadioButton()
{
	for(int i=0; i<(int)_elementVec.size(); i++)
	{
		delete _elementVec[i];
		_elementVec[i] = NULL;
	}
}

void SuiRadioButton::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;

	for(int i=0; i<(int)_elementVec.size(); i++)
		_elementVec[i]->Move(dx, dy);
}

void SuiRadioButton::ResetBounds()
{
	_h = 0;

	float _horMargin = 0;
	float _verMargin = 0;

	_x += _horMargin;
	_w -= 2*_horMargin;

	_y += _verMargin;

	if(_contentAlignment == H_ALIGNMENT)
	{
		float eachBoxW = _w / _elementVec.size();

		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			_elementVec[i]->SetPos(_x+i*eachBoxW, _y);
			_elementVec[i]->SetWidth(eachBoxW);
			_elementVec[i]->ResetBounds();

			if(_elementVec[i]->GetH() > _h)
				_h = _elementVec[i]->GetH();
		}
	}
	else if(_contentAlignment == V_ALIGNMENT)
	{
		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			_elementVec[i]->SetPos(_x,_y+_h);
			_elementVec[i]->SetWidth(_w);
			_elementVec[i]->ResetBounds();

			_h += _elementVec[i]->GetH();
		}
	}

	_x -= _horMargin;
	_w += 2*_horMargin;

	_y -= _verMargin;
	_h += 2*_verMargin;
}


void SuiRadioButton::AddCheckBox(SuiCheckBox* checkBox)
{
	checkBox->SetBoxType(SuiCheckBox::BOX_CIRCLE);
	_elementVec.push_back(checkBox);
}

void SuiRadioButton::Draw()
{
	if(_isBgVisible)
		DrawBackground();

	for(int i=0; i<(int)_elementVec.size(); i++)
		_elementVec[i]->Draw();

	if(_isBorderVisible)
		DrawBorder();
}

SuiEvents SuiRadioButton::UpdateByInput()
{
	SuiEvents eventsVec;

	if(!Contains((float)SuiInput::MX, (float)SuiInput::MY))
		return eventsVec;

	if(SuiInput::IsMouseClicked())
	{
		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			if(_elementVec[i]->Contains((float)SuiInput::MX, (float)SuiInput::MY) && !_elementVec[i]->IsSelected())
			{
				for(int j=0; j<(int)_elementVec.size(); j++)
					_elementVec[j]->SetSelect(false);
				
				_elementVec[i]->SetSelect(true);
				_selectedIndex = i;

				if(_actionListener)
					eventsVec.ACTION_PERFORMED = true;
				break;
			}
		}
	}

	return eventsVec;
}

void SuiRadioButton::SetSelect(int index)
{
	if(index >= 0 && index < (int)_elementVec.size())
	{
		for(int i=0; i<(int)_elementVec.size(); i++)
			_elementVec[i]->SetSelect(false);
		
		_elementVec[index]->SetSelect(true);
		_selectedIndex = index;
	}
}

int SuiRadioButton::GetSelectedIndex()
{
	return _selectedIndex;
}
