#include "PhyCompoundShape.h"
#include "PhyWorld.h"
#include "../Primitives/RectPrim.h"
#include "../CommonLib/AddData.h"
#include "../MathLib/Poly2D.h"

#include "../Primitives/TriPrim.h"
#include "../Primitives/PolyPrim.h"
#include "../Primitives/RectPolyPrim.h"
#include "../Primitives/CirclePrim.h"

PhyCompoundShape::PhyCompoundShape()
{
	_numShapes = 0;
	_body = NULL;
	bodyDef.type = b2_dynamicBody;
}

void PhyCompoundShape::SetBodyType(int bodyType)
{
	if(_body)
	{
		if(bodyType == 0)		_body->SetType(b2_staticBody);
		else if(bodyType == 1)	_body->SetType(b2_kinematicBody);
		else if(bodyType == 2)	_body->SetType(b2_dynamicBody);
	}
	else
	{
		if(bodyType == 0)		bodyDef.type = b2_staticBody;
		else if(bodyType == 1)	bodyDef.type = b2_kinematicBody;
		else if(bodyType == 2)	bodyDef.type = b2_dynamicBody;
	}
}

int PhyCompoundShape::GetNumParts()
{
	return _numShapes;
}

void PhyCompoundShape::AddCircle(float cx, float cy, float radius)
{
	if(_numShapes == 0)
	{
		_pos.x = cx;
		_pos.y = cy;
		bodyDef.position.Set(_pos.x/30.0f, _pos.y/30.0f);
		_body = PhyWorld::GetInstance()->world->CreateBody(&bodyDef);
	}

	cx -= _pos.x;
	cy -= _pos.y;

	CirclePrim* circlePrim = new CirclePrim(cx,cy,radius);
	_primVec.push_back(circlePrim);

	b2CircleShape shape;
	shape.m_radius = radius/30.0f;
	shape.m_p.x = cx/30.0f;
	shape.m_p.y = cy/30.0f;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	_body->CreateFixture(&fixtureDef);

	_numShapes++;
}

void PhyCompoundShape::AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	if(_numShapes == 0)
	{
		_pos.x = x1;
		_pos.y = y1;
		bodyDef.position.Set(_pos.x/30.0f, _pos.y/30.0f);
		_body = PhyWorld::GetInstance()->world->CreateBody(&bodyDef);
	}

	x1 = x1-_pos.x;
	y1 = y1-_pos.y;
	x2 = x2-_pos.x;
	y2 = y2-_pos.y;
	x3 = x3-_pos.x;
	y3 = y3-_pos.y;

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

	TriPrim* triPrim = new TriPrim(x1,y1,x2,y2,x3,y3);
	_primVec.push_back(triPrim);

	float triVertex[6];
	triVertex[0] = x1;
	triVertex[1] = y1;
	triVertex[2] = x2;
	triVertex[3] = y2;
	triVertex[4] = x3;
	triVertex[5] = y3;

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
	_body->CreateFixture(&fixtureDef);

	_numShapes++;
}

void PhyCompoundShape::AddPoly(vector<Point> vec)
{
	if(_numShapes == 0)
	{
		_pos.x = vec[0].x;
		_pos.y = vec[0].y;
		bodyDef.position.Set(_pos.x/30.0f, _pos.y/30.0f);
		_body = PhyWorld::GetInstance()->world->CreateBody(&bodyDef);
	}

	for(int i=0; i<(int)vec.size(); i++)
	{
		vec[i].x -= _pos.x;
		vec[i].y -= _pos.y;
	}

	PolyPrim* polyPrim = new PolyPrim(vec);
	_primVec.push_back(polyPrim);

	int _vertexCount = vec.size();
	float _vertex[16];

	int count = 0;
	for(int i=0; i<(int)vec.size(); i++)
	{
		_vertex[count] = vec[i].x;	count++;
		_vertex[count] = vec[i].y;	count++;
	}

	for(int i=0; i<_vertexCount*2; i++)
		_vertex[i] /= 30.0f;

	b2PolygonShape shape;
	shape.SetVertexArray(_vertex, _vertexCount);

	for(int i=0; i<_vertexCount*2; i++)
		_vertex[i] *= 30.0f;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	_body->CreateFixture(&fixtureDef);

	_numShapes++;
}

void PhyCompoundShape::Update()
{
}

b2Body* PhyCompoundShape::GetBody()
{
	return _body;
}


void PhyCompoundShape::Draw()
{
	b2Vec2 position = _body->GetPosition();
	float32 angleInDegrees = _body->GetAngle()*180.0f/(22.0f/7.0f);

	glPushMatrix();
	glTranslatef(position.x*30, position.y*30, 0);
	glRotatef(angleInDegrees,0,0,1);

	for(int i=0;i<(int)_primVec.size();i++)
		_primVec[i]->Draw();

	glPopMatrix();
}

PhyCompoundShape::~PhyCompoundShape()
{
	if(_body)
	{
		PhyWorld::GetInstance()->world->DestroyBody(_body);
		_body = NULL;
	}
}
