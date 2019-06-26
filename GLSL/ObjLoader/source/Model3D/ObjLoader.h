#ifndef ObjLoader_H
#define ObjLoader_H

#include "ShaderProgram.h"
#include <vector>
#include <string>
using namespace std;


class ObjLoader
{
private:
	unsigned int _vertexCount;

	unsigned int _vertexBufferID;
	unsigned int _normalBufferID;
	unsigned int _uvBufferID;

	unsigned int _baseTexID;

	ShaderProgram* _shaderProgram;

	void ReadObjFile(string filePath);
	void LoadTextures(string folderPath);

public:
    ObjLoader(string filePath);
    void Draw();
};

#endif