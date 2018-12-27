#include "Rect2D.h"

Rect2D::Rect2D( float x, float y, float w, float h, float angle )
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	_angle = angle;
	_centerX = _x+_w/2;
	_centerY = _y+_h/2;
}

Rect2D::~Rect2D()
{
}

void Rect2D::SetRotation( float angle )
{
	_angle = angle;
	_centerX = _x+_w/2;
	_centerY = _y+_h/2;
}

void Rect2D::SetRotation( float angle, float centerX, float centerY )
{
	_angle = angle;
	_centerX = centerX;
	_centerY = centerY;
}

bool Rect2D::Contains( Point* p )
{
	if(_angle != 0)
	{
		Point pp(p->x, p->y);
		pp.SetRotationZ(-_angle, _centerX, _centerY, 0);
		return ( _x <= pp.x && _x+_w >= pp.x && _y <= pp.y && _y+_h >= pp.y );
	}

	return ( _x <= p->x && _x+_w >= p->x && _y <= p->y && _y+_h >= p->y );
}


bool Rect2D::IsColliding(Rect2D* rect)
{
	Point p1(rect->GetX(),					rect->GetY());
	Point p2(rect->GetX()+rect->GetWidth(), rect->GetY());
	Point p3(rect->GetX()+rect->GetWidth(), rect->GetY()+rect->GetHeight());
	Point p4(rect->GetX(),					rect->GetY()+rect->GetHeight());

	p1.SetRotationZ(rect->GetAngle(), rect->GetCenterX(), rect->GetCenterY(),0);
	p2.SetRotationZ(rect->GetAngle(), rect->GetCenterX(), rect->GetCenterY(),0);
	p3.SetRotationZ(rect->GetAngle(), rect->GetCenterX(), rect->GetCenterY(),0);
	p4.SetRotationZ(rect->GetAngle(), rect->GetCenterX(), rect->GetCenterY(),0);

	if(Contains(&p1) || Contains(&p2) || Contains(&p3) || Contains(&p4)) return true;

	Point pp1(GetX(),				GetY());
	Point pp2(GetX()+GetWidth(),	GetY());
	Point pp3(GetX()+GetWidth(),	GetY()+GetHeight());
	Point pp4(GetX(),				GetY()+GetHeight());

	pp1.SetRotationZ(GetAngle(), GetCenterX(), GetCenterY(),0);
	pp2.SetRotationZ(GetAngle(), GetCenterX(), GetCenterY(),0);
	pp3.SetRotationZ(GetAngle(), GetCenterX(), GetCenterY(),0);
	pp4.SetRotationZ(GetAngle(), GetCenterX(), GetCenterY(),0);
	
	return ( rect->Contains(&pp1) || rect->Contains(&pp2) || rect->Contains(&pp3) || rect->Contains(&pp4) );
}

float Rect2D::GetX()		{ return _x; }
float Rect2D::GetY()		{ return _y; }
float Rect2D::GetWidth()	{ return _w; }
float Rect2D::GetHeight() { return _h; }

float Rect2D::GetAngle()	{ return _angle; }
float Rect2D::GetCenterX()	{ return _centerX; }
float Rect2D::GetCenterY()	{ return _centerY; }

Point Rect2D::GetVertex1()
{
	Point p(_x,_y);
	p.SetRotationZ(_angle, _centerX, _centerY,0);
	return p;
}

Point Rect2D::GetVertex2()
{
	Point p(_x+_w, _y);
	p.SetRotationZ(_angle, _centerX, _centerY,0);
	return p;
}
Point Rect2D::GetVertex3()
{
	Point p(_x+_w, _y+_h);
	p.SetRotationZ(_angle, _centerX, _centerY,0);
	return p;
}
Point Rect2D::GetVertex4()
{
	Point p(_x, _y+_h);
	p.SetRotationZ(_angle, _centerX, _centerY,0);
	return p;
}
