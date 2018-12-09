#pragma once
#include "Vector3.h"

class Triangle2D
{
private:
	static float Dist(CVector3 p1, CVector3 p2);
	static CVector3 IntersectionPoint(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p4);

public:
	CVector3 P1;
	CVector3 P2;
	CVector3 P3;

	Triangle2D();
	Triangle2D( CVector3 p1, CVector3 p2, CVector3 p3 );
	Triangle2D( float x1, float y1, float x2, float y2, float x3, float y3 );
	~Triangle2D();

	bool Contains(CVector3* p);
	bool Contains( float x, float y );
	static bool Contains(CVector3* p1, CVector3* p2, CVector3* p3, CVector3* p );

	bool IsColliding(Triangle2D* triangle);

	//static TwoPos GetP3(CVector3 p1, float distanceFromP1, CVector3 p2, float distanceFromP2);

	static CVector3 FindIntersectionByPercentages(CVector3 t1, CVector3 t2, CVector3 t3, float percent13, float percent23);
	static CVector3 FindPercentages(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p);
};
