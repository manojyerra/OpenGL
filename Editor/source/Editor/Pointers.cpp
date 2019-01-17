#include "Pointers.h"
#include "Sui/SuiInput.h"
#include "MathLib/Line2D.h"

Pointers::Pointers()
{
	_model = NULL;
	_anchorW = 9.0f;
	_activePointer = NONE;
	_moveLen = 0.0f;
	_anchorType = ANCHOR_RECT;
}

void Pointers::SetModel(Model3D* model)
{
	_model = model;
}

float Pointers::GetMoveLen()
{
	return _moveLen;
}

bool Pointers::IsXActive(){ return (_activePointer == X_POINTER); }
bool Pointers::IsYActive(){ return (_activePointer == Y_POINTER); }
bool Pointers::IsZActive(){ return (_activePointer == Z_POINTER); }

void Pointers::Update(Cam* cam)
{
	if(_model == NULL)
		return;

	if(_activePointer == NONE)
		UpdatePos(cam);

	if(SuiInput::IsKeyReleased((int)'R'))
		ChangeAnchorType();

	float mx = SuiInput::MX;
	float my = SuiInput::MY;

	CVector3 left = center + leftVec;
	CVector3 up = center + upVec;
	CVector3 forward = center + forwardVec;

	if(SuiInput::IsMouseReleased())
	{
		_activePointer = NONE;
		_moveLen = 0.0f;
	}
	else if(SuiInput::IsMouseClicked() && _activePointer == NONE)
	{
		if( mx >= left.x-_anchorW && mx <= left.x+_anchorW && my >= left.y-_anchorW && my <= left.y+_anchorW)
		{
			_activePointer = X_POINTER;
			_activeVec = left;
			_activeCenter = center;
		}
		else if( mx >= up.x-_anchorW && mx <= up.x+_anchorW && my >= up.y-_anchorW && my <= up.y+_anchorW)
		{
			_activePointer = Y_POINTER;
			_activeVec = up;
			_activeCenter = center;
		}
		else if( mx >= forward.x-_anchorW && mx <= forward.x+_anchorW && my >= forward.y-_anchorW && my <= forward.y+_anchorW)
		{
			_activePointer = Z_POINTER;
			_activeVec = forward;
			_activeCenter = center;
		}
	}
	else if(SuiInput::IsMousePressed() && _activePointer != NONE)
	{
		CVector3 projFirst( SuiInput::PrevMX - center.x, SuiInput::PrevMY - center.y);
		CVector3 projSecond( mx				 - center.x, my				 - center.y);

		CVector3 vec;

		if(_activePointer == X_POINTER)			vec = leftVec;
		else if(_activePointer == Y_POINTER)	vec = upVec;
		else if(_activePointer == Z_POINTER)	vec = forwardVec;

		vec.Normalize();

		float firstLen = vec.Dot(projFirst);
		float secondLen = vec.Dot(projSecond);

		_moveLen = secondLen - firstLen;
	}

	MoveModelOnMouseInput(_model);
}

void Pointers::MoveModelOnMouseInput(Model3D* model)
{
	if(IsXActive() || IsYActive() || IsZActive())
	{
		if(SuiInput::IsMouseDragged())
		{
			if(GetAnchorType() == Pointers::ANCHOR_CIRCLE)
			{
				if(IsXActive())			model->RotInLocal('x', GetMoveLen());
				else if(IsYActive())	model->RotInLocal('y', GetMoveLen());
				else if(IsZActive())	model->RotInLocal('z', GetMoveLen());
			}
			else if(GetAnchorType() == Pointers::ANCHOR_RECT)
			{
				float move = GetMoveLen()/10.0f;

				if(SuiInput::IsKeyPressed(VK_SPACE))
					move = move/100;

				if(IsXActive())			model->TransInLocal('x', move);
				else if(IsYActive())	model->TransInLocal('y', move);
				else if(IsZActive())	model->TransInLocal('z', move);
			}
		}
	}

	//if(_pointers->GetAnchorType() == Pointers::ANCHOR_CIRCLE)
	//{
	//	if(SuiInput::IsMouseReleased())
	//	{
	//		_xRotActive = false;
	//		_yRotActive = false;
	//	}
	//	else if(SuiInput::IsMouseDragged())
	//	{
	//		if(_xRotActive)								model->RotInLocal('x', GetXAngle());
	//		else if(_yRotActive)						model->RotInLocal('y', GetYAngle());
	//		else if(SuiInput::IsKeyPressed((int)'Z'))	model->RotInLocal('z', GetYAngle());

	//		else if(_pointers->IsXActive())				model->RotInLocal('x', _pointers->GetMoveLen());
	//		else if(_pointers->IsYActive())				model->RotInLocal('y', _pointers->GetMoveLen());
	//		else if(_pointers->IsZActive())				model->RotInLocal('z', _pointers->GetMoveLen());

	//		else if(SuiInput::GetDragDist() > 10)
	//		{
	//			float moveAngle = GetMoveAngle();
	//			
	//			if( (moveAngle >= 0 && moveAngle <= 45) || (moveAngle > 315 && moveAngle <= 360) || (moveAngle >= 135 && moveAngle <= 225))
	//				_yRotActive = true;
	//			else if((moveAngle >= 45 && moveAngle <= 135) || (moveAngle >= 225 && moveAngle <= 315))
	//				_xRotActive = true;
	//		}
	//	}
	//}
	//else if(_pointers->GetAnchorType() == Pointers::ANCHOR_RECT)
	//{
	//	float move = _pointers->GetMoveLen()/10.0f;

	//	if(SuiInput::IsKeyPressed(VK_SPACE))
	//		move = move/100;

	//	if(_pointers->IsXActive())			model->TransInLocal('x', move);
	//	else if(_pointers->IsYActive())		model->TransInLocal('y', move);
	//	else if(_pointers->IsZActive())		model->TransInLocal('z', move);
	//}
}

void Pointers::UpdatePos(Cam* cam)
{
	Cam* _cam = cam;

	const FLMStruct* bin = _model->GetFLMStruct();

	float cx = (bin->minX + bin->maxX) / 2.0f;
	float cy = (bin->minY + bin->maxY) / 2.0f;
	float cz = (bin->minZ + bin->maxZ) / 2.0f;

	float maxW = 3.0f;
	float cp[3] = { cx, cy, cz };
	float left[3] = { maxW,cy,cz };
	float up[3] = { cx, maxW, cz };
	float forward[3] = { cx, cy, maxW };

	float cp2D[2];
	float left2D[2];
	float up2D[2];
	float forward2D[2];

	float* mat = _model->GetOrientation().Get();

	_cam->GetPosOnScreen(cp, 1, mat, cp2D);
	_cam->GetPosOnScreen(left, 1, mat, left2D);
	_cam->GetPosOnScreen(up, 1, mat, up2D);
	_cam->GetPosOnScreen(forward, 1, mat, forward2D);

	center = CVector3( cp2D[0], cp2D[1], 0);
	leftVec = CVector3( left2D[0] - cp2D[0], left2D[1] - cp2D[1], 0);
	upVec = CVector3( up2D[0] - cp2D[0], up2D[1] - cp2D[1], 0);
	forwardVec = CVector3( forward2D[0] - cp2D[0], forward2D[1] - cp2D[1], 0);

	float maxLen = 70;

	float leftLen = leftVec.Length();
	float upLen = upVec.Length();
	float forwardLen = forwardVec.Length();

	if(leftLen >= upLen && leftLen >= forwardLen)
	{
		upLen = upLen * maxLen / leftLen;
		forwardLen = forwardLen * maxLen / leftLen;
		leftLen = maxLen;
	}
	else if(upLen >= leftLen && upLen >= forwardLen)
	{
		leftLen = leftLen * maxLen / upLen;
		forwardLen = forwardLen * maxLen / upLen;
		upLen = maxLen;
	}
	else
	{
		leftLen = leftLen * maxLen / forwardLen;
		upLen = upLen * maxLen / forwardLen;
		forwardLen = maxLen;
	}

	leftVec.Normalize();
	leftVec *= leftLen;

	upVec.Normalize();
	upVec *= upLen;

	forwardVec.Normalize();
	forwardVec *= forwardLen;
}

void Pointers::SetAnchorsAsCircles()
{
	_anchorType = ANCHOR_CIRCLE;
}

void Pointers::SetAnchorsAsRect()
{
	_anchorType = ANCHOR_RECT;
}

void Pointers::ChangeAnchorType()
{
	if(_anchorType == ANCHOR_CIRCLE)
		_anchorType = ANCHOR_RECT;
	else
		_anchorType = ANCHOR_CIRCLE;
}

int Pointers::GetAnchorType()
{
	return _anchorType;
}

int Pointers::GetActiveAxis()
{
	return _activePointer;
}

void Pointers::Draw(float windowW, float windowH)
{
	if(_model == NULL || _activePointer != NONE)
		return;

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowW, windowH, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(3.0f);

	glBegin(GL_LINES);
		glColor4ub(255, 0, 0, 255);
		glVertex3f(center.x, center.y, 0);
		glVertex3f(center.x + leftVec.x, center.y + leftVec.y, 0);

		glColor4ub(0, 255, 0, 255);
		glVertex3f(center.x, center.y, 0);
		glVertex3f(center.x + upVec.x, center.y + upVec.y, 0);

		glColor4ub(0, 0, 255, 255);
		glVertex3f(center.x, center.y, 0);
		glVertex3f(center.x + forwardVec.x, center.y + forwardVec.y, 0);
	glEnd();

	if(_anchorType == ANCHOR_CIRCLE)
		glEnable(GL_POINT_SMOOTH);
	else
		glDisable(GL_POINT_SMOOTH);

	glPointSize(_anchorW);
	glBegin(GL_POINTS);
		glColor4ub(255, 0, 0, 255);
		glVertex3f(center.x + leftVec.x, center.y + leftVec.y, 0);

		glColor4ub(0, 255, 0, 255);
		glVertex3f(center.x + upVec.x, center.y + upVec.y, 0);

		glColor4ub(0, 0, 255, 255);
		glVertex3f(center.x + forwardVec.x, center.y + forwardVec.y, 0);
	glEnd();

	glEnable(GL_DEPTH_TEST);
}

Pointers::~Pointers()
{
}
