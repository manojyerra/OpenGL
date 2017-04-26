#ifndef Looper_H
#define Looper_H

#include "ModelsManager.h"

#include "Shapes3D/Box.h"
#include "Shapes3D/Cone.h"
#include "Shapes3D/Cylinder.h"
#include "Shapes3D/Sphere.h"

#include "Sui/Sui.h"
#include "Sui/SuiInput.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	ModelsManager* _modelsMgr;
	FLModel* flModel;

	SuiFrame* _suiFrame;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void UpdateDrawRect();
	void Draw();
};

#endif