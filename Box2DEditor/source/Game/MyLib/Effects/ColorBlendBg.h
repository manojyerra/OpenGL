#ifndef COLORBLENDBG_H
#define COLORBLENDBG_H
#pragma once

#include "ColorRect.h"
#include "AddData.h"

#include <vector>
using namespace std;

class ColorBlendBg
{
private:
	vector<ColorRect> _colorRectVec;
	
	AddData* _addData;
	AddData* _rectAddData;
		
public:
	ColorBlendBg();
	~ColorBlendBg();
	
	void Update();
	void Draw();
};

#endif
