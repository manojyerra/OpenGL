#ifndef GAME_INFO
#define GAME_INFO
#pragma once

class GameInfo
{
private:
	static GameInfo* _ref;
	GameInfo();
	~GameInfo();

public:
	static float deltaTime;
	static GameInfo* GetInstance();

	void Init();
	void Update();
	void Terminate();
};

#endif
