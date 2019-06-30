#ifndef ObjToFLMNew_H
#define ObjToFLMNew_H

#include "DefinesAndIncludes.h"

#include "FloatArray.h"
#include "UIntArray.h"

#include <vector>
#include <string>
#include <map>
#include <iterator>

using namespace std;

class ObjToFLMNew
{
private:
	void WriteData(string folderPath, vector<unsigned int>& indVec,
		vector<CVector3>& vertexVec, vector<CVector3>& normalVecNew, vector<CVector3>& uvVecNew);

public:
	ObjToFLMNew(string filePath);
};

#endif