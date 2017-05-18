#ifndef PhyCylinder_H
#define PhyCylinder_H

#include "btBulletDynamicsCommon.h"
#include "Shapes3D/Cylinder.h"

class PhyCylinder
{
private:
	btRigidBody* _rigidBody;
	btCollisionShape* colliShape;
	btScalar _mass;

	float _r;
	float _h;
	float _d;

	Cylinder* _shape;

	void Init(float* mat, float r, float h, float massVal);

public:
	PhyCylinder(float x, float y, float z, float r, float h, float massVal);
	PhyCylinder(float* mat, float r, float h, float massVal);
	~PhyCylinder();

	void Draw();
	bool isDynamic();
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};

#endif
