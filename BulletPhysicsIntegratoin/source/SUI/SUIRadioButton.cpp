#include "SUIRadioButton.h"
#include "SUIIncludes.h"
#include "SUIInput.h"
#include "SUIEvents.h"

SUIRadioButton::SUIRadioButton(int contentAlignment) : SUIComponent(RADIO_BUTTON)
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

SUIRadioButton::~SUIRadioButton()
{
	for(int i=0; i<(int)_elementVec.size(); i++)
	{
		delete _elementVec[i];
		_elementVec[i] = NULL;
	}
}

void SUIRadioButton::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;

	for(int i=0; i<(int)_elementVec.size(); i++)
		_elementVec[i]->Move(dx, dy);
}

void SUIRadioButton::ResetBounds()
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


void SUIRadioButton::AddCheckBox(SUICheckBox* checkBox)
{
	checkBox->SetBoxType(SUICheckBox::BOX_CIRCLE);
	_elementVec.push_back(checkBox);
}

void SUIRadioButton::Draw()
{
	if(_isBgVisible)
		DrawBackground();

	for(int i=0; i<(int)_elementVec.size(); i++)
		_elementVec[i]->Draw();

	if(_isBorderVisible)
		DrawBorder();
}

void SUIRadioButton::SetShortCut(SUIKeyCombi keyCombi)
{
	_keyCombi = keyCombi;
}

SUIEvents SUIRadioButton::UpdateByInput()
{
	SUIEvents eventsVec;

	bool keyEventFired = IsKeyEventFired();

	if(keyEventFired)
	{
		int unSelIndex = GetSelectedIndex();
		int selIndex = unSelIndex+1;

		if(selIndex >= _elementVec.size())
			selIndex = 0;

		SetSelect(selIndex);

		if(_actionListener)
			eventsVec.ACTION_PERFORMED = true;

		return eventsVec;
	}

	if(!Contains((float)SUIInput::MX, (float)SUIInput::MY))
		return eventsVec;

	if(SUIInput::IsMouseClicked())
	{
		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			if(_elementVec[i]->Contains((float)SUIInput::MX, (float)SUIInput::MY) && !_elementVec[i]->IsSelected())
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

void SUIRadioButton::SetSelect(int index)
{
	if(index >= 0 && index < (int)_elementVec.size())
	{
		for(int i=0; i<(int)_elementVec.size(); i++)
			_elementVec[i]->SetSelect(false);
		
		_elementVec[index]->SetSelect(true);
		_selectedIndex = index;
	}
}

int SUIRadioButton::GetSelectedIndex()
{
	return _selectedIndex;
}
