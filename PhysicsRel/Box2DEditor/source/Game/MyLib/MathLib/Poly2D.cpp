#include "Poly2D.h"
#include "LineSeg2D.h"
#include "Line2D.h"
#include "Triangle2D.h"

Poly2D::Poly2D(vector<Point> vec)
{
    pointsVec = vec;
}

Poly2D::Poly2D(float* vertex, int vertexCount)
{
	int count = 0;
	for(int i=0;i<vertexCount*2;i+=2)
	{
		pointsVec.push_back( Point(vertex[i], vertex[i+1]) );
		count++;
	}
}

Poly2D::~Poly2D()
{
}

bool Poly2D::Contains(Point* p)
{
    if(pointsVec.size()<3) return false;

    Triangle2D triangle2D(Point(0,0,0), Point(0,0,0), Point(0,0,0));

    if(pointsVec.size()==3)
    {
        triangle2D.P1 = pointsVec[0];
        triangle2D.P2 = pointsVec[1];
        triangle2D.P3 = pointsVec[2];

        return triangle2D.Contains(p);
    }
    else
    {
		for(int i=0;i<(int)pointsVec.size();i++)
		{
			if(p->y == pointsVec[i].y && p->x < pointsVec[i].x)
			{
				p->y++;
				break;
			}
		}

		LineSeg2D lineSegHor(p->x, p->y, GetMaxX()+100, p->y);
		LineSeg2D polyLineSeg(Point(0,0), Point(0,0));

		int colliCountHor = 0;

		for(int i=1;i<=(int)pointsVec.size();i++)
		{
			polyLineSeg.P1.x = pointsVec[i-1].x;
			polyLineSeg.P1.y = pointsVec[i-1].y;

			if(i != pointsVec.size())
			{
				polyLineSeg.P2.x = pointsVec[i].x;
				polyLineSeg.P2.y = pointsVec[i].y;
			}
			else
			{
				polyLineSeg.P2.x = pointsVec[0].x;
				polyLineSeg.P2.y = pointsVec[0].y;
			}

			Point collPoint = polyLineSeg.IntersectionPoint(&lineSegHor);

			if(collPoint.EXIST)
				colliCountHor++;
		}

		return (colliCountHor%2 == 1);
	}

	return false;
}



bool Poly2D::Contains( float x, float y)
{
	Point p = Point(x, y);
	return Contains( &p );
}


bool Poly2D::IsConvexPolygon()
{
	//for(int i=0; i<pointsVec.size(); i++)
	//{
	//	vector<Point> poly;

	//	for(int j=0; j<pointsVec.size(); j++)
	//	{
	//		if( j != i)
	//			poly.push_back(pointsVec[j]);
	//	}

	//	if(Contains(&pointsVec[i]) == true)
	//		return false;
	//}

	//return true;

	for(int i=1; i<(int)pointsVec.size(); i++)
	{
		Line2D line( pointsVec[i-1], pointsVec[i]);
		
		int count = 0;

		for(int j=1; j<(int)pointsVec.size(); j++)
		{
			//int jPrev = j-1;
			//int jNext = j+1;

			//if(jPrev < 0)
			//	jPrev = pointsVec.size()-1;
			//else if(jPrev >= pointsVec.size())
			//	jPrev = 0;

			//if(jNext < 0)
			//	jNext = pointsVec.size()-1;
			//else if(jNext >= pointsVec.size())
			//	jNext = 0;

			//if(j!=i && jPrev!=i && jNext!=i)

			LineSeg2D lineSeg( pointsVec[j-1], pointsVec[j]);

			bool noNeedToCheck =	(lineSeg.P1.x == line.P1.x && lineSeg.P1.y == line.P1.y) ||
									(lineSeg.P1.x == line.P2.x && lineSeg.P1.y == line.P2.y) ||
									(lineSeg.P2.x == line.P1.x && lineSeg.P2.y == line.P1.y) ||
									(lineSeg.P2.x == line.P2.x && lineSeg.P2.y == line.P2.y);

			if( noNeedToCheck == false)
			{
				if(lineSeg.IsColliding(&line))
				{
					count++;
					int a =0;
				}
			}
		}

		if(count > 0)
			return false;
	}

	return true;
}


float Poly2D::GetMinX()
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

float Poly2D::GetMinY()
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

float Poly2D::GetMaxX()
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

float Poly2D::GetMaxY()
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
