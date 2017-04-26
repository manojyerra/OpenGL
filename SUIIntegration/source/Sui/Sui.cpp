#include "Sui.h"

void SuiSetup(int windowWidth, int windowHeight)
{
	SuiManager::GetInstance()->Setup(windowWidth, windowHeight);
}

int SuiWindowWidth()
{
	return SuiManager::GetInstance()->GetWindowWidth();
}

int SuiWindowHeight()
{
	return SuiManager::GetInstance()->GetWindowHeight();
}

void SuiRun()
{
	SuiManager::GetInstance()->Run();
}

void SuiQuit()
{
	SuiManager::GetInstance()->DeleteInstance();
}

unsigned int GetTimeInMilliseconds()
{
	return SuiManager::GetInstance()->GetTimeInMilliSeconds();
}