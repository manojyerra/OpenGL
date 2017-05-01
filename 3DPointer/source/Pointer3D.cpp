#include "Pointer3D.h"
#include "Input.h"
#include "Util/GLUtil.h"

Pointer3D::Pointer3D()
{
}


void Pointer3D::Draw(float* mat)
{
	CVector3 center		( mat[12],	mat[13],	mat[14]);
	CVector3 sideVec	( mat[0],	mat[1],		mat[2] );
	CVector3 upVec		( mat[4],	mat[5],		mat[6] );
	CVector3 forwardVec	( mat[8],	mat[9],		mat[10]);

	sideVec.SetLength(3);
	upVec.SetLength(3);
	forwardVec.SetLength(3);

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


	state2D.Begin(0xff0000ff, 2.0f, 1.0f, true, true);

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


void Pointer3D::DrawPointerTri(CVector3 p1, CVector3 p2, unsigned int color)
{
	CVector3 vec = p2 - p1;
	vec.SetLength(14);

	CVector3 normal = CVector3(-vec.y, vec.x);
	normal.SetLength(7);

	glColor(color);

	glVertex2f(p2.x + normal.x, p2.y + normal.y);
	glVertex2f(p2.x - normal.x, p2.y - normal.y);
	glVertex2f(p2.x + vec.x, p2.y + vec.y);
}


Pointer3D::~Pointer3D()
{
}
