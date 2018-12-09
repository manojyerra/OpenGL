#ifndef LBHLEPER_H
#define LBHLEPER_H
#pragma once

#include "LeaderBoardData.h"

#include <string>
using namespace std;

class LBHelper
{
private:

public:
	static void DrawHeading(int x, int y, int w, int h, int fontH, unsigned int color, int rank);
	static void DrawStarsTime(int stars, int time, int x, int y, int fontH);
	static void DrawScore(int score, int x, int y, int fontH);
	static void DrawUserInfo(string name, string country, int x, int y, int fontH, int gap);
	static void DrawNumber(int num, int x, int y, int fontH, unsigned int color);
	static string GetLBFilePath();

	static bool ReadLBData(LeaderBoardData* lbData);
};

#endif