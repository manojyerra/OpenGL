#include "Looper.h"
#include "Input.h"
#include "GLAndUtil.h"
#include "math.h"
#include "Floor.h"
#include <windows.h>
#include <gl/gl.h>

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = windowWidth;
	_windowH = windowHeight;

	GLAndUtil::Init(_windowW, _windowH);
}

void Looper::Update(float deltaTime)
{
}

void Looper::Draw()
{
	util::ClearColor(0.6, 0.6, 0.6, 1);
	util::Clear();
	util::Begin3DDraw();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat qaLightPos[] = {0, 0, 0, 1.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPos);

	util::SetModelViewMatrix();
	util::UpdateCamera();

	GLfloat Ka[] = {0.600000, 0.600000, 0.600000, 1.0};
	GLfloat Kd[] = {0.200000, 0.200000, 0.200000, 1.0};
	GLfloat Ks[] = {0.100000, 0.100000, 0.100000, 1.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);
	glMaterialf(GL_FRONT, GL_SHININESS, 2.0);

	Floor::Draw();

	DrawSphere(0, 0, 0, 2);
}

void Looper::DrawSphere(float cx, float cy, float cz, float r)
{
	glBegin(GL_QUADS);
	
	float degToRad = 0.017453f;

	int delta = 5;
	
	float x = r;
	float y = 0;
	float z = 0;

	for(int zAngle=-90; zAngle<90; zAngle+=2*delta)
	{
		float Xz1 = 0;
		float Yz1 = 0;
		float Zz1 = 0;

		float Xz2 = 0;
		float Yz2 = 0;
		float Zz2 = 0;

		rot(3, zAngle,			x, y, z, &Xz1, &Yz1, &Zz1);
		rot(3, zAngle+2*delta,	x, y, z, &Xz2, &Yz2, &Zz2);

		for(int yAngle = 0; yAngle<=360; yAngle+=delta)
		{
			float x1,y1,z1;
			float x2,y2,z2;
			float x3,y3,z3;
			float x4,y4,z4;

			rot(2, yAngle, Xz1, Yz1, Zz1, &x1, &y1, &z1);
			rot(2, yAngle, Xz2, Yz2, Zz2, &x2, &y2, &z2);

			rot(2, yAngle+delta, Xz1, Yz1, Zz1, &x3, &y3, &z3);
			rot(2, yAngle+delta, Xz2, Yz2, Zz2, &x4, &y4, &z4);

			glNormal3f(x1, y1, z1);
			glVertex3f(cx+x1, cy+y1, cz+z1);
			glNormal3f(x2, y2, z2);
			glVertex3f(cx+x2, cy+y2, cz+z2);
			glNormal3f(x4, y4, z4);
			glVertex3f(cx+x4, cy+y4, cz+z4);
			glNormal3f(x3, y3, z3);
			glVertex3f(cx+x3, cy+y3, cz+z3);
		}
	}
	glEnd();
}


// xAxis : 1
// yAxis : 2
// zAxis : 3

void Looper::rot(int axis, float angleInDegrees, float x, float y, float z, float* newX, float* newY, float* newZ)
{
	float angleInRadians = angleInDegrees*(3.14159f) / 180.0f;		//converting to radiuns

	float cosVal = cosf(angleInRadians);
	float sinVal = sinf(angleInRadians);

	if(axis == 3)
	{
		newX[0] = x*cosVal - y*sinVal;
		newY[0] = x*sinVal + y*cosVal;
		newZ[0] = z;
	}
	else if(axis == 1)
	{
		newY[0] = y*cosVal - z*sinVal;
		newZ[0] = y*sinVal + z*cosVal;
		newX[0] = x;
	}
	else if(axis == 2)
	{
		newZ[0] = z*cosVal - x*sinVal;
		newX[0] = z*sinVal + x*cosVal;
		newY[0] = y;
	}
}

Looper::~Looper()
{
}
