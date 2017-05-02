#include "Pointer3D.h"
#include "Input.h"
#include "Util/GLUtil.h"
#include "Input.h"

static void print(const char * pszFormat, ...)
{
	const int MAX_LEN = 10000;
	char szBuf[MAX_LEN];

	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
	va_end(ap);

	WCHAR wszBuf[MAX_LEN] = {0};
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n");

	//WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
	//printf("%s\n", szBuf);
}

Pointer3D::Pointer3D()
{
	_heldPointer = false;
	_triIndex = -1;
}


void Pointer3D::Draw(float* mat)
{
	if(mat) //&& !_heldPointer)
	{
		triVec.clear();

		CVector3 center		( mat[12],	mat[13],	mat[14]);
		CVector3 sideVec	( mat[0],	mat[1],		mat[2] );
		CVector3 upVec		( mat[4],	mat[5],		mat[6] );
		CVector3 forwardVec	( mat[8],	mat[9],		mat[10]);

		sideVec.SetLength(1);
		upVec.SetLength(1);
		forwardVec.SetLength(1);

		vector<CVector3> vec3D;
		vec3D.push_back( CVector3(center.x, center.y, center.z) );
		vec3D.push_back( CVector3(center.x + sideVec.x,		center.y + sideVec.y,		center.z + sideVec.z) );
		vec3D.push_back( CVector3(center.x + upVec.x,		center.y + upVec.y,			center.z + upVec.z) );
		vec3D.push_back( CVector3(center.x + forwardVec.x,	center.y + forwardVec.y,	center.z + forwardVec.z) );

		float modelMat[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelMat);

		vector<CVector3> vec2D = GLUtil::Get2DPosOnScreenFrom3DPos(vec3D, modelMat);

		float maxLimit = 80;
		CVector3 vec1 = (vec2D[1] - vec2D[0]);
		CVector3 vec2 = (vec2D[2] - vec2D[0]);
		CVector3 vec3 = (vec2D[3] - vec2D[0]);

		float maxLen = vec1.Length();
		if(maxLen < vec2.Length())	maxLen = vec2.Length();
		if(maxLen < vec3.Length())	maxLen = vec3.Length();

		vec1.SetLength( vec1.Length() * maxLimit / maxLen );
		vec2.SetLength( vec2.Length() * maxLimit / maxLen );
		vec3.SetLength( vec3.Length() * maxLimit / maxLen );

		if(vec1.Length() < 7)		vec1.SetLength(0);
		if(vec2.Length() < 7)		vec2.SetLength(0);
		if(vec3.Length() < 7)		vec3.SetLength(0);

		state2D.Begin(0xff0000ff, 1.0f, 1.0f, true, false);

		glBegin(GL_LINES);

		glColor(0x00ff00ff);
		glVertex2f(vec2D[0].x, vec2D[0].y);
		glVertex2f(vec2D[0].x + vec1.x, vec2D[0].y + vec1.y);

		glColor(0x0000ffff);
		glVertex2f(vec2D[0].x, vec2D[0].y);
		glVertex2f(vec2D[0].x + vec2.x, vec2D[0].y + vec2.y);

		glColor(0xff0000ff);
		glVertex2f(vec2D[0].x, vec2D[0].y);
		glVertex2f(vec2D[0].x + vec3.x, vec2D[0].y + vec3.y);

		glEnd();


		glBegin(GL_TRIANGLES);

		DrawPointerTri(vec2D[0], CVector3(vec2D[0].x + vec1.x, vec2D[0].y + vec1.y), 0x00ff00ff);
		DrawPointerTri(vec2D[0], CVector3(vec2D[0].x + vec2.x, vec2D[0].y + vec2.y), 0x0000ffff);
		DrawPointerTri(vec2D[0], CVector3(vec2D[0].x + vec3.x, vec2D[0].y + vec3.y), 0xff0000ff);

		glEnd();

		state2D.End();
	}


	if(mat && Input::IsMousePressed() && _heldPointer && _triIndex >= 0)
	{
		_pos = Get3DPos(mat, _triIndex, Input::MX, Input::MY) - offSetVec;

		//3D axis drawing...

		GLfloat lineWidth = GLUtil::GLLineWidth(1.0f);
		GLboolean lighting = GLUtil::GLEnable(GL_LIGHTING, false);

		glBegin(GL_LINES);

			if(_triIndex == 0)		glColor(0x00ff00ff);
			else if(_triIndex == 1)	glColor(0x0000ffff);
			else if(_triIndex == 2)	glColor(0xff0000ff);

			glVertex3f(points3D[0].x, points3D[0].y, points3D[0].z);
			glVertex3f(points3D[1].x, points3D[1].y, points3D[1].z);
		glEnd();

		GLUtil::GLEnable(GL_LIGHTING, lighting);
		GLUtil::GLLineWidth(lineWidth);
	}


	if(Input::IsMouseClicked())
	{
		_triIndex = GetTri(Input::MX, Input::MY);

		if(_triIndex >= 0)
		{
			_heldPointer = true;

			CVector3 clickPos = Get3DPos(mat, _triIndex, Input::MX, Input::MY);
			offSetVec = CVector3( clickPos.x - mat[12], clickPos.y - mat[13], clickPos.z - mat[14] );
		}
	}
	else if(Input::IsMouseReleased())
	{
		_heldPointer = false;
	}
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

	triVec.push_back(Triangle2D(p2.x + n.x, p2.y + n.y, p2.x - n.x, p2.y - n.y, p2.x + vec.x, p2.y + vec.y));
}


int Pointer3D::GetTri(float x, float y)
{
	if(triVec.size() == 3)
	{
		for(int i=0; i<3; i++)
		{
			if(triVec[i].Contains(x, y))
				return i;
		}
	}

	return -1;
}


bool Pointer3D::IsPointerDragged()
{
	return (_heldPointer && Input::IsMouseDragged());
}


CVector3 Pointer3D::Get3DPos(float* mat, int triIndex, float mouseX, float mouseY)
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


	points3D.clear();
	points3D.push_back( CVector3(center.x + localAxisVec.x*550,	center.y + localAxisVec.y*550,	center.z + localAxisVec.z*550) );
	points3D.push_back( CVector3(center.x - localAxisVec.x*550,	center.y - localAxisVec.y*550,	center.z - localAxisVec.z*550) );

	float modelMat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMat);

	vector<CVector3> points2D = GLUtil::Get2DPosOnScreenFrom3DPos(points3D, modelMat);

	CVector3 a(mouseX-points2D[0].x, mouseY-points2D[0].y);
	CVector3 b(points2D[1] - points2D[0]);

	float aLen = a.Length();
	float bLen = b.Length();

	float aOnBLen = CVector3::projLenAonB(a, b);
	b.SetLength( aOnBLen );


	//Calculate 3D position
	CVector3 vec3D( points3D[1] - points3D[0] );
	CVector3 vec2D( points2D[1] - points2D[0] );

	vec3D.SetLength ( vec3D.Length() * aOnBLen / vec2D.Length() );

	return (points3D[0] + vec3D);
}


Pointer3D::~Pointer3D()
{
}


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