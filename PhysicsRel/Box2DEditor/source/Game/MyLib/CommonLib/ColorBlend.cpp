#include "ColorBlend.h"
#include "GameInfo.h"
#include "Utils.h"

ColorBlend::ColorBlend()
{
	_color1 = RandColor();
	_color2 = RandColor();
	
	_currColor = _color1;
	
    _r1	= (_color1 >> 24) & 255;
	_g1	= (_color1 >> 16) & 255;
	_b1	= (_color1 >> 8) & 255;

    _r2	= (_color2 >> 24) & 255;
	_g2	= (_color2 >> 16) & 255;
	_b2	= (_color2 >> 8) & 255;
	
	_timeCount = 0.0f;
}

ColorBlend::~ColorBlend()
{
}

void ColorBlend::Update()
{
	_timeCount += GameInfo::deltaTime;
	
	if(_timeCount > 1.0f)
	{
		_timeCount = _timeCount - 1.0f;
		
		_color1 = _color2;
		_r1 = _r2;
		_g1 = _g2;
		_b1 = _b2;
		
		_color2 = RandColor();
	    _r2	= (_color2 >> 24) & 255;
		_g2	= (_color2 >> 16) & 255;
		_b2	= (_color2 >> 8) & 255;
	}
	
	int r = _r1+ (_r2-_r1)*_timeCount;
	int g = _g1+ (_g2-_g1)*_timeCount;
	int b = _b1+ (_b2-_b1)*_timeCount;
	
	_currColor = (unsigned int)( (r<<24) + (g<<16) + (b<<8) + 255 );
}

unsigned int ColorBlend::GetColor()
{
	return _currColor;
}

int ColorBlend::GerR()
{
	return ((_currColor >> 24) & 255);
}

int ColorBlend::GerG()
{
	return ((_currColor >> 16) & 255);
}

int ColorBlend::GetB()
{
	return ((_currColor >> 8) & 255);
}

unsigned int ColorBlend::RandColor()
{
	int r = 25 + absInt(randInt()%200);
	int g = 25 + absInt(randInt()%200);
	int b = 25 + absInt(randInt()%200);
	
	int val = absInt(randInt()%10);
	
	if(val == 0)
	{
		r = 0xAA;
		g = 0x34;
		b = 0xF2;		
	}
	else if(val == 1)
	{
		r = 0xFD;
		g = 0xD5;
		b = 0x1C;
	}	
	else if(val == 2)
	{
		r = 0xFE;
		g = 0xD9;
		b = 0x3C;
	}
	else if(val == 3)
	{
		r = 0xEF;
		g = 0x65;
		b = 0x52;
	}
	else if(val == 4)
	{
		r = 0x75;
		g = 0xF8;
		b = 0x79;		
	}
	else if(val == 5)
	{
		r = 0x71;
		g = 0x9C;
		b = 0xFF;
	}
	else if(val == 6)
	{
		r = 0xFB;
		g = 0x71;
		b = 0xE7;
	}
	else if(val == 7)
	{
		r = 255;
		g = 165;
		b = 0;
	}
	else if(val == 8)
	{
		r = 0x74;
		g = 0xDE;
		b = 0xE9;
	}
	else if(val == 9)
	{
		r = 0xF1;
		g = 0x43;
		b = 0x27;
	}
	
	return (unsigned int)( (r<<24) + (g<<16) + (b<<8) + 255 );
}
