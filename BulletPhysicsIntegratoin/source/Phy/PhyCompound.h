#ifndef PhyCompound_H
#define PhyCompound_H

#include "btBulletDynamicsCommon.h"
//#include "Sui/SuiIncludes.h"

#include "Shapes3D/Shape.h"
#include "Shapes3D/Box.h"
#include "Shapes3D/Sphere.h"
#include "Shapes3D/Cone.h"
#include "Shapes3D/Cylinder.h"

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
