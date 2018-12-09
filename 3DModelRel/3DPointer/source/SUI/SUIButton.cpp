#include "SUIButton.h"
#include "SUIFont.h"
#include "SUIInput.h"

SUIButton::SUIButton(string name) : SUIComponent(SUIComponent::BUTTON)
{
	Init(name, SUIComponent::CENTER, NULL);
}

SUIButton::SUIButton(string name, int nameAlignment) : SUIComponent(SUIComponent::BUTTON)
{
	Init(name, nameAlignment, NULL);
}

SUIButton::SUIButton(string name, SUIActionListener* actionListener) : SUIComponent(SUIComponent::BUTTON)
{
	Init(name, SUIComponent::CENTER, actionListener);
}

SUIButton::SUIButton(string name, int nameAlignment, SUIActionListener* actionListener) : SUIComponent(SUIComponent::BUTTON)
{
	Init(name, nameAlignment, actionListener);
}

void SUIButton::Init(string name, int nameAlignment, SUIActionListener* actionListener)
{
	_name = name;
	_nameAlignment = nameAlignment;
	_actionListener = actionListener;

	_h = 22;

	_isBgVisible = true;
	_isBorderVisible = true;
	_isBgGradient = true;

	_bgR = 170;
	_bgG = 170;
	_bgB = 170;

	_mouseListener = NULL;
}

SUIButton::~SUIButton()
{
}

void SUIButton::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;
}

void SUIButton::ResetBounds()
{
}

void SUIButton::Draw()
{
	if(_isBgVisible)
	{
		if(SUIInput::IsMousePressed() && Contains((float)SUIInput::MX, (float)SUIInput::MY))
			DrawBackground(true);
		else
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

SUIEvents SUIButton::UpdateByInput()
{
	SUIEvents eventsVec;

	if(!Contains((float)SUIInput::MX, (float)SUIInput::MY))
		return eventsVec;

	if(_actionListener && SUIInput::IsMouseReleased())
		eventsVec.ACTION_PERFORMED = true;

	return eventsVec;
}
