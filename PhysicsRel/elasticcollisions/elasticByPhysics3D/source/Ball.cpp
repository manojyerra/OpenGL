#include "Ball.h"
#include "Renderer.h"

Ball::Ball(Point* center, float radius)
{
	_cp.Set(center);
	_r = radius;

	_speed = 1.0f;
	_mass = 1.0f;
	_velocityVec = new MyVector(&Point(0,0,0));

	int randNum = rand()%2;
	if(randNum == 0)
	{
		_r = 2;
		_mass = 5.0f;
		_velocityVec->p.Set(0.5,0,0);
		
		_r1 = 1;
		_g1 = 1;
		_b1 = 0;

		_r2 = 0;
		_g2 = 0;
		_b2 = 1;
	}
	else if(randNum == 1)
	{
		_r = 1.5;
		_mass = 3.0f;
		_velocityVec->p.Set(0.6,0.3,0);

		_r1 = 1;
		_g1 = 0;
		_b1 = 1;

		_r2 = 1;
		_g2 = 1;
		_b2 = 1;
	}
	else
	{
		_r = 0.5;
		_mass = 1.0f;
		_velocityVec->p.Set(1,1,0.3);

		_r1 = 0;
		_g1 = 1;
		_b1 = 0;

		_r2 = 0;
		_g2 = 0;
		_b2 = 1;
	}
}

void Ball::SetCenter(Point* p)
{
	_cp.Set(p);
}

Point Ball::GetCenter()
{
	return _cp;
}

void Ball::SetR(float r)
{
	_r = r;
}

float Ball::GetR()
{
	return _r;
}

MyVector* Ball::GetVelocity()
{
	return _velocityVec;
}

void Ball::SetVelocity(float xVelocity, float yVelocity, float zVelocity)
{
	_velocityVec->p.Set(xVelocity, yVelocity, zVelocity);
}

void Ball::SetMass(float mass)
{
	_mass = mass;
}

float Ball::GetMass()
{
	return _mass;
}

void Ball::Update()
{
	_cp.x += _velocityVec->p.x;
	_cp.y += _velocityVec->p.y;
	_cp.z += _velocityVec->p.z;
}

void Ball::Move()
{
	_cp.x += _velocityVec->p.x;
	_cp.y += _velocityVec->p.y;
	_cp.z += _velocityVec->p.z;
}


void Ball::UndoMove()
{
	_cp.x -= _velocityVec->p.x;
	_cp.y -= _velocityVec->p.y;
	_cp.z -= _velocityVec->p.z;
}


void Ball::Draw()
{
	Renderer::SetSphereColor(_r1, _g1, _b1, _r2, _g2, _b2);
	Renderer::DrawSphere(0,_cp.x,_cp.y,_cp.z,_r);
}

