#include"AfterLevelOver.h"
#include "GameInfo.h"
#include "ModuleManager.h"
#include "SaveManager.h"
#include "Input.h"
#include "UIFont.h"
#include "FileReader.h"
#include "Platform.h"
#include "SoundManager.h"


AfterLevelOver::AfterLevelOver()
{
	_isActive = false;
	_currScore = 0;
	_rank = 0;

	float ow = GameInfo::GetInstance()->CodedW();
	float oh = GameInfo::GetInstance()->CodedH();

	_bg = new BlendTransBg();
	
	float btnW = ow/1.6f;
	int btnH = ow / 5;
	float btnX = (ow-btnW)/2;
	float vGap = 25;

	float startY = 222 + btnH + vGap;
	
	_resultBg = new ColorRect(btnX, startY, btnW, ow/2);
	_resultBg->SetColorTrans(false);

	int relY = _resultBg->GetY() + _resultBg->GetH() + vGap;
	
	_leaderBoardBtn = new ColorRect(btnX, relY, btnW, btnH);
	_homeBtn = new ColorRect(btnX, relY + (btnH+vGap)*1, btnW, btnH);
	_replayBtn = new ColorRect(btnX, 222, btnW, btnH);

	_leaderBoardBtn->SetColorTrans(false);
	_homeBtn->SetColorTrans(false);
	_replayBtn->SetColorTrans(false);
	
	_shareBtn = new ColorRect(10,10,170,65);
	_rateBtn = new ColorRect(ow-10-170,10,170,65);
	
	_leaderBoard = new LeaderBoard();
	
	_frameCount = 0;
	_activateLBSudden = false;

	RateDialogInit();
}

void AfterLevelOver::Update()
{
	_frameCount++;

	if (_rateMsgBox->IsActive())
	{
		UpdateRateDialog();
	}
	else if(_leaderBoard && _leaderBoard->IsActive())
	{
		_leaderBoard->Update();
	}
	else
	{
		if(_activateLBSudden && _frameCount == 20)
		{
			if(_leaderBoard)
				_leaderBoard->SetActive(true);

			_activateLBSudden = false;
			return;
		}

		if(_shareBtn && _shareBtn->IsClicked())
		{
			SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
			GameInfo::GetInstance()->SetShareClicked(true);
		}
		else if(_rateBtn && _rateBtn->IsClicked())
		{
			SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
			GameInfo::GetInstance()->SetReviewClicked(true);
		}
		else if(_leaderBoardBtn && _leaderBoardBtn->IsClicked())
		{
			SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
			_leaderBoard->SetActive(true);
			_rank = _leaderBoard->GetRank();
			
			ShowRankImproveMsg(_rank, SaveManager::GetInstance()->GetRank());
		}
		else if(_homeBtn->IsClicked())
		{
			SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
			PagesManager::GetInstance()->SetCurrPage(PagesManager::MAINMENU);
			ModuleManager::GetInstance()->SetModule(ModuleManager::PAGES);
			return;
		}
		else if(_replayBtn->IsClicked())
		{
			SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
			ModuleManager::GetInstance()->SetModule(ModuleManager::GAME);
			GameInfo::GetInstance()->SetTimeToShowAd(true);
			return;
		}
	}
}

void AfterLevelOver::SetCurrScore(int score)
{
	_currScore = score;
}

void AfterLevelOver::SetActive(bool isActive)
{
	_isActive = isActive;
	GameInfo::GetInstance()->SetTimeToShowAd(true);

	if(_leaderBoard)
	{
		_leaderBoard->SetActive(true);
		_rank = _leaderBoard->GetRank();
		_leaderBoard->SetActive(false);

		printLog("Rank : %d, PrevRank : %d, Mode %d ", _rank, SaveManager::GetInstance()->GetRank(), 
																SaveManager::GetInstance()->GetGameMode());
	}

	ShowRankImproveMsg(_rank, SaveManager::GetInstance()->GetRank());

	if(SaveManager::GetInstance()->GetALO() == 4 || SaveManager::GetInstance()->GetALO() == 15)	
		_activateLBSudden = true;
}

void AfterLevelOver::ShowRankImproveMsg(int rank, int prevRank)
{
	if(rank < prevRank && prevRank > 0)
	{
		printLog("Improved Rank from %d to %d", prevRank, rank);

		char chArr[256];
		sprintf(chArr, "Improved Rank from %d to %d", prevRank, rank);
		Platform::GetPlatform()->ShowMsg(chArr);
	}

	if(rank != prevRank)
	{
		SaveManager::GetInstance()->SetRank(rank);
		SaveManager::GetInstance()->SaveGame();
	}
}

bool AfterLevelOver::IsActive()
{
	return _isActive; 
}

void AfterLevelOver::Draw()
{
	_bg->Draw();

	if(_leaderBoard && _leaderBoard->IsActive())
	{
		_leaderBoard->Draw();
	}
	else
	{
		_shareBtn->Draw(3, 0xffffffff, "SHARE", 40, 0xffffffff);
		_rateBtn->Draw(3, 0xffffffff, "RATE", 40, 0xffffffff);
		
		_leaderBoardBtn->Draw(4, 0xffffffff, "LEADERBOARD", 45, 0xffffffff);
		_homeBtn->Draw(4, 0xffffffff, "HOME", 45, 0xffffffff);
		_replayBtn->Draw(4, 0xffffffff, "TRY AGAIN", 45, 0xffffffff);

		DrawStatus();
	}

	DrawRateDialog();
}

void AfterLevelOver::DrawStatus()
{
	_resultBg->Draw(4, 0xffffffff);

	int halfSW = GameInfo::GetInstance()->CodedW()/2;

	SaveManager* saveMgr = SaveManager::GetInstance();
	int bestScore = saveMgr->GetScore();

	char rankArr[128];
	sprintf(rankArr, "RANK : %d", _rank);
	
	UIFont::GetInstance()->Begin(1);
	UIFont::GetInstance()->SetColor(255,255,255,255);

	if(_rank > 0)	UIFont::GetInstance()->DrawFromCenter(rankArr, halfSW,85+65,50,1);
	else			UIFont::GetInstance()->DrawFromCenter("GAME OVER", halfSW,85+65,50,1);
	
	int dy = 150;

	if(bestScore == _currScore)
	{
		char scoreArr[128];
		sprintf(scoreArr, "%d", _currScore);

		UIFont::GetInstance()->DrawFromCenter("NEW BEST", halfSW,370 + dy,50,1);
		UIFont::GetInstance()->DrawFromCenter(scoreArr, halfSW,430 + dy,40,1);
	}
	else
	{
		char scoreArr[128];
		char bestScoreArr[128];

		sprintf(scoreArr, "%d", _currScore);
		sprintf(bestScoreArr, "%d", bestScore);

		UIFont::GetInstance()->DrawFromCenter("SCORE", halfSW,285 + dy,55,1);
		UIFont::GetInstance()->DrawFromCenter(scoreArr, halfSW,335 + dy,45,1);
		
		UIFont::GetInstance()->DrawFromCenter("BEST", halfSW,420 + dy,40,1);
		UIFont::GetInstance()->DrawFromCenter(bestScoreArr, halfSW,465 + dy,34,1);
	}
	
	UIFont::GetInstance()->End();
}


//Begin : RateMsgBox

void AfterLevelOver::RateDialogInit()
{
	GameInfo* gameInfo = GameInfo::GetInstance();
	float ow = GameInfo::GetInstance()->CodedW();
	float oh = GameInfo::GetInstance()->CodedH();

	_rateMsgBox = new MsgBox("Do you like this game?");
	_rateMsgBox->SetBgSize(460, 225);
	_rateMsgBox->SetActive(false);
	//_rateMsgBox->SetInActiveOnBackPress(true);
	_rateMsgBox->AddButton("Yes, I will");
	_rateMsgBox->AddButton("Later");

	float boxW = ow*0.92;
	float boxH = boxW*0.85;

	_rateMsgBg = new ColorRect((ow - boxW) / 2, (oh - boxH) / 2, boxW, boxH);
	_rateMsgBg->SetColorTrans(false);
	_rateMsgBg->SetColor(0x5A9BFCFF);

	_yesBtn = new ColorRect(_rateMsgBg->GetX() + 20, _rateMsgBg->GetY() + boxH - 120, 190, 65);
	_laterBtn = new ColorRect(_rateMsgBg->GetX() + boxW - 230, _rateMsgBg->GetY() + boxH - 120, 190, 65);

	_yesBtn->SetColorTrans(false);
	_yesBtn->SetColor(0xFEB325FF);

	_laterBtn->SetColorTrans(false);

	int alo = SaveManager::GetInstance()->GetALO();

	if (alo > 1 && alo%20 == 0 && SaveManager::GetInstance()->IsRated() == false)
	{ 
		if (Platform::GetPlatform()->IsNetworkAvailable())
			_rateMsgBox->SetActive(true);
	}
}

void AfterLevelOver::UpdateRateDialog()
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
	else if(_laterBtn->IsClicked())
	{
		SoundManager::GetInstance()->PlaySFX(SoundList::BUTTON_CLICK);
		_rateMsgBox->SetActive(false);
	}
}

void AfterLevelOver::DrawRateDialog()
{
	if (_rateMsgBox->IsActive())
	{
		int ow = GameInfo::GetInstance()->CodedW();
		int oh = GameInfo::GetInstance()->CodedH();

		RectPrim::Draw(0, 0, ow, oh, 0, 0x000000cc);

		_rateMsgBg->Draw(5, 0xffffffff);
		_yesBtn->Draw(5, 0xffffffff, "Okay", 35, 0xffffffff);
		_laterBtn->Draw(5, 0xffffffff, "LATER", 29, 0xffffffff);

		UIFont::GetInstance()->SetColor(255, 255, 255, 255);
		UIFont::GetInstance()->Begin(1);

		UIFont::GetInstance()->DrawFromCenter("Please Rate The Game", ow / 2, oh / 2 - 145, 40, -1);

		UIFont::GetInstance()->DrawFromCenter("Please take a moment to rate the", ow / 2, oh / 2 - 30, 32, -1);
		UIFont::GetInstance()->DrawFromCenter("game. Thanks for your support.", ow / 2, oh / 2 + 10, 32, -1);
		UIFont::GetInstance()->End();
	}
}

//End : RateMsgBox


AfterLevelOver::~AfterLevelOver()
{
	if(_bg)
	{
		delete _bg;
		_bg = NULL;
	}

	if(_resultBg)
	{
		delete _resultBg;
		_resultBg = NULL;
	}

	if(_leaderBoard)
	{
		delete _leaderBoard;
		_leaderBoard = NULL;
	}

	if(_leaderBoardBtn)
	{
		delete _leaderBoardBtn;
		_leaderBoardBtn = NULL;
	}

	if(_shareBtn)
	{
		delete _shareBtn;
		_shareBtn = NULL;
	}

	if(_rateBtn)
	{
		delete _rateBtn;
		_rateBtn = NULL;
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
