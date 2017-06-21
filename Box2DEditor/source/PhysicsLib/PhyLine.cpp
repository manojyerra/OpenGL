#include "PhyLine.h"
#include "PhyWorld.h"
#include "../Primitives/RectPrim.h"
#include "../CommonLib/AddData.h"

PhyLine::PhyLine(float x1, float y1, float x2, float y2, int bodyType)
{
	_x1 = x1-x1;
	_y1 = y1-y1;
	_x2 = x2-x1;
	_y2 = y2-y1;

	b2World* world = PhyWorld::GetInstance()->world;
	b2BodyDef bodyDef;
	bodyDef.position.Set(x1/30.0f, y1/30.0f);

	if(bodyType == 0)		bodyDef.type = b2_staticBody;
	else if(bodyType == 1)	bodyDef.type = b2_kinematicBody;
	else if(bodyType == 2)	bodyDef.type = b2_dynamicBody;

	b2EdgeShape shape;
	shape.Set( b2Vec2(_x1/30.0f, _y1/30.0f), b2Vec2(_x2/30.0f, _y2/30.0f) );

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	_body = world->CreateBody(&bodyDef);
	_body->CreateFixture(&fixtureDef);
}

void PhyLine::Update()
{
}

float PhyLine::GetAngle()
{
	return _body->GetAngle()*180.0f/(22.0f/7.0f);
}

b2Body* PhyLine::GetBody()
{
	return _body;
}

void PhyLine::Draw()
{
	float x = _body->GetPosition().x*30.0f;
	float y = _body->GetPosition().y*30.0f;
	float angle = _body->GetAngle()*180.0f/(22.0f/7.0f);

	glPushMatrix();
	glRotatef(angle, 0,0,1);
	glTranslatef(x, y, 0);

	AddData* addData = AddData::GetInstance();
	addData->glColor4f(0,1,0,1);
	addData->glBegin(GL_LINES);
	addData->glVertex3f(_x1, _y1, 0);
	addData->glVertex3f(_x2, _y2, 0);
	addData->glEnd();

	glPopMatrix();
}


PhyLine::~PhyLine()
{
	if(_body)
	{
		PhyWorld::GetInstance()->world->DestroyBody(_body);
		_body = NULL;
	}
}
