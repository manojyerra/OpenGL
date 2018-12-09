#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#pragma once

#include "RectPrim.h"
#include "Button.h"
#include "LeaderBoardData.h"
#include "BlendTransBg.h"
#include "ColorRect.h"

class LeaderBoard
{
private:
	BlendTransBg* _blendBg;
	
	ColorRect* _cRect1;
	ColorRect* _cRect2;

	Sprite* _cellBg;
	Sprite* _topBorder;
	Button* _backBtn;
	Sprite* _circle;
	Sprite* _defaultPic;
	LeaderBoardData* _lbData;

	int _heading;
	int _border;
	int _cellH;
	int _cellsStartY;

	int _showCells;

	bool _isActive;
	float _transY;
	float _speed;
	float _elapsedTime;
	float _speedCoff;
	float _lastScrollTime;

	int _offLineRank;

	void DrawCell(int x, int y, int w, int h, int index, int rank, bool isUser);
	
	void GenerateOffLineRank();
	int GetRealRank();

public:
	LeaderBoard();
	~LeaderBoard();
	void Update();
	void SetActive(bool isActive);
	bool IsActive();
	int GetRank();
	void Draw();
};

#endif