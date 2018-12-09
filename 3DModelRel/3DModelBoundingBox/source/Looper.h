#ifndef Looper_H
#define Looper_H

#include "FLModel.h"

class Looper
{
private:
	float _windowW;
	float _windowH;

	FLModel* _flModel;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void Draw();
};

#endif