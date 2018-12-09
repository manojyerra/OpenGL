#include "GameLoop.h"
#include "Platform.h"
#include "Includes.h"
#include "PNGReadWrite.h"
#include "Input.h"
#include "ModuleManager.h"
#include "TextureManager.h"
#include "GameInfo.h"
#include "LeaderBoardData.h"
#include "SaveManager.h"
#include "UIFont.h"
#include "Flags.h"

GameLoop::GameLoop()
{
	//Begin : Common to all games.
	_frameCountForFPS = 0;
	_timeCountForFPS = 0;
	_fps = 0;
	//End : Common to all games.

	_init = false;
	_onResume = false;
	_bgmBtn = NULL;
	GameInfo::GetInstance()->SetCodedSize(1024, 600);

	
}

void GameLoop::Init()
{
	_gameInit = new GameInit();

	_editor = new Editor();

	_init = true;
}

void GameLoop::onSizeChange(int surfaceW, int surfaceH)
{
	GameInfo::GetInstance()->SetSurfaceSize(surfaceW, surfaceH);
	GameInfo::GetInstance()->CalcDrawArea();
}

void GameLoop::Update(int deltaTimeInMillis)
{
	GameInfo::deltaTime = deltaTimeInMillis/1000.0f;
	Input::Update();
	//ModuleManager::GetInstance()->Update();
	_editor->Update(GameInfo::deltaTime);
}

int GameLoop::Draw(int deltaTimeInMillis)
{
	if(_onResume)
	{
		_onResume = false;
		TextureManager::GetInstance()->DeleteAllTextures();
		TextureManager::GetInstance()->CreateAllTextures();
	}

	if(_init == false)
	{
		Init();
		return 0;
	}

	Update(deltaTimeInMillis);

	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	GameInfo* gameInfo = GameInfo::GetInstance();

	glViewport(gameInfo->drawX, gameInfo->drawY, gameInfo->drawW, gameInfo->drawH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, gameInfo->CodedW(), gameInfo->CodedH(), 0, -1, 1);

	//ModuleManager::GetInstance()->Draw();
	_editor->Draw();

	//RectPrim::Draw(Input::MouseX, Input::MouseY, 3, 4, 255, 0,0,255, 255, 0,0,255);

	if(GameInfo::GetInstance()->IsLeaderBoardOn() == false)
	{
		if(_bgmBtn)
			_bgmBtn->Draw();
	}
	
	//DisplayFPS(deltaTimeInMillis);

	if(ModuleManager::GetInstance()->GetModule() == ModuleManager::PAGES)
	{
		PagesManager* pages = (PagesManager*)(ModuleManager::GetInstance()->GetModuleObject(ModuleManager::PAGES));
		
		if(pages->GetCurrPageID() == PagesManager::MAINMENU)
		{
			MainMenu* mainMenu = (MainMenu*)pages->GetPage(PagesManager::MAINMENU);

			if(mainMenu->IsSubmitDetailsBtnClicked())
				return 3;
			else if(mainMenu->IsReviewBtnClicked())
				return 4;
			else if(mainMenu->IsShareOnGPlusBtnClicked())
				return 5;
			else if(mainMenu->IsExitClicked())
				return 9;
		}
	}

	if(GameInfo::GetInstance()->IsSubmitDetailsBtnClicked())
	{
		GameInfo::GetInstance()->SetSubmitClicked(false);
		return 3;
	}
	else if(GameInfo::GetInstance()->IsShareClicked())
	{
		GameInfo::GetInstance()->SetShareClicked(false);
		return 5;
	}
	else if(GameInfo::GetInstance()->IsReviewClicked())
	{
		GameInfo::GetInstance()->SetReviewClicked(false);
		return 4;
	}
	else if(GameInfo::GetInstance()->IsTimeToShowAd())
	{
		GameInfo::GetInstance()->SetTimeToShowAd(false);
		return 8;
	}

	return 0;
}

void GameLoop::DisplayFPS(int deltaTimeInMillis)
{
	_frameCountForFPS++;
	_timeCountForFPS += deltaTimeInMillis;
	
	if(_timeCountForFPS >= 250)
	{
		_fps = (int) (1000.0f * (float)_frameCountForFPS / (float)_timeCountForFPS);
		//printLog("\nFPS : %d, _timeForFPS = %f, _frameCountForFPS = %d", fps, _timeInMillis, _frameCountForFPS);
		_frameCountForFPS = 0;
		_timeCountForFPS = 0;
	}

	char fpsArr[16];
	sprintf(fpsArr, "FPS : %d", _fps);
	
	UIFont::GetInstance()->SetColor(0xffffffff);
	UIFont::GetInstance()->Begin(1);
	UIFont::GetInstance()->Draw(fpsArr, 50, 50, 35, 0, 0);
	UIFont::GetInstance()->End();
}

void GameLoop::onTouch(int x, int y, int eventID)
{
	Input::onTouch(x, y, eventID);
}

void GameLoop::onPause()
{
}

void GameLoop::onResume()
{
	_onResume = true;
}

void GameLoop::SetLanguageID(int languageID)
{
	GameInfo::GetInstance()->SetLanguage(languageID);
}

void GameLoop::SetBackKeyPressed()
{
	Input::SetBackKeyPressed();
}

void GameLoop::SetAccMeterMoved(int dir)
{
	Input::SetAccMeterMoved(dir);
}

GameLoop::~GameLoop()
{
	if(_bgmBtn)
	{
		delete _bgmBtn;
		_bgmBtn = NULL;
	}

	if(_gameInit)
	{
		delete _gameInit;
		_gameInit = NULL;
	}

	if(_editor)
	{
		delete _editor;
		_editor = NULL;
	}

	Input::ClearTouches();
}

