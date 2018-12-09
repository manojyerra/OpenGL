#include "EditCirclePrim.h"
#include "AddData.h"
#include "UIFont.h"
#include "Input.h"
#include <math.h>

EditCirclePrim::EditCirclePrim(float cx, float cy, float r) : EditPrim(CIRCLE), CirclePrim( cx, cy, r)
{
	_compoundShapeID = -1;
	_isPrimActive = false;
	_activeRectIndex = -1;
	_isSelected = false;
	_isDynamicBody = true;

	_rw = 16;
	_rh = 16;
	_rectPrim = new RectPrim(cx+r - _rw/2, cy-_rh/2, _rw, _rh, 0, 0x00ff00ff);
}

bool EditCirclePrim::Contains(float x, float y)
{
	return (CirclePrim::Contains(x, y) || _rectPrim->Contains(x,y));
}

bool EditCirclePrim::CollidesWithRect(float x, float y, float w, float h)
{
	return CirclePrim::CollidesWithRect(x, y, w, h);
}

EditPrim* EditCirclePrim::Clone()
{
	EditCirclePrim* prim = new EditCirclePrim(GetCX(), GetCY(), GetRadius());
	prim->_isPrimActive = _isPrimActive;
	prim->_activeRectIndex = _activeRectIndex;
	prim->_isSelected = _isSelected;
	prim->_isDynamicBody = _isDynamicBody;
	prim->_isInGroup = _isInGroup;
	prim->_compoundShapeID = _compoundShapeID;

	return (EditPrim*)prim;
}

void EditCirclePrim::SetSelect(bool select)
{
	_isSelected = select;
	_isPrimActive = false;
	_activeRectIndex = -1;
}

bool EditCirclePrim::IsSelected()
{
	return _isSelected;
}

void EditCirclePrim::Update()
{
	if(Input::IsMousePressed() || Input::IsMouseClicked())
	{
		float dx = (float)(Input::MouseX - Input::PrevMouseX);
		float dy = (float)(Input::MouseY - Input::PrevMouseY);

		if(_isPrimActive == false && _activeRectIndex == -1)
		{
			if( _rectPrim->Contains((float)Input::MouseX, (float)Input::MouseY))
				_activeRectIndex = 1;

			if(_activeRectIndex == -1 && CirclePrim::Contains((float)Input::MouseX, (float)Input::MouseY ) )
				_isPrimActive = true;
		}

		if(_activeRectIndex != -1)
		{
			SetRadius(GetRadius() + dx);
			_rectPrim->x = _rectPrim->x + dx;
		}
		else if(_isPrimActive)
		{
			float r = GetRadius();

			SetCenter( GetCX() + dx, GetCY() + dy);
			_rectPrim->SetPos(GetCX() + r - _rw/2, GetCY() - _rh/2);
		}
	}
	else if(Input::IsMouseReleased())
	{
		_isPrimActive = false;
		_activeRectIndex = -1;
	}
}

void EditCirclePrim::Move(float dx, float dy)
{
	SetCenter(GetCX() + dx, GetCY() + dy);
	_rectPrim->SetPos(GetCX()+GetRadius()-_rw/2, GetCY() -_rh/2);
}

void EditCirclePrim::AddRotation(float angleInDegrees, float pivotX, float pivotY)
{
	Point p(GetCX(), GetCY());
	Point pivot(pivotX, pivotY);

	Point newCenter = Point::RotatePointZ(angleInDegrees, &p, &pivot);
	SetCenter(newCenter.x, newCenter.y);
	
	_rectPrim->SetPos(GetCX()+GetRadius()-_rw/2, GetCY() -_rh/2);
}

void EditCirclePrim::Draw()
{
	if (_isDynamicBody)
	{
		SetColor(0, 255, 0, 128);
		SetBorderColor(0, 255, 0, 255);
	}
	else
	{
		SetColor(255, 0, 0, 128);
		SetBorderColor(255, 0, 0, 255);
	}

	if(_angularVel > 0 && _angularVelLoop)
	{
		SetColor(255, 0, 255, 255);
	}
	else if (IsInGroup())
	{
		SetColor(0, 0, 255, 255);
	}

	CirclePrim::Draw();

	if( _compoundShapeID != -1 )
	{
		float fontH = 15;

		char str[128];
		sprintf(str,"%d",_compoundShapeID);
		UIFont::GetInstance()->Begin(1);
		UIFont::GetInstance()->DrawFromCenter(str, GetCX(), GetCY(), fontH, 0);
		UIFont::GetInstance()->End();
	}

	if(_isSelected)
		_rectPrim->Draw();
}

EditCirclePrim::~EditCirclePrim()
{
	if(_rectPrim)
	{
		delete _rectPrim;
		_rectPrim = NULL;
	}
}
