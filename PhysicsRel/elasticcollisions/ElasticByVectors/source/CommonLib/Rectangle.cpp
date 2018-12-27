#include "Rectangle.h"

Rectangle::~Rectangle()
{
	_x = _y = 0.0f;
	_width = _height = 0.0f;
}


Rectangle::Rectangle() 
{
	_x = _y = 0.0f;
	_width = _height = 0.0f;
}


Rectangle::Rectangle(float x, float y, float width, float height) 
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
}

void Rectangle::Init(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
}

void Rectangle::SetBounds(Rectangle* rect)
{ 
	SetBounds(rect->_x, rect->_y, rect->_width, rect->_height); 
}

void Rectangle::SetBounds(float x, float y, float width, float height)
{
	_x = x; 
	_y = y; 
	_width = width; 
	_height = height; 
}

bool Rectangle::Contains(Rectangle* rect) 
{
	return Contains(rect->_x, rect->_y, rect->_width, rect->_height);
}

bool Rectangle::Contains(float X, float Y, float W, float H) 
{
	float w = _width;
	float h = _height;

	if (w<0 ||  h<0 ||  W<0 ||  H < 0) {return false;}
	float x = _x;
	float y = _y;

	if (X < x || Y < y) {    return false;	}
	w += x;
	W += X;

	if (W <= X) {    if (w >= x || W > w) return false;} 
	else {    if (w >= x && W > w) return false;	}

	h += y;
	H += Y;

	if (H <= Y) {    if (h >= y || H > h) return false;} 
	else {    if (h >= y && H > h) return false;}

	return true;
}

bool Rectangle::Contains(float X, float Y) 
{
	float w = _width;
	float h = _height;

	if (w <0 || h< 0) {    return false;}

	float x = _x;
	float y = _y;

	if (X < x || Y < y) {    return false;	}

	w += x;
	h += y;
	//    overflow || floatersect
	return ((w < x || w > X)  && (h < y || h > Y));
}


bool Rectangle::Contains(float posX, float posY, float rectX, float rectY, float rectW, float rectH)
{
	return (posX >= rectX && posX <= rectX+rectW && posY >= rectY && posY <= rectY+rectH);
}

bool Rectangle::Intersects(Rectangle* rect) 
{
	float tw = _width;
	float th = _height;
	float rw = rect->_width;
	float rh = rect->_height;

	if (rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) {   return false;  }

	float tx = _x;
	float ty = _y;
	float rx = rect->_x;
	float ry = rect->_y;
	rw += rx;
	rh += ry;
	tw += tx;
	th += ty;
	//      overflow || floatersect
	return ((rw < rx || rw > tx) &&	(rh < ry || rh > ty) &&	(tw < tx || tw > rx)  &&  (th < ty || th > ry));
}


bool Rectangle::Intersects(float xx, float yy, float ww, float hh) 
{
	float tw = _width;
	float th = _height;
	float rw = ww;
	float rh = hh;

	if (rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) {   return false;  }

	float tx = _x;
	float ty = _y;
	float rx = xx;
	float ry = yy;
	rw += rx;
	rh += ry;
	tw += tx;
	th += ty;
	//      overflow || floatersect
	return ((rw < rx || rw > tx) &&	(rh < ry || rh > ty) &&	(tw < tx || tw > rx)  &&  (th < ty || th > ry));
}