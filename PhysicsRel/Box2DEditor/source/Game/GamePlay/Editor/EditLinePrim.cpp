#include "EditLinePrim.h"
#include "AddData.h"
#include "UIFont.h"
#include "Input.h"
#include "Platform.h"
#include "LinePrim.h"
#include "Vector3.h"
#include <math.h>

EditLinePrim::EditLinePrim(float x1, float y1, float x2, float y2) : EditPrim(LINE)
{
	vec.push_back(Point(x1, y1));
	vec.push_back(Point(x2, y2));

	_rw = 16;
	_rh = 16;

	for (int i = 0; i < vec.size(); i++)
		_rectPrims[i] = new RectPrim(vec[i].x - _rw / 2, vec[i].y - _rh / 2, _rw, _rh, 0, 0x00ff00ff);

	_compoundShapeID = -1;	
	_color = 0x00ff00ff;
	_isPrimActive = false;
	_activeRectIndex = -1;
	_isSelected = false;
	_width = 4.0f;
}

EditPrim* EditLinePrim::Clone()
{
	EditLinePrim* prim = new EditLinePrim(vec[0].x, vec[0].y, vec[1].x, vec[1].y);
	prim->_isPrimActive = _isPrimActive;
	prim->_activeRectIndex = _activeRectIndex;
	prim->_isSelected = _isSelected;
	prim->_isDynamicBody = _isDynamicBody;
	prim->_isInGroup = _isInGroup;
	prim->_compoundShapeID = _compoundShapeID;

	return (EditPrim*)prim;
}

void EditLinePrim::Update()
{
	if(Input::IsMousePressed())
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

			LineSeg2D lineSeg(vec[0], vec[1]);

			if(_activeRectIndex == -1 && lineSeg.ContainsWithErrLimit(Input::MouseX, Input::MouseY, 2.5) )
				_isPrimActive = true;
		}

		if(_activeRectIndex != -1)
		{
			_rectPrims[_activeRectIndex]->x = _rectPrims[_activeRectIndex]->x + dx;
			_rectPrims[_activeRectIndex]->y = _rectPrims[_activeRectIndex]->y + dy;

			vec[_activeRectIndex].x += dx;
			vec[_activeRectIndex].y += dy;
		}
		else if(_isPrimActive)
		{
			for(int i=0;i<2;i++)
			{
				vec[i].x += dx;
				vec[i].y += dy;

				_rectPrims[i]->SetPos(vec[i].x-_rw/2, vec[i].y-_rh/2);
			}
		}
	}
}

void EditLinePrim::Move(float xPos, float yPos)
{
	for(int i=0;i<2;i++)
	{
		vec[i].x += xPos;
		vec[i].y += yPos;
		
		_rectPrims[i]->SetPos(vec[i].x-_rw/2, vec[i].y-_rh/2);
	}
}

void EditLinePrim::AddRotate(float addAngle)
{
	Point center = GetCenter();
	AddRotation(addAngle, center.x, center.y);
}

void EditLinePrim::AddRotation(float addAngle, float pivotX, float pivotY)
{
	Point center = Point(pivotX, pivotY);

	float cx = center.x;
	float cy = center.y;

	for(int i=0;i<2;i++)
	{
		Point p1 = vec[i];
		Point p2 = Point(cx, cy);

		Point p = Point::RotatePointZ( addAngle, &p1, &p2 );

		vec[i].x = p.x;
		vec[i].y = p.y;

		_rectPrims[i]->SetPos(vec[i].x - _rw/2, vec[i].y - _rh/2);
	}
}

void EditLinePrim::SetLineWidth(float width)
{
	_width = width;
}

float EditLinePrim::GetLineWidth()
{
	return _width;
}

Point EditLinePrim::GetCenter()
{
	return Point( (vec[0].x + vec[1].x)/2.0f, (vec[0].y + vec[1].y)/2.0f );
}

void EditLinePrim::SetSelect(bool select)
{
	_isSelected = select;
	_isPrimActive = false;
	_activeRectIndex = -1;
}

bool EditLinePrim::IsSelected()
{
	return _isSelected;
}

bool EditLinePrim::Contains(float x, float y)
{
	for(int i=0;i<2;i++)
		if( _rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
			return true;

	LineSeg2D lineSeg(vec[0], vec[1]);

	return (lineSeg.ContainsWithErrLimit(x,y, _width*1.5));
}

bool EditLinePrim::CollidesWithRect(float x, float y, float w, float h)
{
	LinePrim linePrim(vec[0].x, vec[0].y, vec[1].x, vec[1].y);

	return linePrim.CollidesWithRect(x, y, w, h);
}

void EditLinePrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	addData->glColor( IsInGroup() ? 0x0000ffff : ( _isDynamicBody ? 0x00ff00ff : 0xff0000ff) );

	CVector3 nor(vec[0].y - vec[1].y, vec[1].x - vec[0].x);
	nor.Normalize();
	nor *= _width/2;

	addData->glBegin(GL_TRIANGLE_STRIP);
	addData->glVertex3f(vec[0].x - nor.x, vec[0].y - nor.y, 0);
	addData->glVertex3f(vec[1].x - nor.x, vec[1].y - nor.y, 0);
	addData->glVertex3f(vec[0].x + nor.x, vec[0].y + nor.y, 0);
	addData->glVertex3f(vec[1].x + nor.x, vec[1].y + nor.y, 0);
	addData->glEnd();

	addData->glColor( _isDynamicBody ? 0x00ff00ff : 0xff0000ff);
	addData->glBegin(GL_LINE_LOOP);
	addData->glVertex3f(vec[0].x - nor.x, vec[0].y - nor.y, 0);
	addData->glVertex3f(vec[1].x - nor.x, vec[1].y - nor.y, 0);
	addData->glVertex3f(vec[1].x + nor.x, vec[1].y + nor.y, 0);
	addData->glVertex3f(vec[0].x + nor.x, vec[0].y + nor.y, 0);
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
		for(int i=0;i<2;i++)
		{
			_rectPrims[i]->SetPos(vec[i].x-_rw/2, vec[i].y-_rh/2);
			_rectPrims[i]->Draw();
		}
	}
}

EditLinePrim::~EditLinePrim()
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

