#include "PhyTriangle.h"
#include "PhyWorld.h"
#include "../Primitives/RectPrim.h"
#include "../CommonLib/AddData.h"

PhyTriangle::PhyTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int bodyType )
{
	b2World* world = PhyWorld::GetInstance()->world;

	b2BodyDef bodyDef;
	bodyDef.position.Set(x1/30.0f,y1/30.0f);

	float angle12 = atan2( y2-y1 , x2-x1) * 180.0f/(22.0f/7.0f);
	float angle13 = atan2( y3-y1 , x3-x1) * 180.0f/(22.0f/7.0f);

	if(angle12 > angle13)
	{
		float tempX = 0;
		float tempY = 0;
		tempX = x3;
		tempY = y3;
		x3 = x2;
		y3 = y2;
		x2 = tempX;
		y2 = tempY;
	}

	if(bodyType == 0)		bodyDef.type = b2_staticBody;
	else if(bodyType == 1)	bodyDef.type = b2_kinematicBody;
	else if(bodyType == 2)	bodyDef.type = b2_dynamicBody;

	triVertex[0] = x1-x1;
	triVertex[1] = y1-y1;
	triVertex[2] = x2-x1;
	triVertex[3] = y2-y1;
	triVertex[4] = x3-x1;
	triVertex[5] = y3-y1;

	for(int i=0;i<6;i++)
		triVertex[i] = triVertex[i]/30.0f;
	
	b2PolygonShape shape;
	shape.SetVertexArray(triVertex, 3);

	for(int i=0;i<6;i++)
		triVertex[i] = triVertex[i]*30.0f;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	_body = world->CreateBody(&bodyDef);
	_body->CreateFixture(&fixtureDef);
}

void PhyTriangle::Update()
{
}

float PhyTriangle::GetX()
{
	return _body->GetPosition().x*30;
}
float PhyTriangle::GetY()
{
	return _body->GetPosition().y*30;
}

float PhyTriangle::GetAngle()
{
	return _body->GetAngle()*180.0f/(22.0f/7.0f);
}

void PhyTriangle::Draw()
{
	b2Vec2 position = _body->GetPosition();
	float32 angleInDegrees = _body->GetAngle()*180.0f/(22.0f/7.0f);

	glPushMatrix();
	glTranslatef(position.x*30, position.y*30, 0);
	glRotatef(angleInDegrees,0,0,1);

	glColor4f(0,1,0,0.5);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, triVertex);
	glDrawArrays(GL_TRIANGLES,0,3);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glColor4f(0,1,0,1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, triVertex);
	glDrawArrays(GL_LINE_LOOP,0,3);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

b2Body* PhyTriangle::GetBody()
{
	return _body;
}

PhyTriangle::~PhyTriangle()
{
	if(_body)
	{
		PhyWorld::GetInstance()->world->DestroyBody(_body);
		_body = NULL;
	}
}
