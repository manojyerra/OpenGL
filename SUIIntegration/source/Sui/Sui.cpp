#include "Sui.h"

void SuiSetup(int windowWidth, int windowHeight)
{
	SuiManager::GetInstance()->Setup(windowWidth, windowHeight);
}

void SuiRun()
{
	SuiManager::GetInstance()->Run();
}

void SuiQuit()
{
	SuiManager::GetInstance()->DeleteInstance();
}