#include "PhyBox.h"
#include "PhyManager.h"
//#include "Sui/Sui.h"

PhyBox::PhyBox(float* mat, float w, float h, float d, float massVal)
{
	_w = w;
	_h = h;
	_d = d;

	_shape = new Box(0,0,0, _w,_h,_d);
	_shape->SetGLMatrix(mat);
	_shape->SetColor(rand()%255,rand()%255,rand()%255,255);

	colliShape = new btBoxShape(btVector3(btScalar(_w),btScalar(_h),btScalar(_d)));

	btTransform trans;
	trans.setIdentity();
	trans.setFromOpenGLMatrix(mat);

	_mass = btScalar(massVal);

	btVector3 localInertia(0,0,0);
	if(isDynamic())
		colliShape->calculateLocalInertia(_mass,localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass,myMotionState,colliShape,localInertia);
	_rigidBody = new btRigidBody(rbInfo);

	PhyManager::GetInstance()->GetDynamicWorld()->addRigidBody(_rigidBody);
}

PhyBox::PhyBox(float x, float y, float z, float w, float h, float d, float massVal)
{
	_w = w;
	_h = h;
	_d = d;

	_shape = new Box(x,y,z, _w,_h,_d);
	_shape->SetColor(rand()%255,rand()%255,rand()%255,255);

	colliShape = new btBoxShape(btVector3(btScalar(_w),btScalar(_h),btScalar(_d)));

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(x,y,z));

	_mass = btScalar(massVal);

	btVector3 localInertia(0,0,0);
	if(isDynamic())
		colliShape->calculateLocalInertia(_mass,localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass,myMotionState,colliShape,localInertia);
	_rigidBody = new btRigidBody(rbInfo);

	PhyManager::GetInstance()->GetDynamicWorld()->addRigidBody(_rigidBody);
}

bool PhyBox::isDynamic()
{
	return (_mass != 0.0f);
}

btMotionState* PhyBox::GetMotionState()
{
	return _rigidBody->getMotionState();
}

btCollisionShape* PhyBox::GetCollisionShape()
{
	return colliShape;
}

void PhyBox::GetOrientation(float* mat)
{
	btTransform trans;
	_rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(mat);
}

void PhyBox::Draw()
{
	float mat[16];
	btTransform trans;
	_rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(mat);

	if(_shape)
	{
		_shape->SetGLMatrix(mat);
		_shape->Draw();
	}
}

void PhyBox::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if(_shape)
		_shape->SetColor(r,g,b,a);
}

PhyBox::~PhyBox()
{
	if(_rigidBody)
	{
		delete _rigidBody->getMotionState();
		delete colliShape;
		PhyManager::GetInstance()->GetDynamicWorld()->removeCollisionObject(_rigidBody);
		delete _rigidBody;
	}

	if(_shape)
	{
		delete _shape;
		_shape = NULL;
	}
}
