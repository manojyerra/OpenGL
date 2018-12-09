#ifndef AFTERLEVELOVER_H
#define AFTERLEVELOVER_H
#pragma once

#include "AnimSprite.h"
#include "Sprite.h"
#include "Button.h"
#include "LeaderBoard.h"
#include "MsgBox.h"
#include "BlendTransBg.h"
#include "ColorBlendBg.h"

class AfterLevelOver
{
private:
	BlendTransBg* _bg;
	
	ColorRect* _resultBg;
	
	ColorRect* _shareBtn;
	ColorRect* _rateBtn;
	ColorRect* _leaderBoardBtn;

	ColorRect* _homeBtn;
	ColorRect* _replayBtn;

	LeaderBoard* _leaderBoard;
	
	bool _isActive;
	int _currScore;
	int _rank;
	
	int	_frameCount;
	bool _activateLBSudden;

	void ShowRankImproveMsg(int rank, int prevRank);
	void DrawStatus();

	//Begin : RateMsgBox
	MsgBox* _rateMsgBox;
	ColorRect* _rateMsgBg;
	ColorRect* _yesBtn;
	ColorRect* _laterBtn;

	void RateDialogInit();
	void DrawRateDialog();
	void UpdateRateDialog();
	//End : RateMsgBox

public:
	AfterLevelOver();
	~AfterLevelOver();

	void Update();
	void Draw();
	bool IsActive();
	void SetActive(bool isActive);
	void SetCurrScore(int score);
};
#endif