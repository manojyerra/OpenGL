#include "ModeSelection_High.h"
#include "SaveManager.h"
#include "ModuleManager.h"
#include "PagesManager.h"
#include "GameInfo.h"
#include "Input.h"
#include "Platform.h"

ModeSelection_High::ModeSelection_High()
{
	GameInfo* gameInfo = GameInfo::GetInstance();

	int sw = gameInfo->CodedW();

	_proModeBtn = NULL;
	_surModeBtn = NULL;

	int y = 115;
	int w = 472;
	int h = 87;
	int gap = 60;

	int btnNum = 0;

	_proModeBtn = new Button("ui/probtn.png","ui/probtn.png", (sw-w)/2.0f, y+btnNum*(h+gap), w, h);		btnNum++;
	_surModeBtn = new Button("ui/surbtn.png","ui/surbtn.png", (sw-w)/2.0f, y+btnNum*(h+gap), w, h);		btnNum++;
	_backBtn = new Button("ui/back.png", "ui/back.png", 0,0,110,110);

	_proModeBtn->SetWaveOn(true);
	_surModeBtn->SetWaveOn(true);

	_leaderBoard = new LeaderBoard();
	_leaderBoard->SetActive(true);
	
	_cBlendBg = new ColorBlendBg();
}

void ModeSelection_High::Update()
{
	if(_leaderBoard->IsActive())
	{
		_leaderBoard->Update();
	}
	else
	{
		_leaderBoard->SetActive(true);
		
		PagesManager::GetInstance()->SetCurrPage(PagesManager::MAINMENU);

		/*
		PagesManager* pagesMgr = PagesManager::GetInstance();

		if(_proModeBtn)			_proModeBtn->Update();
		if(_surModeBtn)			_surModeBtn->Update();
		if(_backBtn)			_backBtn->Update();

		if(_proModeBtn && _proModeBtn->IsClicked())
		{
			SaveManager::GetInstance()->SetGameMode( SaveManager::STRATEGY_MODE);
			_leaderBoard->SetActive(true);
		}
		else if(_surModeBtn && _surModeBtn->IsClicked())
		{
			SaveManager::GetInstance()->SetGameMode( SaveManager::TIMER_MODE);
			_leaderBoard->SetActive(true);
		}
		else if(_backBtn->IsClicked() || Input::IsBackPressed())
		{
			if(_leaderBoard->IsActive())
				_leaderBoard->SetActive(false);
			else
				pagesMgr->SetCurrPage(PagesManager::MAINMENU);
		}
		*/
	}
}

void ModeSelection_High::Draw()
{
	if(_leaderBoard->IsActive())
	{
		_leaderBoard->Draw();
	}
	/*
	else
	{
		_cBlendBg->Draw();

		if(_proModeBtn)			_proModeBtn->Draw();
		if(_surModeBtn)			_surModeBtn->Draw();
		if(_backBtn)			_backBtn->Draw();
	}
	*/
}

ModeSelection_High::~ModeSelection_High()
{
	if(_cBlendBg)
	{
		delete _cBlendBg;
		_cBlendBg = NULL;
	}

	if(_backBtn)
	{
		delete _backBtn;
		_backBtn = NULL;
	}

	if(_proModeBtn)
	{
		delete _proModeBtn;
		_proModeBtn = NULL;
	}

	if(_surModeBtn)
	{
		delete _surModeBtn;
		_surModeBtn = NULL;
	}

	if(_leaderBoard)
	{
		delete _leaderBoard;
		_leaderBoard = NULL;
	}
}
