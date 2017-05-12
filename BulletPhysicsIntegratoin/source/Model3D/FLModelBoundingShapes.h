#ifndef FLModelBoundingShapes_H
#define FLModelBoundingShapes_H

#include "Util/GLUtil.h"
#include "Math/GLMat.h"
#include "Math/Vector3.h"
#include "Shapes3D/Shape.h"
#include "Util/GL2DState.h"
#include "Shape2D/Rect.h"
#include "Shapes3D/Box.h"
#include "FLModelReaderWriter.h"
#include "FLModelBorder.h"

#include <windows.h>
#include <gl/gl.h>
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
	Shape* AddBestBoudingShapeByVerticesOnRect(Rect* rect, float* verArr, int numVertex, float* mat);
	Shape* AddBoudingShapeByVerticesOnRect(Rect* rect, int boundingShapeID, float* verArr, int numVertex, float* mat);

	unsigned int Size();
	Shape* Get(unsigned int index);

	void Draw();
};

#endif