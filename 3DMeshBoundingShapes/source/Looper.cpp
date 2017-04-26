#include "Looper.h"
#include "Input.h"
#include "Floor.h"
#include "Util/GLUtil.h"
#include "Math/Vector3.h"
#include "Math/TransformVertexBuf.h"
#include "math.h"

#include <windows.h>
#include <gl/gl.h>

Looper::Looper(int windowWidth, int windowHeight)
{
	_windowW = windowWidth;
	_windowH = windowHeight;

	GLUtil::Init(_windowW, _windowH);

	_modelsMgr = new ModelsManager();

	flModel = _modelsMgr->Add("data/cat", CVector3(0,2,1), CVector3(-45, 30, -15)); 

	_box = new Box(0,0,0, 2,1,2);
	_cone = new Cone(0,0,0, 3,2);
	_cylinder = new Cylinder(0,0,-20, 2, 4);
	_sphere = new Sphere(0,0,-30, 2);

	Box box = Box::CalcBoundingBox(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	_box->SetGLMatrix(box.GetGLMatrix());
	_box->SetSize(box.GetSize().x, box.GetSize().y, box.GetSize().z);

	Sphere sphere = Sphere::CalcBoundingSphere(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	_sphere->SetGLMatrix(sphere.GetGLMatrix());
	_sphere->SetRadius(sphere.GetRadius());

	Cone cone = Cone::CalcBoundingCone(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	_cone->SetGLMatrix(cone.GetGLMatrix());
	_cone->SetRadius(cone.GetRadius());
	_cone->SetHeight(cone.GetHeight());

	Cylinder cylinder = Cylinder::CalcBoundingCylinder(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	_cylinder->SetGLMatrix(cylinder.GetGLMatrix());
	_cylinder->SetRadius(cylinder.GetRadius());
	_cylinder->SetHeight(cylinder.GetHeight());
}

void Looper::Update(float deltaTime)
{
}

void Looper::Draw()
{
	glUtil::ClearColor(0.6, 0.6, 0.6, 1);
	glUtil::Clear();
	glUtil::Begin3DDraw();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat qaLightPos[] = {0, 0, 0, 1.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPos);

	glUtil::SetModelViewMatrix();
	glUtil::UpdateCamera();

	//Floor::Draw();

	flModel->Draw();


	glPushMatrix();
	glMultMatrixf(flModel->GetMat().m);

	//_box->Draw();
	_cone->Draw();
	//_sphere->Draw();
	//_cylinder->Draw();

	glPopMatrix();
}

Looper::~Looper()
{
	if(_modelsMgr)
	{
		delete _modelsMgr;
		_modelsMgr = NULL;
	}
}
