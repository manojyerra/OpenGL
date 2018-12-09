#include "PagesManager.h"

PagesManager* PagesManager::_ref = NULL;

PagesManager::PagesManager()
{
}

PagesManager* PagesManager::GetInstance()
{
	if(_ref == NULL)
		_ref = new PagesManager();
	return _ref;
}

void PagesManager::Init()
{
	_currPage = NULL;
	_mainMenu = NULL;

	_pagesVec.clear();

	_mainMenu = new MainMenu(); 
	_pagesVec.push_back((Page*)_mainMenu);

	_modeSelection = new ModeSelection();
	_pagesVec.push_back((Page*)_modeSelection);

	_modeSelection_high = new ModeSelection_High();
	_pagesVec.push_back((Page*)_modeSelection_high);

	_currPage = (Page*)_mainMenu;
	_nextPage = (Page*)_mainMenu;;
}

void PagesManager::Update()
{
	if(_nextPage != _currPage)
		_currPage = _nextPage;

	if(_currPage)
		_currPage->Update();
}

Page* PagesManager::GetPage(int currPageID)
{
	if(currPageID == MAINMENU)						return (Page*)_mainMenu;
	else if(currPageID == MODE_SELECTION)			return (Page*)_modeSelection;
	else if(currPageID == MODE_SELECTION_HIGH)		return (Page*)_modeSelection_high;

	return NULL;
}

void PagesManager::SetCurrPage(int currPageID)
{
	if(currPageID == MAINMENU)						_nextPage = (Page*)_mainMenu;
	else if(currPageID == MODE_SELECTION)			_nextPage = (Page*)_modeSelection;
	else if(currPageID == MODE_SELECTION_HIGH)		_nextPage = (Page*)_modeSelection_high;
	else											_nextPage = NULL;
}

int PagesManager::GetCurrPageID()
{
	if(_currPage == (Page*)_mainMenu)					return MAINMENU;
	else if(_currPage == (Page*)_modeSelection)			return MODE_SELECTION;
	else if(_currPage == (Page*)_modeSelection_high)	return MODE_SELECTION_HIGH;

	return -1;
}

void PagesManager::Draw()
{
	if(_currPage)
		_currPage->Draw();
}

PagesManager::~PagesManager()
{
	for(int i=0;i<_pagesVec.size();i++)
	{
		if(_pagesVec[i])
		{
			delete _pagesVec[i];
			_pagesVec[i] = NULL;
		}
	}
	_pagesVec.clear();
}


void PagesManager::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}
