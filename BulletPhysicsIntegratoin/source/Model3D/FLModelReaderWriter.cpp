#include "FLModelReaderWriter.h"
#include "ImageBuffer.h"
#include "CFileReader.h"

FLModelReaderWriter::FLModelReaderWriter()
{
	_verticesPointer = NULL;
	_uvsPointer = NULL;
	_normalsPointer = NULL;
	_indicesPointer = NULL;
	_indicesType = NULL;

	_numVertex = NULL;
	_numIndices = NULL;

	_textureID = 0;
}

void FLModelReaderWriter::Load(string folderPath)
{
	if(CFileReader::IsFileExists(folderPath+"/vertex.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath+"/vertex.buf", "rb");
		_verticesPointer = (unsigned char*)malloc(fileReader->GetLength());
		memcpy(_verticesPointer, fileReader->GetData(), fileReader->GetLength());
		_numVertex = fileReader->GetLength() / 12;
		delete fileReader;
	}

	if(CFileReader::IsFileExists(folderPath+"/uv.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath+"/uv.buf", "rb");
		_uvsPointer = (unsigned char*)malloc(fileReader->GetLength());
		memcpy(_uvsPointer, fileReader->GetData(), fileReader->GetLength());
		delete fileReader;
	}

	if(CFileReader::IsFileExists(folderPath+"/normal.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath+"/normal.buf", "rb");
		_normalsPointer = (unsigned char*)malloc(fileReader->GetLength());
		memcpy(_normalsPointer, fileReader->GetData(), fileReader->GetLength());
		delete fileReader;
	}

	if(CFileReader::IsFileExists(folderPath+"/index.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath+"/index.buf", "rb");
		int fileLen = fileReader->GetLength();
		_indicesPointer = (unsigned char*)malloc(fileLen);
		memcpy(_indicesPointer, fileReader->GetData(), fileLen);
		delete fileReader;

		int val = _indicesPointer[fileLen-4];

		if(val == 1)		_indicesType = GL_UNSIGNED_BYTE;
		else if(val == 2)	_indicesType = GL_UNSIGNED_SHORT;
		else if(val == 4)	_indicesType = GL_UNSIGNED_INT;

		_numIndices = (fileLen-4)/val;
	}

	ImageBuffer* imgBuf = new ImageBuffer(folderPath+"/texture.png");

	_textureID = GLUtil::GenerateGLTextureID(	imgBuf->GetWidth(), imgBuf->GetHeight(), 
											imgBuf->GetBytesPerPixel(), imgBuf->GetBuffer());

	delete imgBuf;


	FILE* aabbFile = fopen( GetBBoxFilePath(folderPath).c_str(), "r" );

	if(aabbFile == NULL)
	{
		_aabb = Box::CalcAABB((float*)_verticesPointer, _numVertex*3);
		_aabb.SetRandomColorAlpha(128);
	}
	else
	{
		CVector3 size;
		CVector3 pos;

		fscanf(aabbFile, "%f %f %f %f %f %f", &pos.x, &pos.y, &pos.z, &size.x, &size.y, &size.z);
		fflush(aabbFile);
		fclose(aabbFile);

		_aabb.SetPos(pos.x, pos.y, pos.z);
		_aabb.SetSize(size.x, size.y, size.z);
	}


	FILE* matFile = fopen( GetOrientationFilePath(folderPath).c_str(), "r" );

	float* m = _mat.m;

	if(matFile)
	{
		fscanf(matFile, "%f %f %f %f\n",&m[0] ,&m[1] ,&m[2] ,&m[3]);
		fscanf(matFile, "%f %f %f %f\n",&m[4] ,&m[5] ,&m[6] ,&m[7]);
		fscanf(matFile, "%f %f %f %f\n",&m[8] ,&m[9] ,&m[10] ,&m[11]);
		fscanf(matFile, "%f %f %f %f",&m[12] ,&m[13] ,&m[14] ,&m[15]);

		fclose(matFile);
	}
}

string FLModelReaderWriter::GetBBoxFilePath(string folderPath)
{
	return folderPath+"/bbox.txt";
}

string FLModelReaderWriter::GetOrientationFilePath(string folderPath)
{
	return folderPath+"/transformation.txt";
}

unsigned char* FLModelReaderWriter::GetVerticesPointer()	{ return _verticesPointer;	}
unsigned char* FLModelReaderWriter::GetTexCoordsPointer()	{ return _uvsPointer;		}
unsigned char* FLModelReaderWriter::GetNormalsPointer()		{ return _normalsPointer;	}
unsigned char* FLModelReaderWriter::GetIndicesPointer()		{ return _indicesPointer;	}
unsigned int FLModelReaderWriter::GetIndicesType()			{ return _indicesType;		}

unsigned int FLModelReaderWriter::GetNumVertex()			{ return _numVertex;		}
unsigned int FLModelReaderWriter::GetNumIndices()			{ return _numIndices;		}

unsigned int FLModelReaderWriter::GetTextureID()			{ return _textureID;		}
GLMat FLModelReaderWriter::GetMat()							{ return _mat;				}
Box FLModelReaderWriter::GetAABB()							{ return _aabb;				}


void FLModelReaderWriter::Save()
{
	Save(_folderPath);
}

void FLModelReaderWriter::Save(string folderPath)
{
	SaveOrientation(folderPath, _mat.m);
	SaveBBoxInfo(folderPath, _aabb);
}

void FLModelReaderWriter::SaveOrientation(string folderPath, float* mat)
{
	FILE* matFile = fopen(GetOrientationFilePath(folderPath).c_str(), "w");
	
	fprintf(matFile, "%f %f %f %f\n",mat[0], mat[1], mat[2], mat[3]);
	fprintf(matFile, "%f %f %f %f\n",mat[4], mat[5], mat[6], mat[7]);
	fprintf(matFile, "%f %f %f %f\n",mat[8], mat[9], mat[10], mat[11]);
	fprintf(matFile, "%f %f %f %f",	mat[12], mat[13], mat[14], mat[15]);

	fflush(matFile);
	fclose(matFile);
}

void FLModelReaderWriter::SaveBBoxInfo(string folderPath, Box aabb)
{
	FILE* bBoxFile = fopen( GetBBoxFilePath(folderPath).c_str(), "w" );
	
	CVector3 pos = aabb.GetPos();
	CVector3 size = aabb.GetSize();

	fprintf(bBoxFile, "%f %f %f %f %f %f", pos.x, pos.y, pos.z, size.x, size.y, size.z);

	fflush(bBoxFile);
	fclose(bBoxFile);
}

FLModelReaderWriter::~FLModelReaderWriter()
{
	if(_verticesPointer)
		free(_verticesPointer);
	
	if(_uvsPointer)
		free(_uvsPointer);

	if(_normalsPointer)
		free(_normalsPointer);

	if(_indicesPointer)
		free(_indicesPointer);
}