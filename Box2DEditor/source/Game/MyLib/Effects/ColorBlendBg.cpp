#include "ColorBlendBg.h"
#include "GameInfo.h"

ColorBlendBg::ColorBlendBg()
{
	GameInfo* gameInfo = GameInfo::GetInstance();
	float ow = GameInfo::GetInstance()->CodedW();
	float oh = GameInfo::GetInstance()->CodedH();

	_addData = new AddData(512, false, false, false);
	_rectAddData = new AddData(512, true, false, false);
		
	float initX = 0;
	float initY = 0;
		
	float numCols = 7;
	
	float gridW = ow / numCols;
	float gridH = gridW;
	
	float numRows = (int)(oh/gridH) + 1;
		
	for(int y=0; y< numRows; y++)
	{
		for(int x=0; x< numCols; x++)
		{
			_colorRectVec.push_back( ColorRect(initX+x*gridW, initY+y*gridH, gridW, gridH) );
		}
	}
	
	_addData->glBegin(GL_LINES);

	for(int i=0; i<= numRows; i++)
	{
		_addData->glVertex3f(initX, 				initY+(i*gridH), 0);
		_addData->glVertex3f(initX + gridW*numCols,	initY+(i*gridH), 0);
	}

	for(int i=0; i<= numCols; i++)
	{
		_addData->glVertex3f(initX+(i*gridW), initY, 0);
		_addData->glVertex3f(initX+(i*gridW), initY + gridH*numRows, 0);
	}
}

void ColorBlendBg::Update()
{
}

void ColorBlendBg::Draw()
{
	_rectAddData->glBegin(GL_TRIANGLES);
	
	for(int i=0;i<_colorRectVec.size();i++)
	{
		_colorRectVec[i].Draw(_rectAddData);
	}

	_rectAddData->glEnd();

	glLineWidth(3.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
	_addData->glEnd();
}


ColorBlendBg::~ColorBlendBg()
{
	if(_addData)
	{
		delete _addData;
		_addData = NULL;
	}

	if(_rectAddData)
	{
		delete _rectAddData;
		_rectAddData = NULL;
	}
}

