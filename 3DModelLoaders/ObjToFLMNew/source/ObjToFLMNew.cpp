#include "ObjToFLMNew.h"
#include "CFileReader.h"
#include "UtilFuncs.h"

ObjToFLMNew::ObjToFLMNew(string folderPath)
{
	CFileReader fileReader(folderPath + "/objFile.obj", "rb");

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
			uvVec.push_back(CVector3(tx, ty, 0));
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

	vector<unsigned int> indVec;
	vector<CVector3> uvVecNew;
	vector<CVector3> normalVecNew;

	vector<unsigned int> uvVecIndex;
	vector<unsigned int> normalVecIndex;

	unsigned int vertexVecSize = vertexVec.size();

	if (uvsExist && normalsExist)
	{
		for (int i = 0; i < vertexVecSize; i++)
		{
			uvVecNew.push_back(CVector3());
			normalVecNew.push_back(CVector3());

			uvVecIndex.push_back(0);
			normalVecIndex.push_back(0);
		}

		unsigned int facesStrArrSize = facesArr.size();
		unsigned int* facesStrArrPtr = (unsigned int*)facesArr.getArray();

		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_VTN(line, &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]);

			for(int i=0; i<3; i++)
			{
				int index = v[i] - 1;

				if (!uvVecNew[index].init && !normalVecNew[index].init)
				{
					uvVecNew[index] = uvVec[t[i] - 1];
					normalVecNew[index] = normalVec[n[i] - 1];

					uvVecIndex[index] = t[i];
					normalVecIndex[index] = n[i];

					indVec.push_back(index);
				}
				else if (uvVecIndex[index] == t[i] && normalVecIndex[index] == n[i])
				{
					indVec.push_back(index);
				}
				else
				{

					uvVecNew.push_back(uvVec[t[i] - 1] );
					normalVecNew.push_back(normalVec[n[i] - 1]);
					vertexVec.push_back(vertexVec[v[i] - 1]);

					uvVecIndex.push_back(t[i]);
					normalVecIndex.push_back(n[i]);

					indVec.push_back(vertexVec.size() - 1);
				}
			}

			free(line);
		}
	}
	else if (!uvsExist && normalsExist)
	{
		for (int i = 0; i < vertexVecSize; i++)
		{
			normalVecNew.push_back(CVector3());
			normalVecIndex.push_back(0);
		}

		unsigned int facesStrArrSize = facesArr.size();
		unsigned int* facesStrArrPtr = (unsigned int*)facesArr.getArray();

		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_VN(line, &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]);

			for (int i = 0; i < 3; i++)
			{
				int index = v[i] - 1;

				if (!normalVecNew[index].init)
				{
					normalVecNew[index] = normalVec[n[i] - 1];
					normalVecIndex[index] = n[i];

					indVec.push_back(index);
				}
				else if (normalVecIndex[index] == n[i])
				{
					indVec.push_back(index);
				}
				else
				{
					normalVecNew.push_back(normalVec[n[i] - 1]);
					vertexVec.push_back(vertexVec[v[i] - 1]);

					normalVecIndex.push_back(n[i]);

					indVec.push_back(vertexVec.size() - 1);
				}
			}

			free(line);
		}
	}

	WriteData(folderPath, indVec, vertexVec, normalVecNew, uvVecNew);
}

void ObjToFLMNew::WriteData(string folderPath, vector<unsigned int>& indVec, 
	vector<CVector3>& vertexVec, vector<CVector3>& normalVecNew ,vector<CVector3>& uvVecNew)
{
	if (indVec.size() > 0)
	{
		string bufFilePath = folderPath + "/index.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&indVec[0], 4, indVec.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (vertexVec.size() > 0)
	{
		vector<float> floatVec;

		for (int i = 0; i < vertexVec.size(); i++)
		{
			floatVec.push_back(vertexVec[i].x);
			floatVec.push_back(vertexVec[i].y);
			floatVec.push_back(vertexVec[i].z);
		}

		string bufFilePath = folderPath + "/vertex.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&floatVec[0], 4, floatVec.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (normalVecNew.size() > 0)
	{
		vector<float> floatVec;

		for (int i = 0; i < normalVecNew.size(); i++)
		{
			floatVec.push_back(normalVecNew[i].x);
			floatVec.push_back(normalVecNew[i].y);
			floatVec.push_back(normalVecNew[i].z);
		}

		string bufFilePath = folderPath + "/normal.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&floatVec[0], 4, floatVec.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (uvVecNew.size() > 0)
	{
		vector<float> floatVec;

		for (int i = 0; i < uvVecNew.size(); i++)
		{
			floatVec.push_back(uvVecNew[i].x);
			floatVec.push_back(uvVecNew[i].y);
		}

		string bufFilePath = folderPath + "/uv.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(&floatVec[0], 4, floatVec.size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}
}