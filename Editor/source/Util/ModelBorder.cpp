#include "ModelBorder.h"
#include "AppInfo.h"

ModelBorder::ModelBorder()
{
	_r = 0;
	_g = 0;
	_b = 0;
	_a = 0;
}

ModelBorder::~ModelBorder()
{
}

void ModelBorder::SetModelColor(unsigned int color)
{
	_r = (color >> 24) & 255;
	_g = (color >> 16) & 255;
	_b = (color >> 8) & 255;
	_a = (color) & 255;
}

void ModelBorder::SetModelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void ModelBorder::CalcBorder(Model3D* model3D)
{
	AppInfo* appInfo = AppInfo::GetInstance();

	int width = appInfo->viewW;
	int height = appInfo->viewH;

	x.clear();
	y.clear();

	GLfloat clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
	bool glDepthTest = glIsEnabled(GL_DEPTH_TEST);
	bool glLighting = glIsEnabled(GL_LIGHTING);
	bool glBlend = glIsEnabled(GL_BLEND);

	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	bool drawTexture = model3D->IsTextureDrawing();
	bool wireFrame = model3D->IsInWireFrame();

	model3D->SetColor(0,0,0,255);
	model3D->SetDrawTexture(false);
	model3D->EnableWireFrame(false);
	model3D->Draw();

	model3D->SetDrawTexture(drawTexture);
	model3D->EnableWireFrame(wireFrame);

    GLubyte* data = (GLubyte*)malloc(width*height*4);
	glReadPixels(appInfo->viewX, appInfo->viewY, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	if(glDepthTest)	glEnable(GL_DEPTH_TEST);	else glDisable(GL_DEPTH_TEST);
	if(glLighting)	glEnable(GL_LIGHTING);		else glDisable(GL_LIGHTING);
	if(glBlend)		glEnable(GL_BLEND);			else glDisable(GL_BLEND);

	int prevVal = 0;

	for(int j=0;j<height;j++)
	{
		for(int i=0;i<width;i++)
		{
			int pos = ((j*width) + i) * 4;

			if(i != 0 && data[pos] != prevVal)
			{
				x.push_back(i);
				y.push_back(j);
			}
			prevVal = data[pos];
		}
	}

	prevVal = 0;

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			int pos = ((j*width) + i) * 4;

			if(j != 0 && data[pos] != prevVal)
			{
				x.push_back(i);
				y.push_back(j);
			}
			prevVal = data[pos];
		}
	}

	free(data);
}

void ModelBorder::DrawBorder()
{
	AppInfo* appInfo = AppInfo::GetInstance();

	int screenW = appInfo->viewW;
	int screenH = appInfo->viewH;

	float pointSize = 1.0f;
	float projection[16];
	float modelView[16];
	
	bool glDepthTest = glIsEnabled(GL_DEPTH_TEST);
	bool glLighting = glIsEnabled(GL_LIGHTING);

	glGetFloatv(GL_POINT_SIZE, &pointSize);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenW, screenH, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPointSize(1.0f);
	glDisable(GL_POINT_SMOOTH);

	glColor4f(1.0f,0.0f,0.0f,1.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<x.size();i++)
		glVertex2f(x[i], screenH-y[i]);
	glEnd();

	if(glDepthTest) glEnable(GL_DEPTH_TEST);	else glDisable(GL_DEPTH_TEST);
	if(glLighting)	glEnable(GL_LIGHTING);		else glDisable(GL_LIGHTING);
	glPointSize(pointSize);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(projection);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(modelView);
}