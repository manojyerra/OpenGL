#include "EditRectPrim.h"
#include "AddData.h"
#include "UIFont.h"
#include "Input.h"
#include "Platform.h"
#include "PolyPrim.h"
#include <math.h>

EditRectPrim::EditRectPrim(vector<Point> vec) : EditPrim(RECT)
{
	Init(vec);
}

EditRectPrim::EditRectPrim(float x, float y, float w, float h) : EditPrim(RECT)
{
	vector<Point> vec;

	vec.clear();
	vec.push_back(Point(x,y));
	vec.push_back(Point(x+w,y));
	vec.push_back(Point(x+w,y+h));
	vec.push_back(Point(x,y+h));

	Init(vec);
}

void EditRectPrim::Init(vector<Point> vecPoints)
{
	vec = vecPoints;

	ID = RECT;
	_compoundShapeID = -1;

	_poly2D = new Poly2D(vec);
	
	_rw = 16;
	_rh = 16;

	for(int i=0;i<4;i++)
	{
		RectPrim* rectPrim = new RectPrim(0, 0, _rw, _rh, 0, 0x00ff00ff);
		_rectPrims.push_back(rectPrim);
	}

	SetRectPos();

	_isPrimActive = false;
	_activeRectIndex = -1;
	_activeRectIndexToRotate = 0;
	_isSelected = false;
}

EditPrim* EditRectPrim::Clone()
{
	EditRectPrim* rectPolyPrim = new EditRectPrim(vec);
	rectPolyPrim->_isPrimActive = _isPrimActive;
	rectPolyPrim->_activeRectIndex = _activeRectIndex;
	rectPolyPrim->_activeRectIndexToRotate = _activeRectIndexToRotate;
	rectPolyPrim->_isSelected = _isSelected;
	rectPolyPrim->_isDynamicBody = _isDynamicBody;
	rectPolyPrim->_isInGroup = _isInGroup;
	rectPolyPrim->_compoundShapeID = _compoundShapeID;

	return (EditPrim*)rectPolyPrim;
}

void EditRectPrim::Update()
{
	if(Input::IsMouseClicked() || Input::IsMousePressed())
	{
		for(int i=0;i<_rectPrims.size();i++)
		{
			if(_rectPrims[i]->Contains(Input::MouseX, Input::MouseY))
			{
				_activeRectIndexToRotate = i;
				break;
			}
		}
	}

	if( Input::IsKeyPressed(KEYS_MENU) && Input::IsMousePressed() && _activeRectIndexToRotate != -1)
	{
		float cx = 0;
		float cy = 0;

		for(int i=0;i<4;i++)
		{
			cx += vec[i].x;
			cy += vec[i].y;
		}

		cx /= 4;
		cy /= 4;

		int index = _activeRectIndexToRotate;

		float angle1 = atan2( _rectPrims[index]->GetCY() - cy, _rectPrims[index]->GetCX() - cx );
		float angle2 = atan2( Input::MouseY - cy, Input::MouseX - cx );

		angle1 = angle1 * 180.0f/(22.0f/7.0f);
		angle2 = angle2 * 180.0f/(22.0f/7.0f);

		float diffAngle = angle1 - angle2;

		if(diffAngle !=0)
			AddRotate(-diffAngle);
	}
	else if( (Input::IsMouseClicked() || Input::IsMousePressed()) && Input::IsKeyPressed(KEYS_MENU) == false )
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

			if(_activeRectIndex == -1 && Contains((float)Input::MouseX, (float)Input::MouseY ) )
			{
				_isPrimActive = true;
			}
		}

		if(_activeRectIndex != -1)
		{
			if(_activeRectIndex == 1)
			{
				float vecX = vec[1].x - vec[0].x;
				float vecY = vec[1].y - vec[0].y;

				float vecLength = sqrt( vecX*vecX + vecY*vecY );

				float norVecX = vecX /vecLength;
				float norVecY = vecY /vecLength;

				float dotValue = dx*norVecX + dy*norVecY;

				float newVecX = norVecX * (vecLength+dotValue);
				float newVecY = norVecY * (vecLength+dotValue);

				vec[1].x = vec[0].x + newVecX;
				vec[1].y = vec[0].y + newVecY;

				vec[2].x = vec[3].x + newVecX;
				vec[2].y = vec[3].y + newVecY;
				
			}
			else if(_activeRectIndex == 0)
			{
				float vecX = vec[0].x - vec[1].x;
				float vecY = vec[0].y - vec[1].y;

				float vecLength = sqrt( vecX*vecX + vecY*vecY );

				float norVecX = vecX /vecLength;
				float norVecY = vecY /vecLength;

				float dotValue = dx*norVecX + dy*norVecY;

				float newVecX = norVecX * (vecLength+dotValue);
				float newVecY = norVecY * (vecLength+dotValue);

				vec[0].x = vec[1].x + newVecX;
				vec[0].y = vec[1].y + newVecY;

				vec[3].x = vec[2].x + newVecX;
				vec[3].y = vec[2].y + newVecY;
			}
			else if(_activeRectIndex == 2)
			{
				float vecX = vec[1].x - vec[2].x;
				float vecY = vec[1].y - vec[2].y;

				float vecLength = sqrt( vecX*vecX + vecY*vecY );

				float norVecX = vecX /vecLength;
				float norVecY = vecY /vecLength;

				float dotValue = dx*norVecX + dy*norVecY;

				float newVecX = norVecX * (vecLength+dotValue);
				float newVecY = norVecY * (vecLength+dotValue);

				vec[1].x = vec[2].x + newVecX;
				vec[1].y = vec[2].y + newVecY;

				vec[0].x = vec[3].x + newVecX;
				vec[0].y = vec[3].y + newVecY;
			}
			else if(_activeRectIndex == 3)
			{
				float vecX = vec[3].x - vec[0].x;
				float vecY = vec[3].y - vec[0].y;

				float vecLength = sqrt( vecX*vecX + vecY*vecY );

				float norVecX = vecX /vecLength;
				float norVecY = vecY /vecLength;

				float dotValue = dx*norVecX + dy*norVecY;

				float newVecX = norVecX * (vecLength+dotValue);
				float newVecY = norVecY * (vecLength+dotValue);

				vec[3].x = vec[0].x + newVecX;
				vec[3].y = vec[0].y + newVecY;

				vec[2].x = vec[1].x + newVecX;
				vec[2].y = vec[1].y + newVecY;
			}

			for(int i=0;i<(int)vec.size();i++)
			{
				_poly2D->pointsVec[i].x = vec[i].x;
				_poly2D->pointsVec[i].y = vec[i].y;
			}

			SetRectPos();
		}
		else if(_isPrimActive)
		{
			for(int i=0;i<(int)vec.size();i++)
			{
				vec[i].x += dx;
				vec[i].y += dy;

				_poly2D->pointsVec[i].x = vec[i].x;
				_poly2D->pointsVec[i].y = vec[i].y;
			}

			SetRectPos();
		}
	}
	
	//else if(Input::IsKeyPressed(KEYS_CONTROL))
	//{
	//	//if( Input::IsMousePressed() && _activeRectIndex != -1)
	//	//{
	//	//	float cx = 0;
	//	//	float cy = 0;

	//	//	for(int i=0;i<4;i++)
	//	//	{
	//	//		cx += _vec[i].x;
	//	//		cy += _vec[i].y;
	//	//	}

	//	//	cx /= 4;
	//	//	cy /= 4;

	//	//	float angle1 = atan2( _rectPrims[_activeRectIndex]->GetMidY() - cy, _rectPrims[_activeRectIndex]->GetMidX() - cx );
	//	//	float angle2 = atan2( Input::MouseY - cy, Input::MouseX - cx );

	//	//	angle1 = angle1 * 180.0f/(22.0f/7.0f);
	//	//	angle2 = angle2 * 180.0f/(22.0f/7.0f);

	//	//	float diffAngle = angle1 - angle2;

	//	//	AddRotate(-diffAngle);
	//	//}
	//	//else 
	//}
}

float EditRectPrim::Dist(float x1, float y1, float x2, float y2)
{
	return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

void EditRectPrim::Move(float x, float y)
{
	for(int i=0;i<4;i++)
	{
		vec[i].x += x;
		vec[i].y += y;
	}
	SetRectPos();
}

void EditRectPrim::AddRotate(float addAngle)
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<4;i++)
	{
		cx += vec[i].x;
		cy += vec[i].y;
	}

	cx /= 4.0f;
	cy /= 4.0f;

	AddRotation(addAngle, cx, cy);
}

void EditRectPrim::AddRotation(float addAngle, float pivotX, float pivotY)
{
	Point center = Point(pivotX, pivotY);

	for(int i=0;i<4;i++)
	{
		Point p1 = Point(vec[i].x, vec[i].y);

		Point p = Point::RotatePointZ(addAngle, &p1, &center);

		vec[i].x = p.x;
		vec[i].y = p.y;
	}

	SetRectPos();
}

void EditRectPrim::SetRectPos()
{
	float rectX = (vec[0].x + vec[3].x) / 2.0f;
	float rectY = (vec[0].y + vec[3].y) / 2.0f;
	_rectPrims[0]->SetPos(rectX-_rw/2, rectY-_rh/2);

	rectX = (vec[1].x + vec[2].x) / 2.0f;
	rectY = (vec[1].y + vec[2].y) / 2.0f;
	_rectPrims[1]->SetPos(rectX-_rw/2, rectY-_rh/2);

	rectX = (vec[0].x + vec[1].x) / 2.0f;
	rectY = (vec[0].y + vec[1].y) / 2.0f;
	_rectPrims[2]->SetPos(rectX-_rw/2, rectY-_rh/2);

	rectX = (vec[2].x + vec[3].x) / 2.0f;
	rectY = (vec[2].y + vec[3].y) / 2.0f;
	_rectPrims[3]->SetPos(rectX-_rw/2, rectY-_rh/2);
}

void EditRectPrim::SetSelect(bool select)
{
	_isSelected = select;
	_isPrimActive = false;
	_activeRectIndex = -1;
}

bool EditRectPrim::IsSelected()
{
	return _isSelected;
}

bool EditRectPrim::Contains(float x, float y)
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

bool EditRectPrim::CollidesWithRect(float x, float y, float w, float h)
{
	PolyPrim polyPrim(vec);
	return polyPrim.CollidesWithRect(x, y, w, h);
}

void EditRectPrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	addData->glColor( IsInGroup() ? 0x0000ff88 : ( _isDynamicBody ? 0x00ff0088 : 0xff000088) );

	if(_angularVel > 0 && _angularVelLoop)
		addData->glColor(0xff00ffff );

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
			_rectPrims[i]->Draw();
	}
}

Point EditRectPrim::GetCenter()
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<4;i++)
	{
		cx += vec[i].x;
		cy += vec[i].y;
	}

	cx /= 4.0f;
	cy /= 4.0f;

	return Point(cx, cy, 0);
}

EditRectPrim::~EditRectPrim()
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
