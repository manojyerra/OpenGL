#include "SUILabel.h"
#include "SUIIncludes.h"
#include "SUIFont.h"
#include "SUIInput.h"
#include "SUIEvents.h"

SUILabel::SUILabel(string name, int nameAlignment) : SUIComponent(SUIComponent::LABEL)
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

SUILabel::~SUILabel()
{
}

void SUILabel::SetFontColor(unsigned int fontColor)
{
	_fontColor = fontColor;
}

void SUILabel::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;
}

void SUILabel::Draw()
{
	if(_isBgVisible)
		DrawBackground();

	SUIFont::GetInstance()->SetColor(_fontColor);
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


SUIEvents SUILabel::UpdateByInput()
{
	SUIEvents eventsVec;
	return eventsVec;
}