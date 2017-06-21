#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::_ref = NULL;

PhysicsManager::PhysicsManager(){}		//not using
PhysicsManager::~PhysicsManager(){}		//not using

PhysicsManager* PhysicsManager::GetInstance()
{
	if(_ref == NULL)
		_ref = new PhysicsManager();
	return _ref;
}

void PhysicsManager::Init()
{
	PhyWorld::GetInstance()->Init();
}

PhyBox* PhysicsManager::AddBox(float x, float y, float width, float height, int type)
{
	PhyBox* box = new PhyBox(x, y, width, height, 0, type);
	_bodyVec.push_back(box);
	return box;
}

PhyCircle* PhysicsManager::AddCircle(float x, float y, float r, int type)
{
	PhyCircle* circle = new PhyCircle(x, y, r, type);
	_bodyVec.push_back(circle);
	return circle;
}

PhyLine* PhysicsManager::AddLine(float x1, float y1, float x2, float y2, int type)
{
	PhyLine* line = new PhyLine(x1, y1, x2, y2, type);
	_bodyVec.push_back(line);
	return line;
}

PhyTriangle* PhysicsManager::AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int type)
{
	PhyTriangle* triangle = new PhyTriangle(x1, y1, x2, y2, x3, y3, type);
	_bodyVec.push_back(triangle);
	return triangle;
}

PhyPoly* PhysicsManager::AddPolygon(vector<Point> vec, int type)
{
	PhyPoly* poly = new PhyPoly(vec, type);
	_bodyVec.push_back(poly);
	return poly;
}

PhyCompoundShape* PhysicsManager::AddCompoundShape(PhyCompoundShape* compoundShape)
{
	_bodyVec.push_back(compoundShape);
	return compoundShape;
}

void PhysicsManager::Update(float deltaTime)
{
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	b2World* world = PhyWorld::GetInstance()->world;
	world->Step(deltaTime, velocityIterations, positionIterations);
}

void PhysicsManager::Draw()
{
	for(int i=0;i<(int)_bodyVec.size();i++)
		_bodyVec[i]->Draw();
}

void PhysicsManager::RemoveBodies()
{
	for(int i=0;i<(int)_bodyVec.size();i++)
	{
		if(_bodyVec[i])
		{
			delete _bodyVec[i];
			_bodyVec[i] = NULL;
		}
	}
	_bodyVec.clear();
}

void PhysicsManager::Terminate()
{
	RemoveBodies();
	PhyWorld::GetInstance()->Terminate();

	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}


	//INIT
	//_ground->SetSize(500.0f*2.0f, 50.0f*2.0f);
	//_ground->SetPos(30.0f-_ground->GetWidth()/2.0f, 300.0f-_ground->GetHeight()/2.0f);

	//_crossImg->SetSize(50.0f*2.0f, 25.0f*2.0f);
	//_crossImg->SetPos(150-_crossImg->GetWidth()/2.0f, 30-_crossImg->GetHeight()/2.0f);

	//b2FixtureDef fixtureDef;
	//fixtureDef.shape = &dynamicBox;
	//fixtureDef.density = 1.0f;
	//fixtureDef.friction = 0.3f;
	//dynamicBody->CreateFixture(&fixtureDef);

	//_firstTriVertex[0] = 0;
	//_firstTriVertex[1] = 0;
	//_firstTriVertex[2] = 100;
	//_firstTriVertex[3] = 0;
	//_firstTriVertex[4] = 0;
	//_firstTriVertex[5] = 50;

	//_secondTriVertex[0] = 0;
	//_secondTriVertex[1] = 0;
	//_secondTriVertex[2] = 100;
	//_secondTriVertex[3] = 0;
	//_secondTriVertex[4] = 100;
	//_secondTriVertex[5] = 100;

	//for(int i=0;i<6;i++)
	//{
	//	_firstTriVertex[i] = _firstTriVertex[i]/30.0f;
	//	_secondTriVertex[i] = _secondTriVertex[i]/30.0f;
	//}

	//b2BodyDef triCompoundShapeDef;
	//triCompoundShapeDef.type = b2_dynamicBody;
	//triCompoundShapeDef.position.Set(300.0f/30.0f, .0f/30.0f);
	//triCompoundBody = world->CreateBody(&triCompoundShapeDef);

	//b2PolygonShape triShape1;
	//triShape1.SetVertexArray(_firstTriVertex, 3);

	//b2PolygonShape triShape2;
	//triShape2.SetVertexArray(_secondTriVertex, 3);

	//for(int i=0;i<6;i++)
	//{
	//	_firstTriVertex[i] = _firstTriVertex[i]*30.0f;
	//	_secondTriVertex[i] = _secondTriVertex[i]*30.0f;
	//}

	//b2FixtureDef fixtureDefTri1;
	//fixtureDefTri1.shape = &triShape1;
	//fixtureDefTri1.density = 1.0f;
	//fixtureDefTri1.friction = 0.3f;

	//b2FixtureDef fixtureDefTri2;
	//fixtureDefTri2.shape = &triShape2;
	//fixtureDefTri2.density = 1.0f;
	//fixtureDefTri2.friction = 0.3f;

	//triCompoundBody->CreateFixture(&fixtureDefTri1);
	//triCompoundBody->CreateFixture(&fixtureDefTri2);


	//UPDATE
	//b2Vec2 position = dynamicBody->GetPosition();
	//float32 angle = dynamicBody->GetAngle();
	//float32 angleInDegrees = angle*180.0f/(22.0f/7.0f);

	//_crossImg->SetPos(position.x*30.0f-_crossImg->GetWidth()/2, position.y*30.0f-_crossImg->GetHeight()/2);
	//_crossImg->SetRotation(angleInDegrees);

	//if(Input::IsMouseClicked())
	//	triCompoundBody->ApplyForceToCenter(b2Vec2(100,-350));


	//DRAW
	//glColor4f( 1,0,0,1);
	//glPushMatrix();
	//b2Vec2 position = triCompoundBody->GetPosition();
	//float32 angle = triCompoundBody->GetAngle();
	//float32 angleInDegrees = angle*180.0f/(22.0f/7.0f);

	//glTranslatef(position.x*30, position.y*30, 0);
	//glRotatef( angleInDegrees, 0,0,1);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(2, GL_FLOAT, 0, _firstTriVertex);
	//glDrawArrays(GL_TRIANGLES,0,3);
	//glDisableClientState(GL_VERTEX_ARRAY);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(2, GL_FLOAT, 0, _secondTriVertex);
	//glDrawArrays(GL_TRIANGLES,0,3);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glPopMatrix();