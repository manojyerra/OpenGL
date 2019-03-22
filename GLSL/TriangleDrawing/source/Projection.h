#pragma once
#ifndef Projection_H
#define Projection_H

#include "GLMat.h"

class Projection
{
private:
	GLMat _mat;

public:
	Projection(float l, float r, float b, float t, float zn, float zf);
	void SetProjection(float l, float r, float b, float t, float zn, float zf);
	GLMat getProjection();
};

#endif