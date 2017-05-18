#include "Floor.h"
#include "Util/GLUtil.h"

Floor::Floor()
{
	_visible = true;
	_axisVisible = true;
	_gridVisible = false;
	_gridLinesVisible = true;
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

	float lineWidth_bk = GLUtil::GLLineWidth(1.0f);
	GLboolean glLighting = GLUtil::GLEnable(GL_LIGHTING, false);
	unsigned int prevColor = GLUtil::GLColor(0xffffffff);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, false);

	float start = -16;
	float end = 16;
	float gap = 1;

	if(_axisVisible)
	{
		glLineWidth(1.0f);
		glBegin(GL_LINES);

			glColor4ub(255,	0,0,255);

			glVertex3f(start,	0.0f,	0.0f);
			glVertex3f(end,		0.0f,	0.0f);

			glColor4ub(0,0,255,255);

			glVertex3f(0.0f,	0.0f,	start);
			glVertex3f(0.0f,	0.0f,	end);

			glColor4ub(0,255,0,255);

			glVertex3f(0.0f,	0.0f,	(start+end)/2.0f);
			glVertex3f(0.0f,	end,	0);

		glEnd();
	}


	if(_gridLinesVisible)
	{
		glLineWidth(1.0f);

		glBegin(GL_LINES);
		glColor4ub(255,255,255,60);

		for(int i=(int)start; i<=end; i+=(int)gap)
		{
			if(i == 0)
				continue;

			glVertex3f(start,	0.01f,	(float)i);
			glVertex3f(end,		0.01f,	(float)i);
		}

		for(int i=(int)start; i<=end; i+=(int)gap)
		{
			if(i == 0)
				continue;

			glVertex3f((float)i,	0.01f,	start);
			glVertex3f((float)i,	0.01f,	end);
		}
		glEnd();
	}



	if(_gridVisible)
	{
		glBegin(GL_QUADS);

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
					glColor4ub(c1,c2,158,255);
				else
					glColor4ub(c2,c1,158,255);

				glVertex3f((float)i,		0, outer*gap);
				glVertex3f((float)i+gap,	0, outer*gap);
				glVertex3f((float)i+gap,	0, (outer+1)*gap);
				glVertex3f((float)i,		0, (outer+1)*gap);
			}
		}
		glEnd();

		/*
		glColor4ub(255,255,255,255);
		glBegin(GL_TRIANGLES);
			glVertex3f(start,0,start);
			glVertex3f(start,0,end);
			glVertex3f(end,0,end);
			glVertex3f(start,0,start);
			glVertex3f(end,0,end);
			glVertex3f(end,0,start);
		glEnd();
		*/
	}

	GLUtil::GLColor(prevColor);
	GLUtil::GLLineWidth(lineWidth_bk);
	GLUtil::GLEnable(GL_LIGHTING, glLighting);
	GLUtil::GLEnable(GL_BLEND, blend);
}
