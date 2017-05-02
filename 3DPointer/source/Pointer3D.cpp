#include "Pointer3D.h"
#include "Input.h"
#include "Util/GLUtil.h"
#include "Input.h"

Pointer3D::Pointer3D()
{
	_heldPointer = false;
	_triIndex = -1;
	_transformationType = TRANS;
}


void Pointer3D::Draw(float* mat)
{
	pos = CVector3(0,0,0);
	rot = CVector3(0,0,0);
	scale = CVector3(0,0,0);

	_triVec.clear();


	CheckTransformationType();

	vector<CVector3> points3D = Get3DPointsFromMat( mat );
	vector<CVector3> points2D = Get2DPointsFrom3DPoints(&points3D);

	DrawPointer(&points2D, 80);

	if(_transformationType == TRANS)
	{
		if(mat && Input::IsMousePressed() && _heldPointer && _triIndex >= 0)
		{
			vector<CVector3> line3D = GetAxisLine3DPoints(mat, _triIndex);

			pos = Get3DPos(&line3D, Input::MX, Input::MY) - _offSetVec;

			Draw3DAxisLine(_triIndex, &line3D);
		}
	}
	else if(_transformationType == ROTATE || _transformationType == SCALE)
	{
		if(mat && Input::IsMousePressed() && _heldPointer && _triIndex >= 0 && Input::GetDragDist() > 2)
		{
			CVector3 xAxis2DVec = (points2D[1] - points2D[0]);
			CVector3 yAxis2DVec = (points2D[2] - points2D[0]);
			CVector3 zAxis2DVec = (points2D[3] - points2D[0]);

			CVector3 dragVec(Input::MX - Input::PrevMX, Input::MY - Input::PrevMY);

			float aOnB = 0;
			
			if(_triIndex == 0)		aOnB = CVector3::projLenAonB( dragVec, xAxis2DVec );
			else if(_triIndex == 1)	aOnB = CVector3::projLenAonB( dragVec, yAxis2DVec );
			else if(_triIndex == 2)	aOnB = CVector3::projLenAonB( dragVec, zAxis2DVec );

			if(_transformationType == ROTATE)
			{
				float rotVal = abs(dragVec.Length()) *  aOnB/abs(aOnB);

				if(_triIndex == 0)			rot = CVector3( rotVal, 0, 0);
				else if(_triIndex == 1)		rot = CVector3( 0, rotVal, 0);
				else if(_triIndex == 2)		rot = CVector3( 0, 0, rotVal);
			}
			else if(_transformationType == SCALE)
			{
				float dragVecLen = abs(dragVec.Length()) * 0.005;

				if(Input::IsKeyPressed((int)'U'))
					dragVecLen = 1.0f;

				if(_triIndex == 0)			scale = CVector3( dragVecLen * aOnB/abs(aOnB), 0, 0);
				else if(_triIndex == 1)		scale = CVector3( 0, dragVecLen * aOnB/abs(aOnB), 0);
				else if(_triIndex == 2)		scale = CVector3( 0, 0, dragVecLen * aOnB/abs(aOnB));
			}
		}
	}

	if(Input::IsMouseClicked())
	{
		_triIndex = GetTri(Input::MX, Input::MY);

		if(_triIndex >= 0)
		{
			_heldPointer = true;

			vector<CVector3> line3D = GetAxisLine3DPoints(mat, _triIndex);

			CVector3 clickPos = Get3DPos(&line3D, Input::MX, Input::MY);

			_offSetVec = CVector3( clickPos.x - mat[12], clickPos.y - mat[13], clickPos.z - mat[14] );
		}
	}
	else if(Input::IsMouseReleased())
	{
		_heldPointer = false;
	}
}


void Pointer3D::CheckTransformationType()
{
	if(Input::IsKeyTyped((int)'R'))			_transformationType = (_transformationType == ROTATE) ? TRANS : ROTATE;
	else if(Input::IsKeyTyped((int)'S'))	_transformationType = (_transformationType == SCALE) ? TRANS : SCALE;
	else if(Input::IsKeyTyped((int)'T'))	_transformationType = TRANS;
}


vector<CVector3> Pointer3D::Get3DPointsFromMat(float* mat)
{
	CVector3 center		( mat[12],	mat[13],	mat[14]);
	CVector3 sideVec	( mat[0],	mat[1],		mat[2] );
	CVector3 upVec		( mat[4],	mat[5],		mat[6] );
	CVector3 forwardVec	( mat[8],	mat[9],		mat[10]);

	sideVec.Normalize();
	upVec.Normalize();
	forwardVec.Normalize();

	vector<CVector3> vec3D;

	vec3D.push_back( CVector3(center.x, center.y, center.z) );
	vec3D.push_back( CVector3(center.x + sideVec.x,		center.y + sideVec.y,		center.z + sideVec.z) );
	vec3D.push_back( CVector3(center.x + upVec.x,		center.y + upVec.y,			center.z + upVec.z) );
	vec3D.push_back( CVector3(center.x + forwardVec.x,	center.y + forwardVec.y,	center.z + forwardVec.z) );

	return vec3D;
}


vector<CVector3> Pointer3D::Get2DPointsFrom3DPoints(vector<CVector3>* vec3D)
{
	float modelMat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMat);

	return GLUtil::Get2DPosOnScreenFrom3DPos(vec3D, modelMat);
}


void Pointer3D::DrawPointer(vector<CVector3>* vec2D, float pointerMaxLenLimit)
{
	CVector3 vec1 = (vec2D->at(1) - vec2D->at(0));
	CVector3 vec2 = (vec2D->at(2) - vec2D->at(0));
	CVector3 vec3 = (vec2D->at(3) - vec2D->at(0));

	float maxLen = vec1.Length();
	if(maxLen < vec2.Length())	maxLen = vec2.Length();
	if(maxLen < vec3.Length())	maxLen = vec3.Length();

	vec1.SetLength( vec1.Length() * pointerMaxLenLimit / maxLen );
	vec2.SetLength( vec2.Length() * pointerMaxLenLimit / maxLen );
	vec3.SetLength( vec3.Length() * pointerMaxLenLimit / maxLen );

	if(vec1.Length() < 7)	vec1.SetLength(0);
	if(vec2.Length() < 7)	vec2.SetLength(0);
	if(vec3.Length() < 7)	vec3.SetLength(0);

	state2D.Begin(0xff0000ff, 1.0f, 1.0f, true, false);

		glBegin(GL_LINES);
			glColor(0xff0000ff);
			glVertex2f(vec2D->at(0).x, vec2D->at(0).y);
			glVertex2f(vec2D->at(0).x + vec1.x, vec2D->at(0).y + vec1.y);

			glColor(0x00ff00ff);
			glVertex2f(vec2D->at(0).x, vec2D->at(0).y);
			glVertex2f(vec2D->at(0).x + vec2.x, vec2D->at(0).y + vec2.y);

			glColor(0x0000ffff);
			glVertex2f(vec2D->at(0).x, vec2D->at(0).y);
			glVertex2f(vec2D->at(0).x + vec3.x, vec2D->at(0).y + vec3.y);
		glEnd();


		glBegin(GL_TRIANGLES);
			DrawPointerTri(vec2D->at(0), CVector3(vec2D->at(0).x + vec1.x, vec2D->at(0).y + vec1.y), 0xff0000ff);
			DrawPointerTri(vec2D->at(0), CVector3(vec2D->at(0).x + vec2.x, vec2D->at(0).y + vec2.y), 0x00ff00ff);
			DrawPointerTri(vec2D->at(0), CVector3(vec2D->at(0).x + vec3.x, vec2D->at(0).y + vec3.y), 0x0000ffff);
		glEnd();

	state2D.End();
}


void Pointer3D::DrawPointerTri(CVector3 p1, CVector3 p2, unsigned int color)
{
	CVector3 vec = p2 - p1;
	vec.SetLength(22);

	CVector3 n = CVector3(-vec.y, vec.x);
	n.SetLength(10);

	glColor(color);

	glVertex2f(p2.x + n.x, p2.y + n.y);
	glVertex2f(p2.x - n.x, p2.y - n.y);
	glVertex2f(p2.x + vec.x, p2.y + vec.y);

	_triVec.push_back(Triangle2D(p2.x + n.x, p2.y + n.y, p2.x - n.x, p2.y - n.y, p2.x + vec.x, p2.y + vec.y));
}


void Pointer3D::Draw3DAxisLine(int axisNum, vector<CVector3>* line3D)
{
	GLfloat lineWidth = GLUtil::GLLineWidth(1.0f);
	GLboolean lighting = GLUtil::GLEnable(GL_LIGHTING, false);

	glBegin(GL_LINES);
		if(axisNum == 0)		glColor(0xff0000ff);
		else if(axisNum == 1)	glColor(0x00ff00ff);
		else if(axisNum == 2)	glColor(0x0000ffff);

		CVector3 axisVec1 (line3D->at(1) - line3D->at(0));
		CVector3 axisVec2 = axisVec1;

		axisVec1.SetLength(500);
		axisVec2.SetLength(-500);

		axisVec1 += line3D->at(0);
		axisVec2 += line3D->at(0);

		glVertex3f(axisVec1.x, axisVec1.y, axisVec1.z);
		glVertex3f(axisVec2.x, axisVec2.y, axisVec2.z);
	glEnd();

	GLUtil::GLEnable(GL_LIGHTING, lighting);
	GLUtil::GLLineWidth(lineWidth);
}


int Pointer3D::GetTri(float x, float y)
{
	if(_triVec.size() == 3)
	{
		for(int i=0; i<3; i++)
		{
			if(_triVec[i].Contains(x, y))
				return i;
		}
	}

	return -1;
}


bool Pointer3D::IsPointerDragged()
{
	return (_heldPointer && Input::IsMouseDragged());
}


int Pointer3D::GetTransformationType()
{
	return _transformationType;
}


vector<CVector3> Pointer3D::GetAxisLine3DPoints(float* mat, int triIndex)
{
	CVector3 center		( mat[12],	mat[13],	mat[14]);
	CVector3 sideVec	( mat[0],	mat[1],		mat[2] );
	CVector3 upVec		( mat[4],	mat[5],		mat[6] );
	CVector3 forwardVec	( mat[8],	mat[9],		mat[10]);

	sideVec.Normalize();
	upVec.Normalize();
	forwardVec.Normalize();

	CVector3 localAxisVec;

	if(triIndex == 0)		localAxisVec = sideVec;
	else if(triIndex == 1)	localAxisVec = upVec;
	else if(triIndex == 2)	localAxisVec = forwardVec;

	vector<CVector3> vec;
	vec.push_back( CVector3(center.x + localAxisVec.x,	center.y + localAxisVec.y,	center.z + localAxisVec.z) );
	vec.push_back( CVector3(center.x - localAxisVec.x,	center.y - localAxisVec.y,	center.z - localAxisVec.z) );

	return vec;
}


CVector3 Pointer3D::Get3DPos(vector<CVector3>* line3D, float mouseX, float mouseY)
{
	vector<CVector3> line2D = Get2DPointsFrom3DPoints( line3D );

	CVector3 a(mouseX-line2D[0].x, mouseY-line2D[0].y);
	CVector3 b(line2D[1] - line2D[0]);

	float aLen = a.Length();
	float bLen = b.Length();

	float aOnBLen = CVector3::projLenAonB(a, b);
	b.SetLength( aOnBLen );

	//Calculate 3D position
	CVector3 vec3D( line3D->at(1) - line3D->at(0) );
	CVector3 vec2D( line2D[1] - line2D[0] );

	vec3D.SetLength ( vec3D.Length() * aOnBLen / vec2D.Length() );

	return (line3D->at(0) + vec3D);
}


Pointer3D::~Pointer3D()
{
}
