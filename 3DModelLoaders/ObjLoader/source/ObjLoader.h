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
	vector<Point> vertexVec;
	vector<Point> uvVec;
	vector<Point> normalVec;

	vector<float> vertexFloatArr;
	vector<float> uvFloatArr;
	vector<float> normalFloatArr;

	unsigned int _textureID;

	void ReadObjFile(string filePath);

public:
    ObjLoader(string filePath);
    void Draw();
};
