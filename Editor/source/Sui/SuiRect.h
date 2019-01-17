#ifndef RECTANGLE_H
#define RECTANGLE_H

class SuiRect
{
private:

public:
	unsigned char color[4];
	unsigned char borderColor[4];

	float x;
	float y;
	float w;
	float h;

	SuiRect();
	SuiRect(float X, float Y, float W, float H);
	~SuiRect();

	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetBorderColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	
	void SetBounds(SuiRect* rect);
	void SetBounds(float X, float Y, float W, float H);
	void Move(float dx, float dy);
	bool Contains(float X, float Y);
	bool Intersects(float rectX, float rectY, float rectWidth, float rectHeight);

	void DrawWithBorder();
	void DrawWithoutBorder();
	void DrawBorder();

	static void Draw(float x, float y, float w, float h, int red, int green, int blue, int alpha, bool wireFrame);
};

#endif