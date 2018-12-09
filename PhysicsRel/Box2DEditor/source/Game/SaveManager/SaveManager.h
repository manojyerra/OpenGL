#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H
#pragma once

#include "SaveData.h"

class SaveManager
{
private:
	SaveData* _saveData;
	static SaveManager* _ref;
	SaveManager();
	~SaveManager();

	void SetTimerModeScore(int score);
	int GetTimerModeScore();

	void SetStrategyModeScore(int score);
	int GetStrategyModeScore();

public:
	static const int STRATEGY_MODE = 1;
	static const int TIMER_MODE = 2;

	static SaveManager* GetInstance();
	static void DeleteInstance();

	SaveData* GetSaveData();
	void SaveGame();
	
	void FillDefaultValues();
	void ReadSave();

	void SetMusicOn(bool musicOn);
	void SetSFXOn(bool sfxOn);

	bool IsMusicOn();
	bool IsSFXOn();
	
	int GetALO();
	void SetALO(int num);
	
	bool IsRated();
	void SetRated(bool val);

	void SetGameMode(int gameMode);
	int GetGameMode();

	void SetRank(int rank);
	int GetRank();
	
	void SetScore(int score);
	int GetScore();
	
	void Terminate();
};
#endif
