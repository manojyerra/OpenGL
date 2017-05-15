#ifndef FLModelBorder_H
#define FLModelBorder_H

#include "DefinesAndIncludes.h"
#include "Util/GL2DState.h"
#include "Shape2D/Rect.h"
#include <vector>
#include <string>
using namespace std;

#define U_INT unsigned int

class FLModelBorder
{
private:
	vector<float> _borderVec;
	GL2DState state2D;

public:
	FLModelBorder();
	~FLModelBorder();

	void CalcBorder(float* verArr, U_INT  numIndices, U_INT  indicesType, unsigned char* indicesPointer, float* mat, Rect* rect);
	void DrawBorder();
};

#endif