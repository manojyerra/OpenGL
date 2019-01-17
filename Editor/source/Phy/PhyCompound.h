#ifndef PhyCompound_H
#define PhyCompound_H
#include "btBulletDynamicsCommon.h"
#include "Sui/SuiIncludes.h"

#include "Shapes/Shape.h"
#include "Shapes/Box.h"
#include "Shapes/Sphere.h"
#include "Shapes/Cone.h"
#include "Shapes/Cylinder.h"

#include <vector>
using namespace std;

class PhyCompound
{
private:
	btRigidBody* _rigidBody;
	btCollisionShape* colliShape;
	btScalar _mass;
	
	vector<Shape*> _primVec;

	void DrawCube(float w, float h, float d);

public:
	PhyCompound();
	~PhyCompound();

	void MakeCompoundShape(float* mat, float massVal);
	void MakeCompoundShape(float x, float y, float z, float massVal);

	bool isDynamic();
	btMotionState* GetMotionState();
	btCollisionShape* GetCollisionShape();
	void GetOrientation(float* mat);

	void AddBox(float x, float y, float z, float w, float h, float d);
	void AddBox(float* mat, float w, float h, float d);

	void AddSphere(float x, float y, float z, float r);
	void AddSphere(float* mat, float r);

	void AddCone(float x, float y, float z, float r, float h);
	void AddCone(float* mat, float r, float h);

	void AddCylinder(float x, float y, float z, float r, float h);
	void AddCylinder(float* mat, float r, float h);

	void AddMesh(float* mat, btTriangleMesh* mTriMesh);

	void Draw();
};

#endif