#include "EditPolyPrim.h"
#include "AddData.h"
#include "UIFont.h"
#include "Input.h"
#include "Platform.h"
#include "PolyPrim.h"
#include <math.h>

EditPolyPrim::EditPolyPrim(vector<Point> vecPoints) : EditPrim(POLY)
{
	_compoundShapeID = -1;
	vec = vecPoints;

	_color = 0x00ff0088;
	_borderColor = 0x00ff00ff;

	_poly2D = new Poly2D(vec);
	
	_rw = 16;
	_rh = 16;

	for(int i=0;i<(int)vec.size();i++)
	{
		RectPrim* rectPrim = new RectPrim(vec[i].x-_rw/2, vec[i].y-_rh/2, _rw, _rh, 0, 0x00ff00ff);
		_rectPrims.push_back(rectPrim);
	}

	_isPrimActive = false;
	_activeRectIndex = -1;
	_isSelected = false;
	_isDynamicBody = true;
	_isBgOpaque = false;
}

EditPrim* EditPolyPrim::Clone()
{
	EditPolyPrim* prim = new EditPolyPrim(vec);
	prim->_isPrimActive = _isPrimActive;
	prim->_activeRectIndex = _activeRectIndex;
	prim->_isSelected = _isSelected;
	prim->_isDynamicBody = _isDynamicBody;
	prim->_isInGroup = _isInGroup;
	prim->_compoundShapeID = _compoundShapeID;

	return (EditPrim*)prim;
}

void EditPolyPrim::SetBgOpaque(bool isBgOpaque)
{
	_isBgOpaque = isBgOpaque;
}

void EditPolyPrim::Update()
{
	if(Input::IsMousePressed())
	{
		float dx = (float)(Input::MouseX - Input::PrevMouseX);
		float dy = (float)(Input::MouseY - Input::PrevMouseY);

		if(_isPrimActive == false && _activeRectIndex == -1)
		{
			for(int i=0;i<(int)_rectPrims.size();i++)
			{
				if( _rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
				{
					_activeRectIndex = i;
					break;
				}
			}

			if(_activeRectIndex == -1 && Contains((float)Input::MouseX, (float)Input::MouseY))
				_isPrimActive = true;
		}

		if(_activeRectIndex != -1)
		{
			float prevX = _poly2D->pointsVec[_activeRectIndex].x;
			float prevY = _poly2D->pointsVec[_activeRectIndex].y;

			_poly2D->pointsVec[_activeRectIndex].x += dx;
			_poly2D->pointsVec[_activeRectIndex].y += dy;

			if(_poly2D->IsConvexPolygon())
			{
				_rectPrims[_activeRectIndex]->x = _rectPrims[_activeRectIndex]->x + dx;
				_rectPrims[_activeRectIndex]->y = _rectPrims[_activeRectIndex]->y + dy;

				vec[_activeRectIndex].x += dx;
				vec[_activeRectIndex].y += dy;
			}
			else
			{
				_poly2D->pointsVec[_activeRectIndex].x = prevX;
				_poly2D->pointsVec[_activeRectIndex].y = prevY;
			}

		}
		else if(_isPrimActive)
		{
			for(int i=0;i<(int)vec.size();i++)
			{
				vec[i].x += dx;
				vec[i].y += dy;

				_rectPrims[i]->SetPos(vec[i].x-_rw/2, vec[i].y-_rh/2);

				_poly2D->pointsVec[i].x = vec[i].x;
				_poly2D->pointsVec[i].y = vec[i].y;
			}
		}
	}
}

void EditPolyPrim::Move(float x, float y)
{
	for(int i=0;i<(int)vec.size();i++)
	{
		vec[i].x += x;
		vec[i].y += y;

		_rectPrims[i]->SetPos(vec[i].x-_rw/2, vec[i].y-_rh/2);

		_poly2D->pointsVec[i].x = vec[i].x;
		_poly2D->pointsVec[i].y = vec[i].y;
	}
}

void EditPolyPrim::AddRotate(float addAngle)
{
	Point center = GetCenter();
	AddRotation(addAngle, center.x, center.y);
}

void EditPolyPrim::AddRotation(float addAngle, float pivotX, float pivotY)
{
	Point center = Point(pivotX, pivotY);

	float cx = center.x;
	float cy = center.y;

	for (int i = 0; i<vec.size(); i++)
	{
		Point p1 = Point(vec[i].x, vec[i].y);
		Point p2 = Point(cx, cy);

		Point p = Point::RotatePointZ(addAngle, &p1, &p2);

		vec[i].x = p.x;
		vec[i].y = p.y;

		_poly2D->pointsVec[i].x = vec[i].x;
		_poly2D->pointsVec[i].y = vec[i].y;

		_rectPrims[i]->SetPos(vec[i].x - _rw / 2, vec[i].y - _rh / 2);
	}
}

void EditPolyPrim::SetSelect(bool select)
{
	_isSelected = select;
	_isPrimActive = false;
	_activeRectIndex = -1;
}

bool EditPolyPrim::IsSelected()
{
	return _isSelected;
}

bool EditPolyPrim::Contains(float x, float y)
{
	for(int i=0;i<(int)_rectPrims.size();i++)
		if( _rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
			return true;

	for (int i = 0; i<(int)vec.size(); i++)
	{
		_poly2D->pointsVec[i].x = vec[i].x;
		_poly2D->pointsVec[i].y = vec[i].y;
	}

	if(_poly2D->Contains(x, y))
		return true;

	return false;
}

bool EditPolyPrim::CollidesWithRect(float x, float y, float w, float h)
{
	PolyPrim polyPrim(vec);
	return polyPrim.CollidesWithRect(x, y, w, h);
}

void EditPolyPrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	if(_isBgOpaque)
		addData->glColor( IsInGroup() ? 0x0000ffff : ( _isDynamicBody ? 0x00ff00ff : 0xff0000ff) );
	else
		addData->glColor( IsInGroup() ? 0x0000ff88 : ( _isDynamicBody ? 0x00ff0088 : 0xff000088) );

	if(_angularVel > 0 && _angularVelLoop)
	{
		addData->glColor(0xff00ffff );
	}

	addData->glBegin(GL_TRIANGLE_FAN);
	for(int i=0;i<(int)vec.size();i++)
		addData->glVertex3f(vec[i].x, vec[i].y, 0);
	addData->glEnd();

	//BORDER
	addData->glColor( _isDynamicBody ? 0x00ff00ff : 0xff0000ff);
	addData->glBegin(GL_LINE_LOOP);
	for(int i=0;i<(int)vec.size();i++)
		addData->glVertex3f(vec[i].x, vec[i].y, 0);
	addData->glEnd();

	if( _compoundShapeID != -1 )
	{
		Point pt = GetCenter();
	
		float fontH = 15;

		char str[128];
		sprintf(str,"%d",_compoundShapeID);
		UIFont::GetInstance()->Begin(1);
		UIFont::GetInstance()->DrawFromCenter(str, pt.x, pt.y, fontH, 0);
		UIFont::GetInstance()->End();
	}

	if(_isSelected)
	{
		for(int i=0;i<(int)_rectPrims.size();i++)
		{
			_rectPrims[i]->SetPos(vec[i].x-_rw/2, vec[i].y-_rh/2);
			_rectPrims[i]->Draw();
		}
	}
}

Point EditPolyPrim::GetCenter(  )
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<(int)vec.size();i++)
	{
		cx += vec[i].x;
		cy += vec[i].y;
	}

	cx /= (float)vec.size();
	cy /= (float)vec.size();

	return Point(cx, cy, 0);
}

EditPolyPrim::~EditPolyPrim()
{
	if(_poly2D)
	{
		delete _poly2D;
		_poly2D = NULL;
	}

	for(int i=0;i<(int)_rectPrims.size();i++)
	{
		if(_rectPrims[i])
		{
			delete _rectPrims[i];
			_rectPrims[i] = NULL;
		}
	}
}
