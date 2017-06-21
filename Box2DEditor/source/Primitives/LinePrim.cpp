#include "LinePrim.h"
#include "../CommonLib/AddData.h"
#include "../CommonLib/UIFont.h"
#include <math.h>
#include "../CommonLib/Input.h"
#include "../Platform/Platform.h"

LinePrim::LinePrim(float x1, float y1, float x2, float y2)
{
	_compoundShapeID = -1;
	ID = LINE;

	_x[0] = x1;
	_y[0] = y1;
	_x[1] = x2;
	_y[1] = y2;

	_r = 0;
	_g = 255;
	_b = 0;
	_a = 255;

	_rw = 16;
	_rh = 16;

	for(int i=0;i<2;i++)
		_rectPrims[i] = new RectPrim(_x[i]-_rw/2, _y[i]-_rh/2, _rw, _rh, 0, 0x00ff00ff);

	_isPrimActive = false;
	_activeRectIndex = -1;
	_isSelected = false;
	_isDynamicBody = true;
	_actionPerformed = false;
}

Prim* LinePrim::Clone()
{
	LinePrim* linePrim = new LinePrim(_x[0],_y[0], _x[1],_y[1]);
	linePrim->_isPrimActive = _isPrimActive;
	linePrim->_activeRectIndex = _activeRectIndex;
	linePrim->_isSelected = _isSelected;
	linePrim->_isDynamicBody = _isDynamicBody;

	return (Prim*)linePrim;
}

void LinePrim::SetCompoundShapeID(int id)
{
	_compoundShapeID = id;
}

int LinePrim::GetCompoundShapeID()
{
	return _compoundShapeID;
}

bool LinePrim::Update()
{
	_actionPerformed = false;

	if(Input::IsRightMouseClicked())
	{
		for(int i=0;i<2;i++)
		{
			if(_rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
			{
				_isDynamicBody = !_isDynamicBody;
				_actionPerformed = true;
			}
		}
	}
	else if(Input::IsMousePressed())
	{
		float dx = (float)(Input::MouseX - Input::PrevMouseX);
		float dy = (float)(Input::MouseY - Input::PrevMouseY);

		if(_isPrimActive == false && _activeRectIndex == -1)
		{
			for(int i=0;i<2;i++)
			{
				if(_rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
				{
					_activeRectIndex = i;
					break;
				}
			}

			if(_activeRectIndex == -1 && Input::IsKeyPressed((int)'M') )
			{
				_isPrimActive = true;
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
		else if(_isPrimActive)
		{
			for(int i=0;i<2;i++)
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

void LinePrim::Move(float x, float y)
{
	for(int i=0;i<2;i++)
	{
		_x[i] += x;
		_y[i] += y;
		_rectPrims[i]->SetPos(_x[i]-_rw/2, _y[i]-_rh/2);
	}
	_actionPerformed = true;
}

void LinePrim::AddRotate(float addAngle)
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<2;i++)
	{
		cx += _x[i];
		cy += _y[i];
	}

	cx /= 3.0f;
	cy /= 3.0f;

	for(int i=0;i<2;i++)
	{
		Point p = Point::RotatePointZ( addAngle, &Point(_x[i],_y[i]), &Point(cx, cy));

		_x[i] = p.x;
		_y[i] = p.y;

		_rectPrims[i]->SetPos(_x[i]-_rw/2, _y[i]-_rh/2);
	}
	_actionPerformed = true;
}

void LinePrim::SetSelect(bool select)
{
	_isSelected = select;
	_isPrimActive = false;
	_activeRectIndex = -1;
}

bool LinePrim::IsStaticBody()
{
	return(!_isDynamicBody);
}

bool LinePrim::IsDynamicBody()
{
	return(_isDynamicBody);
}

bool LinePrim::Contains(float x, float y)
{
	for(int i=0;i<2;i++)
		if( _rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
			return true;

	return false;
}


void LinePrim::SetColor(int r, int g, int b, int a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}


void LinePrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	if(_isDynamicBody)
		addData->glColor4f( ((float)_r)/255.0f, ((float)_g)/255.0f,((float)_b)/255.0f,((float)_a)/(2.0f*255.0f));
	else
		addData->glColor4f(1,0,0,0.5);

	addData->glBegin(GL_LINES);
	addData->glVertex3f(_x[0], _y[0], 0);
	addData->glVertex3f(_x[1], _y[1], 0);
	addData->glEnd();

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
	}

	if(_isSelected)
	{
		for(int i=0;i<2;i++)
		{
			_rectPrims[i]->SetPos(_x[i]-_rw/2, _y[i]-_rh/2);
			_rectPrims[i]->Draw();
		}
	}
}

Point LinePrim::GetCenter()
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<2;i++)
	{
		cx += _x[i];
		cy += _y[i];
	}

	cx /= 2.0f;
	cy /= 2.0f;

	return Point(cx, cy);
}

LinePrim::~LinePrim()
{
	for(int i=0;i<2;i++)
	{
		if(_rectPrims[i])
		{
			delete _rectPrims[i];
			_rectPrims[i] = NULL;
		}
	}
}
