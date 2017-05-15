#ifndef FLModelBoundingShapes_H
#define FLModelBoundingShapes_H

#include "DefinesAndIncludes.h"
#include "Util/GLUtil.h"
#include "Math/GLMat.h"
#include "Math/Vector3.h"
#include "Shapes3D/Shape.h"
#include "Util/GL2DState.h"
#include "Shape2D/Rect.h"
#include "Shapes3D/Box.h"
#include "Shapes3D/Cylinder.h"
#include "Shapes3D/Cone.h"
#include "Shapes3D/Sphere.h"
#include "FLModelBorder.h"

#include <vector>
#include <string>
using namespace std;


class FLModelBoundingShapes
{
private:
	vector<Shape*> _boundingShapes;

public:
	FLModelBoundingShapes();
	~FLModelBoundingShapes();

	void AddBoundingShape(Shape* shape);
	void AddBoundingBox(float w, float h, float d, float* mat);
	void AddBoundingCone(float r, float h, float* mat);
	void AddBoudningCylinder(float r, float h, float* mat);
	void AddBoundingSphere(float r, float* mat);

	Shape* AddBestBoudingShapeByVerticesOnRect(Rect* rect, float* verArr, int numVertex, float* mat);
	Shape* AddBoudingShapeByVerticesOnRect(Rect* rect, int boundingShapeID, float* verArr, int numVertex, float* mat);

	unsigned int Size();
	Shape* Get(unsigned int index);

	void Draw();
};

#endif