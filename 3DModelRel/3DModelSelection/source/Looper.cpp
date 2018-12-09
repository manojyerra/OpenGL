#include "Looper.h"
#include "Input.h"
#include "GLAndUtil.h"
#include "Floor.h"

#include "math.h"
#include <windows.h>
#include <gl/gl.h>

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = windowWidth;
	_windowH = windowHeight;

	GLAndUtil::Init(_windowW, _windowH);

	_modelsMgr = new ModelsManager();

	for(int j=0; j<100; j+=25)
	{
		for(int i=0; i<100; i+=25)
		{
			if(rand()%2 == 0)
				_modelsMgr->Add("data/cat", -40+i, 0, -40+j);
			else
				_modelsMgr->Add("data/barrel", -40+i, 0, -40+j);
		}
	}
}

void Looper::Update(float deltaTime)
{
}


void Looper::Draw()
{
	util::ClearColor(0.6, 0.6, 0.6, 1);
	util::Clear();
	util::Begin3DDraw();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat qaLightPos[] = {0, 0, 0, 1.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPos);

	util::SetModelViewMatrix();
	util::UpdateCamera();

	if(Input::IsMouseClicked())
	{
		_modelsMgr->DrawForSelection();
		
		GLubyte data[4];
		glReadPixels(Input::MX, util::GetWindowHeight()-Input::MY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		unsigned int colorVal = (unsigned int)((data[0]<<24) + (data[1]<<16) + (data[2]<<8) + data[3]);

		_modelsMgr->SetBoundingBoxEnabled(false);

		if(colorVal < _modelsMgr->Size())
		{
			FLModel* model = _modelsMgr->Get(colorVal);
			model->SetBoundingBoxEnabled(true);
		}

		util::Clear();
	}

	Floor::Draw();
	_modelsMgr->Draw();
}

Looper::~Looper()
{
	if(_modelsMgr)
	{
		delete _modelsMgr;
		_modelsMgr = NULL;
	}
}
