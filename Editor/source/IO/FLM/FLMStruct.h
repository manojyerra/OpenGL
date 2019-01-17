#ifndef FLMStruct_H
#define FLMStruct_H

#include "Def.h"
#include "MathLib/Point.h"
#include <string>
using namespace std;

class FLMStruct
{
public:
	float* mat;
	char* texturePath;

	float minX;
	float minY;
	float minZ;

	float maxX;
	float maxY;
	float maxZ;

	float* vertexBuf;
	int numVertex;

	float* uvBuf;
	int numUVs;

	float* normalBuf;
	int numNormals;

	byte* indicesBuf;
	int numIndices;
	int indicesFormat;
	
	//int isStaticObject;
	int isHiddenObject;
	int meterialID;

	//For bounding shapes....

	int numBShapes;
	int* shapeIDArr;
	float** bShapeMatArr;
	float** scaleArr;

	FLMStruct();
	~FLMStruct();
	void SetMatByCopy(float* arr);
	void SetTexturePath(string path);
	void SetMinMax(Point min, Point max);
	void SetAsStaticObject(bool isStatic);
	void AddBoundingShape(int shapeID, float* mat, float* scale);
	void RemoveBoundingShape(int index);
	void RemoveAllBoundingShapes();
	void SetMeterialID(int meterialID);
	void DeleteBoundingShapeInfo();
	void Delete_Vertex_UV_Normal_Indices();
};

#endif