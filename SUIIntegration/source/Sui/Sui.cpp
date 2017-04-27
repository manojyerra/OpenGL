#include "SUI.h"

void SUISetup(int windowWidth, int windowHeight)
{
	SuiManager::GetInstance()->Setup(windowWidth, windowHeight);
}

void SUIRun()
{
	SuiManager::GetInstance()->Run();
}

void SUIQuit()
{
	SuiManager::GetInstance()->DeleteInstance();
}