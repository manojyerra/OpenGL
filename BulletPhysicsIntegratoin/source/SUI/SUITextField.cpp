#include "SUITextField.h"
#include "SUIFont.h"
#include "SUIInput.h"

SUITextField::SUITextField(string name) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, SUIComponent::CENTER, NULL);
}

SUITextField::SUITextField(string name, int nameAlignment) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, nameAlignment, NULL);
}

SUITextField::SUITextField(string name, SUIActionListener* actionListener) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, SUIComponent::CENTER, actionListener);
}

SUITextField::SUITextField(string name, int nameAlignment, SUIActionListener* actionListener) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, nameAlignment, actionListener);
}

void SUITextField::Init(string name, int nameAlignment, SUIActionListener* actionListener)
{
	_name = name;
	_nameAlignment = nameAlignment;
	_actionListener = actionListener;

	_h = 22;

	_isBgVisible = true;
	_isBorderVisible = true;
	_isBgGradient = false;

	_bgR = 171;
	_bgG = 171;
	_bgB = 171;
	_bgA = 255;

	_borderR = 57;
	_borderG = 57;
	_borderB = 57;
	_borderA = 255;

	_mouseListener = NULL;
}

SUITextField::~SUITextField()
{
}

void SUITextField::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;
}

void SUITextField::ResetBounds()
{
}

void SUITextField::Draw()
{
	if(_isBgVisible)
	{
		//if(SUIInput::IsMousePressed() && Contains((float)SUIInput::MX, (float)SUIInput::MY))
		//	DrawBackground(true);
		//else
			DrawBackground(false);
	}

	SUIFont::GetInstance()->SetColor(0xffffffff);
	SUIFont::GetInstance()->Begin();

	SUIFont::GetInstance()->HorBorder(_x, _x+_w);

	float horGap = -0.5;
	float fontSize = SUIFont::GetInstance()->GetFontSize();

	if(_nameAlignment == LEFT)			SUIFont::GetInstance()->DrawFromLeft(_name, _x+1, _y+_h/2, fontSize);
	else if(_nameAlignment == RIGHT)	SUIFont::GetInstance()->DrawFromRight(_name, _x+_w+1, _y+_h/2, fontSize);
	else								SUIFont::GetInstance()->DrawFromCenter(_name, _x+1+_w/2, _y+_h/2, fontSize);

	SUIFont::GetInstance()->End();

	if(_isBorderVisible)
		DrawBorder();
}

SUIEvents SUITextField::UpdateByInput()
{
	SUIEvents eventsVec;

	if(!Contains((float)SUIInput::MX, (float)SUIInput::MY))
		return eventsVec;

	if(_actionListener && SUIInput::GetReleasedKey() != 0)
	{
		eventsVec.ACTION_PERFORMED = true;
		_name += SUIInput::GetReleasedKey();
	}

	return eventsVec;
}
