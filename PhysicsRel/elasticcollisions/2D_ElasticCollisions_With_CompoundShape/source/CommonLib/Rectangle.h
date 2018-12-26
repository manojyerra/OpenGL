#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{

public:
	float _x;
	float _y;
	float _width;
	float _height;

	Rectangle();
    ~Rectangle();
	Rectangle(float x, float y, float width, float height);
	void Init(float x, float y, float width, float height);
	
	void SetBounds(Rectangle* rect);
	void SetBounds(float x, float y, float width, float height);

	bool Contains(Rectangle* rect);
	bool Contains(float X, float Y, float W, float H);
	bool Contains(float X, float Y);
	static bool Contains(float posX, float posY, float rectX, float rectY, float rectW, float rectH);


	bool Intersects(Rectangle* rect);
	bool Intersects(float xx, float yy, float ww, float hh) ;
	
	float GetX() {return(_x);}
	float GetY() {return(_y);}

	float GetWidth() {return(_width);}
	float GetHeight() {return(_height);}

	void SetX(float x){_x = x;}
	void SetY(float y){_y = y;}

	void SetWidth(float width){_width = width;}
	void SetHeight(float height){_height = height;}

	void SetLocation(float x, float y){ SetX(x); SetY(y);}
	void SetSize(float width, float height){SetWidth(width); SetHeight(height);}


	void SkipX(float x){SetX(GetX()+x);}
	void SkipY(float y){SetY(GetY()+y);}

	void SkipXY(float x, float y){SkipX(x); SkipY(y);}

	Rectangle* GetRect(){return(this);}

};

#endif