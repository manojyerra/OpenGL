#include "Floor.h"
#include "Macros.h"
#include "GLUtil.h"
#include "ShadersManager.h"

Floor::Floor()
{
	_visible = true;
	_axisVisible = true;
	_gridVisible = false;
	_gridLinesVisible = true;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/ColorArray/ColorArray.vs", "shaders/ColorArray/ColorArray.fs");

	float start = -16;
	float end = 16;
	float gap = 1;

	_axisBuffer = new GLBuffer(100, true, false, false);
	_gridBuffer = new GLBuffer(100, true, false, false);;
	_gridLinesBuffer = new GLBuffer(100, true, false, false);;

	_axisBuffer->glBegin(GL_LINES);

		_axisBuffer->glColor4ub(255,0,0,255);
		_axisBuffer->glVertex3f(start,	0.0f,	0.0f);
		_axisBuffer->glVertex3f(end,	0.0f,	0.0f);

		_axisBuffer->glColor4ub(0,0,255,255);
		_axisBuffer->glVertex3f(0.0f,	0.0f,	start);
		_axisBuffer->glVertex3f(0.0f,	0.0f,	end);

		_axisBuffer->glColor4ub(0,255,0,255);
		_axisBuffer->glVertex3f(0.0f,	0.0f,	(start+end)/2.0f);
		_axisBuffer->glVertex3f(0.0f,	end,	0);

	_axisBuffer->glEnd();


	_gridLinesBuffer->glBegin(GL_LINES);
	_gridLinesBuffer->glColor4ub(255,0,0,60);

	for(int i=(int)start; i<=end; i+=(int)gap)
	{
		if(i == 0)
			continue;

		_gridLinesBuffer->glVertex3f(start,	0.01f,	(float)i);
		_gridLinesBuffer->glVertex3f(end,		0.01f,	(float)i);
	}

	for(int i=(int)start; i<=end; i+=(int)gap)
	{
		if(i == 0)
			continue;

		_gridLinesBuffer->glVertex3f((float)i,	0.01f,	start);
		_gridLinesBuffer->glVertex3f((float)i,	0.01f,	end);
	}
	_gridLinesBuffer->glEnd();


	_gridBuffer->glBegin(GL_QUADS);

	int c1 = 255;
	int c2 = 158;
	for(int outer = (int)start; outer < end; outer++)
	{
		int temp = c1;
		c1 = c2;
		c2 = temp;
		for(int i=(int)start; i<end; i+=(int)gap)
		{
			if(i%2 == 0)
				_gridBuffer->glColor4ub(c1,c2,158,255);
			else
				_gridBuffer->glColor4ub(c2,c1,158,255);

			_gridBuffer->glVertex3f((float)i,		0, outer*gap);
			_gridBuffer->glVertex3f((float)i+gap,	0, outer*gap);
			_gridBuffer->glVertex3f((float)i+gap,	0, (outer+1)*gap);
			_gridBuffer->glVertex3f((float)i,		0, (outer+1)*gap);
		}
	}
	_gridBuffer->glEnd();
}

bool Floor::IsVisible()
{
	return _visible;
}

bool Floor::IsAxisVisible()
{
	return _axisVisible;
}

bool Floor::IsGridVisible()
{
	return _gridVisible;
}

bool Floor::IsGridLinesVisible()
{
	return _gridLinesVisible;
}

void Floor::SetVisible(bool visible)
{
	_visible = visible;
}

void Floor::SetAxisVisible(bool axisVisible)
{
	_axisVisible = axisVisible;
}

void Floor::SetGridVisible(bool gridVisible)
{
	_gridVisible = gridVisible;
}

void Floor::SetGridLinesVisible(bool gridLinesVisible)
{
	_gridLinesVisible = gridLinesVisible;
}

void Floor::Draw()
{
	if(_visible == false)
		return;
	
	glPushMatrix();

	float lineWidth_bk = GLUtil::GLLineWidth(1.0f);
	GLboolean glLighting = GLUtil::GLEnable(GL_LIGHTING, false);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, true);
	GLboolean depthTest = GLUtil::GLEnable(GL_DEPTH_TEST, true);

	_shaderProgram->Begin();

	if(_axisVisible)
	{
		glLineWidth(2.0f);

		GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glEnableVertexAttribArray(colorID);
		glBindBuffer(GL_ARRAY_BUFFER, _axisBuffer->GetColorBufferID());
		glVertexAttribPointer( colorID, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);

		GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
		glEnableVertexAttribArray(vertexID);
		glBindBuffer(GL_ARRAY_BUFFER, _axisBuffer->GetVertexBufferID());
		glVertexAttribPointer( vertexID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_LINES, 0, _axisBuffer->GetVertexCount());
		//disable vertexattibarray here...
	}

	if(_gridLinesVisible)
	{
		glLineWidth(1.0f);

		GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glEnableVertexAttribArray(colorID);
		glBindBuffer(GL_ARRAY_BUFFER, _gridLinesBuffer->GetColorBufferID());
		glVertexAttribPointer( colorID, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

		GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
		glEnableVertexAttribArray(vertexID);
		glBindBuffer(GL_ARRAY_BUFFER, _gridLinesBuffer->GetVertexBufferID());
		glVertexAttribPointer( vertexID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_LINES, 0, _gridLinesBuffer->GetVertexCount());
		//disable vertexattibarray here...
	}

	if(_gridVisible)
	{
		GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glEnableVertexAttribArray(colorID);
		glBindBuffer(GL_ARRAY_BUFFER, _gridBuffer->GetColorBufferID());
		glVertexAttribPointer(colorID, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

		GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
		glEnableVertexAttribArray(vertexID);
		glBindBuffer(GL_ARRAY_BUFFER, _gridBuffer->GetVertexBufferID());
		glVertexAttribPointer( vertexID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_QUADS, 0, _gridBuffer->GetVertexCount());
		//disable vertexattibarray here...
	}

	_shaderProgram->End();

	GLUtil::GLLineWidth(lineWidth_bk);
	GLUtil::GLEnable(GL_LIGHTING, glLighting);
	GLUtil::GLEnable(GL_BLEND, blend);
	GLUtil::GLEnable(GL_DEPTH_TEST, depthTest);

	glPopMatrix();
}

Floor::~Floor()
{
	ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);

	if (_axisBuffer != NULL)
	{
		delete _axisBuffer;
	}

	if (_gridBuffer != NULL)
	{
		delete _gridBuffer;
	}

	if (_gridLinesBuffer != NULL)
	{
		delete _gridLinesBuffer;
	}
}



