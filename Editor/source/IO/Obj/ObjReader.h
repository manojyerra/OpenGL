#ifndef ObjReader_H
#define ObjReader_H

#include "Util/Prim.h"
#include "MathLib/Point.h"
#include "IO/FLM/FLMStruct.h"

#include <vector>
#include <string>
using namespace std;

class ObjReader
{
private:
	bool _placeAtOrigin;
	Point _min;
	Point _max;
	Point _center;
	int indicesType;
	bool oldWayAdjust;

	void ReadVertexUVNormal(string sourceFile);
	void ReadFaces(string sourceFile, bool uvPresent, bool normalPresent);
	void Adjust();
	void Adjust_oldway();
	void GenerateNewVertexVec();
	void GenerateNewUVVec();
	void GenerateNewNormalVec();
	void FindMinMaxCenter();
	void PlaceAtOriginAndRotate(bool placeAtOrigin, float xRot, float yRot, float zRot);

public:
	static const int VERTEX_BASE_INDEX = 0;
	static const int UV_BASE_INDEX = 1;

	vector<Vertex>* vertexVec;
	vector<UV>* uvVec;
	vector<Normal>* normalVec;
	vector<TriFace>* facesVec;

	ObjReader(string sourceFile, bool placeAtOrigin, float xRot, float yRot, float zRot);
	~ObjReader();

	Point GetMin();
	Point GetMax();
	Point GetCenter();
	int GetIndicesType();

	FLMStruct* CreateFLMStruct();
};

#endif