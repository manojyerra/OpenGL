#include "DefinesAndIncludes.h"
#include "Vector3.h"
#include "FloatArray.h"

#include <vector>
#include <string>
using namespace std;

class ObjToBinary
{
private:
	void ReadFileWithGLBuffer(string folderPath);
	void ReadFileWithVectors(string folderPath);
	void ReadFileWithArrays(string folderPath);

public:
	ObjToBinary(string folderPath);
};
