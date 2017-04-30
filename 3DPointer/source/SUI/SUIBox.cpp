#include "SUIBox.h"
#include "SUIIncludes.h"

#include "SUIInput.h"
#include "SUIFont.h"
#include "SUIRect.h"


SUIBox::SUIBox(int alignment) : SUIComponent(BOX)
{
	CommonInit(alignment, 0,0,0,0);
}

SUIBox::SUIBox(int alignment, float horMargin, float verMargin) : SUIComponent(BOX)
{
	CommonInit(alignment, horMargin, horMargin, verMargin, verMargin);
}

SUIBox::SUIBox(int alignment, float leftMargin, float rightMargin, float topMargin, float bottomMargin) : SUIComponent(BOX)
{
	CommonInit(alignment, leftMargin, rightMargin, topMargin, bottomMargin);
}

void SUIBox::CommonInit(int alignment, float leftMargin, float rightMargin, float topMargin, float bottomMargin)
{
	_alignment = alignment;
	_elementVec.clear();
	_isOn = true;
	_isOnOffEnable = false;
	_isBorderVisible = true;

	_titleBgR = 180;
	_titleBgG = 180;
	_titleBgB = 180;
	_titleBgA = 255;

	SetMargin(leftMargin, rightMargin, topMargin, bottomMargin);
}

SUIBox::~SUIBox()
{
	for(int i=0; i<(int)_elementVec.size(); i++)
	{
		delete _elementVec[i];
		_elementVec[i] = NULL;
	}
}

void SUIBox::SetOnOffEnable(bool onOffEnable)
{
	_isOnOffEnable = onOffEnable;
}

void SUIBox::SetOn(bool on)
{
	_isOn = on;
}

vector<SUIComponent*> SUIBox::getComponentList()
{
	return _elementVec;
}

SUIComponent* SUIBox::getComponentAt(float x, float y)
{
	if(_isOn)
	{
		if(Contains(x,y) == false)
			return NULL;

		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			if(_elementVec[i]->IsVisible() == false)
				continue;

			if(_elementVec[i]->GetComponentID() == SUIComponent::BOX)
			{
				SUIComponent* com = ((SUIBox*)_elementVec[i])->getComponentAt(x,y);
				if(com != NULL)
					return com;
			}
			else if(_elementVec[i]->Contains(x,y))
				return _elementVec[i];
		}

		return this;
	}
	else
	{
		if(x >= _x && x <= _x+_w && y >= _y && y <= _y+TITLE_H)
			return this;
	}

	return NULL;
}

SUIEvents SUIBox::UpdateByInput()
{
	SUIEvents eventsVec;

	if(_isOnOffEnable && SUIInput::IsMouseClicked())
	{
		float mx = (float)SUIInput::MX;
		float my = (float)SUIInput::MY;

		if(mx >= _x && mx <= _x+_w && my >= _y && my <= _y+TITLE_H)
			_isOn = !_isOn;
	}

	return eventsVec;
}

void SUIBox::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;

	for(int i=0; i<(int)_elementVec.size(); i++)
		_elementVec[i]->Move(dx, dy);
}

void SUIBox::ResetBounds()
{
	_h = 0;

	if(_isOnOffEnable || _name.length() > 0)
	{
		_h += TITLE_H;
		if(!_isOn)
			return;
	}

	if(_alignment == H_ALIGNMENT)
	{
		float eachBoxW = _w / _elementVec.size();

		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			if(_elementVec[i]->IsVisible() == false)
				continue;

			float xx = _x+i*eachBoxW + _elementVec[i]->LeftMargin();
			float yy = _y			 + _elementVec[i]->TopMargin();
			float ww = eachBoxW		 - (_elementVec[i]->LeftMargin() + _elementVec[i]->RightMargin());

			_elementVec[i]->SetPos(xx, yy);
			_elementVec[i]->SetWidth(ww);
			_elementVec[i]->ResetBounds();

			float endY = _elementVec[i]->GetH()+_elementVec[i]->TopMargin()+_elementVec[i]->BottomMargin();

			if(endY > _h)
				_h = endY;
		}
	}
	else if(_alignment == V_ALIGNMENT)
	{
		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			if(_elementVec[i]->IsVisible() == false)
				continue;

			float xx = _x + _elementVec[i]->LeftMargin();
			float yy = _y + _elementVec[i]->TopMargin();
			float ww = _w - (_elementVec[i]->LeftMargin() + _elementVec[i]->RightMargin());

			_elementVec[i]->SetPos(xx,yy+_h);
			_elementVec[i]->SetWidth(ww);
			_elementVec[i]->ResetBounds();

			_h += _elementVec[i]->GetH()+_elementVec[i]->TopMargin()+_elementVec[i]->BottomMargin();
		}
	}
}

void SUIBox::AddBox(SUIBox* box)
{
	_elementVec.push_back(box);
}

void SUIBox::AddButton(SUIButton* button)
{
	_elementVec.push_back(button);
}

void SUIBox::AddLabel(SUILabel* label)
{
	_elementVec.push_back(label);
}

void SUIBox::AddCheckBox(SUICheckBox* checkBox)
{
	_elementVec.push_back(checkBox);
}

void SUIBox::AddRadioButton(SUIRadioButton* radioButton)
{
	_elementVec.push_back(radioButton);
}

void SUIBox::AddSlider(SUISlider* slider)
{
	_elementVec.push_back(slider);
}

void SUIBox::AddChoice(SUIChoice* choice)
{
	_elementVec.push_back(choice);
}

void SUIBox::AddList(SUIList* list)
{
	_elementVec.push_back(list);
}

void SUIBox::Draw()
{
	if(!_visible)
		return;

	if(_isBgVisible)
		DrawBackground();

	if(_isOnOffEnable || _name.length() > 0)
		SUIRect::Draw(_x, _y, _w, (float)TITLE_H, _titleBgR, _titleBgG, _titleBgB, _titleBgA, false);

	if(_isOnOffEnable)
	{
		float triCY = _y + TITLE_H/2.0f;
		float triCX = _x + 10.0f;
		float triR = TITLE_H/4.50f;

		if(triCX + triR < _x + _w )		//boundtry
		{
			if(!_isOn)
			{
				glColor4ub(0, 0, 0, 255);
				glBegin(GL_TRIANGLES);
				glVertex2f(triCX-triR,	triCY-triR	);
				glVertex2f(triCX-triR,	triCY+triR	);
				glVertex2f(triCX+triR,	triCY		);
				glEnd();
			}
			else
			{
				glColor4ub(0, 0, 0, 255);
				glBegin(GL_TRIANGLES);
				glVertex2f(triCX-triR,	triCY-triR	);
				glVertex2f(triCX+triR,	triCY-triR	);
				glVertex2f(triCX,		triCY+triR	);
				glEnd();
			}
		}
	}

	if(_name.length() > 0)
	{
		float horGap = -0.5;
		float fontSize = SUIFont::GetInstance()->GetFontSize();

		SUIFont::GetInstance()->HorBorder(_x, _x+_w);

		SUIFont::GetInstance()->SetColor(0xffffffff);
		SUIFont::GetInstance()->Begin();
		SUIFont::GetInstance()->DrawFromLeft(_name, _x+TITLE_H, _y+TITLE_H/2.0f, fontSize);
		SUIFont::GetInstance()->End();
	}

	if(_isOn)
	{
		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			if(_elementVec[i]->IsVisible() == false)
				continue;
			_elementVec[i]->Draw();
		}
	}

	if(_isBorderVisible)
		DrawBorder();
}
