#include "SuiLabel.h"
#include "SuiIncludes.h"
#include "SuiFont.h"
#include "SuiInput.h"
#include "SuiEvents.h"

SuiLabel::SuiLabel(string name, int nameAlignment) : SuiComponent(SuiComponent::LABEL)
{
	_name = name;
	_nameAlignment = nameAlignment;
	_h = 22;

	_bgR = _bgR*0.8;
	_bgG = _bgG*0.8;
	_bgB = _bgB*0.8;

	_mouseListener = NULL;
	_isBgVisible = false;
	_isBorderVisible = true;
	_fontColor = 0xffffffff;
}

SuiLabel::~SuiLabel()
{
}

void SuiLabel::SetFontColor(unsigned int fontColor)
{
	_fontColor = fontColor;
}

void SuiLabel::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;
}

void SuiLabel::Draw()
{
	if(_isBgVisible)
		DrawBackground();

	SuiFont::GetInstance()->SetColor(_fontColor);
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


SuiEvents SuiLabel::UpdateByInput()
{
	SuiEvents eventsVec;
	return eventsVec;
}