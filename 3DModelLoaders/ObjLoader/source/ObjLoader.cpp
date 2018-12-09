#include "ObjLoader.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "UtilFuncs.h"

ObjLoader::ObjLoader(string folderPath)
{
	ReadObjFile(folderPath+"/objFile.obj");

	ImageBuffer* imgBuf = new ImageBuffer(folderPath+"/texture.png");
	_textureID = UtilFuncs::GenerateGLTextureID(imgBuf);
}

void ObjLoader::ReadObjFile(string filePath)
{
	CFileReader fileReader(filePath.c_str(), "rb");
	
	char* line = NULL;

	float vx, vy, vz;
	float tx, ty;
	float nx, ny, nz;

	while( (line = fileReader.ReadLine())!= NULL)
	{
		if(line[0] == 'v' && line[1] == ' ')
		{
			sscanf(line, "v %f %f %f", &vx, &vy, &vz);
			vertexVec.push_back(Point(vx, vy, vz));
		}
		else if(line[0] == 'v' && line[1] == 't')
		{
			sscanf(line, "vt %f %f", &tx, &ty);
			uvVec.push_back(Point(tx, 1-ty, 0));
		}
		else if(line[0] == 'v' && line[1] == 'n')
		{
			sscanf(line, "vn %f %f %f", &nx, &ny, &nz);
			normalVec.push_back(Point(nx, ny, nz));
		}

		free(line);
	}

	fileReader.Reset();

	int v[3];
	int t[3];
	int n[3];
			
	for(int i=0; i<3; i++)
	{
		v[i] = 0;
		t[i] = 0;
		n[i] = 0;
	}

	bool uvsExist = uvVec.size() > 1;
	bool normalsExist = normalVec.size() > 1;

	while((line = fileReader.ReadLine())!= NULL)
	{
		if(line[0] == 'f' && line[1] == ' ')
		{
			if(uvsExist && normalsExist)
				sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0],&t[0],&n[0], &v[1],&t[1],&n[1], &v[2],&t[2],&n[2]);
			else if(uvsExist && !normalsExist)
				sscanf(line, "f %d/%d %d/%d %d/%d", &v[0],&t[0], &v[1],&t[1], &v[2],&t[2]);
			else if(!uvsExist && normalsExist)
				sscanf(line, "f %d//%d %d//%d %d//%d", &v[0],&n[0], &v[1],&n[1], &v[2],&n[2]);
			else if(!uvsExist && !normalsExist)
				sscanf(line, "f %d %d %d", &v[0], &v[1], &v[2]);

			for(int i=0; i<3; i++)
			{
				v[i]--;
				t[i]--;
				n[i]--;
			}

			for(int i=0; i<3; i++)
			{
				vertexFloatArr.push_back(vertexVec[v[i]].x);
				vertexFloatArr.push_back(vertexVec[v[i]].y);
				vertexFloatArr.push_back(vertexVec[v[i]].z);

				if(uvsExist)
				{
					uvFloatArr.push_back(uvVec[t[i]].x);
					uvFloatArr.push_back(uvVec[t[i]].y);
				}

				if(normalsExist)
				{
					normalFloatArr.push_back(normalVec[n[i]].x);
					normalFloatArr.push_back(normalVec[n[i]].y);
					normalFloatArr.push_back(normalVec[n[i]].z);	
				}
			}
		}

		free(line);
	}
}

void ObjLoader::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	if(_textureID > 0 && uvFloatArr.size() > 1)
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, &uvFloatArr[0]);
	}
	
	if(normalFloatArr.size() > 1)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, &normalFloatArr[0]);
	}

	glVertexPointer(3, GL_FLOAT, 0, &vertexFloatArr[0]);
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
