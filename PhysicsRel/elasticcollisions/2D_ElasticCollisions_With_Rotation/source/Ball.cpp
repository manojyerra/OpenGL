#include "Ball.h"

Ball::Ball(float centerX, float centerY, float radius)
{
	_cx = centerX;
	_cy = centerY;
	_r = radius;

	_sprite = new Sprite();
	_sprite->Init("data/Ball.png",_cx-_r, _cy-_r, 2*_r, 2*_r);
	_mass = 1.0f;
	_angle = 0;
	_angularVel = 7;
	_rotColckWise = true;
	_velocityVec = new MyVector(&Point(0,0));


	int randNum = rand()%3;
	if(randNum == 0)
	{
		_r = 20;
		_mass = 10.0f;
		_sprite->SetColor(255,255,255,255);
		_velocityVec->p.x = 1;
		_velocityVec->p.y = 0;
	}
	else if(randNum == 1)
	{
		_r = 15;
		_mass = 5.0f;
		_sprite->SetColor(0,255,255,255);
		_velocityVec->p.x = 1;
		_velocityVec->p.y = 1;
	}
	else
	{
		_r = 11;
		_mass = 1.0f;
		_sprite->SetColor(255,255,0,255);
		_velocityVec->p.x = 0;
		_velocityVec->p.y = 1;
	}

	_sprite->SetBounds(_cx-_r, _cy-_r, 2*_r, 2*_r);
}

void Ball::SetCenter(float cx, float cy)
{
	_cx = cx;
	_cy = cy;
}

float Ball::GetCX()
{
	return _cx;
}

float Ball::GetCY()
{
	return _cy;
}

float Ball::GetR()
{
	return _r;
}

void Ball::SetR(float r)
{
	_r = r;
}

MyVector* Ball::GetVelocityVec()
{
	return _velocityVec;
}

float Ball::GetXVelocity()
{
	return (_velocityVec->p.x );
}

float Ball::GetYVelocity()
{
	return ( _velocityVec->p.y );
}

void Ball::SetVelocity(float xVelocity, float yVelocity)
{
	_velocityVec->p.x = xVelocity;
	_velocityVec->p.y = yVelocity;
}

float Ball::GetSpeed()
{
	return _velocityVec->Length();
}

void Ball::SetRotSpeedPercent(float speedPercent)
{
	_angularVel = speedPercent * _angularVel / 100.0f;
}

//void Ball::SetRotSpeed(float rotSpeed)
//{
//}
//
//float Ball::GetRotSpeed()
//{
//}


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
	_cx += _velocityVec->p.x;
	_cy += _velocityVec->p.y;
}


void Ball::Move()
{
	_cx += _velocityVec->p.x;
	_cy += _velocityVec->p.y;
}


void Ball::UndoMove()
{
	_cx -= _velocityVec->p.x;
	_cy -= _velocityVec->p.y;
}

void Ball::SetRotation(float angle)
{
	_angle = angle;
}

void Ball::SetRotClockWise(bool clockWise)
{
	_rotColckWise = clockWise;
}

bool Ball::IsRotClockWise()
{
	return _rotColckWise;
}

void Ball::ReverseRot()
{
	_rotColckWise = !_rotColckWise;
}


void Ball::Draw()
{
	_sprite->SetSize(2*_r, 2*_r);
	_sprite->SetPos(_cx-_r, _cy-_r);
	_sprite->SetRotation(_angle);

	if(_rotColckWise)
	{
		_angle  += _angularVel;
		if(_angle > 360)
			_angle -= 360;
	}
	else
	{
		_angle -= _angularVel;
		if(_angle < 0)
			_angle += 360;
	}

	_sprite->Draw();
}

