#include "SUI.h"

void SUISetup(int windowWidth, int windowHeight)
{
	SUIManager::GetInstance()->Setup(windowWidth, windowHeight);
}

void SUIRun()
{
	SUIManager::GetInstance()->Run();
}

void SUIQuit()
{
	SUIManager::GetInstance()->DeleteInstance();
}