#ifndef COLORBLEND_H
#define COLORBLEND_H

class ColorBlend
{
private:
	unsigned int _color1;
	unsigned int _color2;
	unsigned int _currColor;

	int _r1;
	int _g1;
	int _b1;

	int _r2;
	int _g2;
	int _b2;
	
	float _timeCount;
	
	unsigned int RandColor();

public:

	ColorBlend();
	~ColorBlend();
	
	void Update();
	unsigned int GetColor();
	
	int GerR();
	int GerG();
	int GetB();
};

#endif
