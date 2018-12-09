#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "PlayPart.h"
#include "LeaderBoard.h"
#include "AfterLevelOver.h"
#include "ColorRect.h"

class GameManager
{
private:
	AfterLevelOver* _afterLevelOver;
	LeaderBoard* _leaderBoard;
	PlayPart* _playPart;

	ColorRect* _homeBtn;
	ColorRect* _replayBtn;
	ColorRect* _lbBtn;
	
public:
	GameManager();
	~GameManager();
	void Update();
	void Draw();
};

#endif
