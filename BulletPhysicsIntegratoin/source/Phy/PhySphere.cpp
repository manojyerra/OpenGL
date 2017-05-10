#include "PhySphere.h"
#include "PhyManager.h"
//#include "Sui/Sui.h"

PhySphere::PhySphere(float x, float y, float z, float r, float massVal)
{
	_r = r;
	_shape = new Sphere(x,y,z, _r);

	colliShape = new btSphereShape(btScalar(_r));

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

bool PhySphere::isDynamic()
{
	return (_mass != 0.0f);
}

btMotionState* PhySphere::GetMotionState()
{
	return _rigidBody->getMotionState();
}

btCollisionShape* PhySphere::GetCollisionShape()
{
	return colliShape;
}

void PhySphere::Draw()
{
	float mat[16];
	btTransform trans;
	_rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(mat);

	_shape->SetGLMatrix(mat);
	_shape->Draw();
}

void PhySphere::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_shape->SetColor(r,g,b,a);
}

PhySphere::~PhySphere()
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
