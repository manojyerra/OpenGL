#ifndef FLModelReaderWriter_H
#define FLModelReaderWriter_H

#include "Util/GLUtil.h"
#include "Math/GLMat.h"
#include "Math/Vector3.h"
#include "Shapes3D/Shape.h"
#include "Util/GL2DState.h"
#include "Shape2D/Rect.h"
#include "Shapes3D/Box.h"
#include "Shapes3D/Cylinder.h"
#include "Shapes3D/Cone.h"
#include "Shapes3D/Sphere.h"

#include "FLModelBoundingShapes.h"

#include <vector>
#include <string>
using namespace std;

class FLModelReaderWriter
{
private:
	string _folderPath;

	unsigned char* _verticesPointer;
	unsigned char* _uvsPointer;
	unsigned char* _normalsPointer;
	unsigned char* _indicesPointer;
	unsigned int _indicesType;

	unsigned int _numVertex;
	unsigned int _numIndices;

	unsigned int _textureID;

	GLMat _mat;

	Box _aabb;

	FLModelBoundingShapes* _boundingShapes;

	void WriteMatrixToFile(FILE* matFile, float* mat);
	void ReadMatrixFromFile(FILE* matFile, float* mat);

	string GetBBoxFilePath(string folderPath);
	string GetOrientationFilePath(string folderPath);
	string GetBoundingShapesFilePath(string folderPath);
	void LoadBoundingShapesInfo(string folderPath, FLModelBoundingShapes* boundingShapes);

public:
	FLModelReaderWriter(FLModelBoundingShapes* boundingShapes);
	~FLModelReaderWriter();

	void Load(string folderPath);
	unsigned char* GetVerticesPointer();
	unsigned char* GetTexCoordsPointer();
	unsigned char* GetNormalsPointer();
	unsigned char* GetIndicesPointer();
	unsigned int GetIndicesType();

	unsigned int GetNumVertex();
	unsigned int GetNumIndices();

	unsigned int GetTextureID();

	GLMat GetMat();
	Box GetAABB();

	void Save();
	void Save(string folderPath);
	void SaveOrientation(string folderPath, float* mat);
	void SaveBBoxInfo(string folderPath, Box aabb);
	void SaveBoundingShapesInfo(string folderPath, FLModelBoundingShapes* boundingShapes);
};

#endif