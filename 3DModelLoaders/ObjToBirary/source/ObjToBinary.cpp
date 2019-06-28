#include "ObjToBinary.h"
#include "CFileReader.h"
#include "UtilFuncs.h"
#include "GLBuffer.h"
#include "UIntArray.h"

ObjToBinary::ObjToBinary(string folderPath)
{
	ReadFileWithArrays(folderPath);
	//ReadFileWithGLBuffer(folderPath);
	//ReadFileWithVectors(folderPath);
}

void ObjToBinary::ReadFileWithArrays(string folderPath)
{
	unsigned int startTime = GetTickCount();

	CFileReader fileReader(folderPath + "/objFile.obj", "rb");

	float vx, vy, vz;
	float tx, ty;
	float nx, ny, nz;

	vector<CVector3> vertexVec;
	vector<CVector3> uvVec;
	vector<CVector3> normalVec;

	//vx = vy = vz =0;
	//tx = ty = 0;
	//nx = ny = nz =0;

	UIntArray facesArr(1024*1024);

	char* line = NULL;

	while ((line = fileReader.ReadLine()) != NULL)
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			UtilFuncs::scan_vertex(line, &vx, &vy, &vz);
			vertexVec.push_back(CVector3(vx, vy, vz));
			free(line);
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			UtilFuncs::scan_uv(line, &tx, &ty);
			uvVec.push_back(CVector3(tx, 1 - ty, 0));
			free(line);
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			UtilFuncs::scan_normal(line, &nx, &ny, &nz);
			normalVec.push_back(CVector3(nx, ny, nz));
			free(line);
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			facesArr.push_back((unsigned int)line);
		}
	}

	unsigned int vertexReadTime = GetTickCount() - startTime;

	//fileReader.Reset();

	int v[3];
	int t[3];
	int n[3];

	for (int i = 0; i < 3; i++)
	{
		v[i] = 0;
		t[i] = 0;
		n[i] = 0;
	}

	bool uvsExist = uvVec.size() > 1;
	bool normalsExist = normalVec.size() > 1;
	
	unsigned int initSize = 1024*1024;

	FloatArray vertexFloatArr(initSize * 4);
	FloatArray uvFloatArr(initSize * 3);
	FloatArray normalFloatArr(initSize * 4);

	unsigned int fileParseStartTime = GetTickCount();

	if (uvsExist && normalsExist)
	{
		unsigned int facesStrArrSize = facesArr.size();
		unsigned int* facesStrArrPtr = (unsigned int*)facesArr.getArray();

		for(int arrIndex=0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];
			
			UtilFuncs::scanFace_VTN(line, &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]);

			for (int i = 0; i < 3; i++)
			{
				vertexFloatArr.push_back_3( vertexVec[v[i]-1] );
				uvFloatArr.push_back_2( uvVec[t[i] - 1] );
				normalFloatArr.push_back_3(normalVec[n[i] - 1]);
			}

			free(line);
		}
	}
	else if (uvsExist && !normalsExist)
	{
		unsigned int facesStrArrSize = facesArr.size();
		unsigned int* facesStrArrPtr = (unsigned int*)facesArr.getArray();

		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_VT(line, &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]);

			for (int i = 0; i < 3; i++)
			{
				vertexFloatArr.push_back_3(vertexVec[v[i] - 1]);
				uvFloatArr.push_back_2(uvVec[t[i] - 1]);
			}

			free(line);
		}
	}
	else if (!uvsExist && normalsExist)
	{
		unsigned int facesStrArrSize = facesArr.size();
		unsigned int* facesStrArrPtr = (unsigned int*)facesArr.getArray();

		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_VN(line, &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]);

			for (int i = 0; i < 3; i++)
			{
				vertexFloatArr.push_back_3(vertexVec[v[i] - 1]);
				normalFloatArr.push_back_3(normalVec[n[i] - 1]);
			}

			free(line);
		}
	}
	else if (!uvsExist && !normalsExist)
	{
		unsigned int facesStrArrSize = facesArr.size();
		unsigned int* facesStrArrPtr = (unsigned int*)facesArr.getArray();

		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_V(line, &v[0], &v[1], &v[2]);

			for (int i = 0; i < 3; i++)
			{
				vertexFloatArr.push_back_3(vertexVec[v[i] - 1]);
			}

			free(line);
		}
	}

	unsigned int facesParseTime = GetTickCount() - fileParseStartTime;
	unsigned int fileWriteStartTime = GetTickCount();

	if (vertexFloatArr.size() > 0)
	{
		string bufFilePath = folderPath + "/vertex.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(vertexFloatArr.getArray(), 4, vertexFloatArr.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (uvFloatArr.size() > 0)
	{
		string bufFilePath = folderPath + "/uv.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(uvFloatArr.getArray(), 4, uvFloatArr.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (normalFloatArr.size() > 0)
	{
		string bufFilePath = folderPath + "/normal.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(normalFloatArr.getArray(), 4, normalFloatArr.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	unsigned int fileWritingTime = GetTickCount() - fileWriteStartTime;
	unsigned int totalTime = GetTickCount() - startTime;
}


void ObjToBinary::ReadFileWithGLBuffer(string folderPath)
{
	unsigned int startTime = GetTickCount();

	CFileReader fileReader(folderPath + "/objFile.obj", "rb");

	char* line = NULL;

	float vx, vy, vz;
	float tx, ty;
	float nx, ny, nz;

	vector<CVector3> vertexVec;
	vector<CVector3> uvVec;
	vector<CVector3> normalVec;

	while ((line = fileReader.ReadLine()) != NULL)
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			sscanf(line, "v %f %f %f", &vx, &vy, &vz);
			vertexVec.push_back(CVector3(vx, vy, vz));
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			sscanf(line, "vt %f %f", &tx, &ty);
			uvVec.push_back(CVector3(tx, 1 - ty, 0));
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			sscanf(line, "vn %f %f %f", &nx, &ny, &nz);
			normalVec.push_back(CVector3(nx, ny, nz));
		}

		free(line);
	}

	fileReader.Reset();

	int v[3];
	int t[3];
	int n[3];

	for (int i = 0; i < 3; i++)
	{
		v[i] = 0;
		t[i] = 0;
		n[i] = 0;
	}

	bool uvsExist = uvVec.size() > 1;
	bool normalsExist = normalVec.size() > 1;

	unsigned int vertexReadTime = GetTickCount() - startTime;
	unsigned int fileParseStartTime = GetTickCount();

	GLBuffer* buffer = new GLBuffer(10 * 1024 * 1024, false, uvsExist, normalsExist);
	buffer->glBegin(GL_TRIANGLES);

	if (uvsExist && normalsExist)
	{
		while ((line = fileReader.ReadLine()) != NULL)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]);

				for (int i = 0; i < 3; i++)
				{
					buffer->glTexCoord2f(uvVec[t[i] - 1]);
					buffer->glNormal3f(normalVec[n[i] - 1]);
					buffer->glVertex3f(vertexVec[v[i] - 1]);
				}
			}

			free(line);
		}
	}
	else if (uvsExist && !normalsExist)
	{
		while ((line = fileReader.ReadLine()) != NULL)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				sscanf(line, "f %d/%d %d/%d %d/%d", &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]);

				for (int i = 0; i < 3; i++)
				{
					buffer->glTexCoord2f(uvVec[t[i] - 1]);
					buffer->glVertex3f(vertexVec[v[i] - 1]);
				}
			}

			free(line);
		}
	}
	else if (!uvsExist && normalsExist)
	{
		while ((line = fileReader.ReadLine()) != NULL)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				sscanf(line, "f %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]);

				for (int i = 0; i < 3; i++)
				{
					buffer->glNormal3f(normalVec[n[i] - 1]);
					buffer->glVertex3f(vertexVec[v[i] - 1]);
				}
			}

			free(line);
		}
	}
	else if (!uvsExist && !normalsExist)
	{
		while ((line = fileReader.ReadLine()) != NULL)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				sscanf(line, "f %d %d %d", &v[0], &v[1], &v[2]);

				for (int i = 0; i < 3; i++)
				{
					buffer->glVertex3f(vertexVec[v[i] - 1]);
				}
			}

			free(line);
		}
	}

	unsigned int facesParseTime = GetTickCount() - fileParseStartTime;
	unsigned int fileWriteStartTime = GetTickCount();

	unsigned int vertexCount = buffer->GetVertexCount();

	GLfloat* vertexArr = buffer->GetVertexArr();

	if (vertexCount > 0)
	{
		string bufFilePath = folderPath + "/vertex.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&vertexArr[0], 12, vertexCount, bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (uvsExist)
	{
		GLfloat* uvArr = buffer->GetUVArr();

		string bufFilePath = folderPath + "/uv.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&uvArr[0], 8, vertexCount, bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (normalsExist)
	{
		GLfloat* normalArr = buffer->GetNormalArr();

		string bufFilePath = folderPath + "/normal.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&normalArr[0], 12, vertexCount, bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	unsigned int fileWritingTime = GetTickCount() - fileWriteStartTime;
	unsigned int totalTime = GetTickCount() - startTime;

	delete buffer;
}


void ObjToBinary::ReadFileWithVectors(string folderPath)
{
	unsigned int startTime = GetTickCount();

	CFileReader fileReader(folderPath + "/objFile.obj", "rb");

	char* line = NULL;

	float vx, vy, vz;
	float tx, ty;
	float nx, ny, nz;

	vector<CVector3> vertexVec;
	vector<CVector3> uvVec;
	vector<CVector3> normalVec;

	while ((line = fileReader.ReadLine()) != NULL)
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			sscanf(line, "v %f %f %f", &vx, &vy, &vz);
			vertexVec.push_back(CVector3(vx, vy, vz));
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			sscanf(line, "vt %f %f", &tx, &ty);
			uvVec.push_back(CVector3(tx, 1 - ty, 0));
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			sscanf(line, "vn %f %f %f", &nx, &ny, &nz);
			normalVec.push_back(CVector3(nx, ny, nz));
		}

		free(line);
	}

	fileReader.Reset();

	int v[3];
	int t[3];
	int n[3];

	for (int i = 0; i < 3; i++)
	{
		v[i] = 0;
		t[i] = 0;
		n[i] = 0;
	}

	bool uvsExist = uvVec.size() > 1;
	bool normalsExist = normalVec.size() > 1;

	vector<float> vertexFloatArr;
	vector<float> uvFloatArr;
	vector<float> normalFloatArr;

	if (uvsExist && normalsExist)
	{
		while ((line = fileReader.ReadLine()) != NULL)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]);

				for (int i = 0; i < 3; i++)
				{
					vertexFloatArr.push_back(vertexVec[v[i] - 1].x);
					vertexFloatArr.push_back(vertexVec[v[i] - 1].y);
					vertexFloatArr.push_back(vertexVec[v[i] - 1].z);

					uvFloatArr.push_back(uvVec[t[i] - 1].x);
					uvFloatArr.push_back(uvVec[t[i] - 1].y);

					normalFloatArr.push_back(normalVec[n[i] - 1].x);
					normalFloatArr.push_back(normalVec[n[i] - 1].y);
					normalFloatArr.push_back(normalVec[n[i] - 1].z);
				}
			}

			free(line);
		}
	}
	else if (uvsExist && !normalsExist)
	{
		while ((line = fileReader.ReadLine()) != NULL)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				sscanf(line, "f %d/%d %d/%d %d/%d", &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]);

				for (int i = 0; i < 3; i++)
				{
					vertexFloatArr.push_back(vertexVec[v[i] - 1].x);
					vertexFloatArr.push_back(vertexVec[v[i] - 1].y);
					vertexFloatArr.push_back(vertexVec[v[i] - 1].z);

					uvFloatArr.push_back(uvVec[t[i] - 1].x);
					uvFloatArr.push_back(uvVec[t[i] - 1].y);
				}
			}

			free(line);
		}
	}
	else if (!uvsExist && normalsExist)
	{
		while ((line = fileReader.ReadLine()) != NULL)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				sscanf(line, "f %d//%d %d//%d %d//%d", &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]);

				for (int i = 0; i < 3; i++)
				{
					vertexFloatArr.push_back(vertexVec[v[i] - 1].x);
					vertexFloatArr.push_back(vertexVec[v[i] - 1].y);
					vertexFloatArr.push_back(vertexVec[v[i] - 1].z);

					normalFloatArr.push_back(normalVec[n[i] - 1].x);
					normalFloatArr.push_back(normalVec[n[i] - 1].y);
					normalFloatArr.push_back(normalVec[n[i] - 1].z);
				}
			}

			free(line);
		}
	}
	else if (!uvsExist && !normalsExist)
	{
		while ((line = fileReader.ReadLine()) != NULL)
		{
			if (line[0] == 'f' && line[1] == ' ')
			{
				sscanf(line, "f %d %d %d", &v[0], &v[1], &v[2]);

				for (int i = 0; i < 3; i++)
				{
					vertexFloatArr.push_back(vertexVec[v[i] - 1].x);
					vertexFloatArr.push_back(vertexVec[v[i] - 1].y);
					vertexFloatArr.push_back(vertexVec[v[i] - 1].z);
				}
			}

			free(line);
		}
	}

	if (vertexFloatArr.size() > 0)
	{
		string bufFilePath = folderPath + "/vertex.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&vertexFloatArr[0], 4, vertexFloatArr.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (uvFloatArr.size() > 0)
	{
		string bufFilePath = folderPath + "/uv.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&uvFloatArr[0], 4, uvFloatArr.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (normalFloatArr.size() > 0)
	{
		string bufFilePath = folderPath + "/normal.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&normalFloatArr[0], 4, normalFloatArr.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	unsigned int totalTime = GetTickCount() - startTime;
}
