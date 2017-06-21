#include "PolyPrim.h"
#include "../CommonLib/AddData.h"
#include "../CommonLib/UIFont.h"
#include <math.h>
#include "../CommonLib/Input.h"
#include "../Platform/Platform.h"

PolyPrim::PolyPrim(vector<Point> vec)
{
	_compoundShapeID = -1;
	ID = POLY;

	_vec = vec;

	_r = 0;
	_g = 255;
	_b = 0;
	_a = 255;

	_poly2D = new Poly2D(vec);
	
	_rw = 16;
	_rh = 16;

	for(int i=0;i<(int)_vec.size();i++)
	{
		RectPrim* rectPrim = new RectPrim(_vec[i].x-_rw/2, _vec[i].y-_rh/2, _rw, _rh, 0, 0x00ff00ff);
		_rectPrims.push_back(rectPrim);
	}

	_isPrimActive = false;
	_activeRectIndex = -1;
	_isSelected = false;
	_isDynamicBody = true;
}

Prim* PolyPrim::Clone()
{
	PolyPrim* prim = new PolyPrim(_vec);
	prim->_isPrimActive = _isPrimActive;
	prim->_activeRectIndex = _activeRectIndex;
	prim->_isSelected = _isSelected;
	prim->_isDynamicBody = _isDynamicBody;
	return (Prim*)prim;
}


void PolyPrim::SetCompoundShapeID(int id)
{
	_compoundShapeID = id;
}

int PolyPrim::GetCompoundShapeID()
{
	return _compoundShapeID;
}


bool PolyPrim::Update()
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
			for(int i=0;i<(int)_rectPrims.size();i++)
			{
				if( _rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
				{
					_activeRectIndex = i;
					break;
				}
			}

			if(_activeRectIndex == -1 && Contains((float)Input::MouseX, (float)Input::MouseY))
			{
				_isPrimActive = true;
			}
		}

		if(_activeRectIndex != -1)
		{
			float prevX = _poly2D->pointsVec[_activeRectIndex].x;
			float prevY = _poly2D->pointsVec[_activeRectIndex].y;

			_poly2D->pointsVec[_activeRectIndex].x += dx;
			_poly2D->pointsVec[_activeRectIndex].y += dy;

			if(_poly2D->IsConvexPolygon())
			{
				_rectPrims[_activeRectIndex]->SetX(_rectPrims[_activeRectIndex]->GetX()+dx);
				_rectPrims[_activeRectIndex]->SetY(_rectPrims[_activeRectIndex]->GetY()+dy);

				_vec[_activeRectIndex].x += dx;
				_vec[_activeRectIndex].y += dy;
				actionPerformed = true;
			}
			else
			{
				_poly2D->pointsVec[_activeRectIndex].x = prevX;
				_poly2D->pointsVec[_activeRectIndex].y = prevY;
			}

		}
		else if(_isPrimActive)
		{
			for(int i=0;i<(int)_vec.size();i++)
			{
				_vec[i].x += dx;
				_vec[i].y += dy;

				_rectPrims[i]->SetPos(_vec[i].x-_rw/2, _vec[i].y-_rh/2);

				_poly2D->pointsVec[i].x = _vec[i].x;
				_poly2D->pointsVec[i].y = _vec[i].y;
				actionPerformed = true;
			}
		}
	}
	else if(Input::IsKeyTyped(KEYS_RIGHT))				Move(1, 0);
	else if(Input::IsKeyTyped(KEYS_LEFT))				Move(-1, 0);
	else if(Input::IsKeyTyped(KEYS_UP))					Move(0, -1);
	else if(Input::IsKeyTyped(KEYS_DOWN))				Move(0, 1);
	else if(Input::IsKeyPressedStill(KEYS_RIGHT, 0.3f))		Move(2, 0);
	else if(Input::IsKeyPressedStill(KEYS_LEFT, 0.3f))		Move(-2, 0);
	else if(Input::IsKeyPressedStill(KEYS_UP, 0.3f))			Move(0, -2);
	else if(Input::IsKeyPressedStill(KEYS_DOWN, 0.3f))		Move(0, 2);

	return actionPerformed;
}

void PolyPrim::Move(float x, float y)
{
	for(int i=0;i<(int)_vec.size();i++)
	{
		_vec[i].x += x;
		_vec[i].y += y;

		_rectPrims[i]->SetPos(_vec[i].x-_rw/2, _vec[i].y-_rh/2);

		_poly2D->pointsVec[i].x = _vec[i].x;
		_poly2D->pointsVec[i].y = _vec[i].y;
	}
}

void PolyPrim::SetSelect(bool select)
{
	_isSelected = select;
	_isPrimActive = false;
	_activeRectIndex = -1;
}

bool PolyPrim::IsStaticBody()
{
	return(!_isDynamicBody);
}

bool PolyPrim::IsDynamicBody()
{
	return(_isDynamicBody);
}


bool PolyPrim::Contains(float x, float y)
{
	for(int i=0;i<(int)_rectPrims.size();i++)
		if( _rectPrims[i]->Contains((float)Input::MouseX, (float)Input::MouseY))
			return true;

	if(_poly2D->Contains(x, y))
		return true;

	return false;
}


void PolyPrim::SetColor(int r, int g, int b, int a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}


void PolyPrim::Draw()
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
		for(int i=0;i<(int)_rectPrims.size();i++)
		{
			_rectPrims[i]->SetPos(_vec[i].x-_rw/2, _vec[i].y-_rh/2);
			_rectPrims[i]->Draw();
		}
	}
}

Point PolyPrim::GetCenter(  )
{
	float cx = 0;
	float cy = 0;

	for(int i=0;i<(int)_vec.size();i++)
	{
		cx += _vec[i].x;
		cy += _vec[i].y;
	}

	cx /= (float)_vec.size();
	cy /= (float)_vec.size();

	return Point(cx, cy, 0);
}

PolyPrim::~PolyPrim()
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
