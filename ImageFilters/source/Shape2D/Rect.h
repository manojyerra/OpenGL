#ifndef Rect_H
#define Rect_H

class Rect
{
private:
	unsigned int color;

public:
	float x;
	float y;
	float w;
	float h;

	Rect();
	Rect(float x, float y, float w, float h);

	void SetBounds(float x, float y, float w, float h);
	void SetBounds(Rect* rect);
	void SetBoundsByPoints(float x1, float y1, float x2, float y2);
	void FitIntoBoundry(float x, float y, float w, float h);

	unsigned int GetColor();
	void SetColor(unsigned int color);

	bool Contains(float xx, float yy);

	void Draw();
	void DrawWithLines();
};

#endif
