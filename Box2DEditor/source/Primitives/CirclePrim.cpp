#include "CirclePrim.h"
#include "../CommonLib/AddData.h"
#include "../CommonLib/UIFont.h"
#include "../CommonLib/Input.h"
#include <math.h>

CirclePrim::CirclePrim(float cx, float cy, float r)
{
	_compoundShapeID = -1;
	ID = CIRCLE;

	_cx = cx;
	_cy = cy;
	_radius = r;

	_red = 0;
	_green = 255;
	_blue = 0;
	_alpha = 255;

	_rw = 16;
	_rh = 16;

	_rectPrim = new RectPrim(_cx+_radius-_rw/2, _cy-_rh/2, _rw, _rh, 0, 0x00ff00ff);

	_isPrimActive = false;
	_activeRectIndex = -1;
	_isSelected = false;
	_isDynamicBody = true;
}

void CirclePrim::SetCompoundShapeID(int id)
{
	_compoundShapeID = id;
}

int CirclePrim::GetCompoundShapeID()
{
	return _compoundShapeID;
}

Prim* CirclePrim::Clone()
{
	CirclePrim* prim = new CirclePrim(_cx, _cy, _radius);
	prim->_isPrimActive = _isPrimActive;
	prim->_activeRectIndex = _activeRectIndex;
	prim->_isSelected = _isSelected;
	prim->_isDynamicBody = _isDynamicBody;
	return (Prim*)prim;
}

void CirclePrim::SetSelect(bool select)
{
	_isSelected = select;
	_isPrimActive = false;
	_activeRectIndex = -1;
}

bool CirclePrim::IsStaticBody()
{
	return(!_isDynamicBody);
}

bool CirclePrim::IsDynamicBody()
{
	return(_isDynamicBody);
}

bool CirclePrim::Update()
{
	bool actionPerformed = false;

	if(Input::IsRightMouseClicked())
	{
		if(Contains((float)Input::MouseX, (float)Input::MouseY))
		{
			_isDynamicBody = !_isDynamicBody;
			actionPerformed = true;
		}
	}
	else if(Input::IsMousePressed())
	{
		float dx = (float)(Input::MouseX - Input::PrevMouseX);
		float dy = (float)(Input::MouseY - Input::PrevMouseY);

		if(_isPrimActive == false && _activeRectIndex == -1)
		{
			if( _rectPrim->Contains((float)Input::MouseX, (float)Input::MouseY))
				_activeRectIndex = 1;

			if(_activeRectIndex == -1 && Contains((float)Input::MouseX, (float)Input::MouseY ) )
				_isPrimActive = true;
		}

		if(_activeRectIndex != -1)
		{
			_radius += dx;
			actionPerformed = true;
		}
		else if(_isPrimActive)
		{
			_cx += dx;
			_cy += dy;
			actionPerformed = true;
		}
	}
	else if(Input::IsMouseReleased())
	{
		_isPrimActive = false;
		_activeRectIndex = -1;
	}

	return actionPerformed;
}

void CirclePrim::Move(float dx, float dy)
{
	_cx += dx;
	_cy += dy;
}

bool CirclePrim::Contains( float x, float y )
{
	return ((_cx-x)*(_cx-x)+(_cy-y)*(_cy-y)) < _radius*_radius;
}

void CirclePrim::SetColor(int r, int g, int b, int a)
{
	_red = r;
	_green = g;
	_blue = b;
	_alpha = a;
}

void CirclePrim::SetPos(float cx, float cy)
{
	_cx = cx;
	_cy = cy;
}

void CirclePrim::SetRadius(float r)
{
	_radius = r;
}

void CirclePrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	if(_isDynamicBody)
		addData->glColor4f( ((float)_red)/255.0f, ((float)_green)/255.0f,((float)_blue)/255.0f,((float)_alpha)/(2.0f*255.0f));
	else
		addData->glColor4f(1,0,0,0.5);

	addData->glBegin(GL_TRIANGLE_FAN);
	addData->glVertex3f(_cx, _cy, 0);
	addData->glVertex3f(_cx+_radius, _cy, 0);

	for( int angle=10; angle<360; angle += 10 )
	{
		float radians = angle*(22.0f/7.0f)/180.0f;
		float x = _cx+_radius*cosf(radians);
		float y = _cy+_radius*sinf(radians);
		addData->glVertex3f(x,y,0);
	}
	addData->glVertex3f(_cx+_radius, _cy, 0);
	addData->glEnd();


	//BORDER
	if(_isSelected)
		addData->glColor4f(0,0,1,1);
	else
		addData->glColor4f( ((float)_red)/255.0f, ((float)_green)/255.0f,((float)_blue)/255.0f,((float)_alpha)/255.0f);

	addData->glBegin(GL_LINE_STRIP);
	addData->glVertex3f(_cx+_radius, _cy, 0);

	for( int angle=10; angle<360; angle += 10 )
	{
		float radians = angle*(22.0f/7.0f)/180.0f;
		float x = _cx+_radius*cosf(radians);
		float y = _cy+_radius*sinf(radians);
		addData->glVertex3f(x,y,0);
	}
	addData->glVertex3f(_cx+_radius, _cy, 0);
	addData->glEnd();


	if( _compoundShapeID != -1 )
	{
		float fontW = 15;
		float fontH = 15;

		char str[128];
		sprintf(str,"%d",_compoundShapeID);
		UIFont::GetInstance()->Begin();
		UIFont::GetInstance()->DrawFromCenter(str, _cx, _cy, fontW, fontH, -2);
		UIFont::GetInstance()->End();
	}

	_rw = _radius * 0.4;
	_rh = _radius * 0.4;

	_rectPrim->SetBounds(_cx+_radius-_rw/2, _cy-_rh/2, _rw, _rh);

	if(_isSelected)
	{
		_rectPrim->Draw();
	}
}

CirclePrim::~CirclePrim()
{
	if(_rectPrim)
	{
		delete _rectPrim;
		_rectPrim = NULL;
	}
}