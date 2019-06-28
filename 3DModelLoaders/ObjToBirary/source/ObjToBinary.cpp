#include "ObjToBinary.h"
#include "CFileReader.h"
#include "UtilFuncs.h"
#include "GLBuffer.h"
#include "DefinesAndIncludes.h"

ObjToBinary::ObjToBinary(string folderPath)
{
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

	GLBuffer* buffer = new GLBuffer(10*1024*1024, false, uvsExist, normalsExist);
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
					buffer->glTexCoord2f(uvVec[t[i]-1]);
					buffer->glNormal3f(normalVec[n[i]-1]);
					buffer->glVertex3f(vertexVec[v[i]-1]);
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
					buffer->glTexCoord2f(uvVec[t[i]-1]);
					buffer->glVertex3f(vertexVec[v[i]-1]);
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
					buffer->glNormal3f(normalVec[n[i]-1]);
					buffer->glVertex3f(vertexVec[v[i]-1]);
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
					buffer->glVertex3f(vertexVec[v[i]-1]);
				}
			}

			free(line);
		}
	}

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

	if(uvsExist)
	{
		GLfloat* uvArr = buffer->GetUVArr();

		string bufFilePath = folderPath + "/uv.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&uvArr[0], 8, vertexCount, bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}
	
	if(normalsExist)
	{
		GLfloat* normalArr = buffer->GetNormalArr();

		string bufFilePath = folderPath + "/normal.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&normalArr[0], 12, vertexCount, bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	delete buffer;
}
