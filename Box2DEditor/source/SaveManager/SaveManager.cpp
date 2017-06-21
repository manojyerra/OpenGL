#include "SaveManager.h"
#include "../Platform/Platform.h"

SaveManager* SaveManager::_ref = 0;

SaveManager::SaveManager(){}
SaveManager::~SaveManager(){}

SaveManager* SaveManager::GetInstance()
{
	if(_ref == 0)
		_ref =new SaveManager();
	return _ref;
}

void SaveManager::Init()
{
	_saveData = new SaveData();
}

void SaveManager::Terminate()
{
	if(_saveData)
	{
		delete _saveData;
		_saveData = NULL;
	}

	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}