#ifndef GAMEINIT_H
#define GAMEINIT_H
#pragma once

class GameInit
{
private:
	unsigned int _startTime;
public:
	GameInit();
	~GameInit();
	void InitGLSettings();
};

#endif

