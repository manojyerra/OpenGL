#ifndef PhyCone_H
#define PhyCone_H
#include "btBulletDynamicsCommon.h"
//#include "Sui/SuiIncludes.h"
#include "Shapes3D/Cone.h"

class PhyCone
{
private:
	btRigidBody* _rigidBody;
	btCollisionShape* colliShape;

	float _r;
	float _h;
	btScalar _mass;

	Cone* _shape;

public:
	PhyCone(float x, float y, float z, float r, float h, float massVal);
	~PhyCone();

	void Draw();
	bool isDynamic();
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};

#endif
