#ifndef Looper_H
#define Looper_H

#include "ObjLoader.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	ObjLoader* _objLoader;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void Draw();
};

#endif