#ifndef Looper_H
#define Looper_H

#include "ModelsManager.h"

#include "Shapes3D/Box.h"
#include "Shapes3D/Cone.h"
#include "Shapes3D/Cylinder.h"
#include "Shapes3D/Sphere.h"

#include "SUI/SUI.h"
#include "SUI/SUIInput.h"
#include "SUI/SUIActionListener.h"

#include "MainFrame.h"
#include "ModelPropsFrame.h"

class Looper //: SUIActionListener
{
private:
	float _windowW;
	float _windowH;

	float _rx;
	float _ry;
	float _rw;
	float _rh;

	ModelsManager* _modelsMgr;
	FLModel* flModel;

	MainFrame* _mainFrame;
	ModelPropsFrame* _modelPropsFrame;

	void SelectModel(int mx, int my);

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void UpdateDrawRect();
	void Draw();

	//void actionPerformed(SUIActionEvent SUIActionEvent);
};

#endif