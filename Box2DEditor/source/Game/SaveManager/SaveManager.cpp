#include "SaveManager.h"
#include "GameInfo.h"
#include "Platform.h"

SaveManager* SaveManager::_ref = 0;

SaveManager::SaveManager()
{
	_saveData = NULL;
	ReadSave();
}

SaveManager* SaveManager::GetInstance()
{
	if(_ref == 0)
		_ref = new SaveManager();
	return _ref;
}

void SaveManager::ReadSave()
{
	if(_saveData != NULL)
		delete _saveData;

	_saveData = new SaveData();

	int result = Platform::GetPlatform()->GetSaveData(_saveData, sizeof(SaveData));

	if(result == 0)
		FillDefaultValues();
}

void SaveManager::FillDefaultValues()
{
	_saveData->ALO = 0;
	_saveData->Rated = 0;

	_saveData->StrategyMode_Score = 0;
	_saveData->TimerMode_Score = 0;

	_saveData->StrategyMode_Rank = 0;
	_saveData->TimerMode_Rank = 0;

	_saveData->GameMode = STRATEGY_MODE;

	_saveData->MusicOn = 1;
	_saveData->SFXOn = 1;
}

void SaveManager::SaveGame()
{
	Platform::GetPlatform()->SetSaveData(_saveData, sizeof(SaveData));
}

SaveData* SaveManager::GetSaveData()
{
	return _saveData;
}

void SaveManager::SetMusicOn(bool musicOn)
{
	_saveData->MusicOn = (musicOn) ? 1 : 0;
}

void SaveManager::SetSFXOn(bool sfxOn)
{
	_saveData->SFXOn = (sfxOn) ? 1 : 0;
}

bool SaveManager::IsMusicOn()
{
	return (_saveData->MusicOn == 1);
}

bool SaveManager::IsSFXOn()
{
	return (_saveData->SFXOn == 1);
}

int SaveManager::GetALO()
{
	return _saveData->ALO;
}

void SaveManager::SetALO(int num)
{
	_saveData->ALO = num;
}

bool SaveManager::IsRated()
{
	return (_saveData->Rated == 1);
}

void SaveManager::SetRated(bool val)
{
	if(val)
		_saveData->Rated = 1;
	else
		_saveData->Rated = 0;
}


//***************************************** Level Data Saving... ******************************************//


void SaveManager::SetGameMode(int gameMode)
{
	_saveData->GameMode = gameMode;
}

int SaveManager::GetGameMode()
{
	return _saveData->GameMode;
}

void SaveManager::SetStrategyModeScore(int score)	{	_saveData->StrategyMode_Score = score;	}
int SaveManager::GetStrategyModeScore()				{	return _saveData->StrategyMode_Score;	}

void SaveManager::SetTimerModeScore(int score)		{	_saveData->TimerMode_Score = score;		}
int SaveManager::GetTimerModeScore()				{	return _saveData->TimerMode_Score;		}

void SaveManager::SetScore(int score)
{
	if(_saveData->GameMode == STRATEGY_MODE)
		_saveData->StrategyMode_Score = score;
	else if(_saveData->GameMode == TIMER_MODE)
		_saveData->TimerMode_Score = score;
}

int SaveManager::GetScore()
{
	if(_saveData->GameMode == STRATEGY_MODE)
		return _saveData->StrategyMode_Score;
	
	if(_saveData->GameMode == TIMER_MODE)
		return _saveData->TimerMode_Score;
	
	return 0;
}

void SaveManager::SetRank(int rank)
{
	if(GetGameMode() == SaveManager::STRATEGY_MODE)
		_saveData->StrategyMode_Rank = rank;
	else if(GetGameMode() == SaveManager::TIMER_MODE)
		_saveData->TimerMode_Rank = rank;
}

int SaveManager::GetRank()
{
	if(GetGameMode() == SaveManager::STRATEGY_MODE)
		return _saveData->StrategyMode_Rank;
	else if(GetGameMode() == SaveManager::TIMER_MODE)
		return _saveData->TimerMode_Rank;

	return 0;
}

SaveManager::~SaveManager()
{
	if(_saveData)
	{
		delete _saveData;
		_saveData = NULL;
	}
}

void SaveManager::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}
