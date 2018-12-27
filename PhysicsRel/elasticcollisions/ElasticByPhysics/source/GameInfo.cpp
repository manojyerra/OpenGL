#include "GameInfo.h"

float GameInfo::deltaTime = 0.0f;
GameInfo* GameInfo::_ref = 0;

GameInfo::GameInfo(){}
GameInfo::~GameInfo(){}

GameInfo* GameInfo::GetInstance()
{
	if(_ref)
		_ref = new GameInfo();
	return _ref;
}

void GameInfo::Init()
{
	deltaTime = 0.0f;
}

void GameInfo::Update()
{
}

void GameInfo::Terminate()
{
	if(_ref)
	{
		delete _ref;
		_ref = 0;
	}
}
