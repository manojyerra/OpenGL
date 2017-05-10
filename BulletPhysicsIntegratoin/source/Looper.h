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
#include "Floor.h"

#include "Phy/PhyBox.h"
#include "Phy/PhyManager.h"
#include "Shape2D/Rect.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	Rect _rect;

	Pointer3D _pointer3D;

	GL2DState state2D;

	ModelsManager* _modelsMgr;
	Shape* shape;

	MainFrame* _mainFrame;
	ModelPropsFrame* _modelPropsFrame;

	Floor* _floor;

	PhyBox* _floorBox;
	PhyBox* _phyBox;

	bool SelectModel(int mx, int my);
	void UpdateDrawRect();
	void UpdatePhysics(float deltaTime);

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Draw(float deltaTime);
};

#endif