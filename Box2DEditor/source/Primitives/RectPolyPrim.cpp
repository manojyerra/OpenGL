#include "RectPolyPrim.h"
#include "../CommonLib/AddData.h"
#include "../CommonLib/UIFont.h"
#include <math.h>
#include "../CommonLib/Input.h"
#include "../Platform/Platform.h"


RectPolyPrim::RectPolyPrim(vector<Point> vec)
{
	_compoundShapeID = -1;
	ID = RECT_POLY_PRIM;

	_vec = vec;

	_r = 0;
	_g = 255;
	_b = 0;
	_a = 255;

	_poly2D = new Poly2D(_vec);
	
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
	_isDynamicBody = true;
}

RectPolyPrim::RectPolyPrim(float x, float y, float w, float h)
{
	_compoundShapeID = -1;
	ID = RECT_POLY_PRIM;

	_vec.clear();
	_vec.push_back(Point(x,y));
	_vec.push_back(Point(x+w,y));
	_vec.push_back(Point(x+w,y+h));
	_vec.push_back(Point(x,y+h));

	_r = 0;
	_g = 255;
	_b = 0;
	_a = 255;

	_poly2D = new Poly2D(_vec);
	
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
	_isDynamicBody = true;
	_actionPerformed = false;
}

Prim* RectPolyPrim::Clone()
{
	RectPolyPrim* rectPolyPrim = new RectPolyPrim(_vec);
	rectPolyPrim->_isPrimActive = _isPrimActive;
	rectPolyPrim->_activeRectIndex = _activeRectIndex;
	rectPolyPrim->_activeRectIndexToRotate = _activeRectIndexToRotate;
	rectPolyPrim->_isSelected = _isSelected;
	rectPolyPrim->_isDynamicBody = _isDynamicBody;

	return (Prim*)rectPolyPrim;
}

void RectPolyPrim::SetCompoundShapeID(int id)
{
	_compoundShapeID = id;
}

int RectPolyPrim::GetCompoundShapeID()
{
	return _compoundShapeID;
}

bool RectPolyPrim::Update()
{
	_actionPerformed = false;

	if(Input::IsKeyPressed(KEYS_MENU) && Input::IsMousePressed() && _activeRectIndexToRotate != -1)
	{
		float cx = 0;
		float cy = 0;

		for(int i=0;i<4;i++)
		{
			cx += _vec[i].x;
			cy += _vec[i].y;
		}

		cx /= 4;
		cy /= 4;

		int index = _activeRectIndexToRotate;

		float angle1 = atan2( _rectPrims[index]->GetMidY() - cy, _rectPrims[index]->GetMidX() - cx );
		float angle2 = atan2( Input::MouseY - cy, Input::MouseX - cx );

		angle1 = angle1 * 180.0f/(22.0f/7.0f);
		angle2 = angle2 * 180.0f/(22.0f/7.0f);

		float diffAngle = angle1 - angle2;

		if(diffAngle !=0)
			AddRotate(-diffAngle);

		_actionPerformed = true;
	}
	else if(Input::IsRightMouseClicked())
	{
		if(Contains((float)Input::MouseX, (float)Input::MouseY))
		{
			_isDynamicBody = !_isDynamicBody;
			_actionPerformed = true;
		}
	}
	else if(Input::IsMouseReleased())
	{
		_isPrimActive = false;
		_activeRectIndex = -1;
	}
	else if(Input::IsMousePressed() && Input::IsKeyPressed(KEYS_MENU) == false )
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
					_activeRectIndexToRotate = i;
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
			_actionPerformed = true;

			if(_activeRectIndex == 1)
			{
				float vecX = _vec[1].x - _vec[0].x;
				float vecY = _vec[1].y - _vec[0].y;

				float vecLength = sqrt( vecX*vecX + vecY*vecY );

				float norVecX = vecX /vecLength;
				float norVecY = vecY /vecLength;

				float dotValue = dx*norVecX + dy*norVecY;

				float newVecX = norVecX * (vecLength+dotValue);
				float newVecY = norVecY * (vecLength+dotValue);

				_vec[1].x = _vec[0].x + newVecX;
				_vec[1].y = _vec[0].y + newVecY;

				_vec[2].x = _vec[3].x + newVecX;
				_vec[2].y = _vec[3].y + newVecY;
				
			}
			else if(_activeRectIndex == 0)
			{
				float vecX = _vec[0].x - _vec[1].x;
				float vecY = _vec[0].y - _vec[1].y;

				float vecLength = sqrt( vecX*vecX + vecY*vecY );

				float norVecX = vecX /vecLength;
				float norVecY = vecY /vecLength;

				float dotValue = dx*norVecX + dy*norVecY;

				float newVecX = norVecX * (vecLength+dotValue);
				float newVecY = norVecY * (vecLength+dotValue);

				_vec[0].x = _vec[1].x + newVecX;
				_vec[0].y = _vec[1].y + newVecY;

				_vec[3].x = _vec[2].x + newVecX;
				_vec[3].y = _vec[2].y + newVecY;
			}
			else if(_activeRectIndex == 2)
			{
				float vecX = _vec[1].x - _vec[2].x;
				float vecY = _vec[1].y - _vec[2].y;

				float vecLength = sqrt( vecX*vecX + vecY*vecY );

				float norVecX = vecX /vecLength;
				float norVecY = vecY /vecLength;

				float dotValue = dx*norVecX + dy*norVecY;

				float newVecX = norVecX * (vecLength+dotValue);
				float newVecY = norVecY * (vecLength+dotValue);

				_vec[1].x = _vec[2].x + newVecX;
				_vec[1].y = _vec[2].y + newVecY;

				_vec[0].x = _vec[3].x + newVecX;
				_vec[0].y = _vec[3].y + newVecY;
			}
			else if(_activeRectIndex == 3)
			{
				float vecX = _vec[3].x - _vec[0].x;
				float vecY = _vec[3].y - _vec[0].y;

				float vecLength = sqrt( vecX*vecX + vecY*vecY );

				float norVecX = vecX /vecLength;
				float norVecY = vecY /vecLength;

				float dotValue = dx*norVecX + dy*norVecY;

				float newVecX = norVecX * (vecLength+dotValue);
				float newVecY = norVecY * (vecLength+dotValue);

				_vec[3].x = _vec[0].x + newVecX;
				_vec[3].y = _vec[0].y + newVecY;

				_vec[2].x = _vec[1].x + newVecX;
				_vec[2].y = _vec[1].y + newVecY;
			}


			for(int i=0;i<(int)_vec.size();i++)
			{
				_poly2D->pointsVec[i].x = _vec[i].x;
				_poly2D->pointsVec[i].y = _vec[i].y;
			}

			SetRectPos();

		}
		else if(_isPrimActive)
		{
			for(int i=0;i<(int)_vec.size();i++)
			{
				_vec[i].x += dx;
				_vec[i].y += dy;

				_poly2D->pointsVec[i].x = _vec[i].x;
				_poly2D->pointsVec[i].y = _vec[i].y;
			}
			SetRectPos();
			_actionPerformed = true;
		}
	}
	else if(Input::IsKeyPressed(KEYS_CONTROL))
	{
		//if( Input::IsMousePressed() && _activeRectIndex != -1)
		//{
		//	float cx = 0;
		//	float cy = 0;

		//	for(int i=0;i<4;i++)
		//	{
		//		cx += _vec[i].x;
		//		cy += _vec[i].y;
		//	}

		//	cx /= 4;
		//	cy /= 4;

		//	float angle1 = atan2( _rectPrims[_activeRectIndex]->GetMidY() - cy, _rectPrims[_activeRectIndex]->GetMidX() - cx );
		//	float angle2 = atan2( Input::MouseY - cy, Input::MouseX - cx );

		//	angle1 = angle1 * 180.0f/(22.0f/7.0f);
		//	angle2 = angle2 * 180.0f/(22.0f/7.0f);

		//	float diffAngle = angle1 - angle2;

		//	AddRotate(-diffAngle);
		//}
		//else 
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

void RectPolyPrim::Move(float x, float y)
{
	for(int i=0;i<4;i++)
	{
		_vec[i].x += x;
		_vec[i].y += y;
	}
	SetRectPos();
	_actionPerformed = true;
}


void RectPolyPrim::AddRotate(float addAngle)
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<4;i++)
	{
		cx += _vec[i].x;
		cy += _vec[i].y;
	}

	cx /= 4.0f;
	cy /= 4.0f;

	for(int i=0;i<4;i++)
	{
		Point p = Point::RotatePointZ( addAngle, &Point(_vec[i].x,_vec[i].y), &Point(cx, cy));

		_vec[i].x = p.x;
		_vec[i].y = p.y;
	}
	SetRectPos();
	_actionPerformed = true;
}

void RectPolyPrim::SetRectPos()
{
	float rectX = (_vec[0].x + _vec[3].x) / 2.0f;
	float rectY = (_vec[0].y + _vec[3].y) / 2.0f;
	_rectPrims[0]->SetPos(rectX-_rw/2, rectY-_rh/2);

	rectX = (_vec[1].x + _vec[2].x) / 2.0f;
	rectY = (_vec[1].y + _vec[2].y) / 2.0f;
	_rectPrims[1]->SetPos(rectX-_rw/2, rectY-_rh/2);

	rectX = (_vec[0].x + _vec[1].x) / 2.0f;
	rectY = (_vec[0].y + _vec[1].y) / 2.0f;
	_rectPrims[2]->SetPos(rectX-_rw/2, rectY-_rh/2);

	rectX = (_vec[2].x + _vec[3].x) / 2.0f;
	rectY = (_vec[2].y + _vec[3].y) / 2.0f;
	_rectPrims[3]->SetPos(rectX-_rw/2, rectY-_rh/2);
}

void RectPolyPrim::SetSelect(bool select)
{
	_isSelected = select;
	_isPrimActive = false;
	_activeRectIndex = -1;
}

bool RectPolyPrim::IsStaticBody()
{
	return(!_isDynamicBody);
}

bool RectPolyPrim::IsDynamicBody()
{
	return(_isDynamicBody);
}


bool RectPolyPrim::Contains(float x, float y)
{
	for(int i=0;i<(int)_rectPrims.size();i++)
		if( _rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
			return true;

	if(_poly2D->Contains(x, y))
		return true;

	return false;
}


void RectPolyPrim::SetColor(int r, int g, int b, int a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}


void RectPolyPrim::Draw()
{
	AddData* addData = AddData::GetInstance();

	if(_isDynamicBody)
		addData->glColor4f( ((float)_r)/255.0f, ((float)_g)/255.0f,((float)_b)/255.0f,((float)_a)/(2.0f*255.0f));
	else
		addData->glColor4f(1,0,0,0.5);

	addData->glBegin(GL_POLYGON);
	for(int i=0;i<(int)_vec.size();i++)
		addData->glVertex3f(_vec[i].x, _vec[i].y, 0);
	addData->glEnd();

	//BORDER
	if(_isSelected)
		addData->glColor4f(0,0,1,1);
	else
		addData->glColor4f( ((float)_r)/255.0f, ((float)_g)/255.0f,((float)_b)/255.0f,((float)_a)/255.0f);

	addData->glBegin(GL_LINE_LOOP);

	for(int i=0;i<(int)_vec.size();i++)
		addData->glVertex3f(_vec[i].x, _vec[i].y, 0);

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
		for(int i=0;i<(int)_rectPrims.size();i++)
			_rectPrims[i]->Draw();
	}
}

Point RectPolyPrim::GetCenter()
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<4;i++)
	{
		cx += _vec[i].x;
		cy += _vec[i].y;
	}

	cx /= 4.0f;
	cy /= 4.0f;

	return Point(cx, cy, 0);
}

RectPolyPrim::~RectPolyPrim()
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
