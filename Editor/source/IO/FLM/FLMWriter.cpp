#include "FLMWriter.h"
#include "Util/VectorByte.h"

FLMWriter::FLMWriter(FLMStruct* FLMStruct)
{
	_bin = FLMStruct;
}

void FLMWriter::Write(string filePath)
{
	BinaryObjectWriter bow;

	bow.AddFloatPointer(_bin->mat, 16);
	bow.AddBytePointer((byte*)_bin->texturePath);

	bow.AddFloat(_bin->minX);
	bow.AddFloat(_bin->minY);
	bow.AddFloat(_bin->minZ);

	bow.AddFloat(_bin->maxX);
	bow.AddFloat(_bin->maxY);
	bow.AddFloat(_bin->maxZ);

	bow.AddFloatPointer(_bin->vertexBuf, _bin->numVertex*3);
	bow.AddInt(_bin->numVertex);

	bow.AddFloatPointer(_bin->uvBuf, _bin->numUVs*2);
	bow.AddInt(_bin->numUVs);

	bow.AddFloatPointer(_bin->normalBuf, _bin->numNormals*3);
	bow.AddInt(_bin->numNormals);

	int indicesBufSize = 0;

	if(_bin->numVertex < 256)
		indicesBufSize = _bin->numIndices*1;
	else if(_bin->numVertex < 65000)
		indicesBufSize = _bin->numIndices*2;
	else
		indicesBufSize = _bin->numIndices*4;

	bow.AddBytePointer(_bin->indicesBuf, indicesBufSize);
	bow.AddInt(_bin->numIndices);
	bow.AddInt(_bin->indicesFormat);
	
	//bow.AddInt(_bin->isStaticObject);
	bow.AddInt(_bin->isHiddenObject);
	bow.AddInt(_bin->meterialID);

	//For bounding shapes....

	bow.AddInt(_bin->numBShapes);
	bow.AddIntPointer(_bin->shapeIDArr, _bin->numBShapes);
	bow.AddFloatDDPointer(_bin->bShapeMatArr, _bin->numBShapes, 16);
	bow.AddFloatDDPointer(_bin->scaleArr, _bin->numBShapes, 3);

	bow.Write(filePath);
}

FLMWriter::~FLMWriter()
{
}
