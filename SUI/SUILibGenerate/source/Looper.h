#ifndef Looper_H
#define Looper_H

#include "SUIFrame.h"
#include "SUIButton.h"
#include "SUILifyCycle.h"
using namespace SUI;


class Looper
{
private:
	float _windowW;
	float _windowH;

	SUIFrame* _suiFrame;
	SUIButton* _suiButton;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void Draw();
};

#endif