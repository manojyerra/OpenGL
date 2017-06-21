#include "PhyBox.h"
#include "PhyWorld.h"
#include "../Primitives/RectPrim.h"

PhyBox::PhyBox(float x, float y, float width, float height, float angle, int bodyType)
{
	_width = width;
	_height = height;

	width /= 2.0f;
	height /= 2.0f;

	x = x+width;
	y = y+height;

	b2World* world = PhyWorld::GetInstance()->world;

	b2BodyDef bodyDef;
	bodyDef.position.Set(x/30.0f, y/30.0f);
	bodyDef.angle = angle*(22.0f/7.0f) / 180.0f;

	if(bodyType == 0)		bodyDef.type = b2_staticBody;
	else if(bodyType == 1)	bodyDef.type = b2_kinematicBody;
	else if(bodyType == 2)	bodyDef.type = b2_dynamicBody;

	b2PolygonShape shape;
	shape.SetAsBox(width/30.0f, height/30.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	_body = world->CreateBody(&bodyDef);
	_body->CreateFixture(&fixtureDef);
}

void PhyBox::Update()
{
}

float PhyBox::GetX()
{
	return _body->GetPosition().x*30-_width/2;
}
float PhyBox::GetY()
{
	return _body->GetPosition().y*30-_height/2;
}
float PhyBox::GetWidth()
{
	return _width;
}
float PhyBox::GetHeight()
{
	return _height;
}

float PhyBox::GetAngle()
{
	return _body->GetAngle()*180.0f/(22.0f/7.0f);
}

b2Body* PhyBox::GetBody()
{
	return _body;
}

void PhyBox::Draw()
{
	float x = _body->GetPosition().x;
	float y = _body->GetPosition().y;
	float angle = _body->GetAngle()*180.0f/(22.0f/7.0f);
	RectPrim::Draw(x*30.0f-_width/2, y*30.0f-_height/2, _width, _height, angle, 0x00ff00ff);
}

PhyBox::~PhyBox()
{
	if(_body)
	{
		PhyWorld::GetInstance()->world->DestroyBody(_body);
		_body = NULL;
	}
}
