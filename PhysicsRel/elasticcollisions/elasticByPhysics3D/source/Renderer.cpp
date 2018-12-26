#ifdef _WINDOWS
#include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include "Renderer.h"
#include "MathLib/MyVector.h"

int Renderer::makeDiffColor = 0;

int Renderer::sphereR1 = 1;
int Renderer::sphereG1 = 0;
int Renderer::sphereB1 = 0;

int Renderer::sphereR2 = 0;
int Renderer::sphereG2 = 1;
int Renderer::sphereB2 = 0;


Renderer::Renderer(){}
Renderer::~Renderer(){}


void Renderer::DrawLine2D(Line2D* line2D, int r, int g, int b, int a)
{
	Point* p1 = &line2D->P1;
	Point* p2 = &line2D->P2;

	GLfloat vertex[4];

	vertex[0] = p1->x;	vertex[1] = p1->y;
	vertex[2] = p2->x;	vertex[3] = p2->y;

	glColor4ub(r, g, b, a);

	glEnableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();

	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_LINES, 0, 2);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
}


void Renderer::DrawLine3D(Line3D* line3D, int r, int g, int b, int a)
{
	Point* p1 = &line3D->P1;
	Point* p2 = &line3D->P2;

	GLfloat vertex[6];

	vertex[0] = p1->x;	vertex[1] = p1->y;	vertex[2] = p1->z;
	vertex[3] = p2->x;	vertex[4] = p2->y;	vertex[5] = p2->z;

	glColor4ub(r, g, b, a);

	glEnableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();

	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_LINES, 0, 2);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
}

void Renderer::DrawTriangle2D(Triangle2D* triangle2D, int r, int g, int b, int a)
{
	Point* p1 = &triangle2D->P1;
	Point* p2 = &triangle2D->P2;
	Point* p3 = &triangle2D->P3;

	GLfloat vertex[6];

	vertex[0] = p1->x;	vertex[1] = p1->y;
	vertex[2] = p2->x;	vertex[3] = p2->y;
	vertex[4] = p3->x;	vertex[5] = p3->y;

	glColor4ub(r, g, b, a);

	glEnableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();

	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);	
}

void Renderer::DrawTriangle3D(Triangle3D* triangle3D, int r, int g, int b, int a)
{
	Point* p1 = &triangle3D->P1;
	Point* p2 = &triangle3D->P2;
	Point* p3 = &triangle3D->P3;

	GLfloat vertex[9];

	vertex[0] = p1->x;	vertex[1] = p1->y;	vertex[2] = p1->z;
	vertex[3] = p2->x;	vertex[4] = p2->y;	vertex[5] = p2->z;
	vertex[6] = p3->x;	vertex[7] = p3->y;	vertex[8] = p3->z;

	glColor4ub(r, g, b, a);

	glEnableClientState(GL_VERTEX_ARRAY);
	glPushMatrix();

	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
}


void Renderer::DrawCube(float cx, float cy, float cz, float w, float h, float d)
{
	GLfloat vertex[6*6*3];
	GLfloat colors[6*6*4];

	int n = 0;
	int c = 0;

	//front face
	vertex[n++] = cx-w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz+d/2;

	//red
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 0; colors[c++] = 1;


	//back
	vertex[n++] = cx-w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;

	//blue
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;


	//left
	vertex[n++] = cx-w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz+d/2;

	//green
	colors[c++] = 0; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;


	//right
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz+d/2;

	//pink
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 0; colors[c++] = 1; colors[c++] = 1;



	//up
	vertex[n++] = cx-w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy+h/2;	vertex[n++] = cz-d/2;

	colors[c++] = 0; colors[c++] = 1; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 1; colors[c++] = 1;
	colors[c++] = 0; colors[c++] = 1; colors[c++] = 1; colors[c++] = 1;


	//down
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz+d/2;
	vertex[n++] = cx-w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;
	vertex[n++] = cx+w/2;	vertex[n++] = cy-h/2;	vertex[n++] = cz-d/2;

	colors[c++] = 1; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;
	colors[c++] = 1; colors[c++] = 1; colors[c++] = 0; colors[c++] = 1;



	static float angle = 0.0f;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glPushMatrix();
	glRotatef(angle,1,0,0);
	glRotatef(angle,0,1,0);
	glRotatef(angle,0,0,1);

	angle+=0.1;

	glColorPointer(4, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Renderer::DrawWireRect3D(Point* p1, Point* p2, int numPartsOnLine1, Point* p3, Point* p4, int numPartsOnLine2)
{

	MyVector p1p2Vec(p2->x-p1->x, p2->y-p1->y, p2->z-p1->z);
	MyVector p3p4Vec(p4->x-p3->x, p4->y-p3->y, p4->z-p3->z);

	float deltaLenthOnLine_P1_P2 = p1p2Vec.Length/numPartsOnLine1;
	float deltaLenthOnLine_P3_P4 = p3p4Vec.Length/numPartsOnLine1;

	p1p2Vec.Normalize();
	p3p4Vec.Normalize();

	glBegin(GL_LINES);
		
		glVertex3f(p1->x, p1->y, p1->z);
		glVertex3f(p3->x, p3->y, p3->z);

		for(int i=1;i<=numPartsOnLine1;i++)
		{
			p1p2Vec.SetLength( i*deltaLenthOnLine_P1_P2);
			p3p4Vec.SetLength( i*deltaLenthOnLine_P3_P4);

			glVertex3f(p1->x+p1p2Vec.p.x, p1->y+p1p2Vec.p.y, p1->z+p1p2Vec.p.z);
			glVertex3f(p3->x+p3p4Vec.p.x, p3->y+p3p4Vec.p.y, p3->z+p3p4Vec.p.z);
		}

	glEnd();
}

void Renderer::SetSphereColor(int r1, int g1, int b1, int r2, int g2, int b2)
{
	sphereR1 = r1;
	sphereG1 = g1;
	sphereB1 = b1;

	sphereR2 = r2;
	sphereG2 = g2;
	sphereB2 = b2;
}

void Renderer::DrawSphere(int textureID, float cx, float cy, float cz, float radius)
{
	Point center(cx, cy, cz);
	Point point(cx+radius, cy, cz);

	int increAngle = 15;
	int startZ = 90;

	makeDiffColor = 0;
	int color = 0;

	if(textureID > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	float u = 0;
	float v = 0;
	float dv = increAngle*1.0f/180.0f;
	float du = increAngle*1.0f/360.0f;

	for(int z=startZ; z>=startZ-180; z-=increAngle)
	{
		makeDiffColor++;

		int tempZ = z;

		if(tempZ<0)
			tempZ = 360+tempZ;

		Point rotZ = Point::RotatePointZ(tempZ, &point, &center);

		u = 0;

		for(int y=0; y<=360-increAngle; y+=increAngle)
		{
			Point v1 = Point::RotatePointY(y, &rotZ, &center);
			Point v2 = Point::RotatePointY(y+increAngle, &rotZ, &center);

			Point rotZIncre = Point::RotatePointZ((tempZ+increAngle)%360, &point, &center);

			Point v3 = Point::RotatePointY(y, &rotZIncre, &center);
			Point v4 = Point::RotatePointY(y+increAngle, &rotZIncre, &center);

			color++;

			//glPolygonMode(GL_FRONT, GL_LINE);
			//glPolygonMode(GL_BACK, GL_LINE);

			glBegin(GL_QUADS);

			if(textureID > 0)
			{
				glTexCoord2f(u , v);
				glTexCoord2f(u+du, v);
				glTexCoord2f(u+du ,v+dv);
				glTexCoord2f(u , v+dv);
			}
			else
			{
				if(makeDiffColor%2 == 0)
				{
					if(color%2 == 0)	glColor3f(sphereR1,sphereG1,sphereB1);
					else				glColor3f(sphereR2,sphereG2,sphereB2);
				}
				else
				{
					if(color%2 == 0)	glColor3f(sphereR2,sphereG2,sphereB2);
					else				glColor3f(sphereR1,sphereG1,sphereB1);
				}
			}

			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v4.x, v4.y, v4.z);
			glVertex3f(v3.x, v3.y, v3.z);

			glEnd();

			u += du;
		}

		v += dv;
	}
}


