#ifndef PHY_SHAPE_H
#define PHY_SHAPE_H
#pragma once

class PhyShape
{
public:
	static const int STATIC_BODY = 1;
	static const int DYNAMIC_BODY = 2;

	PhyShape(){}
	virtual ~PhyShape(){}
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual b2Body* GetBody()=0;
};

#endif
