#ifndef PLAYPART_H
#define PLAYPART_H

#include "BlendTransBg.h"

class PlayPart
{
private:
	BlendTransBg* _bTransBg;
	
	bool _gameOver;
	
	bool _showGameOverText;
	float _showGOX;
	float _timeCount_ForGO;
	float _showLastBlast;
	
	int _score;
	float _playTimeCount;
	float _maxTime;
	bool _isTappedOnBig;
	bool _isTappedOnBig_temp;
	
	void DisplayScore();
	void SaveStatus();
	void RunGamePlay();

public:
	PlayPart(int gameMode);
	~PlayPart();

	void Update();
	void Draw();

	bool IsGameOver();
	int GetScore();
};

#endif