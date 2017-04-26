#ifndef SUI_H
#define SUI_H

#include "SuiManager.h"
#include "SuiCycle.h"

void SuiSetup(int windowWidth, int windowHeight);
void SuiRun();
void SuiQuit();

unsigned int GetTimeInMilliseconds();

int SuiWindowWidth();
int SuiWindowHeight();

#endif