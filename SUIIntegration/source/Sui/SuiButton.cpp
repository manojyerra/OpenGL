#include "SuiButton.h"
#include "SuiFont.h"
#include "SuiInput.h"

SuiButton::SuiButton(string name) : SuiComponent(SuiComponent::BUTTON)
{
	_name = name;
	_nameAlignment = SuiComponent::CENTER;
	InitGlobas();
}

SuiButton::SuiButton(string name, int nameAlignment) : SuiComponent(SuiComponent::BUTTON)
{
	_name = name;
	_nameAlignment = nameAlignment;
	InitGlobas();
}

SuiButton::SuiButton(string name, SuiActionListener* actionListener) : SuiComponent(SuiComponent::BUTTON)
{
	_name = name;
	_nameAlignment = SuiComponent::CENTER;
	InitGlobas();
	AddActionListener(actionListener);
}

void SuiButton::InitGlobas()
{
	_h = 22;

	_isBgVisible = true;
	_isBorderVisible = true;
	_isBgGradient = true;

	_bgR = 170;
	_bgG = 170;
	_bgB = 170;

	_actionListener = NULL;
	_mouseListener = NULL;
}

SuiButton::~SuiButton()
{
}

void SuiButton::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;
}

void SuiButton::ResetBounds()
{
}

void SuiButton::Draw()
{
	if(_isBgVisible)
	{
		if(SuiInput::IsMousePressed() && Contains(SuiInput::MX, SuiInput::MY))
			DrawBackground(true);
		else
			DrawBackground(false);
	}

	SuiFont::GetInstance()->SetColor(0xffffffff);
	SuiFont::GetInstance()->Begin();

	SuiFont::GetInstance()->HorBorder(_x, _x+_w);

	float horGap = -0.5;
	float fontSize = SuiFont::GetInstance()->GetFontSize();

	if(_nameAlignment == LEFT)			SuiFont::GetInstance()->DrawFromLeft(_name, _x+1, _y+_h/2, fontSize);
	else if(_nameAlignment == RIGHT)	SuiFont::GetInstance()->DrawFromRight(_name, _x+_w+1, _y+_h/2, fontSize);
	else								SuiFont::GetInstance()->DrawFromCenter(_name, _x+1+_w/2, _y+_h/2, fontSize);

	SuiFont::GetInstance()->End();

	if(_isBorderVisible)
		DrawBorder();
}

SuiEvents SuiButton::UpdateByInput()
{
	SuiEvents eventsVec;

	if(!Contains((float)SuiInput::MX, (float)SuiInput::MY))
		return eventsVec;

	if(_actionListener && SuiInput::IsMouseReleased())
		eventsVec.ACTION_PERFORMED = true;

	return eventsVec;
}
