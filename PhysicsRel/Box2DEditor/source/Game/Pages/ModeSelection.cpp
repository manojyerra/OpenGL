#include "ModeSelection.h"
#include "SaveManager.h"
#include "ModuleManager.h"
#include "PagesManager.h"
#include "GameInfo.h"
#include "Input.h"
#include "Platform.h"
#include "UIFont.h"

ModeSelection::ModeSelection()
{
	GameInfo* gameInfo = GameInfo::GetInstance();

	int sw = gameInfo->CodedW();

	_proModeBtn = NULL;
	_surModeBtn = NULL;

	int y = 90;
	int w = 736;
	int h = 164;
	int gap = 90;

	int btnNum = 0;

	/*
	_proModeBtn = new Button("ui/promode.png","ui/promode.png", (sw-w)/2.0f, y+btnNum*(h+gap), w, h);		btnNum++;
	_surModeBtn = new Button("ui/surmode.png", "ui/surmode.png", (sw-w)/2.0f, y+btnNum*(h+gap), w, h);		btnNum++;
	_backBtn = new Button("ui/back.png", "ui/back.png", 0,0,110,110);

	_cBlendBg = new ColorBlendBg();
	*/

	y = 250;
	w = 420;
	h = 130;
	gap = 125;

	_proModeBtn = new ColorRect((sw-w)/2.0f, y+btnNum*(h+gap), w, h);		btnNum++;
	_surModeBtn = new ColorRect((sw-w)/2.0f, y+btnNum*(h+gap), w, h);		btnNum++;
	_backBtn = new ColorRect(0,0,170,70);

	_cBlendBg = new BlendTransBg();
}

void ModeSelection::Update()
{
	PagesManager* pagesMgr = PagesManager::GetInstance();

	//if(_proModeBtn)			_proModeBtn->Update();
	//if(_surModeBtn)			_surModeBtn->Update();
	//if(_backBtn)			_backBtn->Update();

	if(_proModeBtn && _proModeBtn->IsClicked())
	{
		SaveManager::GetInstance()->SetGameMode( SaveManager::STRATEGY_MODE);

		ModuleManager::GetInstance()->SetModule(ModuleManager::GAME);
	}
	else if(_surModeBtn && _surModeBtn->IsClicked())
	{
		SaveManager::GetInstance()->SetGameMode( SaveManager::TIMER_MODE);

		ModuleManager::GetInstance()->SetModule(ModuleManager::GAME);
	}
	else if(_backBtn->IsClicked() || Input::IsBackPressed())
	{
		pagesMgr->SetCurrPage(PagesManager::MAINMENU);
	}
}

void ModeSelection::Draw()
{
	_cBlendBg->Draw();

	if(_proModeBtn)			_proModeBtn->Draw(4, 0xffffffff, "Strategy Mode", 50, 0xffffffff);
	if(_surModeBtn)			_surModeBtn->Draw(4, 0xffffffff, "Timer Mode", 50, 0xffffffff);
	if(_backBtn)			_backBtn->Draw(2, 0xffffffff, "Back", 40, 0xffffffff);
}

ModeSelection::~ModeSelection()
{
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

	if(_backBtn)
	{
		delete _backBtn;
		_backBtn = NULL;
	}
	
	if(_cBlendBg)
	{
		delete _cBlendBg;
		_cBlendBg = NULL;
	}
}
