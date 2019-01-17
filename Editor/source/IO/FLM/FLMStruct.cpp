#include "FLMStruct.h"
#include <stdio.h>

FLMStruct::FLMStruct()
{
	mat = NULL;
	texturePath = NULL;

	minX = 0;
	minY = 0;
	minZ = 0;

	maxX = 0;
	maxY = 0;
	maxZ = 0;

	vertexBuf = NULL;
	numVertex = 0;

	uvBuf = NULL;
	numUVs = 0;

	normalBuf = NULL;
	numNormals = 0;

	indicesBuf = NULL;
	numIndices = 0;
	indicesFormat = 0;

	//isStaticObject = 0;
	isHiddenObject = 0;
	meterialID = 0;

	//For bounding shapes...
	numBShapes = 0;
	shapeIDArr = NULL;
	bShapeMatArr = NULL;
	scaleArr = NULL;
}

void FLMStruct::AddBoundingShape(int shapeID, float* mat, float* scale)
{
	//Create temp memory
	int currBShapes = numBShapes+1;

	int* temp_shapeIDArr = new int[currBShapes];

	float** temp_bShapeMatArr = new float*[currBShapes];
	for(int i=0;i<currBShapes;i++)
		temp_bShapeMatArr[i]=new float[16];

	float** temp_scaleArr = new float*[currBShapes];
	for(int i=0;i<currBShapes;i++)
		temp_scaleArr[i]=new float[3];


	//Copy old memory
	for(int i=0;i<numBShapes; i++)
	{
		temp_shapeIDArr[i] = shapeIDArr[i];
		
		for(int j=0;j<16;j++)
			temp_bShapeMatArr[i][j] = bShapeMatArr[i][j];

		for(int j=0;j<3;j++)
			temp_scaleArr[i][j] = scaleArr[i][j];
	}

	DeleteBoundingShapeInfo();

	//Add Bounding shape
	int lastIndex = currBShapes-1;

	temp_shapeIDArr[lastIndex] = shapeID;
	
	for(int j=0;j<16;j++)
		temp_bShapeMatArr[lastIndex][j] = mat[j];

	for(int j=0;j<3;j++)
		temp_scaleArr[lastIndex][j] = scale[j];

	//Assign back 
	shapeIDArr =  temp_shapeIDArr;
	bShapeMatArr = temp_bShapeMatArr;
	scaleArr = temp_scaleArr;

	numBShapes = currBShapes;
}

void FLMStruct::RemoveBoundingShape(int index)
{
	if( (index >=0 && index < numBShapes) == false)
		return;


	//Create new memory
	int currBShapes = numBShapes-1;

	int* temp_shapeIDArr = new int[currBShapes];

	float** temp_bShapeMatArr = new float*[currBShapes];
	for(int i=0;i<currBShapes;i++)
		temp_bShapeMatArr[i]=new float[16];

	float** temp_scaleArr = new float*[currBShapes];
	for(int i=0;i<currBShapes;i++)
		temp_scaleArr[i]=new float[3];


	//Copy old to new memory
	int count = 0;
	for(int i=0;i<numBShapes; i++)
	{
		if(i == index)
			continue;

		temp_shapeIDArr[count] = shapeIDArr[i];
		
		for(int j=0;j<16;j++)
			temp_bShapeMatArr[count][j] = bShapeMatArr[i][j];

		for(int j=0;j<3;j++)
			temp_scaleArr[count][j] = scaleArr[i][j];

		count++;
	}

	DeleteBoundingShapeInfo();

	//Assign back 
	shapeIDArr =  temp_shapeIDArr;
	bShapeMatArr = temp_bShapeMatArr;
	scaleArr = temp_scaleArr;

	numBShapes = currBShapes;
}

void FLMStruct::RemoveAllBoundingShapes()
{
	DeleteBoundingShapeInfo();
	numBShapes = 0;
}

void FLMStruct::SetMatByCopy(float* arr)
{
	if(mat == NULL)
	{
		mat = new float[16];
		memset(mat, 0, 16*sizeof(float));
		mat[0] = mat[5] = mat[10]= mat[15] = 1.0f;
	}

	for(int i=0;i<16;i++)
		mat[i] = arr[i];
}

void FLMStruct::SetTexturePath(string path)
{
	if(path.length() > 0)
	{
		if(texturePath)
		{
			delete[] texturePath;
			texturePath = NULL;
		}

		texturePath = new char[path.length()+1];
		sprintf(texturePath, "%s\0", path.c_str());
	}
	else
	{
		if(texturePath)
		{
			delete[] texturePath;
			texturePath = NULL;
		}
	}
}

void FLMStruct::SetMinMax(Point min, Point max)
{
	minX = min.x;
	minY = min.y;
	minZ = min.z;

	maxX = max.x;
	maxY = max.y;
	maxZ = max.z;
}

void FLMStruct::SetAsStaticObject(bool isStatic)
{
	meterialID = 0;
}

void FLMStruct::SetMeterialID(int meterial)
{
	meterialID = meterial;
}

void FLMStruct::Delete_Vertex_UV_Normal_Indices()
{
	if(vertexBuf)
	{
		delete[] vertexBuf;
		vertexBuf = NULL;
	}
	if(uvBuf)
	{
		delete[] uvBuf;
		uvBuf = NULL;
	}
	if(normalBuf)
	{
		delete[] normalBuf;
		normalBuf = NULL;
	}
	if(indicesBuf)
	{
		delete[] indicesBuf;
		indicesBuf = NULL;
	}
}

void FLMStruct::DeleteBoundingShapeInfo()
{
	if(shapeIDArr)
	{
		delete[] shapeIDArr;
		shapeIDArr = NULL;
	}

	if(bShapeMatArr)
	{
		for(int i=0;i<numBShapes;i++)
		{
			delete[] bShapeMatArr[i];
			bShapeMatArr[i] = NULL;
		}

		delete[] bShapeMatArr;
		bShapeMatArr = NULL;
	}

	if(scaleArr)
	{
		for(int i=0;i<numBShapes;i++)
		{
			delete[] scaleArr[i];
			scaleArr[i] = NULL;
		}

		delete[] scaleArr;
		scaleArr = NULL;
	}
}

FLMStruct::~FLMStruct()
{
	if(mat)
	{
		delete[] mat;
		mat = NULL;
	}

	if(texturePath)
	{
		delete[] texturePath;
		texturePath = NULL;
	}

	DeleteBoundingShapeInfo();

	Delete_Vertex_UV_Normal_Indices();
}