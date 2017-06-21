#include "PhyCircle.h"
#include "PhyWorld.h"
#include "RectPrim.h"

PhyCircle::PhyCircle(float x, float y, float r, int bodyType)
{
	_r = r;

	b2World* world = PhyWorld::GetInstance()->world;

	b2BodyDef bodyDef;
	bodyDef.position.Set(x/30.0f, y/30.0f);

	if(bodyType == 0)		bodyDef.type = b2_staticBody;
	else if(bodyType == 1)	bodyDef.type = b2_kinematicBody;
	else if(bodyType == 2)	bodyDef.type = b2_dynamicBody;

	b2CircleShape shape;
	shape.m_radius = _r/30.0f;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.001f;		//temp

	_body = world->CreateBody(&bodyDef);
	//_body->SetFixedRotation(true);
	_fixture = _body->CreateFixture(&fixtureDef);

	_cirPrim = new CirclePrim(0,0,_r);
	_cirPrim->SetColor(0,255,0,128);
	_cirPrim->SetBorderColor(0,255,0,255);
}

void PhyCircle::Update()
{
}

float PhyCircle::GetX()
{
	return _body->GetPosition().x*30;
}
float PhyCircle::GetY()
{
	return _body->GetPosition().y*30;
}

float PhyCircle::GetR()
{
	return _r;
}

float PhyCircle::GetAngle()
{
	return _body->GetAngle()*180.0f/(3.14159265f);
}

b2Body* PhyCircle::GetBody()
{
	return _body;
}

void PhyCircle::Draw()
{
	if( _angularVelLoop && _angularVel > 0 )
		_body->SetAngularVelocity(_angularVel);

	glPushMatrix();
	glTranslatef(GetX(), GetY(), 0);
	glRotatef(GetAngle(), 0,0,1);

	_cirPrim->Draw();

	glPopMatrix();
}


PhyCircle::~PhyCircle()
{
	if(_body)
	{
		PhyWorld::GetInstance()->world->DestroyBody(_body);
		_body = NULL;
	}

	if (_cirPrim)
	{
		delete _cirPrim;
		_cirPrim = NULL;
	}
}
