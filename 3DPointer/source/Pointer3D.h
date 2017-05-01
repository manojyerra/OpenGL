#ifndef Pointer3D_H
#define Pointer3D_H

#include "Util/GL2DState.h"
#include "Math/Vector3.h"

class Pointer3D
{
private:
	GL2DState state2D;

	void DrawPointerTri(CVector3 p1, CVector3 p2, unsigned int color);

public:
	Pointer3D();
	~Pointer3D();

	void Draw(float* mat);
};

#endif