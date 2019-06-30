#ifndef Looper_H
#define Looper_H

#include "ObjToFLMNew.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	ObjToFLMNew* _objToFLM;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void Draw();
};

#endif