#ifndef Looper_H
#define Looper_H

#include "ModelsManager.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	ModelsManager* _modelsMgr;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void Draw();
};

#endif