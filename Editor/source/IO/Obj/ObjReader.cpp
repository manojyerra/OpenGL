#include "IO/Obj/ObjReader.h"
#include "IO/File/FileReader.h"
#include "Util/UtilFuncs.h"
#include "Util/VectorByte.h"
#include "Def.h"

ObjReader::ObjReader(string sourceFile, bool placeAtOrigin, float xRot, float yRot, float zRot)
{
	_placeAtOrigin = placeAtOrigin;

	indicesType = VERTEX_BASE_INDEX;

	vertexVec = new vector<Vertex>();
	uvVec = new vector<UV>();
	normalVec = new vector<Normal>();
	facesVec = new vector<TriFace>();

	ReadVertexUVNormal( sourceFile );
	
	if(vertexVec->size() == 0)
	{
		printf("Invalid State : There is no vertex data present.");
		Exception();
	}

	bool uvPresent = (uvVec->size() > 0) ? true : false;
	bool normalPresent = (normalVec->size() > 0) ? true : false;
	
	ReadFaces(sourceFile, uvPresent, normalPresent);
	
	pr("\nSource OBJ Details : \n");
	pr("Vertex Count   = %d\n",vertexVec->size());
	pr("UV Count       = %d\n",uvVec->size());
	pr("Normal Count   = %d\n",normalVec->size());
	pr("Faces Count    = %d\n\n",facesVec->size());

	//unsigned long startTime = GetTickCount();

	Adjust();

	//pr("\n\nTime Taken : %d\n\n",(int)(GetTickCount()-startTime));

	FindMinMaxCenter();
	PlaceAtOriginAndRotate(_placeAtOrigin, xRot, yRot, zRot);
}


void ObjReader::ReadVertexUVNormal(string sourceFile)
{
	int lineNum = 0;
	FileReader fileReader(sourceFile.c_str());

	string line = "";
	while( (line = fileReader.ReadLine()).compare("null") != 0)
	{
		lineNum++;

		if(StringUtils::startsWith(line, "vt "))
		{
			vector<string> tokens = StringUtils::split( line, ' ' );
					
			float u = atof(tokens[1].c_str());
			float v = atof(tokens[2].c_str());

			uvVec->push_back(UV(u,v));
		}
		else if(StringUtils::startsWith(line, "vn "))
		{
			vector<string> tokens = StringUtils::split( line, ' ' );

			float nx = atof(tokens[1].c_str());
			float ny = atof(tokens[2].c_str());
			float nz = atof(tokens[3].c_str());

			normalVec->push_back(Normal(nx,ny,nz));
		}
		else if(StringUtils::startsWith(line, "v "))
		{
			vector<string> tokens = StringUtils::split( line, ' ' );

			float x = atof(tokens[1].c_str());
			float y = atof(tokens[2].c_str());
			float z = atof(tokens[3].c_str());
							
			vertexVec->push_back(Vertex(x,y,z));
		}
	}

	fileReader.Close();
}

void ObjReader::ReadFaces(string sourceFile, bool uvPresent, bool normalPresent)
{
	FileReader fileReader(sourceFile.c_str());
	int lineNum = 0;

	string line;

	while( (line = fileReader.ReadLine()).compare("null") != 0)
	{
		lineNum++;
		if(StringUtils::startsWith(line, "f "))
		{
			vector<string> tokens = StringUtils::split(line, ' ');
			
			if(tokens.size() > 4)
			{
				printf("Invalid State : No support for non - triangle faces.(Line Num : %d)",lineNum);
				Exception();
			}
		
			if(uvPresent && normalPresent)
			{
				vector<string> subToken1 = StringUtils::split( tokens[1], '/');
				vector<string> subToken2 = StringUtils::split( tokens[2], '/');
				vector<string> subToken3 = StringUtils::split( tokens[3], '/');

				if(subToken1.size() == 1)
				{
					printf("Invalid State : Some of triangle faces has no uv and normal data (Line Num : %d)",lineNum);
					Exception();						
				}
				else if(subToken1.size() == 2)
				{
					printf("Invalid State : Some of triangle faces has no normal data (Line Num : %d)",lineNum);
					Exception();											
				}

				//creating triangle face with vertex, uv and normal index.

				int v1 = atoi(subToken1[0].c_str()) - 1;
				int t1 = atoi(subToken1[1].c_str()) - 1;
				int n1 = atoi(subToken1[2].c_str()) - 1;

				int v2 = atoi(subToken2[0].c_str()) - 1;
				int t2 = atoi(subToken2[1].c_str()) - 1;
				int n2 = atoi(subToken2[2].c_str()) - 1;

				int v3 = atoi(subToken3[0].c_str()) - 1;
				int t3 = atoi(subToken3[1].c_str()) - 1;
				int n3 = atoi(subToken3[2].c_str()) - 1;

				facesVec->push_back(TriFace(v1,t1,n1, v2,t2,n2, v3,t3,n3));
			}
			else if(uvPresent)
			{
				vector<string> subToken1 = StringUtils::split( tokens[1], '/');
				vector<string> subToken2 = StringUtils::split( tokens[2], '/');
				vector<string> subToken3 = StringUtils::split( tokens[3], '/');
				
				if(subToken1.size() != 2)
				{
					printf("Invalid State : Some of triangle faces has no uv data(Line Num : %d)",lineNum);
					Exception();
				}

				//creating triangle face with vertex and uv index.
				
				int v1 = atoi(subToken1[0].c_str()) - 1;
				int t1 = atoi(subToken1[1].c_str()) - 1;
				
				int v2 = atoi(subToken2[0].c_str()) - 1;
				int t2 = atoi(subToken2[1].c_str()) - 1;
				
				int v3 = atoi(subToken3[0].c_str()) - 1;
				int t3 = atoi(subToken3[1].c_str()) - 1;
					
				facesVec->push_back(TriFace(v1,t1, v2,t2, v3,t3));
			}
			else if(normalPresent)
			{
				vector<string> subToken1 = StringUtils::split( tokens[1], '/' );
				vector<string> subToken2 = StringUtils::split( tokens[2], '/' );
				vector<string> subToken3 = StringUtils::split( tokens[3], '/' );

				if(subToken1.size() != 3)
				{
					printf("Invalid State : Some of triangle faces has no normal's data(Line Num : %d) ",lineNum);
					Exception();
				}
				
				//creating triangle face with vertex and normal index.
				
				int v1 = atoi(subToken1[0].c_str()) - 1;
				int n1 = atoi(subToken1[2].c_str()) - 1;
				
				int v2 = atoi(subToken2[0].c_str()) - 1;
				int n2 = atoi(subToken2[2].c_str()) - 1;
				
				int v3 = atoi(subToken3[0].c_str()) - 1;
				int n3 = atoi(subToken3[2].c_str()) - 1;

				facesVec->push_back(TriFace(v1,n1, v2,n2, v3,n3, false));
			}				
			else
			{
				int v1 = atoi(tokens[1].c_str()) - 1;
				int v2 = atoi(tokens[2].c_str()) - 1;
				int v3 = atoi(tokens[3].c_str()) - 1;

				facesVec->push_back(TriFace(v1,v2,v3));
			}
		}
	}

	fileReader.Close();
}

int ObjReader::GetIndicesType()
{
	return indicesType;
}

void ObjReader::Adjust_oldway()
{
	oldWayAdjust = true;

    if(normalVec->size() > 0)
        GenerateNewNormalVec();
	
	if(uvVec->size() > 0)
	{
		if(vertexVec->size() > uvVec->size())
		{
			printf("Vertex count is more than UV count, Do you want to continue(y/n)");
            GenerateNewUVVec();
            indicesType = VERTEX_BASE_INDEX;
		}
		else
		{
			GenerateNewVertexVec();
			indicesType = UV_BASE_INDEX;
		}
	}
}

void ObjReader::Adjust()
{
	oldWayAdjust = false;

	int verSize = vertexVec->size();

	vector<Vertex> vVec;
	vector<UV> uVec;
	vector<Normal> nVec;

	for(int i=0; i<verSize; i++)
	{
		vVec.push_back(Vertex());
		uVec.push_back(UV());
		nVec.push_back(Normal());
	}

	for(int i=0; i<facesVec->size(); i++)
	{
		TriFace tri = facesVec->at(i);

		int index = 0;


		//v1...

		if(vVec[tri.v1].valid == false)
		{
			index = tri.v1;

			vVec[index] = vertexVec->at(tri.v1);
			uVec[index] = uvVec->at(tri.t1);
			nVec[index] = normalVec->at(tri.n1);

			vVec[index].valid = true;
			uVec[index].valid = true;
			nVec[index].valid = true;
		}
		else
		{
			bool add = true;
			for(int j=0;j<i;j++)
			{
				TriFace temp = facesVec->at(j);

				if(temp.v1 == tri.v1 || temp.v2 == tri.v1 || temp.v3 == tri.v1)
				{
					if( (temp.v1 == tri.v1 && temp.t1 == tri.t1)||// && temp.n1 == tri.n1)||
						(temp.v2 == tri.v1 && temp.t2 == tri.t1)||// && temp.n2 == tri.n1)||
						(temp.v3 == tri.v1 && temp.t3 == tri.t1))// && temp.n3 == tri.n1))
					{
						add = false;
						break;
					}
				}
			}

			if(add)
			{
				vVec.push_back(Vertex());
				uVec.push_back(UV());
				nVec.push_back(Normal());

				index = vVec.size()-1;
				facesVec->at(i).v1 = index;

				vVec[index] = vertexVec->at(tri.v1);
				uVec[index] = uvVec->at(tri.t1);
				nVec[index] = normalVec->at(tri.n1);

				vVec[index].valid = true;
				uVec[index].valid = true;
				nVec[index].valid = true;
			}
		}


		//v2...

		if(vVec[tri.v2].valid == false)
		{
			index = tri.v2;

			vVec[index] = vertexVec->at(tri.v2);
			uVec[index] = uvVec->at(tri.t2);
			nVec[index] = normalVec->at(tri.n2);

			vVec[index].valid = true;
			uVec[index].valid = true;
			nVec[index].valid = true;
		}
		else
		{
			bool add = true;
			for(int j=0;j<i;j++)
			{
				TriFace temp = facesVec->at(j);

				if(temp.v1 == tri.v2 || temp.v2 == tri.v2 || temp.v3 == tri.v2)
				{
					if( (temp.v1 == tri.v2 && temp.t1 == tri.t2)|| //&& temp.n1 == tri.n2)||
						(temp.v2 == tri.v2 && temp.t2 == tri.t2)|| //&& temp.n2 == tri.n2)||
						(temp.v3 == tri.v2 && temp.t3 == tri.t2)) //&& temp.n3 == tri.n2))
					{
						add = false;
						break;
					}
				}
			}

			if(add)
			{
				vVec.push_back(Vertex());
				uVec.push_back(UV());
				nVec.push_back(Normal());

				index = vVec.size()-1;
				facesVec->at(i).v2 = index;

				vVec[index] = vertexVec->at(tri.v2);
				uVec[index] = uvVec->at(tri.t2);
				nVec[index] = normalVec->at(tri.n2);

				vVec[index].valid = true;
				uVec[index].valid = true;
				nVec[index].valid = true;
			}
		}

		//v3...

		if(vVec[tri.v3].valid == false)
		{
			index = tri.v3;

			vVec[index] = vertexVec->at(tri.v3);
			uVec[index] = uvVec->at(tri.t3);
			nVec[index] = normalVec->at(tri.n3);

			vVec[index].valid = true;
			uVec[index].valid = true;
			nVec[index].valid = true;
		}
		else
		{
			bool add = true;
			for(int j=0;j<i;j++)
			{
				TriFace temp = facesVec->at(j);

				if(temp.v1 == tri.v3 || temp.v2 == tri.v3 || temp.v3 == tri.v3)
				{
					if( (temp.v1 == tri.v3 && temp.t1 == tri.t3)||// && temp.n1 == tri.n3)||
						(temp.v2 == tri.v3 && temp.t2 == tri.t3)||// && temp.n2 == tri.n3)||
						(temp.v3 == tri.v3 && temp.t3 == tri.t3))// && temp.n3 == tri.n3))
					{
						add = false;
						break;
					}
				}
			}

			if(add)
			{
				vVec.push_back(Vertex());
				uVec.push_back(UV());
				nVec.push_back(Normal());

				index = vVec.size()-1;
				facesVec->at(i).v3 = index;

				vVec[index] = vertexVec->at(tri.v3);
				uVec[index] = uvVec->at(tri.t3);
				nVec[index] = normalVec->at(tri.n3);

				vVec[index].valid = true;
				uVec[index].valid = true;
				nVec[index].valid = true;
			}
		}
	}

	int newVSize = vVec.size();
	int newUSize = uVec.size();
	int newNSize = nVec.size();

	vertexVec->clear();
	uvVec->clear();
	normalVec->clear();

	for(int i=0; i<newVSize; i++)
	{
		vertexVec->push_back(vVec[i]);
		uvVec->push_back(uVec[i]);
		normalVec->push_back(nVec[i]);
	}
}


void ObjReader::GenerateNewVertexVec()
{
	vector<Vertex>* newVertexVec = new vector<Vertex>();
	vector<Normal>* newNormalVec = NULL;
	
	//Generating new vertex vector with uv vector size.
	bool normalsPresent = (normalVec->size()>0) ? true : false;
	
	if(normalsPresent)
		newNormalVec = new vector<Normal>();

	int uvVecSize = uvVec->size();
	for(int i=0;i<uvVecSize;i++)
	{
		newVertexVec->push_back(Vertex(0,0,0));
		if(normalsPresent)
			newNormalVec->push_back(Normal(0,0,0));
	}
	
	int faceCount = facesVec->size();
	
	for(int i=0; i<faceCount; i++)
	{
		TriFace triFace = facesVec->at(i);
		
		(*newVertexVec)[triFace.t1] = vertexVec->at(triFace.v1);
		(*newVertexVec)[triFace.t2] = vertexVec->at(triFace.v2);
		(*newVertexVec)[triFace.t3] = vertexVec->at(triFace.v3);
		
		if(normalsPresent)
		{
			(*newNormalVec)[triFace.t1] = normalVec->at(triFace.v1);
			(*newNormalVec)[triFace.t2] = normalVec->at(triFace.v2);
			(*newNormalVec)[triFace.t3] = normalVec->at(triFace.v3);		
		}
	}
	

	vertexVec->clear();
	delete vertexVec;
	vertexVec = newVertexVec;
	
	if(normalsPresent)
	{
		normalVec->clear();
		delete normalVec;
		normalVec = newNormalVec;
	}
}


void ObjReader::GenerateNewUVVec()
{
	vector<UV>* newUVVec = new vector<UV>();
	
	//Generating new uv vector with vertex vector size.
	int vertexVecSize = vertexVec->size();
    
	for(int i=0;i<vertexVecSize;i++)
		newUVVec->push_back(UV(0,0));

	int faceCount = facesVec->size();
	for(int i=0; i<faceCount; i++)
	{
		TriFace triFace = facesVec->at(i);

		(*newUVVec)[triFace.v1] = uvVec->at(triFace.t1);
		(*newUVVec)[triFace.v2] = uvVec->at(triFace.t2);
		(*newUVVec)[triFace.v3] = uvVec->at(triFace.t3);
	}

	uvVec->clear();
	delete uvVec;

	uvVec = newUVVec;
}


void ObjReader::GenerateNewNormalVec()
{
	vector<Normal>* newNormalVec = new vector<Normal>();
	
	//Generating new uv vector with vertex vector size.
	int vertexVecSize = vertexVec->size();
    
	for(int i=0;i<vertexVecSize;i++)
		newNormalVec->push_back(Normal(0,0,0));

	int faceCount = facesVec->size();
	
	for(int i=0; i<faceCount; i++)
	{
		TriFace triFace = facesVec->at(i);
		
		(*newNormalVec)[triFace.v1] = normalVec->at(triFace.n1);
		(*newNormalVec)[triFace.v2] = normalVec->at(triFace.n2);
		(*newNormalVec)[triFace.v3] = normalVec->at(triFace.n3);
	}

	normalVec->clear();
	delete normalVec;

	normalVec = newNormalVec;
}

void ObjReader::FindMinMaxCenter()
{
	_min.Set(0,0,0);
	_max.Set(0,0,0);
	_center.Set(0,0,0);
	
	for(int i=0;i<vertexVec->size();i++)
	{
		float x = vertexVec->at(i).x;
		float y = vertexVec->at(i).y;
		float z = vertexVec->at(i).z;

		if(i == 0)
		{
			_min.Set(x,y,z);
			_max.Set(x,y,z);
		}
		else
		{
			if( x < _min.x ) _min.x = x;
			if( y < _min.y ) _min.y = y;
			if( z < _min.z ) _min.z = z;

			if( x > _max.x ) _max.x = x;
			if( y > _max.y ) _max.y = y;
			if( z > _max.z ) _max.z = z;
		}				
	}

	_center.x = (_min.x + _max.x)/2.0f;
	_center.y = (_min.y + _max.y)/2.0f;
	_center.z = (_min.z + _max.z)/2.0f;
}

void ObjReader::PlaceAtOriginAndRotate(bool placeAtOrigin, float xRot, float yRot, float zRot)
{
	bool rotate = (xRot != 0 || yRot != 0 || zRot != 0);

	if(_placeAtOrigin || rotate)
	{
		for(int i=0;i<vertexVec->size();i++)
		{
			Vertex* vertex = &(vertexVec->at(i));
			
			if(placeAtOrigin)
				vertex->Move(-_center.x, -_center.y, -_center.z);

			if(rotate)
				vertex->RotateInXYZ(xRot, yRot, zRot);
		}
	}

	FindMinMaxCenter();
}

Point ObjReader::GetMin()
{
	return _min;
}

Point ObjReader::GetMax()
{
	return _max;
}

Point ObjReader::GetCenter()
{
	return _center;
}

FLMStruct* ObjReader::CreateFLMStruct()
{
	FLMStruct* bin = new FLMStruct();

	bin->mat = new float[16];
	memset(bin->mat, 0, 16*sizeof(float));
	bin->mat[0] = bin->mat[5] = bin->mat[10] = bin->mat[15] = 1.0f;

	bin->SetMinMax(GetMin(), GetMax());

	//Reading vertex Data...
	bin->numVertex = vertexVec->size();
	bin->vertexBuf = new float[bin->numVertex*3];

	for(int i=0;i<bin->numVertex;i++)
	{
		bin->vertexBuf[3*i+0] = vertexVec->at(i).x;
		bin->vertexBuf[3*i+1] = vertexVec->at(i).y;
		bin->vertexBuf[3*i+2] = vertexVec->at(i).z;
	}

	//Reading uv Data...
	bin->numUVs = uvVec->size();
	bin->uvBuf = new float[bin->numUVs*2];

	for(int i=0;i<bin->numUVs;i++)
	{
		bin->uvBuf[2*i+0] = uvVec->at(i).u;
		bin->uvBuf[2*i+1] = 1-uvVec->at(i).v;
	}


	//Reading normals Data...
	bin->numNormals = normalVec->size();
	bin->normalBuf = new float[bin->numNormals*3];

	for(int i=0;i<bin->numNormals;i++)
	{
		bin->normalBuf[3*i+0] = normalVec->at(i).nx;
		bin->normalBuf[3*i+1] = normalVec->at(i).ny;
		bin->normalBuf[3*i+2] = normalVec->at(i).nz;
	}


	//Reading indices data...
	int facesCount = facesVec->size();
	bin->numIndices = facesCount*3;
    
	if(bin->numVertex < 256)
	{
		bin->indicesBuf = new byte[bin->numIndices];
		bin->indicesFormat = 5121; //GLbyte;
	}
	else if(bin->numVertex < 65000)
	{
		bin->indicesBuf = new byte[bin->numIndices*2];
		bin->indicesFormat = 5123; //GLshort;
	}
	else
	{
		bin->indicesBuf = new byte[bin->numIndices*4];
		bin->indicesFormat = 5125; //GLint;
	}

	VectorByte vectorByte;

	int f1 = 0;
	int f2 = 0;
	int f3 = 0;

	for(int i=0; i<facesCount; i++)
	{
		if(oldWayAdjust)
		{
			f1 = facesVec->at(i).t1;
			f2 = facesVec->at(i).t2;
			f3 = facesVec->at(i).t3;

			if(GetIndicesType() == ObjReader::VERTEX_BASE_INDEX)
			{
				f1 = facesVec->at(i).v1;
				f2 = facesVec->at(i).v2;
				f3 = facesVec->at(i).v3;
			}
		}
		else
		{
			f1 = facesVec->at(i).v1;
			f2 = facesVec->at(i).v2;
			f3 = facesVec->at(i).v3;
		}

		if(bin->numVertex < 256)
		{
			vectorByte.add((byte)f1);
			vectorByte.add((byte)f2);
			vectorByte.add((byte)f3);
		}
		else if(bin->numVertex < 65000)
		{
            vectorByte.addShort( (short)f1 );
			vectorByte.addShort( (short)f2 );
			vectorByte.addShort( (short)f3 );
        }
		else
		{
			vectorByte.addInt( f1 );
			vectorByte.addInt( f2 );
			vectorByte.addInt( f3 );
		}
	}

	memcpy(bin->indicesBuf, vectorByte.arr, vectorByte.size());


	return bin;
}

ObjReader::~ObjReader()
{
	vertexVec->clear();
	uvVec->clear();
	normalVec->clear();
	facesVec->clear();

	delete vertexVec;
	delete uvVec;
	delete normalVec;
	delete facesVec;
}