#include "PhyPoly.h"
#include "PhyWorld.h"
#include "RectPrim.h"
#include "AddData.h"
#include "Poly2D.h"

PhyPoly::PhyPoly(vector<Point> vec, int bodyType, float density)
{
	if(vec.size() <= 4)
		ArrangeInCWDirection(&vec);

	//bool isConvex = IsConvexPolygon(&pointsVec);

	float vertex[16];

	for(int i=0; i<(int)vec.size(); i++)
	{
		vertex[i*2 + 0] = vec[i].x;
		vertex[i*2 + 1] = vec[i].y;
	}

	Init(vertex, vec.size(), bodyType, density);
}

PhyPoly::PhyPoly(float* vertex, int vertexCount, int bodyType, float density)
{
	Init(vertex, vertexCount, bodyType, density);
}

void PhyPoly::Init(float* vertex, int vertexCount, int bodyType, float density)
{
	_vertexCount = vertexCount;

	for (int i = 0; i < _vertexCount*2; i++)
		_vertex[i] = vertex[i];

	
	float midX = 0;
	float midY = 0;

	for (int i = 0; i < _vertexCount * 2; i+=2)
	{
		midX += _vertex[i];
		midY += _vertex[i + 1];
	}

	midX = midX / vertexCount;
	midY = midY / vertexCount;

	//float vx1 = _vertex[0];
	//float vy1 = _vertex[1];

	b2World* world = PhyWorld::GetInstance()->world;
	b2BodyDef bodyDef;
	bodyDef.position.Set(midX / 30.0f, midY / 30.0f);

	if (bodyType == 0)		bodyDef.type = b2_staticBody;
	else if (bodyType == 1)	bodyDef.type = b2_kinematicBody;
	else if (bodyType == 2)	bodyDef.type = b2_dynamicBody;

	for (int i = 0; i<_vertexCount * 2; i += 2)
	{
		_vertex[i] -= midX;
		_vertex[i + 1] -= midY;

		_vertex[i] /= 30.0f;
		_vertex[i + 1] /= 30.0f;
	}

	b2PolygonShape shape;
	shape.SetVertexArray(_vertex, _vertexCount);

	for (int i = 0; i<_vertexCount * 2; i++)
		_vertex[i] = _vertex[i] * 30.0f;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = 0.3f;

	_body = world->CreateBody(&bodyDef);
	_body->CreateFixture(&fixtureDef);

	_r = 0;
	_g = 255;
	_b = 0;
	_a = 128;
}

bool PhyPoly::IsConvexPolygon(vector<Point>* pointsVec)
{
	for(int i=0; i<(int)pointsVec->size(); i++)
	{
		vector<Point> poly;

		for(int j=0; j<(int)pointsVec->size(); j++)
		{
			if( j != i)
				poly.push_back(pointsVec->at(j));
		}

		if(Poly2D(poly).Contains(&pointsVec->at(i)) == true)
			return false;
	}

	return true;
}

void PhyPoly::ArrangeInCWDirection(vector<Point>* pointsVec)
{
	float angles[8];
	for(int i=0;i<8;i++)
		angles[i] = -1;

	float x1 = 0;
	float y1 = 0;

	for(int i=0; i<(int)pointsVec->size(); i++)
	{
		x1 += pointsVec->at(i).x;
		y1 += pointsVec->at(i).y;
	}

	x1 /= pointsVec->size();
	y1 /= pointsVec->size();

	for(int i=0; i<(int)pointsVec->size(); i++)
	{
		float x2 = pointsVec->at(i).x;
		float y2 = pointsVec->at(i).y;
		angles[i] = atan2(y2-y1, x2-x1) * 180.0f/(22.0f/7.0f);
	}

	for(int i=0;i<(int)pointsVec->size();i++)
	{
		for(int j=i+1;j<(int)pointsVec->size();j++)
		{
			if(angles[i] > angles[j])
			{
				float tempAngle = 0;
				tempAngle = angles[i];
				angles[i] = angles[j];
				angles[j] = tempAngle;

				Point tempPoint(0,0);
				tempPoint = pointsVec->at(i);
				pointsVec->at(i) = pointsVec->at(j);
				pointsVec->at(j) = tempPoint;
			}
		}
	}
}

void PhyPoly::SetColor(int r, int g, int b, int a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void PhyPoly::Update()
{
}

float PhyPoly::GetX()
{
	return _body->GetPosition().x*30;
}

float PhyPoly::GetY()
{
	return _body->GetPosition().y*30;
}

float PhyPoly::GetAngle()
{
	return _body->GetAngle()*180.0f/(22.0f/7.0f);
}

b2Body* PhyPoly::GetBody()
{
	return _body;
}

void PhyPoly::Draw()
{
	//b2Vec2 pos(_body->GetPosition().x + 0.01, _body->GetPosition().y);
	//_body->SetTransform(pos, 0);

	if( _angularVelLoop && _angularVel > 0 )
		_body->SetAngularVelocity(_angularVel);

	glColor4ub(_r,_g,_b,_a);

	glPushMatrix();

	glTranslatef(GetX(), GetY(), 0);
	glRotatef(GetAngle(),0,0,1);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, _vertex);
	glDrawArrays(GL_TRIANGLE_FAN,0,_vertexCount);
	glDisableClientState(GL_VERTEX_ARRAY);

	glColor4f(0,1,0,1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, _vertex);
	glDrawArrays(GL_LINE_LOOP,0,_vertexCount);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

PhyPoly::~PhyPoly()
{
	if(_body)
	{
		PhyWorld::GetInstance()->world->DestroyBody(_body);
		_body = NULL;
	}
}
