#ifndef PRIM_H
#define PRIM_H
#pragma once

class EditPrim
{
protected:
	bool _isInGroup;
	bool _isDynamicBody;
	int _compoundShapeID;
	float _angularVel;
	bool _angularVelLoop;

public:
	int ID;

	static const int LINE = 1;
	static const int RECT = 2;
	static const int CIRCLE = 3;
	static const int POLY = 4;

	EditPrim(int id)
	{
		ID = id;
		_isInGroup = false;
		_isDynamicBody = true;
		_compoundShapeID = -1;
		_angularVel = 0.0f;
		_angularVelLoop = false;
	}

	virtual ~EditPrim(){};
	virtual void Update() = 0;

	virtual bool Contains(float x, float y)=0;
	virtual bool CollidesWithRect(float x, float y, float w, float h) = 0;
	virtual EditPrim* Clone()=0;
	virtual void Move(float x, float y)=0;
	virtual void AddRotation(float angleInDegrees, float pivotX, float pivotY)=0;
	virtual void SetSelect(bool select)=0;
	virtual bool IsSelected() = 0;
	virtual void Draw() = 0;

	void SetCompoundShapeID(int id)
	{
		_compoundShapeID = id;
	}

	int GetCompoundShapeID()
	{
		return _compoundShapeID;
	}

	bool IsStaticBody()
	{
		return(!_isDynamicBody);
	}

	bool IsDynamicBody()
	{
		return(_isDynamicBody);
	}

	void SetDynamicBody(bool val)
	{
		_isDynamicBody = val;
	}

	bool IsInGroup()
	{
		return _isInGroup;
	}

	void SetInGroup(bool setInGroup)
	{
		_isInGroup = setInGroup;
	}

	void SetAngularVelocity(float vel, bool loop)
	{
		_angularVel = vel;
		_angularVelLoop = loop;
	}

	float GetAngularVelocity()
	{
		return _angularVel;
	}

	bool IsAngularVelLoop()
	{
		return _angularVelLoop;
	}
};

#endif
