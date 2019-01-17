#ifndef ModelBorder_H
#define ModelBorder_H

#include "Model3D/Model3D.h"
#include <vector>
using namespace std;

class ModelBorder
{
private:
	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;

	vector<int> x;
	vector<int> y;

public:
	ModelBorder();
	~ModelBorder();

	void SetModelColor(unsigned int color);
	void SetModelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void CalcBorder(Model3D* model3D);
	void DrawBorder();
};

#endif