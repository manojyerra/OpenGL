#include "GameManager.h"
#include "SaveManager.h"
#include "UIFont.h"
#include "Input.h"
#include "FileReader.h"
#include "GameInfo.h"
#include "Includes.h"
#include "Dir.h"
#include "Platform.h"
#include "SoundManager.h"
#include "ModuleManager.h"

GameManager::GameManager()
{
	int ow = GameInfo::GetInstance()->CodedW();
	int oh = GameInfo::GetInstance()->CodedH();

	_playPart = new PlayPart(SaveManager::GetInstance()->GetGameMode());
	_afterLevelOver = new AfterLevelOver();
	_leaderBoard = new LeaderBoard();

	int btnW = 170;
	int btnH = 65;

	_homeBtn = new ColorRect(10,3,btnW,btnH);
	_replayBtn = new ColorRect((ow-btnW)/2,3,btnW,btnH);
	_lbBtn = new ColorRect(ow-btnW-10,3,btnW,btnH);
}

void GameManager::Update()
{
	if(_afterLevelOver && _afterLevelOver->IsActive())
	{
		_afterLevelOver->Update();
	}
	else if(_leaderBoard && _leaderBoard->IsActive())
	{
		_leaderBoard->Update();
	}
	else
	{
		if(_homeBtn->IsClicked())
		{
			SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
			PagesManager::GetInstance()->SetCurrPage(PagesManager::MAINMENU);
			ModuleManager::GetInstance()->SetModule(ModuleManager::PAGES);
		}
		else if(_replayBtn->IsClicked())
		{
			SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
			ModuleManager::GetInstance()->SetModule(ModuleManager::GAME);

			GameInfo::GetInstance()->SetTimeToShowAd(true);

			SaveManager::GetInstance()->SetALO( SaveManager::GetInstance()->GetALO() + 1 );
			SaveManager::GetInstance()->SaveGame();
		}
		else if(_lbBtn->IsClicked())
		{
			SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
			_leaderBoard->SetActive(true);
		}
		else
		{
			_playPart->Update();

			if(_playPart->IsGameOver())
			{
				_afterLevelOver->SetCurrScore( _playPart->GetScore() );
				_afterLevelOver->SetActive(true);
			}
		}
	}
}

void GameManager::Draw()
{
	if(_afterLevelOver && _afterLevelOver->IsActive())
	{
		_afterLevelOver->Draw();
	}
	else if(_leaderBoard && _leaderBoard->IsActive())
	{
		_leaderBoard->Draw();
	}
	else
	{
		_playPart->Draw();

		_homeBtn->Draw(2, 0xffffffff, "Home", 40, 0xffffffff);
		_replayBtn->Draw(2, 0xffffffff, "Replay", 40, 0xffffffff);
		_lbBtn->Draw(2, 0xffffffff, "Scores", 40, 0xffffffff);		
	}
}


GameManager::~GameManager()
{
	if(_afterLevelOver)
	{
		delete _afterLevelOver;
		_afterLevelOver = NULL;
	}

	if(_leaderBoard)
	{
		delete _leaderBoard;
		_leaderBoard = NULL;
	}

	if(_homeBtn)
	{
		delete _homeBtn;
		_homeBtn = NULL;
	}
	
	if(_replayBtn)
	{
		delete _replayBtn;
		_replayBtn = NULL;
	}

	if(_lbBtn)
	{
		delete _lbBtn;
		_lbBtn = NULL;
	}	

	if(_playPart)
	{
		delete _playPart;
		_playPart = NULL;
	}
}
