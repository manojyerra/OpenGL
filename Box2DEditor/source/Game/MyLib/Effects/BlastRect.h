#ifndef BLASTRECT_H
#define BLASTRECT_H
#pragma once

#include "RectPrim.h"
#include "AddData.h"
#include <vector>

using namespace std;

class RectPiece
{
private:
	RectPrim* _rectPrim;

	float _x;
	float _y;
	float _w;
	float _h;
	
	float _speed;
	float _rotSpeed;
	
	float _xVel;
	float _yVel;
	float _gVel;
	
	float _oh;
	int _frameCount;

public:	
	RectPiece(float x, float y, float w, float h);
	~RectPiece();

	void Reset();
	void SetPos(float x, float y);
	void SetBounds(float x, float y, float w, float h);	
	void SetColor(unsigned int color);
	
	void Draw(AddData* addData);
};

class BlastRect
{
private:
	vector<RectPiece*> _blastVec;	

public:
	BlastRect(float x, float y, float w, float h);
	~BlastRect();
	
	void Reset();
	void SetColor(unsigned int color);
	void Draw(AddData* addData);
};

#endif