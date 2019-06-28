#include <windows.h>
#include <gl/gl.h>
#include <vector>
#include <string>
#include "Vector3.h"

using namespace std;

class ObjToBinary
{
private:
	vector<CVector3> vertexVec;
	vector<CVector3> uvVec;
	vector<CVector3> normalVec;

	vector<float> vertexFloatArr;
	vector<float> uvFloatArr;
	vector<float> normalFloatArr;

public:
	ObjToBinary(string folderPath);
};
