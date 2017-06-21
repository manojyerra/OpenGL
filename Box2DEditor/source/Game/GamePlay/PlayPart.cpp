#include "PlayPart.h"
#include "GameInfo.h"
#include "UIFont.h"
#include "Input.h"
#include "SaveManager.h"
#include "SoundManager.h"

PlayPart::PlayPart(int gameMode)
{
	_playTimeCount = 0.0f;
	_score = 0;
	
	_showGameOverText = false;
	_showGOX = -100;
	_timeCount_ForGO = 0;
	_showLastBlast = false;
	_gameOver = false;
	_isTappedOnBig = false;
	_isTappedOnBig_temp = false;
	_maxTime = 10.1;

	_bTransBg = new BlendTransBg();

	float ow = GameInfo::GetInstance()->CodedW();
	float oh = GameInfo::GetInstance()->CodedH();
	
	float initX = 20;
	float initY = 215;
	float numCols = 4;
	float numRows = 5;
	
	float gridW = (ow-2*initX) / numCols;
	float gridH = gridW;
	
}

void PlayPart::Update()
{
	RunGamePlay();
}

void PlayPart::RunGamePlay()
{
	if(_showGameOverText)
	{
		_timeCount_ForGO += GameInfo::deltaTime;
		
		if(_timeCount_ForGO > 1.35)
		{
			_timeCount_ForGO = 0;
			_gameOver = true;
		}
	}
	else
	{
		if (_score > 400) _maxTime = 3.1;
		else if (_score > 200) _maxTime = 4.1;
		else if (_score > 100) _maxTime = 6.1;

		if(_playTimeCount >= _maxTime)
		{
			_showGameOverText = true;
			SaveStatus();
		}

		_playTimeCount += GameInfo::deltaTime;

		if(_isTappedOnBig)
		{
			_isTappedOnBig = false;
		}
		else if(_isTappedOnBig_temp)
		{
			_isTappedOnBig = true;
			_isTappedOnBig_temp = false;
		}
	}
}

bool PlayPart::IsGameOver()
{
	return _gameOver;
}

int PlayPart::GetScore()
{
	return _score;
}

void PlayPart::Draw()
{
	_bTransBg->Draw();

	DisplayScore();
	
	if(_showGameOverText)
	{
		float ow = GameInfo::GetInstance()->CodedW();
		float y = GameInfo::GetInstance()->CodedH()/2.0f;
		
		_showGOX += GameInfo::deltaTime * 600;
		
		if(_showGOX > ow/2)
			_showGOX = ow/2;
		
		UIFont::GetInstance()->SetColor(255,255,0,150);
		UIFont::GetInstance()->Begin(1);
		
		if(_playTimeCount >= 4)
		{
			UIFont::GetInstance()->DrawFromCenter("Time Up", _showGOX, y, 65, 0);
		}
		else
		{
			UIFont::GetInstance()->DrawFromCenter("Game Over", _showGOX, y, 65, 0);
		}
		UIFont::GetInstance()->End();
	}
}

void PlayPart::DisplayScore()
{
	char scoreArr[32];
	sprintf(scoreArr, "Score : %d", (int)_score);

	int timeLeft = (_maxTime - (int)_playTimeCount);

	if (timeLeft < 0)
		timeLeft = 0;

	char timeArr[32];
	sprintf(timeArr, "Time Left : %d", timeLeft);

	float ow = GameInfo::GetInstance()->CodedW();

	UIFont::GetInstance()->SetColor(255, 255, 255, 255);
	UIFont::GetInstance()->Begin(1);
	UIFont::GetInstance()->DrawFromCenter(scoreArr, ow - 150, 150, 38, 0);
	UIFont::GetInstance()->DrawFromCenter(timeArr, 150, 150, 38, 0);
	UIFont::GetInstance()->End();
}

void PlayPart::SaveStatus()
{
	SaveManager* saveMgr = SaveManager::GetInstance();

	if(saveMgr->GetScore() < GetScore())
		saveMgr->SetScore(GetScore());
	
	saveMgr->SetALO( saveMgr->GetALO() + 1 );
	saveMgr->SaveGame();
}

PlayPart::~PlayPart()
{
	if(_bTransBg)
	{
		delete _bTransBg;
		_bTransBg = NULL;
	}
}