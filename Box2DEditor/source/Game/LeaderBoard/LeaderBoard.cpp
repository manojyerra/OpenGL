#include "LeaderBoard.h"
#include "GameInfo.h"
#include "Platform.h"
#include "UIFont.h"
#include "Input.h"
#include "SaveManager.h"
#include "Flags.h"
#include "LBHelper.h"

LeaderBoard::LeaderBoard()
{
	_isActive = false;
	_transY = 0;
	_speed = 0;
	_elapsedTime = 0;
	_speedCoff = 1.0f;
	_lastScrollTime = 0;
	_offLineRank = 0;
	_lbData = NULL;

	int ow = GameInfo::GetInstance()->CodedW();
	int oh = GameInfo::GetInstance()->CodedH();

	_blendBg = new BlendTransBg();

	_border = 10;
	_heading = 75;
	_cellH = 130;
	_cellsStartY = _heading;

	_topBorder = new Sprite("ui/lb/cellbg.png", 10,-50,ow-20,128);
	_backBtn = new Button("ui/lb/lbback.png",10,0,80,80);
	_cellBg = new Sprite("ui/lb/cellbg.png", 0,0,0,0);

	_defaultPic = new Sprite("ui/lb/defaultpic.png",10,0,61,73);
	_circle = new Sprite("ui/lb/circlebg.png", 0,0,90,90);

	_cRect1 = new ColorRect(0,0,0,0);
	_cRect2 = new ColorRect(0,0,0,0);
	
	_cRect1->SetColor(0xff00ffaa);
	_cRect2->SetColor(0x00ffffaa);
	
	_cRect1->SetColorTrans(false);
	_cRect2->SetColorTrans(false);

	_showCells = SHOW_PLAYERS_ON_LB;
}

void LeaderBoard::SetActive(bool isActive)
{
	_isActive = isActive;
	_transY = 0;

	GameInfo::GetInstance()->SetLeaderBoardOn(_isActive);

	if(isActive)
	{
		_lbData = new LeaderBoardData();

		_offLineRank = 0;

		if(LBHelper::ReadLBData(_lbData))
			GenerateOffLineRank();

		int sh = GameInfo::GetInstance()->CodedH();

		int realRank = (GetRealRank() > FETCH_NUM_PLAYERS) ? FETCH_NUM_PLAYERS : GetRealRank();

		if(realRank > 1)
			_transY = - ( _cellsStartY + (realRank+6) * _cellH - sh + _border );
	}
	else
	{
		if(_lbData)
		{
			delete _lbData;
			_lbData = NULL;
		}
	}
}

void LeaderBoard::GenerateOffLineRank()
{
	string name(_lbData->UserName);
	string country(_lbData->UserCountryName);

	int score = SaveManager::GetInstance()->GetScore();
	int* scoreArray = _lbData->StrategyModeScoreArray;
	
	if(SaveManager::GetInstance()->GetGameMode() == SaveManager::TIMER_MODE)
		scoreArray = _lbData->TimerModeScoreArray;

	for(int i=0;i<FETCH_NUM_PLAYERS;i++)
	{
		if(score > scoreArray[i])
		{
			_offLineRank = i+1;
			break;
		}
		else if(score == scoreArray[i])
		{
			if(scoreArray[i] == 0)
			{
				_offLineRank = i+1;
				break;
			}
			else
			{
				string otherName(_lbData->NamesArray[i]);
	
				if(name.compare(otherName) <= 0)
				{
					_offLineRank = i+1;
					break;
				}
			}
		}
	}

	printLog("*** offline rank is %d **** ", _offLineRank);

	if(_offLineRank == 0 && _lbData->Rank > 0)
	{
		if(_lbData->Rank > FETCH_NUM_PLAYERS)
			_offLineRank = _lbData->Rank;
		else
			_offLineRank = FETCH_NUM_PLAYERS;
	}
}

bool LeaderBoard::IsActive()
{
	return _isActive;
}

int LeaderBoard::GetRank()
{
	return _offLineRank;
}

void LeaderBoard::Update()
{
	_elapsedTime += GameInfo::deltaTime;

	if(_backBtn)
		_backBtn->Update();

	if((_backBtn && _backBtn->IsClicked()) || Input::IsBackPressed())
	{
		SetActive(false);
		return;
	}

	_transY -= _speed;
	_speed *= 0.98;

	if(_speed > -1 && _speed < 1)
		_speed = 0;

	if(Input::IsMousePressed() && Input::IsMouseClicked() == false)
	{
		_speed = 0;
		_transY += (Input::MouseY - Input::PrevMouseY);
	}
	else if(Input::IsMouseReleased())
	{
		float dist = Input::GetDistBetweenClickAndRelease();
		float time = Input::GetTimeBetweenClickAndRelease();

		_speed = (int)((dist/time)/35);

		if(_elapsedTime > 0 && _elapsedTime - _lastScrollTime < 0.2)
			_speedCoff += 0.5f;
		else
			_speedCoff = 1.0f;

		_speed *= _speedCoff;

		if(Input::MouseClickY - Input::MouseReleaseY < 0)
			_speed = -_speed;
	}

	int sh = GameInfo::GetInstance()->CodedH();
	int len = _cellsStartY + SHOW_PLAYERS_ON_LB * _cellH - sh + _border;

	if(_transY > 0)
	{
		_transY = 0;
		_speed = 0;
	}
	else if(_transY < -len)
	{
		_transY = -len;
		_speed = 0;
	}

	if(_speed > 40 || _speed < -40)
		_lastScrollTime = _elapsedTime;
}

void LeaderBoard::Draw()
{
	_blendBg->Draw();

	int ow = GameInfo::GetInstance()->CodedW();
	int oh = GameInfo::GetInstance()->CodedH();
	
	char ch[256];

	if(_lbData)
	{
		int cellX = _border;
		int cellW = ow - 2*_border;

		glPushMatrix();
		glTranslatef(0,_transY,0);
		
		int gap = 0;

		if(_offLineRank <= SHOW_PLAYERS_ON_LB)
		{
			int rankIndex = _offLineRank-1;

			for(int i=0; i<rankIndex; i++)
				DrawCell(cellX, _cellsStartY+(i*_cellH), cellW, _cellH-gap, i, i+1, false);

			DrawCell(cellX, _cellsStartY+(rankIndex*_cellH), cellW, _cellH-gap, rankIndex, _offLineRank, true);

			for(int i=rankIndex; i<(int)SHOW_PLAYERS_ON_LB-1; i++)
				DrawCell(cellX, _cellsStartY+((i+1)*_cellH), cellW, _cellH-gap, i, i+2, false);
		}
		else
		{
			for(int i=0; i<(int)SHOW_PLAYERS_ON_LB-1; i++)
				DrawCell(cellX, _cellsStartY+(i*_cellH), cellW, _cellH-gap, i, i+1, false);

			DrawCell(cellX, _cellsStartY+((SHOW_PLAYERS_ON_LB-1)*_cellH), cellW, _cellH-15, _offLineRank-1, _offLineRank, true);
		}

		glPopMatrix();

		//_topBorder->Draw();
		RectPrim::Draw(_topBorder->GetX(), _topBorder->GetY(), _topBorder->GetWidth(), _topBorder->GetHeight(), 0, 0xffad1eee);
		RectPrim::DrawBorder(_topBorder->GetX(), _topBorder->GetY(), _topBorder->GetWidth(), _topBorder->GetHeight(), 4, 0xffffffff);
				

		LBHelper::DrawHeading(0, 0, ow, _heading, 27, 0xffffffff, GetRealRank());

		if(_backBtn)
			_backBtn->Draw();
	}
}

void LeaderBoard::DrawCell(int x, int y, int w, int h, int index, int rank, bool isUser)
{
	int sw = GameInfo::GetInstance()->CodedW();
	int sh = GameInfo::GetInstance()->CodedH();

	float ty = y + _transY;
	if( ((ty > 0 && ty < sh) || (ty+h > 0 && ty+h < sh)) == false)
		return;

	int stars = 0;
	int time = 0;
	int score = 0;
	int levelsPlayed = 0;
	string name = "";
	string country = "";

	if(isUser)
	{
		score = SaveManager::GetInstance()->GetScore();
		name = string(_lbData->UserName);
		country = string(_lbData->UserCountryName);
	}
	else
	{
		if(SaveManager::GetInstance()->GetGameMode() == SaveManager::STRATEGY_MODE)
			score = _lbData->StrategyModeScoreArray[index];
		else
			score = _lbData->TimerModeScoreArray[index];
		
		name = string(_lbData->NamesArray[index]);
		country = string(_lbData->CountryNameArray[index]);
	}

	//_cellBg->SetBounds(x,y,w,h);
	//_cellBg->SetColor( isUser ? 0x00ff00ff : 0xffffffff );
	//_cellBg->Draw();

	if(isUser)
	{
		RectPrim::Draw(x,y,w,h, 0, 0x0000ff99);
	}
	else
	{
		if(rank % 2 == 0)
			RectPrim::Draw(x,y,w,h, 0, 0xffad1eaa);
		else
			RectPrim::Draw(x,y,w,h, 0, 0x24FEFEaa);
			
			//RectPrim::Draw(x,y,w,h, 0, 0xFED93CFF);
	}
	
	RectPrim::DrawBorder(x,y,w,h, 4, 0xffffffff);

	
	_defaultPic->SetPos(x+18,y+30);
	_defaultPic->Draw();

	Flags::GetInstance()->Draw(country, x+370,y+47, 64,64);

	_circle->SetPos(sw-_border-_circle->GetWidth()-3, y+_border+12);
	//_circle->Draw();
	
	float rankX = _circle->GetX();
	float rankY = _circle->GetY();
	float rankW = _circle->GetWidth();
	float rankH = _circle->GetHeight();
	
	RectPrim::Draw(rankX-3, rankY+2, rankW-4, rankH-4, 0, 0xFED93C77);
	RectPrim::DrawBorder(rankX-3, rankY+2, rankW-4, rankH-4, 4, 0xffffffff);

	float numFontH = _circle->GetHeight()*0.36;
	LBHelper::DrawNumber(rank, -5+_circle->GetX()+_circle->GetWidth()/2, -2+_circle->GetY()+_circle->GetHeight()/2, numFontH, 0xffffffff);

	int fontH = 28;

	LBHelper::DrawScore(score, x+96, y+h-5, fontH);

	LBHelper::DrawUserInfo(name, country, x+96, y+10, fontH, 4);
}


int LeaderBoard::GetRealRank()
{
	return _offLineRank;
}

LeaderBoard::~LeaderBoard()
{
	if(_backBtn)
	{
		delete _backBtn;
		_backBtn = NULL;
	}

	if(_lbData)
	{
		delete _lbData;
		_lbData = NULL;
	}
	
	if(_topBorder)
	{
		delete _topBorder;
		_topBorder = NULL;
	}

	if(_defaultPic)
	{
		delete _defaultPic;
		_defaultPic = NULL;
	}

	if(_circle)
	{
		delete _circle;
		_circle = NULL;
	}

	if(_cellBg)
	{
		delete _cellBg;
		_cellBg = NULL;
	}

	if(_blendBg)
	{
		delete _blendBg;
		_blendBg = NULL;
	}

	if(_cRect1)
	{
		delete _cRect1;
		_cRect1 = NULL;
	}

	if(_cRect2)
	{
		delete _cRect2;
		_cRect2 = NULL;
	}
}
