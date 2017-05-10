#include "Util/GLUtil.h"
#include "Math/GLMat.h"
#include "Math/Vector3.h"
#include "Shapes3D/Shape.h"
#include "Util/GL2DState.h"
#include "Shape2D/Rect.h"
#include "Shapes3D/Box.h"
#include "FLModelReaderWriter.h"

#include <windows.h>
#include <gl/gl.h>
#include <vector>
#include <string>
using namespace std;


class FLModel
{
private:
	FLModelReaderWriter* _flmReaderWriter;

	string _folderPath;

	unsigned char* _verticesPointer;
	unsigned char* _uvsPointer;
	unsigned char* _normalsPointer;
	unsigned char* _indicesPointer;
	unsigned int _indicesType;

	unsigned int _numVertex;
	unsigned int _numIndices;

	unsigned int _textureID;

	Box _aabb;

	bool _isTextureEnabled;
	bool _wireFrameLinesEnabled;
	bool _wireFramePointsEnabled;
	bool _boundingBoxEnabled;
	bool _bounding2DRectEnabled;
	bool _lightingEnabled;
	bool _showBoundingShapes;
	bool _showModel;
	bool _drawLocalAxis;
	bool _isMarked;

	vector<float> _borderVec;
	
	GLfloat _ka[4];
	GLfloat _kd[4];
	GLfloat _ks[4];
	GLfloat _shininess;

	GLMat _mat;

	vector<Shape*> _boundingShapes;

	GL2DState state2D;
	

	void DrawBoundingBox();
	void GetBounding2DRect(int* x, int* y, int* w, int* h, bool multWithLocalMat);
	void DrawBounding2DRect();

	string GetOrientationFilePath(string folderPath);

	//void SaveOrientation(string folderPath);
	//void SaveBBoxInfo(string folderPath);

public:
	FLModel(string folderPath);
	FLModel(string folderPath, float xPos, float yPos, float zPos);
	FLModel(string folderPath, CVector3 pos);
	FLModel(string folderPath, CVector3 pos, CVector3 rot);
	FLModel(string folderPath, CVector3 pos, CVector3 rot, string rotOrder);
	FLModel(string folderPath, float* mat);
	~FLModel();

	void Reset(string folderPath, float* mat);

	void Save();
	void Save(string folderPath);

	float* GetVerticesPointer();
	unsigned int GetNumVertices();

	GLMat GetMat();
	
	void SetTextureEnabled(bool enable);
	bool IsTextureEnabled();
	void SetWireFrameLinesEnabled(bool enable);
	bool IsWireFrameLinesEnabled();
	void SetWireFramePointsEnabled(bool enable);
	bool IsWireFramePointsEnabled();
	void SetBoundingBoxEnabled(bool enable);
	bool IsBoundingBoxEnabled();
	void SetBounding2DRectEnabled(bool enable);
	bool IsBounding2DRectEnabled();
	void SetLightingEnabled(bool enable);
	bool IsLightingEnabled();
	void ShowBoundingShapes(bool show);
	bool IsShowingBoundingShapes();
	void ShowModel(bool show);
	bool IsShowingModel();

	void SetMeterial(int lightParam, float r, float g, float b, float a);
	void SetShininess(float val);

	unsigned int GetMeterial(int lightParam);
	float GetShininess();

	void SetPos(float x, float y, float z);
	void SetPos(CVector3 pos);
	CVector3 GetPos();

	void AddTransInWorld(float x, float y, float z);
	void AddRotateInWorld(char axis, float angle);

	void AddTransInLocal(char axis, float move);
	void AddRotateInLocal(char axis, float angle);

	CVector3 GetRotation();
	void SetRotation(CVector3 rot);

	void AddScale(CVector3 scale);
	void AddUniformScale(float scale);

	Shape* AddBestBoudingShapeByVerticesOnRect(Rect* rect);
	Shape* AddBoudingShapeByVerticesOnRect(Rect* rect, int boundingShapeID);

	vector<float> GetVerticesOnRect(float x, float y, float w, float h);
	Shape* AddBestBoudingShapeByVerticesOnRect(float x, float y, float w, float h);
	Shape* AddBoudingShapeByVerticesOnRect(float x, float y, float w, float h, int boundingShapeID);

	void AddBoundingShape(Shape* shape);

	void Draw();

	void CalcBorder();
	void DrawBorder();

	vector<CVector3> GetAxisLine3DPoints(float* mat, int triIndex);
	void DrawLocalAxis();
	void ShowLocalAxis(bool show);
	bool IsShowingLocalAxis();
	bool IsMarked();
	void SetMarked(bool mark);
};
