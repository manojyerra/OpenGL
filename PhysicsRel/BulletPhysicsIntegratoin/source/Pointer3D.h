#ifndef Pointer3D_H
#define Pointer3D_H

#include "DefinesAndIncludes.h"
#include "Util/GL2DState.h"
#include "Math/Vector3.h"
#include "Math/Triangle2D.h"

#include <vector>
using namespace std;

class Pointer3D
{
private:
	GL2DState state2D;
	bool _heldPointer;
	int _triIndex;
	vector<Triangle2D> _triVec;	
	int _transformationType;
	CVector3 _offSetVec;

	char _invisibleAxis;
	char _topAxis;
	char _sideAxis;
	int _topAxisSign;
	int _sideAxisSign;

	void CheckTransformationType();
	vector<CVector3> Get3DPointsFromMat( float* mat );
	vector<CVector3> Get2DPointsFrom3DPoints(vector<CVector3>* vec3D);
	vector<CVector3> GetAxisLine3DPoints(float* mat, int triIndex);
	CVector3 Get3DPos(vector<CVector3>* line3D, float mouseX, float mouseY);
	int GetTri(float x, float y);

	void DrawPointer(vector<CVector3>* vec2D, float pointerMaxLenLimit);
	void DrawPointerTri(CVector3 p1, CVector3 p2, unsigned int color);
	void Draw3DAxisLine(int axisNum, vector<CVector3>* line3D);

public:
	static const int TRANS = 1;
	static const int ROTATE = 2;
	static const int SCALE = 3;

	CVector3 pos;
	CVector3 rot;
	CVector3 scale;

	Pointer3D();
	~Pointer3D();

	int GetTransformationType();
	bool IsPointerDragged();
	bool IsHeldPointer();
	char GetInvisibleAxis();
	char GetTopAxis();
	char GetSideAxis();
	int GetTopAxisSign();
	int GetSideAxisSign();

	void Draw(float* mat);
};

#endif



			////2D axis drawing...
			//state2D.Begin(0xff0000ff, 1.0f, 1.0f, true, false);
			//glLineWidth(1);
			//glBegin(GL_LINES);
			//	glColor(0x00ff00ff);
			//	glVertex3f(point2D[0].x, point2D[0].y, point2D[0].z);
			//	glVertex3f(point2D[1].x, point2D[1].y, point2D[1].z);
			//glColor(0x00ff00ff);
			//glEnd();

			//glPointSize(5);
			//glBegin(GL_POINTS);
			//	glColor(0x000000ff);
			//	glVertex2f(b.x + point2D[0].x, b.y + point2D[0].y);
			//glEnd();

