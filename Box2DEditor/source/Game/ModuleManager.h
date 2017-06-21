#ifndef MODULE_H
#define MODULE_H
#pragma once

#include "PagesManager.h"
#include "GameManager.h"

class ModuleManager
{
private:
	static ModuleManager* _ref;
	ModuleManager();
	~ModuleManager();

	int _currModule;
	int _nextModule;

	PagesManager* _pagesManager;
	GameManager* _game;

public:
	static const int NONE = 0;
	static const int LOGOS = 1;
	static const int PAGES = 2;
	static const int GAME = 3;

	static ModuleManager* GetInstance();
	static void DeleteInstance();

	void* GetModuleObject(int moduleID);

	void Update();
	void SetModule(int module);
	int GetModule();
	void CheckModuleChange();
	void Draw();
};


#endif
