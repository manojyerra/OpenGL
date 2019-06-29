#ifndef ObjLoader_H
#define ObjLoader_H

#include "DefinesAndIncludes.h"
#include "FloatArray.h"
#include "UIntArray.h"

#include <vector>
#include <string>
using namespace std;

class Point
{
public:
	float x;
	float y;
	float z;

	Point(float xVal, float yVal, float zVal)
	{
		x = xVal;
		y = yVal;
		z = zVal;
	}
};


class ObjLoader
{
private:
	FloatArray vertexFloatArr;
	FloatArray uvFloatArr;
	FloatArray normalFloatArr;

	unsigned int _textureID;

	void ReadObjFile(string filePath);

public:
    ObjLoader(string filePath);
    void Draw();
};

#endif
