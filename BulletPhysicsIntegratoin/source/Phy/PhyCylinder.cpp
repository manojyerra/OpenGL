#include "PhyCylinder.h"
#include "PhyManager.h"
#include "DefinesAndIncludes.h"

PhyCylinder::PhyCylinder(float x, float y, float z, float r, float h, float massVal)
{
	_r = r;
	_h = h/2.0f;
	_d = r;

	_color[0] = _color[1] = _color[2] = _color[3] = 255;

	colliShape = new btCylinderShape( btVector3(btScalar(_r),btScalar(_h),btScalar(_d)) );

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
	_rigidBody->setFriction(btScalar(10.0f));
	_rigidBody->setRollingFriction(btScalar(10.0f));

	PhyManager::GetInstance()->GetDynamicWorld()->addRigidBody(_rigidBody);
}

bool PhyCylinder::isDynamic()
{
	return (_mass != 0.0f);
}

void PhyCylinder::Draw()
{
	float mat[16];
	btTransform trans;
	_rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(mat);

	glColor4ub(_color[0], _color[1], _color[2], _color[3]);
	glPushMatrix();
	glMultMatrixf(mat);

	float radius = _r;
	float halfLength = _h;
	float piVal = 3.14159265f;

	glBegin(GL_TRIANGLE_STRIP);
	for(int i=0; i<=360; i+=20)
	{
		float theta = i*piVal/180.0f;
		float nextTheta = (i+20)*piVal/180.0f;
		glVertex3f(radius*cos(theta),		halfLength,		radius*sin(theta));
		glVertex3f(radius*cos(nextTheta),	-halfLength,	radius*sin(nextTheta));
	}
	glEnd();
	
	glPopMatrix();
}

void PhyCylinder::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
	_color[3] = a;
}

PhyCylinder::~PhyCylinder()
{
	if(_rigidBody)
	{
		delete _rigidBody->getMotionState();
		delete colliShape;
		PhyManager::GetInstance()->GetDynamicWorld()->removeCollisionObject(_rigidBody);
		delete _rigidBody;
	}
}
