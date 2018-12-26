#ifndef __RENDERER
#define __RENDERER
#pragma once

#include "MathLib/Line2D.h"
#include "MathLib/Line3D.h"
#include "MathLib/Triangle2D.h"
#include "MathLib/Triangle3D.h"

class Renderer
{
private:

	static int makeDiffColor;
	static int sphereR1;
	static int sphereG1;
	static int sphereB1;

	static int sphereR2;
	static int sphereG2;
	static int sphereB2;

public:
	Renderer();
	~Renderer();

	static void DrawLine2D(Line2D* line2D, int r=255, int g=255, int b=255, int a=255);
	static void DrawLine3D(Line3D* line3D, int r=255, int g=255, int b=255, int a=255);

	static void DrawTriangle2D(Triangle2D* triangle2D, int r=255, int g=255, int b=255, int a=255);
	static void DrawTriangle3D(Triangle3D* triangle3D, int r=255, int g=255, int b=255, int a=255);

	static void DrawCube(float cx, float cy, float cz, float w, float h, float d);
	
	static void SetSphereColor(int r1, int g1, int b1, int r2, int g2, int b2);
	static void DrawSphere(int textureID, float cx, float cy, float cz, float radius);

	static void DrawWireRect3D(Point* p1, Point* p2, int numPartsOnLine1, Point* p3, Point* p4, int numPartsOnLine2);
};

#endif
