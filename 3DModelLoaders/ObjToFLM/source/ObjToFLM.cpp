#include "ObjToFLM.h"
#include "CFileReader.h"
#include "UtilFuncs.h"
#include "DefinesAndIncludes.h"

ObjToFLM::ObjToFLM(string folderPath)
{
	normalArrType = SHORT_ARR;

	CFileReader fileReader(folderPath+"/objFile.obj", "rb");
	
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

			UtilFuncs::Normalize( &nx, &ny, &nz );

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

	for(int i=0; i<vertexVec.size(); i++)
	{
		faceVec.push_back(Face());
	}

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
				Point V(	vertexVec[v[i]].x,	vertexVec[v[i]].y,	vertexVec[v[i]].z	);
				Point T(	uvVec[t[i]].x,		uvVec[t[i]].y,		uvVec[t[i]].z		);
				Point N(	normalVec[n[i]].x,	normalVec[n[i]].y,	normalVec[n[i]].z	);

				Face face(V,T,N);

				if(v[i] >= faceVec.size())
					writeConsole("error");

				int containIndex = ContainsFace(&face);

				if(containIndex != -1)
				{
					indVec.push_back(containIndex);
				}
				else if(faceVec[v[i]].filled)
				{
					if(faceVec[v[i]].IsSame(&face) == false)
					{
						faceVec.push_back(face);
						indVec.push_back(faceVec.size()-1);
					}
					else
					{
						indVec.push_back(v[i]);
					}
				}
				else
				{
					faceVec[v[i]] = face;
					indVec.push_back(v[i]);
				}
			}
		}

		free(line);
	}

	for(int i=0; i<faceVec.size(); i++)
	{
		vertexIndArr.push_back(faceVec[i].v.x);
		vertexIndArr.push_back(faceVec[i].v.y);
		vertexIndArr.push_back(faceVec[i].v.z);

		uvIndArr.push_back(faceVec[i].t.x);
		uvIndArr.push_back(faceVec[i].t.y);

		if(normalArrType == SHORT_ARR)
		{
			normalIndShortArr.push_back(UtilFuncs::ConvFloatToShort(faceVec[i].n.x));
			normalIndShortArr.push_back(UtilFuncs::ConvFloatToShort(faceVec[i].n.y));
			normalIndShortArr.push_back(UtilFuncs::ConvFloatToShort(faceVec[i].n.z));
		}
		else
		{
			normalIndFloatArr.push_back(faceVec[i].n.x);
			normalIndFloatArr.push_back(faceVec[i].n.y);
			normalIndFloatArr.push_back(faceVec[i].n.z);
		}
	}

	if(vertexIndArr.size() > 0)
	{
		string bufFilePath = folderPath+"/vertex.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&vertexIndArr[0], 4, vertexIndArr.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if(uvIndArr.size() > 0)
	{
		string bufFilePath = folderPath+"/uv.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&uvIndArr[0], 4, uvIndArr.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if(normalArrType == FLOAT_ARR)
	{
		if(normalIndFloatArr.size() > 0)
		{
			string bufFilePath = folderPath+"/normal.buf";
			FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
			fwrite(&normalIndFloatArr[0], 4, normalIndFloatArr.size(), bufFile);
			fflush(bufFile);
			fclose(bufFile);
		}
	}
	else if(normalArrType == SHORT_ARR)
	{
		if(normalIndShortArr.size() > 0)
		{
			string bufFilePath = folderPath+"/normal.buf";
			FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
			fwrite(&normalIndShortArr[0], 2, normalIndShortArr.size(), bufFile);
			fflush(bufFile);
			fclose(bufFile);
		}
	}

	if(indVec.size() > 0)
	{
		string bufFilePath = folderPath+"/index.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");

		int type = GetIndicesType();
		
		if(type == 4)
		{
			fwrite(&indVec[0], 4, indVec.size(), bufFile);
		}
		else if(type == 2)
		{
			vector<unsigned short> shortIndVec;

			for(int i=0; i<indVec.size(); i++)
				shortIndVec.push_back( (unsigned short)indVec[i] );

			fwrite(&shortIndVec[0], 2, shortIndVec.size(), bufFile);
		}
		else if(type == 1)
		{
			vector<unsigned char> byteIndVec;

			for(int i=0; i<indVec.size(); i++)
				byteIndVec.push_back( (unsigned char)indVec[i] );

			fwrite(&byteIndVec[0], 1, byteIndVec.size(), bufFile);
		}

		fwrite(&type, 4, 1, bufFile);

		fflush(bufFile);
		fclose(bufFile);
	}
}


int ObjToFLM::GetIndicesType()
{
	int UNSIGNED_BYTE_MAX_VAL = 255;
	int UNSIGNED_SHORT_MAX_VAL = 65535;
	int UNSIGNED_3BYTES_MAX_VAL = 16777216;

	bool unsigned_short = false;
	bool unsigned_3bytes = false;
	bool unsigned_int = false;

	for(int i=0; i<indVec.size(); i++)
	{
		if(indVec[i] > UNSIGNED_3BYTES_MAX_VAL)
		{
			unsigned_int = true;
		}
		else if(indVec[i] > UNSIGNED_SHORT_MAX_VAL)
		{
			unsigned_3bytes = true;
		}
		else if(indVec[i] > UNSIGNED_BYTE_MAX_VAL)
		{
			unsigned_short = true;
		}
	}

	if(unsigned_int || unsigned_3bytes)
		return 4;

	if(unsigned_short)
		return 2;

	return 1;
}


int ObjToFLM::ContainsFace(Face* face)
{
	int size = faceVec.size();

	for(int i=0; i<size; i++)
	{
		if(faceVec[i].v.x == face->v.x)
		{
			if(faceVec[i].filled)
			{
				if(faceVec[i].IsSame(face))
				{
					return i;
				}
			}
		}
	}

	return -1;
}
