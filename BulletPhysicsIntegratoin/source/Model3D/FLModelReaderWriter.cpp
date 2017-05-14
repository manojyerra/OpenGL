#include "FLModelReaderWriter.h"
#include "ImageBuffer.h"
#include "CFileReader.h"

FLModelReaderWriter::FLModelReaderWriter(FLModelBoundingShapes* boundingShapes)
{
	_boundingShapes = boundingShapes;

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
	_folderPath = folderPath;

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


	FILE* aabbFile = fopen( GetAABBFilePath(folderPath).c_str(), "r" );

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
		ReadMatrixFromFile(matFile, m);
		fclose(matFile);
	}

	ReadBoundingShapesInfo(folderPath, _boundingShapes);
}

void FLModelReaderWriter::ReadBoundingShapesInfo(string folderPath, FLModelBoundingShapes* boundingShapes)
{
	string boundingFilePath = GetBoundingShapesFilePath(folderPath);

	if(CFileReader::IsFileExists(boundingFilePath))
	{
		CFileReader* fileReader = new CFileReader(boundingFilePath, "r");

		char* line = NULL;

		while( (line = fileReader->ReadLine()) != NULL)
		{
			string str(line);

			if(str.length() > 3 && str[0]=='B' && str[1]=='o'&& str[2]=='x')
			{
				float w = 0;
				float h = 0;
				float d = 0;

				sscanf(line, "Box %f %f %f\n", &w, &h, &d);

				float mat[16];
				
				ReadMatrixFromCFileReader(fileReader, mat);

				boundingShapes->AddBoundingBox(w, h, d, mat);
			}
			else if(str.length() > 8 && str[0]=='C' && str[1]=='y'&& str[2]=='l' && str[3] == 'i')
			{
				float r = 0;
				float h = 0;

				sscanf(line, "Cylinder %f %f\n", &r, &h);

				float mat[16];
				
				ReadMatrixFromCFileReader(fileReader, mat);

				boundingShapes->AddBoudningCylinder(r, h, mat);
			}
			else if(str.length() > 8 && str[0]=='C' && str[1]=='o'&& str[2]=='n' && str[3] == 'e')
			{
				float r = 0;
				float h = 0;

				sscanf(line, "Cone %f %f\n", &r, &h);

				float mat[16];
				
				ReadMatrixFromCFileReader(fileReader, mat);

				boundingShapes->AddBoundingCone(r, h, mat);
			}
			else if(str.length() > 8 && str[0]=='S' && str[1]=='p'&& str[2]=='h' && str[3] == 'e')
			{
				float r = 0;

				sscanf(line, "Sphere %f\n", &r);

				float mat[16];
				
				ReadMatrixFromCFileReader(fileReader, mat);

				boundingShapes->AddBoundingSphere(r, mat);
			}

			free(line);
		}

		delete fileReader;
	}
}

string FLModelReaderWriter::GetAABBFilePath(string folderPath)				{ return folderPath+"/aabb.txt";			}
string FLModelReaderWriter::GetBoundingShapesFilePath(string folderPath)	{ return folderPath+"/boundingShapes.txt";	}
string FLModelReaderWriter::GetOrientationFilePath(string folderPath)		{ return folderPath+"/transformation.txt";	}

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


void FLModelReaderWriter::WriteOrientation(string folderPath, float* mat)
{
	FILE* matFile = fopen(GetOrientationFilePath(folderPath).c_str(), "w");
	
	WriteMatrixToFile(matFile, mat);

	fflush(matFile);
	fclose(matFile);
}

void FLModelReaderWriter::WriteAABBInfo(string folderPath, Box aabb)
{
	FILE* bBoxFile = fopen( GetAABBFilePath(folderPath).c_str(), "w" );
	
	CVector3 pos = aabb.GetPos();
	CVector3 size = aabb.GetSize();

	fprintf(bBoxFile, "%f %f %f %f %f %f", pos.x, pos.y, pos.z, size.x, size.y, size.z);

	fflush(bBoxFile);
	fclose(bBoxFile);
}

void FLModelReaderWriter::WriteBoundingShapesInfo(string folderPath, FLModelBoundingShapes* boundingShapes)
{
	FILE* bShapesFile = fopen( GetBoundingShapesFilePath(folderPath).c_str(), "w" );

	for(int i=0; i<boundingShapes->Size(); i++)
	{
		Shape* shape = boundingShapes->Get(i);

		float* mat = shape->GetGLMatrix();
		
		if(shape->GetID() == Shape::BOX)
		{
			Box* box = (Box*)shape;

			CVector3 size = box->GetSize();

			fprintf(bShapesFile, "Box %f %f %f\n", size.x, size.y, size.z);
			WriteMatrixToFile(bShapesFile, mat);
		}
		else if(shape->GetID() == Shape::CYLINDER)
		{
			Cylinder* cylinder = (Cylinder*)shape;

			float r = cylinder->GetRadius();
			float h = cylinder->GetHeight();

			fprintf(bShapesFile, "Cylinder %f %f\n", r, h);
			WriteMatrixToFile(bShapesFile, mat);
		}
		else if(shape->GetID() == Shape::CONE)
		{
			Cone* cone = (Cone*)shape;

			float r = cone->GetRadius();
			float h = cone->GetHeight();

			fprintf(bShapesFile, "Cone %f %f\n", r, h);
			WriteMatrixToFile(bShapesFile, mat);
		}
		else if(shape->GetID() == Shape::SPHERE)
		{
			Sphere* sphere = (Sphere*)shape;

			float r = sphere->GetRadius();
			
			fprintf(bShapesFile, "Sphere %f\n", r);
			WriteMatrixToFile(bShapesFile, mat);
		}
	}
}

void FLModelReaderWriter::WriteMatrixToFile(FILE* matFile, float* mat)
{
	fprintf(matFile, "%f %f %f %f\n",mat[0], mat[1], mat[2], mat[3]);
	fprintf(matFile, "%f %f %f %f\n",mat[4], mat[5], mat[6], mat[7]);
	fprintf(matFile, "%f %f %f %f\n",mat[8], mat[9], mat[10], mat[11]);
	fprintf(matFile, "%f %f %f %f\n",mat[12], mat[13], mat[14], mat[15]);
}

void FLModelReaderWriter::ReadMatrixFromFile(FILE* matFile, float* m)
{
	fscanf(matFile, "%f %f %f %f\n",&m[0], &m[1], &m[2], &m[3]);
	fscanf(matFile, "%f %f %f %f\n",&m[4], &m[5], &m[6], &m[7]);
	fscanf(matFile, "%f %f %f %f\n",&m[8], &m[9], &m[10],&m[11]);
	fscanf(matFile, "%f %f %f %f\n",&m[12],&m[13],&m[14],&m[15]);
}

void FLModelReaderWriter::ReadMatrixFromVectorLines(vector<string>* vecLines, float* m)
{
	sscanf(vecLines->at(0).c_str(), "%f %f %f %f\n",&m[0], &m[1], &m[2], &m[3]);
	sscanf(vecLines->at(1).c_str(), "%f %f %f %f\n",&m[4], &m[5], &m[6], &m[7]);
	sscanf(vecLines->at(2).c_str(), "%f %f %f %f\n",&m[8], &m[9], &m[10],&m[11]);
	sscanf(vecLines->at(3).c_str(), "%f %f %f %f\n",&m[12],&m[13],&m[14],&m[15]);
}

void FLModelReaderWriter::ReadMatrixFromCFileReader(CFileReader* fileReader, float* mat)
{
	vector<string> vecLines;
	char* line1;
	char* line2;
	char* line3;
	char* line4;

	vecLines.push_back( (line1 = fileReader->ReadLine()) ); free(line1);
	vecLines.push_back( (line2 = fileReader->ReadLine()) ); free(line2);
	vecLines.push_back( (line3 = fileReader->ReadLine()) ); free(line3);
	vecLines.push_back( (line4 = fileReader->ReadLine()) ); free(line4);

	ReadMatrixFromVectorLines(&vecLines, mat);
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
