#include "SuiBox.h"
#include "SuiIncludes.h"

#include "SuiInput.h"
#include "SuiFont.h"
#include "SuiRect.h"


SuiBox::SuiBox(int alignment) : SuiComponent(BOX)
{
	CommonInit(alignment, 0,0,0,0);
}

SuiBox::SuiBox(int alignment, float horMargin, float verMargin) : SuiComponent(BOX)
{
	CommonInit(alignment, horMargin, horMargin, verMargin, verMargin);
}

SuiBox::SuiBox(int alignment, float leftMargin, float rightMargin, float topMargin, float bottomMargin) : SuiComponent(BOX)
{
	CommonInit(alignment, leftMargin, rightMargin, topMargin, bottomMargin);
}

void SuiBox::CommonInit(int alignment, float leftMargin, float rightMargin, float topMargin, float bottomMargin)
{
	_alignment = alignment;
	_elementVec.clear();
	_isOn = true;
	_isOnOffEnable = false;

	_titleBgR = 180;
	_titleBgG = 180;
	_titleBgB = 180;
	_titleBgA = 255;

	SetMargin(leftMargin, rightMargin, topMargin, bottomMargin);
}

SuiBox::~SuiBox()
{
	for(int i=0; i<(int)_elementVec.size(); i++)
	{
		delete _elementVec[i];
		_elementVec[i] = NULL;
	}
}

void SuiBox::SetOnOffEnable(bool onOffEnable)
{
	_isOnOffEnable = onOffEnable;
}

void SuiBox::SetOn(bool on)
{
	_isOn = on;
}

vector<SuiComponent*> SuiBox::getComponentList()
{
	return _elementVec;
}

SuiComponent* SuiBox::getComponentAt(float x, float y)
{
	if(_isOn)
	{
		if(Contains(x,y) == false)
			return NULL;

		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			if(_elementVec[i]->IsVisible() == false)
				continue;

			if(_elementVec[i]->GetComponentID() == SuiComponent::BOX)
			{
				SuiComponent* com = ((SuiBox*)_elementVec[i])->getComponentAt(x,y);
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

SuiEvents SuiBox::UpdateByInput()
{
	SuiEvents eventsVec;

	if(_isOnOffEnable && SuiInput::IsMouseClicked())
	{
		float mx = (float)SuiInput::MX;
		float my = (float)SuiInput::MY;

		if(mx >= _x && mx <= _x+_w && my >= _y && my <= _y+TITLE_H)
			_isOn = !_isOn;
	}

	return eventsVec;
}

void SuiBox::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;

	for(int i=0; i<(int)_elementVec.size(); i++)
		_elementVec[i]->Move(dx, dy);
}

void SuiBox::ResetBounds()
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

void SuiBox::AddBox(SuiBox* box)
{
	_elementVec.push_back(box);
}

void SuiBox::AddButton(SuiButton* button)
{
	_elementVec.push_back(button);
}

void SuiBox::AddLabel(SuiLabel* label)
{
	_elementVec.push_back(label);
}

void SuiBox::AddCheckBox(SuiCheckBox* checkBox)
{
	_elementVec.push_back(checkBox);
}

void SuiBox::AddRadioButton(SuiRadioButton* radioButton)
{
	_elementVec.push_back(radioButton);
}

void SuiBox::AddSlider(SuiSlider* slider)
{
	_elementVec.push_back(slider);
}

void SuiBox::AddChoice(SuiChoice* choice)
{
	_elementVec.push_back(choice);
}

void SuiBox::AddList(SuiList* list)
{
	_elementVec.push_back(list);
}

void SuiBox::Draw()
{
	if(!_visible)
		return;

	if(_isBgVisible)
		DrawBackground();

	if(_isOnOffEnable || _name.length() > 0)
		SuiRect::Draw(_x, _y, _w, (float)TITLE_H, _titleBgR, _titleBgG, _titleBgB, _titleBgA, false);

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
		float fontSize = SuiFont::GetInstance()->GetFontSize();

		SuiFont::GetInstance()->HorBorder(_x, _x+_w);

		SuiFont::GetInstance()->SetColor(0xffffffff);
		SuiFont::GetInstance()->Begin();
		SuiFont::GetInstance()->DrawFromLeft(_name, _x+TITLE_H, _y+TITLE_H/2.0f, fontSize);
		SuiFont::GetInstance()->End();
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
