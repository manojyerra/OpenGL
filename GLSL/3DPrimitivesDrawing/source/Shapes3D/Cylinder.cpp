#include "Cylinder.h"
#include "math.h"
#include "GLUtil.h"
#include "TransformVertexBuf.h"
#include "ShadersManager.h"

Cylinder::Cylinder() : Shape(Shape::CYLINDER)
{
	_r = 0;
	_h = 0;

	InitCommon();
}


Cylinder::Cylinder(float* mat, float r, float h) : Shape(Shape::CYLINDER)
{
	memcpy(m, mat, 16*4);

	_r = r;
	_h = h;

	InitCommon();
}


Cylinder::Cylinder(Cylinder* cylinder)
{
	memcpy(m, cylinder->m, 16*4);

	_r = cylinder->GetRadius();
	_h = cylinder->GetHeight();
	_id = cylinder->GetID();

	InitCommon();
}


Cylinder::Cylinder(float x, float y, float z, float r, float h) : Shape(Shape::CYLINDER)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;

	_r = r;
	_h = h;

	InitCommon();
}

void Cylinder::InitCommon()
{
	_vertexBufferID = 0;
	_colorBufferID = 0;

	_vertexCount = 0;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/ColorArray/ColorArray.vs", "shaders/ColorArray/ColorArray.fs");
	GenerateBufferID();
}


CVector3 Cylinder::GetPos()
{
	return CVector3(m[12], m[13], m[14]);
}


float Cylinder::GetRadius()			{ return _r; }
float Cylinder::GetHeight()			{ return _h; }

void Cylinder::SetRadius(float r)
{ 
	if(r > 0)
		_r = r; 
}

void Cylinder::SetHeight(float h)
{
	if(h > 0)
		_h = h;
}


float Cylinder::Volume()
{
	return PI_VAL * _r * _r * _h;
}

Cylinder Cylinder::CalcBoundingCylinder(float* vertexBuf, int arrSize)
{
	float* localVertexBuf = new float[arrSize];
	memcpy(localVertexBuf, vertexBuf, arrSize*sizeof(float));

	CVector3 center = TransformVertexBuf::CalcCenter(localVertexBuf, arrSize);
	TransformVertexBuf::Subtract(localVertexBuf, arrSize, center);

	float delta = 30.0f;
	CVector3 start(0,0,0);
	CVector3 end(360,360,360);

	CVector3 rot(0,0,0);
	Cylinder prevCylinder;
	bool once = true;

	for(int loop=0; loop<5; loop++)
	{
		if(loop != 0)
		{
			start = CVector3(rot.x-delta, rot.y-delta, rot.z-delta);
			end	  = CVector3(rot.x+delta, rot.y+delta, rot.z+delta);
		}

        if(loop == 1)		delta = 10;
        else if(loop == 2)	delta = 5;
        else if(loop == 3)	delta = 2;
        else if(loop == 4)	delta = 1;

		for(float zAng=start.z; zAng<end.z; zAng+=delta)
		{
			for(float yAng=start.y; yAng<end.y; yAng+=delta)
			{
				for(float xAng=start.x; xAng<end.x; xAng+=delta)
				{
					Cylinder bCylinder = GetBoundingCylinderAfterRotXYZ(localVertexBuf, arrSize, xAng, yAng, zAng);

					if(once || bCylinder.Volume() < prevCylinder.Volume())
					{
						prevCylinder = bCylinder;
						rot = CVector3(xAng, yAng, zAng);
						once = false;
					}
				}
			}
		}
	}

	delete[] localVertexBuf;

	GLMat mat;
	mat.glTranslatef(center.x, center.y, center.z);
	mat.glRotatef(-rot.x, 1,0,0);
	mat.glRotatef(-rot.y, 0,1,0);
	mat.glRotatef(-rot.z, 0,0,1);

	CVector3 trans = prevCylinder.GetPos();
	mat.glTranslatef(trans.x, trans.y, trans.z);
	
	return Cylinder(mat.m, prevCylinder.GetRadius(), prevCylinder.GetHeight());
}


Cylinder Cylinder::GetBoundingCylinderAfterRotXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng)
{
	float cosOfXAng = cos(xAng * DEG_RAD);
	float cosOfYAng = cos(yAng * DEG_RAD);
	float cosOfZAng = cos(zAng * DEG_RAD);

	float sinOfXAng = sin(xAng * DEG_RAD);
	float sinOfYAng = sin(yAng * DEG_RAD);
	float sinOfZAng = sin(zAng * DEG_RAD);

	float x = vertexBuf[0];
	float y = vertexBuf[1];
	float z = vertexBuf[2];

	float minY = y;
	float maxY = y;

	float rPow2 = x*x + z*z;

	for(int i=0; i<arrSize; i+=3)
	{
		x = vertexBuf[i+0];
		y = vertexBuf[i+1];
		z = vertexBuf[i+2];

		float x1 = x;
		float y1 = y*cosOfXAng - z*sinOfXAng;
		float z1 = y*sinOfXAng + z*cosOfXAng;

		float y2 = y1;
		float z2 = z1*cosOfYAng - x1*sinOfYAng;
		float x2 = z1*sinOfYAng + x1*cosOfYAng;

		x = x2*cosOfZAng - y2*sinOfZAng;
		y = x2*sinOfZAng + y2*cosOfZAng;
		z = z2;

		if(y < minY)	minY = y;
		if(y > maxY)	maxY = y;

		if(rPow2 < x*x + z*z)
			rPow2 = x*x + z*z;
	}

	float xPos = 0;
	float yPos = (minY+maxY)/2;
	float zPos = 0;

	float h = abs(maxY-minY);

	return Cylinder(0,yPos,0, sqrt(rPow2),h);
}


void Cylinder::Draw()
{
	if(!_visible)
		return;

	unsigned int prevColor = GLUtil::GLColor(_color);

	glPushMatrix();
	glMultMatrixf(m);

	glScalef(_r*2, _h, _r*2);

	_shaderProgram->Begin();

	GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
	glEnableVertexAttribArray(colorID);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glVertexAttribPointer( colorID, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexID);
	glDisableVertexAttribArray(colorID);

	_shaderProgram->End();

	glPopMatrix();

	GLUtil::GLColor(prevColor);
}

void Cylinder::GenerateBufferID()
{
	GLBuffer* buffer = new GLBuffer(100, true, false, false);

	float radius = 0.5f;
	float halfLength = 0.5f;
	float piVal = 3.14159265f;

	buffer->glBegin(GL_TRIANGLES);

	if(_useRandomColors)
		_randomColor.Reset();

	for(int i=20; i<=360; i+=20)
	{
		float theta = (i-20)*piVal/180.0f;
		float nextTheta = (i)*piVal/180.0f;

		if(_useRandomColors)
			buffer->glColor( _randomColor.NextColor() );
		
		buffer->glVertex3f(radius*cos(nextTheta),	halfLength,		radius*sin(nextTheta));
		buffer->glVertex3f(radius*cos(theta),	-halfLength,	radius*sin(theta));
		buffer->glVertex3f(radius*cos(theta),	halfLength,		radius*sin(theta));

		buffer->glVertex3f(radius*cos(theta),	-halfLength,	radius*sin(theta));
		buffer->glVertex3f(radius*cos(nextTheta),	halfLength,		radius*sin(nextTheta));
		buffer->glVertex3f(radius*cos(nextTheta),	-halfLength,	radius*sin(nextTheta));

		buffer->glColor( _randomColor.NextColor() );
		buffer->glVertex3f(radius*cos(nextTheta),	halfLength,		radius*sin(nextTheta));

		if(_useRandomColors) buffer->glColor( _randomColor.NextColor() );
		buffer->glVertex3f(radius*cos(theta),	halfLength,		radius*sin(theta));
		buffer->glVertex3f(0, halfLength, 0);		

		if(_useRandomColors) buffer->glColor( _randomColor.NextColor() );
		buffer->glVertex3f(0, -halfLength, 0);
		
		if(_useRandomColors) buffer->glColor( _randomColor.NextColor() );
		buffer->glVertex3f(radius*cos(theta),	-halfLength,		radius*sin(theta));
		buffer->glVertex3f(radius*cos(nextTheta),	-halfLength,		radius*sin(nextTheta));
	}

	buffer->glEnd();

	_vertexBufferID = buffer->GetVertexBufferID();
	_colorBufferID = buffer->GetColorBufferID();

	_vertexCount = buffer->GetVertexCount();

	delete buffer;
}

Cylinder::~Cylinder()
{
	if (_shaderProgram != NULL)
	{
		string vertexShaderPath = _shaderProgram->GetVertexShaderFilePath();
		string fragementShaderPath = _shaderProgram->GetFragmentShaderFilePath();

		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
	}
}
