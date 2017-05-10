#ifndef PhyCylinder_H
#define PhyCylinder_H

#include "btBulletDynamicsCommon.h"
//#include "Sui/Sui.h"

class PhyCylinder
{
private:
	btRigidBody* _rigidBody;
	btCollisionShape* colliShape;

	float _r;
	float _h;
	float _d;

	btScalar _mass;

	unsigned char _color[4];

public:
	PhyCylinder(float x, float y, float z, float r, float h, float massVal);
	~PhyCylinder();

	void Draw();
	bool isDynamic();
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};

#endif