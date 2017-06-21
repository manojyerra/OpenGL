#include "TriPrim.h"
#include "../CommonLib/AddData.h"
#include "../CommonLib/UIFont.h"
#include <math.h>
#include "../CommonLib/Input.h"
#include "../Platform/Platform.h"

TriPrim::TriPrim(float x1, float y1, float x2, float y2, float x3, float y3)
{
	_compoundShapeID = -1;
	ID = TRIANGLE;

	_x[0] = x1;
	_y[0] = y1;
	_x[1] = x2;
	_y[1] = y2;
	_x[2] = x3;
	_y[2] = y3;

	_r = 0;
	_g = 255;
	_b = 0;
	_a = 255;

	_triangle2D = new Triangle2D(_x[0],_y[0], _x[1],_y[1], _x[2],_y[2]);

	_rw = 16;
	_rh = 16;

	for(int i=0;i<3;i++)
		_rectPrims[i] = new RectPrim(_x[i]-_rw/2, _y[i]-_rh/2, _rw, _rh, 0, 0x00ff00ff);

	_isTriActive = false;
	_activeRectIndex = -1;
	_isSelected = false;
	_isDynamicBody = true;
	_actionPerformed = false;
}

Prim* TriPrim::Clone()
{
	TriPrim* triPrim = new TriPrim(_x[0],_y[0], _x[1],_y[1], _x[2],_y[2]);
	triPrim->_isTriActive = _isTriActive;
	triPrim->_activeRectIndex = _activeRectIndex;
	triPrim->_isSelected = _isSelected;
	triPrim->_isDynamicBody = _isDynamicBody;

	return (Prim*)triPrim;
}

void TriPrim::SetCompoundShapeID(int id)
{
	_compoundShapeID = id;
}

int TriPrim::GetCompoundShapeID()
{
	return _compoundShapeID;
}


bool TriPrim::Update()
{
	_actionPerformed = false;

	if(Input::IsRightMouseClicked())
	{
		if(Contains((float)Input::MouseX, (float)Input::MouseY))
		{
			_isDynamicBody = !_isDynamicBody;
			_actionPerformed = true;
		}
	}
	else if(Input::IsMousePressed())
	{
		float dx = (float)(Input::MouseX - Input::PrevMouseX);
		float dy = (float)(Input::MouseY - Input::PrevMouseY);

		if(_isTriActive == false && _activeRectIndex == -1)
		{
			for(int i=0;i<3;i++)
			{
				if(_rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
				{
					_activeRectIndex = i;
					break;
				}
			}

			if(_activeRectIndex == -1 && Contains((float)Input::MouseX, (float)Input::MouseY ) )
			{
				_isTriActive = true;
			}
		}

		if(_activeRectIndex != -1)
		{
			_rectPrims[_activeRectIndex]->SetX(_rectPrims[_activeRectIndex]->GetX()+dx);
			_rectPrims[_activeRectIndex]->SetY(_rectPrims[_activeRectIndex]->GetY()+dy);

			_x[_activeRectIndex] += dx;
			_y[_activeRectIndex] += dy;
			_actionPerformed = true;
		}
		else if(_isTriActive)
		{
			for(int i=0;i<3;i++)
			{
				_x[i] += dx;
				_y[i] += dy;
				_rectPrims[i]->SetPos(_x[i]-_rw/2, _y[i]-_rh/2);
			}
			_actionPerformed = true;
		}
	}
	else if(Input::IsKeyPressed(KEYS_CONTROL))
	{
			if(Input::IsKeyTyped(KEYS_RIGHT))				AddRotate(1);
		else if(Input::IsKeyTyped(KEYS_LEFT))				AddRotate(-1);
		else if(Input::IsKeyPressedStill(KEYS_RIGHT, 0.3f))		AddRotate(2);
		else if(Input::IsKeyPressedStill(KEYS_LEFT, 0.3f))		AddRotate(-2);
	}
	else if(Input::IsKeyTyped(KEYS_RIGHT))				Move(1, 0);
	else if(Input::IsKeyTyped(KEYS_LEFT))				Move(-1, 0);
	else if(Input::IsKeyTyped(KEYS_UP))					Move(0, -1);
	else if(Input::IsKeyTyped(KEYS_DOWN))				Move(0, 1);
	else if(Input::IsKeyPressedStill(KEYS_RIGHT, 0.3f))		Move(2, 0);
	else if(Input::IsKeyPressedStill(KEYS_LEFT, 0.3f))		Move(-2, 0);
	else if(Input::IsKeyPressedStill(KEYS_UP, 0.3f))			Move(0, -2);
	else if(Input::IsKeyPressedStill(KEYS_DOWN, 0.3f))		Move(0, 2);

	return _actionPerformed;
}

void TriPrim::Move(float x, float y)
{
	for(int i=0;i<3;i++)
	{
		_x[i] += x;
		_y[i] += y;
		_rectPrims[i]->SetPos(_x[i]-_rw/2, _y[i]-_rh/2);
	}
	_actionPerformed = true;
}

void TriPrim::AddRotate(float addAngle)
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<3;i++)
	{
		cx += _x[i];
		cy += _y[i];
	}

	cx /= 3.0f;
	cy /= 3.0f;

	for(int i=0;i<3;i++)
	{
		Point p = Point::RotatePointZ( addAngle, &Point(_x[i],_y[i]), &Point(cx, cy));

		_x[i] = p.x;
		_y[i] = p.y;

		_rectPrims[i]->SetPos(_x[i]-_rw/2, _y[i]-_rh/2);
	}
	_actionPerformed = true;
}

void TriPrim::SetSelect(bool select)
{
	_isSelected = select;
	_isTriActive = false;
	_activeRectIndex = -1;
}

bool TriPrim::IsStaticBody()
{
	return(!_isDynamicBody);
}

bool TriPrim::IsDynamicBody()
{
	return(_isDynamicBody);
}

bool TriPrim::Contains(float x, float y)
{
	_triangle2D->P1.x = _x[0];
	_triangle2D->P1.y = _y[0];
	_triangle2D->P2.x = _x[1];
	_triangle2D->P2.y = _y[1];
	_triangle2D->P3.x = _x[2];
	_triangle2D->P3.y = _y[2];
	if(_triangle2D->Contains(x, y))
		return true;

	for(int i=0;i<3;i++)
		if( _rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
			return true;

	return false;
}


void TriPrim::SetColor(int r, int g, int b, int a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}


void TriPrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	if(_isDynamicBody)
		addData->glColor4f( ((float)_r)/255.0f, ((float)_g)/255.0f,((float)_b)/255.0f,((float)_a)/(2.0f*255.0f));
	else
		addData->glColor4f(1,0,0,0.5);

	addData->glBegin(GL_TRIANGLES);
	addData->glVertex3f(_x[0], _y[0], 0);
	addData->glVertex3f(_x[1], _y[1], 0);
	addData->glVertex3f(_x[2], _y[2], 0);
	addData->glEnd();

	//BORDER
	if(_isSelected)
		addData->glColor4f(0,0,1,1);
	else
		addData->glColor4f( ((float)_r)/255.0f, ((float)_g)/255.0f,((float)_b)/255.0f,((float)_a)/255.0f);

	addData->glBegin(GL_LINE_LOOP);
	addData->glVertex3f(_x[0], _y[0], 0);
	addData->glVertex3f(_x[1], _y[1], 0);
	addData->glVertex3f(_x[2], _y[2], 0);
	addData->glEnd();

	//int gap = 50;
	//int numColorParts = 255/gap;
	if( _compoundShapeID != -1 )
	{
		Point pt = GetCenter();
		float fontW = 15;
		float fontH = 15;

		char str[128];
		sprintf(str,"%d",_compoundShapeID);
		UIFont::GetInstance()->Begin();
		UIFont::GetInstance()->DrawFromCenter(str, pt.x, pt.y, fontW, fontH, -2);
		UIFont::GetInstance()->End();

		//int colorR = _compoundShapeID/(numColorParts*numColorParts);
		//int colorG = _compoundShapeID/numColorParts;
		//int colorB = _compoundShapeID%numColorParts;
		//glPointSize(5.0f);
		//addData->glBegin(GL_POINTS);
		//addData->glColor4ub(colorR*gap, colorG*gap, colorB*gap, 255);
		//Point pt = GetCenter();
		//addData->glVertex3f(pt.x, pt.y, 0);
		//addData->glEnd();
	}

	if(_isSelected)
	{
		for(int i=0;i<3;i++)
		{
			_rectPrims[i]->SetPos(_x[i]-_rw/2, _y[i]-_rh/2);
			_rectPrims[i]->Draw();
		}
	}
}


void TriPrim::Draw(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int color)
{
	float r	= (float)((color >> 24) & 255);
	float g	= (float)((color >> 16) & 255);
	float b	= (float)((color >> 8) & 255);
	float a = (float)((color ) & 255);

	AddData* addData = AddData::GetInstance();

	addData->glColor4f( ((float)r)/255.0f, ((float)g)/255.0f,((float)b)/255.0f,((float)a)/(2.0f*255.0f));
	addData->glBegin(GL_TRIANGLES);
	addData->glVertex3f(x1, y1, 0);
	addData->glVertex3f(x2, y2, 0);
	addData->glVertex3f(x3, y3, 0);
	addData->glEnd();

	//BORDER
	addData->glColor4f( ((float)r)/255.0f, ((float)g)/255.0f,((float)b)/255.0f,((float)a)/255.0f);
	addData->glBegin(GL_LINE_LOOP);
	addData->glVertex3f(x1, y1, 0);
	addData->glVertex3f(x2, y2, 0);
	addData->glVertex3f(x3, y3, 0);
	addData->glEnd();
}

Point TriPrim::GetCenter()
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<3;i++)
	{
		cx += _x[i];
		cy += _y[i];
	}

	cx /= 3.0f;
	cy /= 3.0f;

	return Point(cx, cy);
}

TriPrim::~TriPrim()
{
	if(_triangle2D)
	{
		delete _triangle2D;
		_triangle2D = NULL;
	}

	for(int i=0;i<3;i++)
	{
		if(_rectPrims[i])
		{
			delete _rectPrims[i];
			_rectPrims[i] = NULL;
		}
	}
}
