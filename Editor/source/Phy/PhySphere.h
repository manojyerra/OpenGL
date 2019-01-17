#ifndef PhySphere_H
#define PhySphere_H
#include "btBulletDynamicsCommon.h"
#include "Sui/Sui.h"
#include "Shapes/Sphere.h"

class PhySphere
{
private:
	btRigidBody* _rigidBody;
	btCollisionShape* colliShape;
	btScalar _mass;
	float _r;

	Sphere* _shape;

public:
	PhySphere(float x, float y, float z, float r, float massVal);
	~PhySphere();

	void Draw();
	bool isDynamic();
	btMotionState* GetMotionState();
	btCollisionShape* GetCollisionShape();
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};

#endif