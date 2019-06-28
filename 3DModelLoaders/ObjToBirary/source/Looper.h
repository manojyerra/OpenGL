#ifndef Looper_H
#define Looper_H

#include "ObjToBinary.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	ObjToBinary* _objToBinary;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void Draw();
};

#endif