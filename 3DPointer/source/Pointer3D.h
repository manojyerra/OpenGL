#ifndef Pointer3D_H
#define Pointer3D_H

#include "Util/GL2DState.h"
#include "Math/Vector3.h"
#include "Math/Triangle2D.h"

#include <vector>
using namespace std;

class Pointer3D
{
private:
	GL2DState state2D;

	bool _heldPointer;
	int _triIndex;
	
	vector<Triangle2D> triVec;
	CVector3 offSetVec;


	void DrawPointerTri(CVector3 p1, CVector3 p2, unsigned int color);
	int GetTri(float x, float y);
	CVector3 Get3DPos(float* mat, int triIndex, float mouseX, float mouseY);

public:
	CVector3 _pos;

	Pointer3D();
	~Pointer3D();

	bool IsPointerDragged();
	float Len();

	void Draw(float* mat);
};

#endif