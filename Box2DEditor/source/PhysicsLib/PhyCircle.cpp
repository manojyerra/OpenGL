#include "PhyCircle.h"
#include "PhyWorld.h"
#include "../Primitives/RectPrim.h"
#include "../CommonLib/AddData.h"

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
	fixtureDef.friction = 0.3f;

	_body = world->CreateBody(&bodyDef);
	_fixture = _body->CreateFixture(&fixtureDef);
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
	return _body->GetAngle()*180.0f/(22.0f/7.0f);
}

b2Body* PhyCircle::GetBody()
{
	return _body;
}

void PhyCircle::Draw()
{
	float x = _body->GetPosition().x*30.0f;
	float y = _body->GetPosition().y*30.0f;
	float angle = _body->GetAngle()*180.0f/(22.0f/7.0f);

	float r = _r;

	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef( angle, 0,0,1);
	glTranslatef( -x, -y, 0);

	AddData* addData = AddData::GetInstance();
	addData->glColor4f(0,1,0,0.5);
	addData->glBegin(GL_TRIANGLE_FAN);

	addData->glVertex3f(x, y, 0);
	addData->glVertex3f(x+_r, y, 0);

	for( int angle=10; angle<350; angle += 10 )
	{
		float radians = angle*(22.0f/7.0f)/180.0f;
		addData->glVertex3f(x+_r*cosf(radians), y+_r*sinf(radians),0);
	}
	addData->glVertex3f(x+_r, y, 0);
	addData->glEnd();


	//BORDER
	glLineWidth(3.0f);
	addData->glColor4f(1,0,0,1);
	addData->glBegin(GL_LINE_STRIP);
	addData->glVertex3f(x+_r, y, 0);

	for( int angle=10; angle<360; angle += 10 )
	{
		if(angle > 180)
			addData->glColor4f(0,0,1,1);
		float radians = angle*(22.0f/7.0f)/180.0f;
		float x1 = x+_r*cosf(radians);
		float y1 = y+_r*sinf(radians);
		addData->glVertex3f(x1,y1,0);
	}
	
	addData->glVertex3f(x+_r, y, 0);
	addData->glEnd();

	glPopMatrix();

	glLineWidth(1.0f);
}


PhyCircle::~PhyCircle()
{
	if(_body)
	{
		PhyWorld::GetInstance()->world->DestroyBody(_body);
		_body = NULL;
	}
}
