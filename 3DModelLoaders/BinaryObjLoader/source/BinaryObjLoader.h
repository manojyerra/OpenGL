#ifndef BinaryObjLoader_H
#define BinaryObjLoader_H

#include "DefinesAndIncludes.h"
#include "FloatArray.h"
#include "UIntArray.h"

#include <vector>
#include <string>
using namespace std;

class BinaryObjLoader
{
private:
	unsigned char* _verticesPointer;
	unsigned char* _normalsPointer;
	unsigned char* _uvsPointer;

	unsigned int _numVertex;

	unsigned int _textureID;

	void ReadBinaryObjFile(string folderPath);

public:
	BinaryObjLoader(string filePath);
    void Draw();
};

#endif
