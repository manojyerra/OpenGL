#ifndef PRIM_H
#define PRIM_H
#pragma once

class Prim
{
private:
	
public:
	int ID;
	static const int TRIANGLE = 1;
	static const int CIRCLE = 2;
	static const int RECT_POLY_PRIM = 3;
	static const int POLY = 4;
	static const int LINE = 5;


	virtual ~Prim(){};
	virtual bool Update() = 0;
	virtual bool Contains(float x, float y)=0;
	virtual Prim* Clone()=0;
	virtual void SetCompoundShapeID( int id ) = 0;
	virtual int GetCompoundShapeID() = 0;
	virtual void Draw()=0;
	virtual void Move(float x, float y)=0;
	virtual void SetSelect(bool select)=0;
	virtual bool IsStaticBody()=0;
	virtual bool IsDynamicBody()=0;
	//virtual bool IsKinematicBody();

};

#endif
