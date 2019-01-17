#ifndef PhyBox_H
#define PhyBox_H
#include "btBulletDynamicsCommon.h"
#include "Shapes/Box.h"
	
class PhyBox
{
private:
	btRigidBody* _rigidBody;
	btCollisionShape* colliShape;
	btScalar _mass;

	float _w;
	float _h;
	float _d;

	Box* _shape;

	void DrawCube(float w, float h, float d);

public:
	PhyBox(float* mat, float w, float h, float d, float massVal);
	PhyBox(float x, float y, float z, float w, float h, float d, float massVal);
	~PhyBox();

	void Draw();
	bool isDynamic();
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	btMotionState* GetMotionState();
	btCollisionShape* GetCollisionShape();
	void GetOrientation(float* mat);
};

#endif