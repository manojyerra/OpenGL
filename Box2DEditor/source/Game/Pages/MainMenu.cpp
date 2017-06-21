#include "MainMenu.h"
#include "SaveManager.h"
#include "ModuleManager.h"
#include "PagesManager.h"
#include "GameInfo.h"
#include "Input.h"
#include "Platform.h"
#include "SoundManager.h"
#include "Utils.h"
#include "UIFont.h"

MainMenu::MainMenu()
{
	GameInfo* gameInfo = GameInfo::GetInstance();
	float ow = GameInfo::GetInstance()->CodedW();
	float oh = GameInfo::GetInstance()->CodedH();

	int sw = gameInfo->CodedW();

	_playBtn = NULL;
	_highScoresBtn = NULL;
	_submitDetailsBtn = NULL;
	_reviewBtn = NULL;
	_shareOnGPlusBtn = NULL;
	_exitBtn = NULL;
	_musicOnOffBtn = NULL;
	_sfxOnOffBtn = NULL;

	int y = 215;
	int w = 200*1.9;
	int h = 41*1.9;
	int gap = 45;

	int btnNum = 0;

	_playBtn = new Button("ui/play.png","ui/play.png", (sw-w)/2.0f, y+btnNum*(h+gap), w, h);							btnNum++;
	_highScoresBtn = new Button("ui/highscores.png", "ui/highscores.png", (sw-w)/2.0f, y+btnNum*(h+gap), w, h);											btnNum++;
	_submitDetailsBtn = new Button("ui/editprofile.png", "ui/editprofile.png", (sw-w)/2.0f, y+btnNum*(h+gap), w, h);	btnNum++;
	_exitBtn = new Button("ui/exit.png", "ui/exit.png", (sw-w)/2.0f, y+btnNum*(h+gap), w, h);													btnNum++;
	
	gap = 2;
	w = 130*1.7;

	_reviewBtn = new Button("ui/rate.png", "ui/rate.png", 10, oh-20-h, w, h);			
	_shareOnGPlusBtn = new Button("ui/shareon.png", "ui/shareon.png", (sw-w-10), oh-20-h, w, h);			

	_musicOnOffBtn = new Button("ui/musicon.png", "ui/musicon.png", "ui/musicoff.png", 10, 10, 100, 100);
	_sfxOnOffBtn = new Button("ui/sfxon.png", "ui/sfxon.png", "ui/sfxoff.png", sw-100-10, 10, 100, 100);

	_musicOnOffBtn->SetEnable(SaveManager::GetInstance()->IsMusicOn());
	_sfxOnOffBtn->SetEnable(SaveManager::GetInstance()->IsSFXOn());

	_playBtn->SetWaveOn(true);
	_highScoresBtn->SetWaveOn(true);
	_submitDetailsBtn->SetWaveOn(true);
	_exitBtn->SetWaveOn(true);
	_reviewBtn->SetWaveOn(true);
	_shareOnGPlusBtn->SetWaveOn(true);
	
	_cBlendBg = new ColorBlendBg();
	
	RateDialogInit();
}

void MainMenu::Update()
{
	if(_rateMsgBox->IsActive())
	{
		UpdateRateDialog();
	}	
	else
	{
		PagesManager* pagesMgr = PagesManager::GetInstance();

		if(_playBtn)				_playBtn->Update();
		if(_highScoresBtn)			_highScoresBtn->Update();
		if(_submitDetailsBtn)		_submitDetailsBtn->Update();
		if(_reviewBtn)				_reviewBtn->Update();
		if(_shareOnGPlusBtn)		_shareOnGPlusBtn->Update();
		if(_exitBtn)				_exitBtn->Update();
		
		if(_musicOnOffBtn)			_musicOnOffBtn->Update();
		if(_sfxOnOffBtn)			_sfxOnOffBtn->Update();

		if(_playBtn && _playBtn->IsClicked())
		{
			SaveManager::GetInstance()->SetGameMode(SaveManager::STRATEGY_MODE);
			ModuleManager::GetInstance()->SetModule(ModuleManager::GAME);
		}
		else if(_highScoresBtn && _highScoresBtn->IsClicked())
		{
			SaveManager::GetInstance()->SetGameMode(SaveManager::STRATEGY_MODE);			
			PagesManager::GetInstance()->SetCurrPage(PagesManager::MODE_SELECTION_HIGH);			
		}
		else if(_musicOnOffBtn && _musicOnOffBtn->IsClicked())
		{
			_musicOnOffBtn->SetEnable(!_musicOnOffBtn->IsEnabled());
			//SoundManager::GetInstance()->SetMusicOn(_musicOnOffBtn->IsEnabled());
			
			GameInfo::gray = !_musicOnOffBtn->IsEnabled();
			TextureManager::GetInstance()->MakeGrayScale(GameInfo::gray);
		}
		else if(_sfxOnOffBtn && _sfxOnOffBtn->IsClicked())
		{
			_sfxOnOffBtn->SetEnable(!_sfxOnOffBtn->IsEnabled());
			SoundManager::GetInstance()->SetSFXOn(_sfxOnOffBtn->IsEnabled());			
		}
	}
}

bool MainMenu::IsReviewBtnClicked()
{
	return (_reviewBtn) ? _reviewBtn->IsClicked() : false;
}

bool MainMenu::IsShareOnGPlusBtnClicked()
{
	return (_shareOnGPlusBtn) ? _shareOnGPlusBtn->IsClicked() : false;
}

bool MainMenu::IsShareOnFacebookBtnClicked()
{
	//return (_shareOnFacebookBtn) ? _shareOnFacebookBtn->IsClicked() : false;
	return false;
}

bool MainMenu::IsSubmitDetailsBtnClicked()
{
	return (_submitDetailsBtn) ? _submitDetailsBtn->IsClicked() : false;
}

bool MainMenu::IsExitClicked()
{
	return (_exitBtn) ? _exitBtn->IsClicked() : false;
}

void MainMenu::Draw()
{
	_cBlendBg->Draw();
	
	int borderW = 2;
	unsigned int color = 0xffffffff;

	if(_playBtn)			_playBtn->Draw();
	if(_highScoresBtn)		_highScoresBtn->Draw();
	if(_submitDetailsBtn)	_submitDetailsBtn->Draw();
	if(_reviewBtn)			_reviewBtn->Draw();
	if(_shareOnGPlusBtn)	_shareOnGPlusBtn->Draw();
	if(_exitBtn)			_exitBtn->Draw();
	if(_musicOnOffBtn)		_musicOnOffBtn->Draw();
	if(_sfxOnOffBtn)		_sfxOnOffBtn->Draw();

	DrawRateDialog();
}

void MainMenu::RateDialogInit()
{
	GameInfo* gameInfo = GameInfo::GetInstance();
	float ow = GameInfo::GetInstance()->CodedW();
	float oh = GameInfo::GetInstance()->CodedH();

	_rateMsgBox = new MsgBox("Do you like this game?");
	_rateMsgBox->SetBgSize(460, 225);
	_rateMsgBox->SetActive(false);
	//_rateMsgBox->SetInActiveOnBackPress(true);
	_rateMsgBox->AddButton("Yes, I will");
	//_rateMsgBox->AddButton("Later");

	float boxW = ow*0.92;
	float boxH = boxW*0.85;

	_rateMsgBg = new ColorRect((ow - boxW) / 2, (oh - boxH) / 2, boxW, boxH);
	_rateMsgBg->SetColorTrans(false);
	_rateMsgBg->SetColor(0x5A9BFCFF);

	_yesBtn = new ColorRect(_rateMsgBg->GetX() + (boxW - 190 * 1.3) / 2, _rateMsgBg->GetY() + boxH - 120, 190 * 1.3, 65 * 1.3);
	_laterBtn = new ColorRect(_rateMsgBg->GetX() + boxW - 230, _rateMsgBg->GetY() + boxH - 120, 190, 65);

	_yesBtn->SetColorTrans(false);
	_yesBtn->SetColor(0xFEB325FF);

	_laterBtn->SetColorTrans(false);

	if (SaveManager::GetInstance()->GetALO() >= 45 && SaveManager::GetInstance()->IsRated() == false)
	{
		//if (Platform::GetPlatform()->IsNetworkAvailable())
		//	_rateMsgBox->SetActive(true);
	}
}

void MainMenu::UpdateRateDialog()
{
	if (_yesBtn->IsClicked())
	{
		_rateMsgBox->SetActive(false);

		SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
		GameInfo::GetInstance()->SetReviewClicked(true);
		SaveManager::GetInstance()->SetRated(true);
		SaveManager::GetInstance()->SaveGame();

		return;
	}
	//else if(_laterBtn->IsClicked())
	//{
	//	SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
	//	_rateMsgBox->SetActive(false);
	//}
}

void MainMenu::DrawRateDialog()
{
	if (_rateMsgBox->IsActive())
	{
		int ow = GameInfo::GetInstance()->CodedW();
		int oh = GameInfo::GetInstance()->CodedH();

		RectPrim::Draw(0, 0, ow, oh, 0, 0x000000cc);

		_rateMsgBg->Draw(5, 0xffffffff);
		_yesBtn->Draw(5, 0xffffffff, "Okay", 35, 0xffffffff);
		//_laterBtn->Draw(5, 0xffffffff, "LATER", 29, 0xffffffff);

		UIFont::GetInstance()->SetColor(255, 255, 255, 255);
		UIFont::GetInstance()->Begin(1);

		UIFont::GetInstance()->DrawFromCenter("Please Rate The Game", ow / 2, oh / 2 - 145, 40, -1);

		UIFont::GetInstance()->DrawFromCenter("Please take a moment to rate the", ow / 2, oh / 2 - 30, 32, -1);
		UIFont::GetInstance()->DrawFromCenter("game. Thanks for your support.", ow / 2, oh / 2 + 10, 32, -1);
		UIFont::GetInstance()->End();
	}
}

MainMenu::~MainMenu()
{
	if(_playBtn)
	{
		delete _playBtn;
		_playBtn = NULL;
	}

	if(_highScoresBtn)
	{
		delete _highScoresBtn;
		_highScoresBtn = NULL;
	}

	if(_submitDetailsBtn)
	{
		delete _submitDetailsBtn;
		_submitDetailsBtn = NULL;
	}

	if(_reviewBtn)
	{
		delete _reviewBtn;
		_reviewBtn = NULL;
	}

	if(_shareOnGPlusBtn)
	{
		delete _shareOnGPlusBtn;
		_shareOnGPlusBtn = NULL;
	}

	if(_musicOnOffBtn)
	{
		delete _musicOnOffBtn;
		_musicOnOffBtn = NULL;
	}

	if(_sfxOnOffBtn)
	{
		delete _sfxOnOffBtn;
		_sfxOnOffBtn = NULL;
	}

	if(_exitBtn)
	{
		delete _exitBtn;
		_exitBtn = NULL;
	}
	
	if(_cBlendBg)
	{
		delete _cBlendBg;
		_cBlendBg = NULL;
	}
	
	if(_rateMsgBox)
	{
		delete _rateMsgBox;
		_rateMsgBox = NULL;
	}
	
	if(_rateMsgBg)
	{
		delete _rateMsgBg;
		_rateMsgBg = NULL;
	}

	if(_yesBtn)
	{
		delete _yesBtn;
		_yesBtn = NULL;
	}

	if(_laterBtn)
	{
		delete _laterBtn;
		_laterBtn = NULL;
	}
	
}

