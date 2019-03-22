#include "GameLoop.h"

GameLoop::GameLoop(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	GLSettings();

	_zNear = 1.0f;
	_zFar = 10000.0f;
	_zNearPlaneW = 0.2f;

	_cam = Cam::GetInstance();
	_cam->Init(_sw, _sh, _zNear, _zFar, _zNearPlaneW);

	_floor = new Floor();

	CVector3 v1(0.0f, 0.0f, 0.0f);
	CVector3 v2(1.0f, 0.0f, 0.0f);
	CVector3 v3(1.0f, 1.0f, 0.0f);

	_triangle = new Triangle(v1, v2, v3);

	_box = new Box(0, 0, 0, 2, 3, 4);
	_box->SetSize(3, 1, 6);
	_box->SetPos(-10, 0, -10);

	_cylinder = new Cylinder(0, 0, 0, 3, 2);
	_cylinder->SetRadius(1.5);
	_cylinder->SetHeight(2);
	_cylinder->SetPos(0, 0, -10);

	_cone = new Cone(0, 0, 0, 2, 3);
	_cone->SetRadius(1.5);
	_cone->SetHeight(2);
	_cone->SetPos(-5, 0, -10);

	_sphere = new Sphere(0, 0, 0, 2);
	_sphere->SetPos(5, 0, 0);
	_sphere->SetRadius(5);

	_fbo = new GLFBO(_sw*0.7, _sh*0.7);
	_texture = new GLTexture(_sw*0.7, _sh*0.7);
}

void GameLoop::GLSettings()
{
	//unsigned int framebuffer;
	//glGenFramebuffersEXT(1, &framebuffer);
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

	glShadeModel(GL_SMOOTH);
	//glFrontFace	( GL_CCW		);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);

	glDisable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameLoop::Update(float deltaTime)
{

}

void GameLoop::Draw()
{
	_fbo->BindFBO();

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _fbo->GetW(), _fbo->GetH());
	//glEnable(GL_CULL_FACE);

	_cam->SetProjection();
	_cam->SetModelViewMatrix();
	_cam->UpdateCamera();

	_floor->Draw();
	_triangle->Draw();
	_box->Draw();
	_cylinder->Draw();
	_cone->Draw();
	_sphere->Draw();

	_fbo->UnBindFBO();
	

	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _sw, _sh);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, _sw, _sh, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, _fbo->GetTextureID());

	_texture->Draw();

	glBindTexture(GL_TEXTURE_2D, 0);
}

GameLoop::~GameLoop()
{
	if (_sphere != NULL)
	{
		delete _sphere;
		_sphere = NULL;
	}

	if (_cone != NULL)
	{
		delete _cone;
		_cone = NULL;
	}

	if (_cylinder != NULL)
	{
		delete _cylinder;
		_cylinder = NULL;
	}

	if (_box != NULL)
	{
		delete _box;
		_box = NULL;
	}

	if (_triangle != NULL)
	{
		delete _triangle;
		_triangle = NULL;
	}

	if (_floor != NULL)
	{
		delete _floor;
		_floor = NULL;
	}

	Cam::GetInstance()->DeleteInstance();
	ShadersManager::GetInstance()->DeleteInstance();
}
