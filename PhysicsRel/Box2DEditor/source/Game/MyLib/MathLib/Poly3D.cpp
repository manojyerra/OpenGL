#include "Poly3D.h"
#include "Triangle3D.h"
#include "LineSeg2D.h"

Poly3D::Poly3D(vector<Point> vec)
{
	pointsVec = vec;
}

Poly3D::~Poly3D()
{
}

bool Poly3D::Contains(Point* p)
{
	if(pointsVec.size()<3) return false;

	Triangle3D triangle3D(Point(0,0,0), Point(0,0,0), Point(0,0,0));

	if(pointsVec.size()==3)
	{
		triangle3D.P1 = pointsVec[0];
		triangle3D.P2 = pointsVec[1];
		triangle3D.P3 = pointsVec[2];

		return triangle3D.Contains(p);
	}
	else
	{
		LineSeg2D lineSegHor(p->x, p->y, GetMaxX()+100, p->y);
		LineSeg2D lineSegVer(p->x, p->y, p->x, GetMaxY()+100);

		LineSeg2D polyLineSeg(Point(0,0), Point(0,0));

		int colliCountHor = 0;
		int colliCountVer = 0;

		int prevColliXHor = 0;
		int prevColliYHor = 0;

		int prevColliXVer = 0;
		int prevColliYVer = 0;

		bool isFirstColliPointHor = true;
		bool isFirstColliPointVer = true;

		for(int i=0;i<(int)pointsVec.size();i++)
		{
			polyLineSeg.P1.x = pointsVec[i].x;
			polyLineSeg.P1.y = pointsVec[i].y;

			if(i+1 == pointsVec.size())
			{
				polyLineSeg.P2.x = pointsVec[0].x;
				polyLineSeg.P2.y = pointsVec[0].y;
			}
			else
			{
				polyLineSeg.P2.x = pointsVec[i+1].x;
				polyLineSeg.P2.y = pointsVec[i+1].y;
			}

			Point pHor = polyLineSeg.IntersectionPoint(&lineSegHor);
			Point pVer = polyLineSeg.IntersectionPoint(&lineSegVer);

			if(pHor.EXIST)
			{
				float A1 = polyLineSeg.P2.y- polyLineSeg.P1.y;
				float B1 = polyLineSeg.P1.x - polyLineSeg.P2.x;
				float C1 = polyLineSeg.P2.x * polyLineSeg.P1.y - polyLineSeg.P1.x * polyLineSeg.P2.y;

				float A2 = lineSegHor.P2.y- lineSegHor.P1.y;
				float B2 = lineSegHor.P1.x - lineSegHor.P2.x;
				float C2 = lineSegHor.P2.x * lineSegHor.P1.y - lineSegHor.P1.x * lineSegHor.P2.y;

				float denom = A1*B2 - A2*B1;

				if(denom)
				{
					//if(isFirstColliPointHor)
					//{
					//	isFirstColliPointHor = false;
					//	prevColliXHor = pHor.x;
					//	prevColliYHor = pHor.y;
					//	colliCountHor++;
					//}
					//else if( (pHor.x == prevColliXHor && pHor.y == prevColliYHor) == false)
					//{
					//	prevColliXHor = pHor.x;
					//	prevColliYHor = pHor.y;
						colliCountHor++;
					//}
				}
			}

			if(pVer.EXIST)
			{
				float A1 = polyLineSeg.P2.y- polyLineSeg.P1.y;
				float B1 = polyLineSeg.P1.x - polyLineSeg.P2.x;
				float C1 = polyLineSeg.P2.x * polyLineSeg.P1.y - polyLineSeg.P1.x * polyLineSeg.P2.y;

				float A2 = lineSegVer.P2.y - lineSegVer.P1.y;
				float B2 = lineSegVer.P1.x - lineSegVer.P2.x;
				float C2 = lineSegVer.P2.x * lineSegVer.P1.y - lineSegVer.P1.x * lineSegVer.P2.y;

				float denom = A1*B2 - A2*B1;

				if(denom)
				{
					//if(isFirstColliPointVer)
					//{
					//	isFirstColliPointVer = false;
					//	prevColliXVer = pVer.x;
					//	prevColliYVer = pVer.y;
					//	colliCountVer++;
					//}
					//else if( (pVer.x == prevColliXVer && pVer.y == prevColliYVer) == false)
					//{
					//	prevColliXVer = pVer.x;
					//	prevColliYVer = pVer.y;
						colliCountVer++;
					//}
				}
			}
		}

		return( (colliCountHor != 0 && colliCountHor%2 == 1) || (colliCountVer != 0 && colliCountVer%2 == 1));
	}

	return false;
}

bool Poly3D::Contains( float x, float y, float z )
{
	Point p = Point(x, y, z);
	return Contains( &p );
}


float Poly3D::GetMinX()
{
	if(pointsVec.size()<=0)
		return 0;

	float minX = pointsVec[0].x;

	for(int i=1;i<(int)pointsVec.size();i++)
	{
		if(pointsVec[i].x < minX)
			minX = pointsVec[i].x;
	}
	return minX;
}

float Poly3D::GetMinY()
{
	if(pointsVec.size()<=0)
		return 0;

	float minY = pointsVec[0].y;

	for(int i=1;i<(int)pointsVec.size();i++)
	{
		if(pointsVec[i].y < minY)
			minY = pointsVec[i].y;
	}
	return minY;
}

float Poly3D::GetMaxX()
{
	if(pointsVec.size()<=0)
		return 0;

	float maxX = pointsVec[0].x;

	for(int i=1;i<(int)pointsVec.size();i++)
	{
		if(pointsVec[i].x > maxX)
			maxX = pointsVec[i].x;
	}
	return maxX;
}

float Poly3D::GetMaxY()
{
	if(pointsVec.size()<=0)
		return 0;

	float maxY = pointsVec[0].y;

	for(int i=1;i<(int)pointsVec.size();i++)
	{
		if(pointsVec[i].y > maxY)
			maxY = pointsVec[i].y;
	}
	return maxY;
}
