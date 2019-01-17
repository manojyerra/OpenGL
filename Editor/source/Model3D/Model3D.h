#ifndef SUI_OBJ_LOADER_H
#define SUI_OBJ_LOADER_H

#include "Util/ImageBuffer.h"
#include "MathLib/Point.h"
#include "MathLib/MatrixLib/MatrixUtil.h"
#include "MathLib/MatrixLib/GLMat.h"
#include "Shapes/Cylinder.h"
#include "Shapes/Sphere.h"
#include "Shapes/Box.h"
#include "Shapes/Cone.h"
#include "IO/FLM/FLMStruct.h"
#include "IO/FLM/FLMReader.h"
#include "Model3D_Helper.h"
#include "MathLib/Vector3.h"
#include "Phy/PhyCompound.h"

#include <string>
#include <vector>
using namespace std;

class Model3D
{
private:
	string _modelPath;
	int _drawMode;

	unsigned int _textureID;
	unsigned int _vertexBufID;
	unsigned int _uvBufID;
	unsigned int _normalBufID;
	unsigned int _indicesBufID;
	int _displayListID;
	bool _drawTexture;
	bool _drawBoundingShapes;
	string _meterialName;
	PhyCompound* _compoundShape;

	FLMStruct* _flm;
	string _texturePath;
	bool _wireFrame;

	Point _pos;
	GLMat _rot;
	float _scaleUnits;

	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;

	Model3D_Helper* _helper;

	vector<float> bVerVec;

	void ReadTexture(string folderName);
	void CreateBufferObects();
	void CreateCallList();
	void Delete_Vertex_UV_Normal_Indices();

public:
	static const int VBO_MODE = 1;
	static const int DISPLAY_LIST_MODE = 2;
	static const int IMMEDIATE_MODE = 3;

	Model3D(string objFilePath, int drawMode);
	~Model3D();

	void SetColor(int r, int g, int b, int a);
	string GetModelPath();
	const FLMStruct* GetFLMStruct();
	Model3D* Clone();
	
	//Transformations...
	void SetPos(float x, float y, float z);
	Point GetPos();
	Point GetScale();
	void SetRotXYZ(float x, float y, float z);
	Point GetRotXYZ();
	GLMat GetOrientation();
	void SetOrientation(float* mat16ElementsArray);
	void TransInWorld(float x, float y, float z);
	void RotInWorld(char axis, float angle);
	void TransInLocal(char axis, float move);
	void RotInLocal(char axis, float angle);
	void UniformScaleBuf(float units);
	float GetScaleUnits();

	//Texture...
	void SetTexture(string texturePath);
	void SetDrawTexture(bool drawTexture);
	bool IsTextureDrawing();
	unsigned int GetGLTextureID();
	void SetGLTextureID(unsigned int id);
	string GetTexturePath();

	//Physics...
	void SetAsPhysicsObj(bool makePhysicsObj);
	//void SetAsStaticObject(bool val);
	bool IsStaticObject();

	//Bounding shapes...
	void SetDrawBoundingShapes(bool drawBoundingShapes);
	void AddBoundingShape(int shapeID, float* mat, float* scale);
	void AddBoundingShape(int shapeID, float* mat, Point scale);
	void RemoveAllBoundingShapes();
	void DeleteBoundingBox(int index);
	void DrawBoundingShapes();
	void DrawBoundingShape(int index, int r, int g, int b, int a);

	//Meterial...
	int GetMeterialID();
	float GetMass();
	void SetMeterialID(int meterialID, string meterialName);
	string GetMeterialName();

	//WireFrame...
	bool IsInWireFrame();
	void EnableWireFrame(bool wireFrame);

	void Write();
	void Write(string path);
	float* GetConnectedVertex(int triIndex, int* size);
	void AddConnectedVIndex(int val, vector<int>*  connectedVIndexArr, int* indicesBuf, int totIndiLen, unsigned char* existArr);

	unsigned int Draw();
	void DrawWithUniqueColors();
};

#endif
