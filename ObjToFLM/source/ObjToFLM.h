#include <windows.h>
#include <gl/gl.h>
#include <vector>
#include <string>
using namespace std;

class Point
{
public:
	float x;
	float y;
	float z;

	Point()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Point(float xVal, float yVal, float zVal)
	{
		x = xVal;
		y = yVal;
		z = zVal;
	}

	bool IsSame(Point* p)
	{
		return(p->x == x && p->y == y && p->z == z);
	}

	bool IsSame(float xx, float yy, float zz)
	{
		return(xx == x && yy == y && zz == z);
	}
};

class Face
{
public:
	Point v;
	Point t;
	Point n;
	bool filled;

	Face()
	{
		filled = false;
	}

	Face(Point ver, Point uv, Point nor)
	{
		v = ver;
		t = uv;
		n = nor;
		filled = true;
	}

	bool IsSame(Face* face)
	{
		return (face->v.IsSame(&v) && face->t.IsSame(&t) && face->n.IsSame(&n) );
	}
};


class ObjToFLM
{
private:
	vector<Point> vertexVec;
	vector<Point> uvVec;
	vector<Point> normalVec;

	vector<float> vertexFloatArr;
	vector<float> uvFloatArr;
	vector<float> normalFloatArr;

	vector<Face> _faceVec;
	vector<int> _indVec;
	vector<float> vertexIndArr;
	vector<float> uvIndArr;
	vector<float> normalIndArr;

	int ContainsFace(Face* face);
	int GetIndicesType();

public:
    ObjToFLM(string filePath);
};
