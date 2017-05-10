#pragma once
#include "Vector3.h"

class Line2D
{
private:

public:
	CVector3 P1;
	CVector3 P2;

	Line2D();
	Line2D( CVector3 p1, CVector3 p2 );
	Line2D( CVector3* p1, CVector3* p2 );
	Line2D( float x1, float y1, float x2, float y2 );
	~Line2D();

	//float Slope();
	//float AngleInDegrees();

	bool AtSameSide(CVector3* p1, CVector3* p2);
	CVector3 IntersectionPoint(Line2D* line, bool* intersectionPointExist);
	float PerpendicularDist(CVector3* p);
	static float PerpendicularDist( CVector3* p1, CVector3* p2, CVector3* p );
	static CVector3 FootOfPerpendicular( CVector3* p1, CVector3* p2, CVector3* p );
};
