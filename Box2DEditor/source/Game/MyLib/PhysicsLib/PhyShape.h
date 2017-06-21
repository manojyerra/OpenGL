#ifndef PHY_SHAPE_H
#define PHY_SHAPE_H
#pragma once

class PhyShape
{
protected:
	float _angularVel;
	bool _angularVelLoop;

public:
	static const int STATIC_BODY = 1;
	static const int DYNAMIC_BODY = 2;

	PhyShape()
	{
		_angularVel = 0.0f;
		_angularVelLoop = false;
	}

	virtual ~PhyShape(){}
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual b2Body* GetBody()=0;

	void SetAngularVelocity(float vel, bool loop)
	{
		_angularVel = vel;
		_angularVelLoop = loop;

		if(loop == false)
		{
			GetBody()->SetAngularVelocity(vel);
		}
	}
};

#endif
