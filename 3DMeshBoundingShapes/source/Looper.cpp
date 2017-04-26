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

	rx = ry = rw = rh = 0;

	_modelsMgr = new ModelsManager();

	flModel = _modelsMgr->Add("data/barrel", CVector3(0,0,0), CVector3(0, 0, 0)); 

	//_box = new Box(0,0,0, 2,1,2);
	//_cone = new Cone(0,0,0, 3,2);
	//_cylinder = new Cylinder(0,0,-20, 2, 4);
	//_sphere = new Sphere(0,0,-30, 2);

	//Box box = Box::CalcBoundingBox(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	//_box->SetGLMatrix(box.GetGLMatrix());
	//_box->SetSize(box.GetSize().x, box.GetSize().y, box.GetSize().z);

	//Sphere sphere = Sphere::CalcBoundingSphere(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	//_sphere->SetGLMatrix(sphere.GetGLMatrix());
	//_sphere->SetRadius(sphere.GetRadius());

	//Cone cone = Cone::CalcBoundingCone(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	//_cone->SetGLMatrix(cone.GetGLMatrix());
	//_cone->SetRadius(cone.GetRadius());
	//_cone->SetHeight(cone.GetHeight());

	//Cylinder cylinder = Cylinder::CalcBoundingCylinder(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
	//_cylinder->SetGLMatrix(cylinder.GetGLMatrix());
	//_cylinder->SetRadius(cylinder.GetRadius());
	//_cylinder->SetHeight(cylinder.GetHeight());

	//shape = Shape::GetBestFitBoundingShape(flModel->GetVerticesPointer(), flModel->GetNumVertices()*3);
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

	Floor::Draw();

	flModel->Draw();

	UpdateDrawRect();

	if(Input::IsKeyReleased((int)'A'))
	{
		if(rw > 0 && rh > 0)
			flModel->AddBoudingShapeByVerticesOnRect(rx, ry, rw, rh);
	}

	GLMat modelMat = glUtil::GetModelViewMatrix();
	GLMat projMat = glUtil::GetProjectionMatrix();
	bool depthTest = glUtil::GLEnable(GL_DEPTH_TEST, false);
	bool lighting = glUtil::GLEnable(GL_LIGHTING, false);
	bool blend = glUtil::GLEnable(GL_BLEND, false);
	unsigned int prevColor = glUtil::GLColor(0xff0000aa);
	GLfloat prevLineWidth = glUtil::GLLineWidth(2);

	GLUtil::Begin2DDraw();

	glBegin(GL_LINE_LOOP);
	glVertex2f(rx, ry);
	glVertex2f(rx+rw, ry);
	glVertex2f(rx+rw, ry+rh);
	glVertex2f(rx, ry+rh);
	glEnd();

	glUtil::GLLineWidth(prevLineWidth);
	glUtil::GLColor(prevColor);
	glUtil::GLEnable(GL_BLEND, blend);
	glUtil::GLEnable(GL_LIGHTING, depthTest);
	glUtil::SetProjectionMatrix(projMat);
	glUtil::SetModelViewMatrix(modelMat);
	glUtil::GLEnable(GL_DEPTH_TEST, depthTest);
}

void Looper::UpdateDrawRect()
{
	if(Input::IsMouseClicked() || Input::IsRightMouseClicked())
	{
		//rect = Rect();
		rx = ry = rw = rh = 0;
	}
	else if(Input::IsKeyPressed((int)'D') && Input::IsMousePressed())
	{
		float x1 = Input::MouseClickX;
		float y1 = Input::MouseClickY;
		float x2 = Input::MX;
		float y2 = Input::MY;

		if(x1 > x2)
		{
			x1 = Input::MX;
			x2 = Input::MouseClickX;
		}

		if(y1 > y2)
		{
			y1 = Input::MY;
			y2 = Input::MouseClickY;
		}

		//rect = Rect(x1, y1, x2-x1, y2-y1);
		rx = x1;
		ry = y1;
		rw = x2-x1;
		rh = y2-y1;
	}
}

Looper::~Looper()
{
	if(_modelsMgr)
	{
		delete _modelsMgr;
		_modelsMgr = NULL;
	}

	if(shape)
	{
		delete shape;
		shape = NULL;
	}
}
