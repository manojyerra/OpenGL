#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H
#pragma once

#include "SaveData.h"

class SaveManager
{
	SaveData* _saveData;
	static SaveManager* _ref;
	SaveManager();
	~SaveManager();

public:
	static SaveManager* GetInstance();
	void Init();
	void SaveGame();
	SaveData* GetSaveData();
	void Terminate();
};
#endif