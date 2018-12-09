#ifndef PAGESMANAGER_H
#define PAGESMANAGER_H
#pragma once

#include "MainMenu.h"
#include "ModeSelection.h"
#include "ModeSelection_High.h"

#include "Page.h"
#include <vector>
using namespace std;

class PagesManager
{
private:
	static PagesManager* _ref;

	Page* _currPage;
	Page* _nextPage;
	
	MainMenu* _mainMenu;
	ModeSelection* _modeSelection;
	ModeSelection_High* _modeSelection_high;

	PagesManager();
	~PagesManager();

public:
	vector<Page*> _pagesVec;

	static const int NONE					= 0;
	static const int MAINMENU				= 1;
	static const int MODE_SELECTION			= 2;
	static const int MODE_SELECTION_HIGH	= 3;

	static PagesManager* GetInstance();
	static void DeleteInstance();

	void Init();
	void Update();
	void Draw();
	
	Page* GetPage(int currPageID);
	void SetCurrPage(int currPageID);
	int GetCurrPageID();
};

#endif
