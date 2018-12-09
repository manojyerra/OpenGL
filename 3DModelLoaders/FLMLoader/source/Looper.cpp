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

	_flModel = new FLModel("data/cat");
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

	GLfloat qaLightPos[]	= {0, 0, 0, 1.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPos);

	util::SetModelViewMatrix();
	util::UpdateCamera();

	Floor::Draw();
	
	GLfloat qaGreen[] = {0.4, 0.8, 0.4, 1.0};
	GLfloat qaWhite[] = {0.1, 0.1, 0.1, 0.0};
	GLfloat qaBlack[] = {0.3, 0.3, 0.3, 1.0};

	GLfloat Ka[] = { 0.600000, 0.600000, 0.600000, 1.0};
	GLfloat Kd[] = { 0.600000, 0.600000, 0.600000, 1.0};
	GLfloat Ks[] = { 0.900000, 0.900000, 0.900000, 1.0};

	glShadeModel( GL_SMOOTH );
	//glMaterialfv(GL_FRONT, GL_AMBIENT, qaBlack);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);

	glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);

	glMaterialf(GL_FRONT, GL_SHININESS, 1.0);

	_flModel->Draw();
}

Looper::~Looper()
{
	if(_flModel)
	{
		delete _flModel;
	}
}
