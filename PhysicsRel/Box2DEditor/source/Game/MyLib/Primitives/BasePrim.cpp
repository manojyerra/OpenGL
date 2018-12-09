#include "BasePrim.h"

BasePrim::BasePrim()
{
	_color = 0x00ff0099;
	_borderColor = 0x00ff00ff;
	_angle = 0;
}

BasePrim::~BasePrim()
{
}

void BasePrim::SetColor(unsigned int color)
{
	_color = color;
}

void BasePrim::SetColor(int r, int g, int b, int a)
{
	_color = (unsigned int)((r << 24) + (g << 16) + (b << 8) + a);
}

void BasePrim::SetBorderColor(unsigned int color)
{
	_borderColor = color;
}

void BasePrim::SetBorderColor(int r, int g, int b, int a)
{
	_borderColor = (unsigned int)((r << 24) + (g << 16) + (b << 8) + a);
}

unsigned int BasePrim::GetColor() 
{
	return _color;
}

unsigned int BasePrim::GetBorderColor() 
{
	return _borderColor;
}
