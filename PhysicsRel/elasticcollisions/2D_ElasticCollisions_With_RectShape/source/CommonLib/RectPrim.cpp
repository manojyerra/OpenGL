#include "RectPrim.h"
#include "stdio.h"
//#include "MyGL.h"

RectPrim::RectPrim(){}	//not using
RectPrim::~RectPrim(){} //not using

void RectPrim::Init(float x, float y, float width, float height, int borderWidth)
{
	_x = x;
	_y = y;
	_w = width;
	_h = height;


	_cx = 0;
	_cy = 0;
	_cw = 480;
	_ch = 320;

	_borderWidth = borderWidth;

	_color = 0xffffffff;
	_red = 255;
	_green = 255;
	_blue = 255;
	_alpha = 255;

	_color2 = 0xffffffff;
	_red2 = 255;
	_green2 = 255;
	_blue2 = 255;
	_alpha2 = 255;

	_fill = false;
	_flip = false;
	_clipEnable = false;
}


void RectPrim::Init(float x, float y, float width, float height, bool fill)
{
	_x = x;
	_y = y;
	_w = width;
	_h = height;

	_cx = 0;
	_cy = 0;
	_cw = 480;
	_ch = 320;

	_color = 0xffffffff;
	_red = 255;
	_green = 255;
	_blue = 255;
	_alpha = 255;

	_color2 = 0xffffffff;
	_red2 = 255;
	_green2 = 255;
	_blue2 = 255;
	_alpha2 = 255;

	_fill = fill;
	_flip = false;
	_clipEnable = false;
}


void RectPrim::Update(){}

void RectPrim::SetAlpha(int alpha)
{
	_alpha = alpha;
	_alpha2 = alpha;
}

int RectPrim::GetAlpha()
{
	return _alpha;
}


void RectPrim::SetX(float x) { _x = x; }
void RectPrim::SetY(float y) { _y = y; }
void RectPrim::SetWidth(float w) { _w = w; }
void RectPrim::SetHeight(float h) { _h = h; }

void RectPrim::SetSize(float w, float h)
{
	_w = w;
	_h = h;
}

void RectPrim::SetPos(float X, float Y)
{
	_x = X;
	_y = Y;
}

void RectPrim::SetBounds(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_w = width;
	_h = height;

}

float RectPrim::GetX(){ return _x; }
float RectPrim::GetY(){ return _y; }
float RectPrim::GetWidth(){ return _w; }
float RectPrim::GetHeight(){ return _h; }


void RectPrim::SetBorderWidth(int borderWidth)
{
	_borderWidth = borderWidth;
}


void RectPrim::SetColor(int red, int green, int blue, int alpha)
{
	if(red<0)	_red	= 0; else if(red	> 255)	_red	= 255; else _red	= red;
	if(green<0) _green	= 0; else if(green	> 255)	_green	= 255; else _green	= green;
	if(blue<0)	_blue	= 0; else if(blue	> 255)	_blue	= 255; else _blue	= blue;
	if(alpha<0) _alpha	= 0; else if(alpha	> 255)	_alpha	= 255; else _alpha	= alpha;

	_color = (unsigned int)( (_red<<24) + (_green<<16) + (_blue<<8) + _alpha );
}


void RectPrim::SetColor(int color)
{
	_color	= color;
	_red	= (_color >> 24) & 255;
	_green	= (_color >> 16) & 255;
	_blue	= (_color >> 8) & 255;
	_alpha	= (_color ) & 255;
}


void RectPrim::SetBottomColor(int red, int green, int blue, int alpha)
{
	if(red<0)	_red2	= 0; else if(red	> 255)	_red2	= 255; else _red2	= red;
	if(green<0) _green2	= 0; else if(green	> 255)	_green2	= 255; else _green2	= green;
	if(blue<0)	_blue2	= 0; else if(blue	> 255)	_blue2	= 255; else _blue2	= blue;
	if(alpha<0) _alpha2	= 0; else if(alpha	> 255)	_alpha2	= 255; else _alpha2	= alpha;
}


void RectPrim::SetClipArea(int x, int y, int w, int h)
{
	_clipEnable = true;

	_cx = x;
	_cy = y;
	_cw = w;
	_ch = h;
}


void RectPrim::SetFlip(bool flip)
{
	_flip = flip;
}


void RectPrim::Draw(float x, float y, float w, float h, int red, int green, int blue, int alpha, bool wireFrame)
{
	if(!wireFrame)
	{
		GLfloat ver[12];

		ver[0] = x;			ver[1] = y;
		ver[2] = x+w;		ver[3] = y;
		ver[4] = x;			ver[5] = y+h;
		ver[6] = x+w;		ver[7] = y;
		ver[8] = x;			ver[9] = y+h;
		ver[10] = x+w;		ver[11] = y+h;

		glColor4ub(red, green, blue, alpha);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, ver);
		glDrawArrays(GL_TRIANGLES,0,6);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	else
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//GLfloat ver[8];

		//ver[0] = x;			ver[1] = y;
		//ver[2] = x+w;		ver[3] = y;
		//ver[4] = x+w;		ver[5] = y+h;
		//ver[6] = x;			ver[7] = y+h;

		//glColor4ub(red, green, blue, alpha);

		//glEnableClientState(GL_VERTEX_ARRAY);
		//glVertexPointer(2, GL_FLOAT, 0, ver);
		//glDrawArrays(GL_LINE_LOOP,0,4);
		//glDisableClientState(GL_VERTEX_ARRAY);

		//if(glGetError() != GL_NO_ERROR)
		//{
		//	printf("Error in drawing ");
		//}

		glColor4ub(red, green, blue, alpha);

		glBegin(GL_LINE_LOOP);
			glVertex2f(x,y);
			glVertex2f(x+w,y);
			glVertex2f(x+w,y+h);
			glVertex2f(x,y+h);
		glEnd();

	}

}


void RectPrim::Draw()
{

}


//void RectPrim::DrawAndFill()
//{
//	GLfloat ver[12];
//
//	ver[0] = _x;			ver[1] = _y;
//	ver[2] = _x+_w;			ver[3] = _y;
//	ver[4] = _x;			ver[5] = _y+_h;
//	ver[6] = _x+_w;			ver[7] = _y;
//	ver[8] = _x;			ver[9] = _y+_h;
//	ver[10] = _x+_w;		ver[11] = _y+_h;
//
//
//	if(_clipEnable)
//	{
//		for(int i=0;i<12;i++)
//		{
//			if(i%2 == 0)
//			{
//				if(ver[i] < _cx)			
//					ver[i] = _cx;
//				else if(ver[i] > _cx+_cw)
//					ver[i] = _cx+_cw;
//			}
//			else
//			{
//				if(ver[i] < _cy)
//					ver[i] = _cy;
//				else if(ver[i] > _cy+_ch)
//					ver[i] = _cy+_ch;
//			}
//		}
//	}
//
//
//	GLfloat color[24];
//
//	float c1[4];
//	float c2[4];
//
//	if(_flip == false)
//	{
//		c1[0]=_red; c1[1]=_green; c1[2]=_blue; c1[3]=_alpha;
//		c2[0]=_red2; c2[1]=_green2; c2[2]=_blue2; c2[3]=_alpha2;
//	}
//	else
//	{
//		c2[0]=_red; c2[1]=_green; c2[2]=_blue; c2[3]=_alpha;
//		c1[0]=_red2; c1[1]=_green2; c1[2]=_blue2; c1[3]=_alpha2;
//	}
//
//	color[0] = c1[0]/255.0f;
//	color[1] = c1[1]/255.0f;
//	color[2] = c1[2]/255.0f;
//	color[3] = c1[3]/255.0f;
//
//	color[4] = c1[0]/255.0f;
//	color[5] = c1[1]/255.0f;
//	color[6] = c1[2]/255.0f;
//	color[7] = c1[3]/255.0f;
//
//	color[8] = c2[0]/255.0f;
//	color[9] = c2[1]/255.0f;
//	color[10] = c2[2]/255.0f;
//	color[11] = c2[3]/255.0f;
//
//	color[12] = c1[0]/255.0f;
//	color[13] = c1[1]/255.0f;
//	color[14] = c1[2]/255.0f;
//	color[15] = c1[3]/255.0f;
//
//	color[16] = c2[0]/255.0f;
//	color[17] = c2[1]/255.0f;
//	color[18] = c2[2]/255.0f;
//	color[19] = c2[3]/255.0f;
//
//	color[20] = c2[0]/255.0f;
//	color[21] = c2[1]/255.0f;
//	color[22] = c2[2]/255.0f;
//	color[23] = c2[3]/255.0f;
//
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
//
//	glColorPointer(4, GL_FLOAT, 0, color);
//	glVertexPointer(2, GL_FLOAT, 0, ver);
//	glDrawArrays(GL_TRIANGLES,0,6);
//
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_VERTEX_ARRAY);
//}


bool RectPrim::Contains(float X, float Y) 
{
		if (_w <0 || _h< 0) {    return false;}

		float x = _x;
		float y = _y;

		if (X < x || Y < y) {    return false;	}

		float w = _w+x;
		float h = _h+y;
		//    overflow || floatersect
		return ((w < x || w > X)  && (h < y || h > Y));	
}

void RectPrim::Terminate()
{

}
