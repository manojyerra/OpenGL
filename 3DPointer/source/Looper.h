#ifndef Looper_H
#define Looper_H

#include "ModelsManager.h"

#include "Shapes3D/Box.h"
#include "Shapes3D/Cone.h"
#include "Shapes3D/Cylinder.h"
#include "Shapes3D/Sphere.h"
#include "Shapes3D/Shape.h"

#include "SUI/SUI.h"
#include "SUI/SUIInput.h"
#include "SUI/SUIActionListener.h"

#include "MainFrame.h"
#include "ModelPropsFrame.h"
#include "Pointer3D.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	float _rx;
	float _ry;
	float _rw;
	float _rh;

	GL2DState state2D;

	ModelsManager* _modelsMgr;
	//FLModel* flModel;
	Shape* shape;

	MainFrame* _mainFrame;
	ModelPropsFrame* _modelPropsFrame;

	Pointer3D _pointer3D;

	void SelectModel(int mx, int my);

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void UpdateDrawRect();
	void Draw();
};

#endif