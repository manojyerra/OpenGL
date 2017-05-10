#include "PhyCompound.h"
#include "PhyManager.h"
//#include "Sui/Sui.h"

PhyCompound::PhyCompound()
{
	_primVec.clear();
	colliShape = new btCompoundShape();
}


void PhyCompound::MakeCompoundShape(float x, float y, float z, float massVal)
{
	float mat[16];
	for(int i=0;i<16;i++)
		mat[i] = 0;

	mat[0] = 1;
	mat[5] = 1;
	mat[10] = 1;
	mat[15] = 1;

	mat[12] = x;
	mat[13] = y;
	mat[14] = z;

	MakeCompoundShape(mat, massVal);
}

void PhyCompound::MakeCompoundShape(float* mat, float massVal)
{
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

void PhyCompound::AddBox(float x, float y, float z, float w, float h, float d)
{
	Box* prim = new Box(x,y,z, w,h,d);
	prim->SetColor(rand()%255, rand()%255, rand()%255, 255);
	_primVec.push_back((Shape*)prim);

	btCollisionShape* shape = new btBoxShape(btVector3(w,h,d));
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(x,y,z));
	((btCompoundShape*)colliShape)->addChildShape(trans,shape);
}

void PhyCompound::AddBox(float* mat, float w, float h, float d)
{
	Box* prim = new Box(0,0,0, w,h,d);
	prim->SetGLMatrix(mat);
	prim->SetColor(rand()%255, rand()%255, rand()%255, 255);
	_primVec.push_back((Shape*)prim);

	btCollisionShape* shape = new btBoxShape(btVector3(w,h,d));
	btTransform trans;
	trans.setFromOpenGLMatrix(mat);
	((btCompoundShape*)colliShape)->addChildShape(trans,shape);
}

void PhyCompound::AddSphere(float x, float y, float z, float r)
{
	Sphere* prim = new Sphere(x,y,z, r);
	prim->SetColor(rand()%255, rand()%255, rand()%255, 255);
	_primVec.push_back((Shape*)prim);

	btCollisionShape* shape = new btSphereShape(btScalar(r));
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(x,y,z));
	((btCompoundShape*)colliShape)->addChildShape(trans,shape);
}

void PhyCompound::AddSphere(float* mat, float r)
{
	Sphere* prim = new Sphere(0,0,0, r);
	prim->SetGLMatrix(mat);
	prim->SetColor(rand()%255, rand()%255, rand()%255, 255);
	_primVec.push_back((Shape*)prim);

	btCollisionShape* shape = new btSphereShape(btScalar(r));
	btTransform trans;
	trans.setFromOpenGLMatrix(mat);
	((btCompoundShape*)colliShape)->addChildShape(trans,shape);
}

void PhyCompound::AddCone(float x, float y, float z, float r, float h)
{
	Cone* prim = new Cone(x,y,z, r,h);
	prim->SetColor(rand()%255, rand()%255, rand()%255, 255);
	_primVec.push_back((Shape*)prim);

	btCollisionShape* shape = new btConeShape(btScalar(r), btScalar(h));
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(x,y,z));
	((btCompoundShape*)colliShape)->addChildShape(trans,shape);
}

void PhyCompound::AddCone(float* mat, float r, float h)
{
	Cone* prim = new Cone(0,0,0, r,h);
	prim->SetGLMatrix(mat);
	prim->SetColor(rand()%255, rand()%255, rand()%255, 255);
	_primVec.push_back((Shape*)prim);

	btCollisionShape* shape = new btConeShape(btScalar(r), btScalar(h));
	btTransform trans;
	trans.setFromOpenGLMatrix(mat);
	((btCompoundShape*)colliShape)->addChildShape(trans,shape);
}
void PhyCompound::AddCylinder(float x, float y, float z, float r, float h)
{
	Cylinder* prim = new Cylinder(x,y,z, r,h);
	prim->SetColor(rand()%255, rand()%255, rand()%255, 255);
	_primVec.push_back((Shape*)prim);

	btCollisionShape* shape = new btConeShape(btScalar(r), btScalar(h));
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(x,y,z));
	((btCompoundShape*)colliShape)->addChildShape(trans,shape);
}

void PhyCompound::AddCylinder(float* mat, float r, float h)
{
	Cylinder* prim = new Cylinder(0,0,0, r,h);
	prim->SetGLMatrix(mat);
	prim->SetColor(rand()%255, rand()%255, rand()%255, 255);
	_primVec.push_back((Shape*)prim);

	btCollisionShape* shape = new btCylinderShape(btVector3(btScalar(r),btScalar(h),btScalar(r)));
	btTransform trans;
	trans.setFromOpenGLMatrix(mat);
	((btCompoundShape*)colliShape)->addChildShape(trans,shape);
}

void PhyCompound::AddMesh(float* mat, btTriangleMesh* mTriMesh)
{
	btCollisionShape* shape = new btBvhTriangleMeshShape(mTriMesh,true);
	btTransform trans;
	trans.setFromOpenGLMatrix(mat);
	((btCompoundShape*)colliShape)->addChildShape(trans,shape);
}

bool PhyCompound::isDynamic()
{
	return (_mass != 0.0f);
}

btMotionState* PhyCompound::GetMotionState()
{
	return _rigidBody->getMotionState();
}

btCollisionShape* PhyCompound::GetCollisionShape()
{
	return colliShape;
}

void PhyCompound::GetOrientation(float* mat)
{
	btTransform trans;
	_rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(mat);
}

void PhyCompound::Draw()
{
	float mat[16];
	btTransform trans;
	_rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(mat);

	glPushMatrix();
	glMultMatrixf(mat);

	for(unsigned int i=0;i<_primVec.size();i++)
		_primVec[i]->Draw();

	glPopMatrix();
}

void PhyCompound::DrawCube(float w, float h, float d)
{
	glBegin(GL_QUADS);

	// Front Face
	glVertex3f(-w, -h,  d);  // Bottom Left Of The Texture and Quad
	glVertex3f( w, -h,  d);  // Bottom Right Of The Texture and Quad
	glVertex3f( w,  h,  d);  // Top Right Of The Texture and Quad
	glVertex3f(-w,  h,  d);  // Top Left Of The Texture and Quad
	// Back Face
	glVertex3f(-w, -h, -d);  // Bottom Right Of The Texture and Quad
	glVertex3f(-w,  h, -d);  // Top Right Of The Texture and Quad
	glVertex3f( w,  h, -d);  // Top Left Of The Texture and Quad
	glVertex3f( w, -h, -d);  // Bottom Left Of The Texture and Quad
	// Top Face
	glVertex3f(-w,  h, -d);  // Top Left Of The Texture and Quad
	glVertex3f(-w,  h,  d);  // Bottom Left Of The Texture and Quad
	glVertex3f( w,  h,  d);  // Bottom Right Of The Texture and Quad
	glVertex3f( w,  h, -d);  // Top Right Of The Texture and Quad
	// Bottom Face
	glVertex3f(-w, -h, -d);  // Top Right Of The Texture and Quad
	glVertex3f( w, -h, -d);  // Top Left Of The Texture and Quad
	glVertex3f( w, -h,  d);  // Bottom Left Of The Texture and Quad
	glVertex3f(-w, -h,  d);  // Bottom Right Of The Texture and Quad
	// Right face
	glVertex3f( w, -h, -d);  // Bottom Right Of The Texture and Quad
	glVertex3f( w,  h, -d);  // Top Right Of The Texture and Quad
	glVertex3f( w,  h,  d);  // Top Left Of The Texture and Quad
	glVertex3f( w, -h,  d);  // Bottom Left Of The Texture and Quad
	// Left Face
	glVertex3f(-w, -h, -d);  // Bottom Left Of The Texture and Quad
	glVertex3f(-w, -h,  d);  // Bottom Right Of The Texture and Quad
	glVertex3f(-w,  h,  d);  // Top Right Of The Texture and Quad
	glVertex3f(-w,  h, -d);  // Top Left Of The Texture and Quad

	glEnd();
}

PhyCompound::~PhyCompound()
{
	int numChilds = ((btCompoundShape*)colliShape)->getNumChildShapes();

	for(int i=0;i<numChilds;)
	{
		btCollisionShape* childColliShape = ((btCompoundShape*)colliShape)->getChildShape(i);
		((btCompoundShape*)colliShape)->removeChildShape(childColliShape);
		numChilds = ((btCompoundShape*)colliShape)->getNumChildShapes();
		delete childColliShape;
	}

	delete ((btCollisionShape*)colliShape);

	if(_rigidBody)
	{
		btCollisionObject* obj = (btCollisionObject*)_rigidBody;
		btRigidBody* body = btRigidBody::upcast(obj);
		if(body && body->getMotionState())
			delete body->getMotionState();
		PhyManager::GetInstance()->GetDynamicWorld()->removeCollisionObject(obj);
		delete obj;
	}

	for(unsigned int i=0;i<_primVec.size();i++)
	{
		if(_primVec[i])
		{
			delete _primVec[i];
			_primVec[i] = NULL;
		}
	}
	_primVec.clear();
}
