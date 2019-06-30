#ifndef Looper_H
#define Looper_H

#include "ObjToFLM.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	ObjToFLM* _objToFLM;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void Draw();
};

#endif