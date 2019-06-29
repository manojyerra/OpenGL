#include "ObjLoader.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "UtilFuncs.h"
#include "Vector3.h"

ObjLoader::ObjLoader(string folderPath)
{
	ReadObjFile(folderPath+"/objFile.obj");

	ImageBuffer* imgBuf = new ImageBuffer(folderPath+"/texture.png");
	_textureID = UtilFuncs::GenerateGLTextureID(imgBuf);
}

void ObjLoader::ReadObjFile(string filePath)
{
	unsigned int startTime = GetTickCount();

	CFileReader fileReader(filePath, "rb");

	float vx, vy, vz;
	float tx, ty;
	float nx, ny, nz;

	vector<CVector3> vertexVec;
	vector<CVector3> uvVec;
	vector<CVector3> normalVec;

	UIntArray facesArr(1024 * 1024);

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

	unsigned int initSize = 1024 * 1024;

	vertexFloatArr.addCapacity(initSize * 4);
	uvFloatArr.addCapacity(initSize * 3);
	normalFloatArr.addCapacity(initSize * 4);

	unsigned int fileParseStartTime = GetTickCount();

	if (uvsExist && normalsExist)
	{
		unsigned int facesStrArrSize = facesArr.size();
		unsigned int* facesStrArrPtr = (unsigned int*)facesArr.getArray();

		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_VTN(line, &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]);

			for (int i = 0; i < 3; i++)
			{
				vertexFloatArr.push_back_3(vertexVec[v[i] - 1]);
				uvFloatArr.push_back_2(uvVec[t[i] - 1]);
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
}


void ObjLoader::WriteBinaryData(string folderPath)
{
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

}

void ObjLoader::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	if(_textureID > 0 && uvFloatArr.size() > 1)
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, uvFloatArr.getArray());
	}
	
	if(normalFloatArr.size() > 1)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, normalFloatArr.getArray());
	}

	glVertexPointer(3, GL_FLOAT, 0, vertexFloatArr.getArray());
	glDrawArrays(GL_TRIANGLES, 0, vertexFloatArr.size()/3);

	if(normalFloatArr.size() > 1)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if(_textureID > 0 && uvFloatArr.size() > 1)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}
