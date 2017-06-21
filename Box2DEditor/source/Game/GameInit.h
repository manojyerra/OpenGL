#ifndef GAMEINIT_H
#define GAMEINIT_H
#pragma once

#include "CommonLib/Sprite.h"

class GameInit
{
private:
	unsigned int _startTime;

	void Draw(Sprite* img);

public:
	GameInit();
	~GameInit();
	void InitGLSettings();
};

#endif

