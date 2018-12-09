#ifndef BLENDTRANSBG_H
#define BLENDTRANSBG_H

#include "ColorRect.h"
#include <vector>
using namespace std;

class BlendTransBg
{
private:
	AddData* _addData;
	vector<ColorRect> _colorRectVec;
	
public:
	BlendTransBg();
	~BlendTransBg();
	
	void Draw();
};

#endif
