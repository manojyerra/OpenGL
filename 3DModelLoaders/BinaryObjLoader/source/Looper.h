#ifndef Looper_H
#define Looper_H

#include "BinaryObjLoader.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	BinaryObjLoader* _binaryObjLoader;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void Draw();
};

#endif