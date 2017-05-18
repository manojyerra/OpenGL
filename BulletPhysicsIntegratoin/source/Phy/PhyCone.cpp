#include "PhyCone.h"
#include "PhyManager.h"
//#include "Sui/Sui.h"



PhyCone::PhyCone(float* mat, float r, float h, float massVal)
{
	Init(mat, r, h, massVal);
}

PhyCone::PhyCone(float x, float y, float z, float r, float h, float massVal)
{
	float mat[16];
	memset(mat, 0, 16*sizeof(float));
	mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;

	mat[12] = x;
	mat[13] = y;
	mat[14] = z;

	Init(mat, r, h, massVal);
}

void PhyCone::Init(float* mat, float r, float h, float massVal)
{
	_r = r;
	_h = h;

	_shape = new Cone(mat, r,h);

	colliShape = new btConeShape(btScalar(_r),btScalar(_h));

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



bool PhyCone::isDynamic()
{
	return (_mass != 0.0f);
}

void PhyCone::Draw()
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

	//glColor4ub(_color[0], _color[1], _color[2], _color[3]);
	//glPushMatrix();
	//glMultMatrixf(mat);
	//
	//float radius = _r;
	//float halfLength = _h/2.0f;
	//float piVal = 3.14159265f;
	//
	//glBegin(GL_TRIANGLE_STRIP);
	//for(int i=0; i<=360; i+=20)
	//{
	//	float theta = i*piVal/180.0f;
	//	float nextTheta = (i+20)*piVal/180.0f;
	//	glVertex3f(0,		halfLength,		0);
	//	glVertex3f(radius*cos(nextTheta),	-halfLength,	radius*sin(nextTheta));
	//}
	//glEnd();
	//
	//glPopMatrix();
}

void PhyCone::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if(_shape)
		_shape->SetColor(r,g,b,a);
}

PhyCone::~PhyCone()
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
