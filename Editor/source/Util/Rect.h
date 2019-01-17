#ifndef Rect_H
#define Rect_H

class Rect
{
public:
	float x;
	float y;
	float w;
	float h;

	Rect()
	{
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}

	Rect(float x, float y, float w, float h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	bool Contains(float xx, float yy)
	{
		return (xx >= x && xx <= x+w && yy >= y && yy <= y+h);
	}
};

#endif