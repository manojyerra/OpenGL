#include "Projection.h"

Projection::Projection(float l, float r, float b, float t, float zn, float zf)
{
	SetProjection(l, r, b, t, zn, zf);
}

void Projection::SetProjection(float l, float r, float b, float t, float n, float f)
{
	_mat.m[0] = 2 * n / (r - l);
	_mat.m[1] = 0;
	_mat.m[2] = 0;
	_mat.m[3] = 0;

	_mat.m[4] = 0;
	_mat.m[5] = 2 * n / (t - b);
	_mat.m[6] = 0;
	_mat.m[7] = 0;

	_mat.m[8] = (r + l) / (r - l);
	_mat.m[9] = (t + b) / (t - b);
	_mat.m[10] = -(f + n) / (f - n);
	_mat.m[11] = -1;

	_mat.m[12] = 0;
	_mat.m[13] = 0;
	_mat.m[14] = -(2 * f*n) / (f - n);
	_mat.m[15] = 0;
}

GLMat Projection::getProjection()
{
	return _mat;
}

