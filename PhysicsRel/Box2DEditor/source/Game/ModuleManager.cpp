#include "ModuleManager.h"
#include "SaveManager.h"

ModuleManager* ModuleManager::_ref = NULL;

ModuleManager::ModuleManager()
{
	_pagesManager = NULL;
	_game = NULL;

	_currModule = NONE;
	_nextModule = _currModule;

	_pagesManager = PagesManager::GetInstance();
	_pagesManager->Init();

	//temp
	SetModule(PAGES);
	CheckModuleChange();
}

ModuleManager* ModuleManager::GetInstance()
{
	if(_ref == NULL)
		_ref = new ModuleManager();
	return _ref;
}

void ModuleManager::SetModule(int module)
{
	_nextModule = module;

	if(_nextModule == _currModule && _nextModule == GAME)
	{
		if(_game)
		{
			delete _game;
			_game = NULL;
		}
		if(_game == NULL)
		{
			_game = new GameManager();
		}
	}
}

int ModuleManager::GetModule()
{
	return _currModule;
}

void* ModuleManager::GetModuleObject(int moduleID)
{
	if(moduleID == PAGES)
	{
		return _pagesManager;
	}
	else if(moduleID == GAME)
	{
		return _game;
	}
	
	return NULL;
}

void ModuleManager::Update()
{
	CheckModuleChange();

	if(_currModule == PAGES)
	{
		if(_pagesManager)
			_pagesManager->Update();
	}
	else if(_currModule == GAME)
	{
		if(_game)
			_game->Update();
	}
}


void ModuleManager::CheckModuleChange()
{
	if(_nextModule != _currModule)
	{
		if(_currModule == GAME)
		{
			if(_game)
			{
				delete _game;
				_game = NULL;
			}
		}

		//Loading Next Module here...
		if(_nextModule == PAGES)
		{
			if(_pagesManager == NULL)
			{
				_pagesManager = PagesManager::GetInstance();
				_pagesManager->Init();
			}
		}
		else if(_nextModule == GAME)
		{
			if(_game == NULL)
			{
				_game = new GameManager();
			}
		}

		_currModule = _nextModule;
	}
}

void ModuleManager::Draw()
{
	if(_currModule == PAGES)
	{
		if(_pagesManager)
			_pagesManager->Draw();
	}
	else if(_currModule == GAME)
	{
		if(_game)
			_game->Draw();
	}
}

ModuleManager::~ModuleManager()
{
	if(_pagesManager)
	{
		PagesManager::DeleteInstance();
		_pagesManager = NULL;
	}

	if(_game)
	{
		delete _game;
		_game = NULL;
	}
}

void ModuleManager::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}