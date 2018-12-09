#include "PhyRect.h"
#include "PhyWorld.h"
#include "RectPrim.h"

PhyRect::PhyRect(float centerX, float centerY, float halfW, float halfH, float angle, int bodyType)
{
	_halfW = halfW;
	_halfH = halfH;

	b2World* world = PhyWorld::GetInstance()->world;

	b2BodyDef bodyDef;
	bodyDef.position.Set(centerX/30.0f, centerY/30.0f);
	bodyDef.angle = angle * (3.14159265f) / 180.0f;

	if(bodyType == 0)		bodyDef.type = b2_staticBody;
	else if(bodyType == 1)	bodyDef.type = b2_kinematicBody;
	else if(bodyType == 2)	bodyDef.type = b2_dynamicBody;

	b2PolygonShape shape;
	shape.SetAsBox(_halfW/30.0f, _halfH/30.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	_body = world->CreateBody(&bodyDef);
	_body->CreateFixture(&fixtureDef);
}

void PhyRect::Update()
{
}

float PhyRect::GetCX()
{
	return _body->GetPosition().x*30;
}

float PhyRect::GetCY()
{
	return _body->GetPosition().y*30;
}

float PhyRect::GetHalfW()
{
	return _halfW;
}

float PhyRect::GetHalfH()
{
	return _halfH;
}

float PhyRect::GetAngle()
{
	return _body->GetAngle() * 180.0f/(3.14159265f);
}

b2Body* PhyRect::GetBody()
{
	return _body;
}

void PhyRect::Draw()
{
	RectPrim::Draw(GetCX()-_halfW, GetCY()-_halfH, 2*_halfW, 2*_halfH, GetAngle(), 0x00ff00ff);
}

PhyRect::~PhyRect()
{
	if(_body)
	{
		PhyWorld::GetInstance()->world->DestroyBody(_body);
		_body = NULL;
	}
}
