#include "BlastRect.h"
#include "GameInfo.h"
#include "Utils.h"

BlastRect::BlastRect(float x, float y, float w, float h)
{
	float bw = w/3;
	float bh = h/3;
	
	for(int j=0;j<3;j++)
	{
		for(int i=0;i<3;i++)
		{	
			_blastVec.push_back(new RectPiece(x+bw*i, y+bh*j, bw, bh));
		}
	}	
}

BlastRect::~BlastRect()
{
	for(int i=0;i<_blastVec.size();i++)
	{
		if(_blastVec[i])
		{
			delete _blastVec[i];
			_blastVec[i] = NULL;
		}
	}
}

void BlastRect::Reset()
{
	for(int i=0;i<_blastVec.size();i++)
		_blastVec[i]->Reset();
}

void BlastRect::SetColor(unsigned int color)
{
	for(int i=0;i<_blastVec.size();i++)
		_blastVec[i]->SetColor(color);
}

void BlastRect::Draw(AddData* addData)
{
	for(int i=0;i<_blastVec.size();i++)
		_blastVec[i]->Draw(addData);
}


//RectPiece


RectPiece::RectPiece(float x, float y, float w, float h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	
	_oh = GameInfo::GetInstance()->CodedH();
	
	_rectPrim = new RectPrim(x, y, w, h, 0, 0xffffffff);
	
	Reset();
}

void RectPiece::Reset()
{
	_rectPrim->SetBounds(_x, _y, _w, _h);
	
	_gVel = 0.0f;
	
	_xVel = 2 + absInt(randInt()%3);
	_yVel = -(2 + absInt(randInt()%3));
	
	if(absInt(randInt()%2) == 0)
		_xVel = -_xVel;
		
	_rotSpeed = 15+randInt()%10;
	
	_frameCount = 0;
}

void RectPiece::SetPos(float x, float y)
{
	_rectPrim->SetPos(x,y);
}

void RectPiece::SetBounds(float x, float y, float w, float h)
{
	_rectPrim->SetBounds(x, y, w, h);
}

void RectPiece::SetColor(unsigned int color)
{
	_rectPrim->SetColor(color);
}

void RectPiece::Draw(AddData* addData)
{
	if(_frameCount < 5)
	{
		float x = _rectPrim->x;
		float y = _rectPrim->y;
		float w = _rectPrim->w;
		float h = _rectPrim->h;
		
		unsigned int color = _rectPrim->GetColor();
				
		//RectPrim::Draw(x+1,y+1,w-2,h-2, 0, 0x00000088, addData);
		RectPrim::Draw(x+2,y+2,w-4,h-4, 0, color, addData);
	}
	else
	{
		if(_rectPrim->y < _oh)
		{ 
			_gVel += GameInfo::deltaTime * 9.8f;
	
			_rectPrim->y = _rectPrim->y + _yVel + _gVel;
			_rectPrim->x = _rectPrim->x + _xVel;
			_rectPrim->SetRotation( _rectPrim->GetRotation() + _rotSpeed );

			unsigned int color = _rectPrim->GetColor();
			float angle = _rectPrim->GetRotation();
	
			//RectPrim::Draw(_rectPrim->GetX(), _rectPrim->GetY(), _rectPrim->GetWidth(), _rectPrim->GetHeight(), angle, 0x00000088, addData);
			RectPrim::Draw(_rectPrim->x+1, _rectPrim->y+1, _rectPrim->w-2, _rectPrim->h-2, angle, color, addData);
		}
	}
	
	_frameCount++;
}

RectPiece::~RectPiece()
{
	if(_rectPrim)
	{
		delete _rectPrim;
		_rectPrim = NULL;
	}
}
