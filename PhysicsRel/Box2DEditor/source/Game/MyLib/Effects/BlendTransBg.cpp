#include "BlendTransBg.h"
#include "GameInfo.h"

BlendTransBg::BlendTransBg()
{
	float ow = GameInfo::GetInstance()->CodedW();
	float oh = GameInfo::GetInstance()->CodedH();
	
	_addData = new AddData(512, true, false, false);

	float totW = ow*1.8;
	
	float rectH = 86*1.25;
	float rows = 17;
	
	for(int y=-3; y<rows; y++)
	{
		_colorRectVec.push_back( ColorRect(-totW/2, y*rectH, totW*2, rectH) );
	}
} 

void BlendTransBg::Draw()
{
	float ow = GameInfo::GetInstance()->CodedW();
	float oh = GameInfo::GetInstance()->CodedH();

	static float bgRot = 0;
	static float rotFactor = 1;
	
	static float xScale = 1.0f;
	static float yScale = 1.0f;
	static float scaleFactor = -1.0f;
	
	bgRot += 0.03*rotFactor;
	
	if(bgRot > 360)
		rotFactor = -1;
	else if(bgRot < 0)
		rotFactor = 1;
	
	xScale += 0.001 * scaleFactor;
	yScale += 0.001 * scaleFactor;
	
	if(xScale < 0.75)
		scaleFactor = 1;
	else if(xScale > 1.1)
		scaleFactor = -1;
	
		
	glPushMatrix();

	glTranslatef(ow/2, oh/2, 0);
	glRotatef(bgRot, 0,0,1);
	glScalef(xScale,yScale,1);	
	glTranslatef(-ow/2, -oh/2, 0);
	
	_addData->glBegin(GL_TRIANGLES);
	
	for(int i=0;i<_colorRectVec.size();i++)
	{
		_colorRectVec[i].Draw(_addData);
	}

	_addData->glEnd();

	glPopMatrix();
}

BlendTransBg::~BlendTransBg()
{
	if(_addData)
	{
		delete _addData;
		_addData = NULL;
	}
}
