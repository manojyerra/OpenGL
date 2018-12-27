#include "Ball.h"

Ball::Ball(float centerX, float centerY, float radius)
{
	_cx = centerX;
	_cy = centerY;
	_r = radius;

	_sprite = new Sprite();
	_sprite->Init("data/Ball.png",_cx-_r, _cy-_r, 2*_r, 2*_r);
	_speed = 1.0f;
	_mass = 1.0f;

	//int randNum = rand()%3;

	//if(randNum == 0)
	//{
	//	_speed = 1.0f;
	//	_mass = 1.0f;
	//	//_sprite->SetColor(0, 0, 0, 255);
	//}
	//else if(randNum == 1)
	//{
	//	_speed = 2.0f;
	//	_mass = 1.0f;
	//	//_sprite->SetColor(0, 0, 255, 255);
	//}
	//else if(randNum == 2)
	//{
	//	_speed = 3.0f;
	//	//_mass = 1.0f;
	//	_sprite->SetColor(255, 0, 0, 255);
	//}


	_dirVec = new MyVector(&Point(1.0f,1.0f));
	_dirVec->Normalize();

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

MyVector* Ball::GetDirectionVec()
{
	return _dirVec;
}

void Ball::SetSpeed(float speed)
{
	_speed = speed;
}

float Ball::GetXVelocity()
{
	return (_speed * _dirVec->p.x );
}

float Ball::GetYVelocity()
{
	return (_speed * _dirVec->p.y );
}

void Ball::SetVelocity(float xVelocity, float yVelocity)
{
	_speed = sqrt( xVelocity*xVelocity + yVelocity*yVelocity );
	_dirVec->SetPoint(&Point(xVelocity, yVelocity));
	_dirVec->Normalize();
}

void Ball::SetMass(float mass)
{
	_mass = mass;
}

float Ball::GetMass()
{
	return _mass;
}

void Ball::SetDirection(Point* p)
{
	_dirVec->SetPoint(p);
	_dirVec->Normalize();
}


void Ball::Update()
{
	_cx += _dirVec->p.x * _speed;
	_cy += _dirVec->p.y * _speed;
}


void Ball::Move()
{
	_cx += _dirVec->p.x * _speed;
	_cy += _dirVec->p.y * _speed;
}


void Ball::UndoMove()
{
	_cx -= _dirVec->p.x * _speed;
	_cy -= _dirVec->p.y * _speed;
}


void Ball::Draw()
{
	_sprite->SetPos(_cx-_r, _cy-_r);
	_sprite->Draw();


}

