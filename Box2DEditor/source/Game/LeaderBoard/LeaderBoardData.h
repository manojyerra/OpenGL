#ifndef LEADERBOARDDATA_H
#define LEADERBOARDDATA_H
#pragma once

//temp FETCH_NUM_PLAYERS, SHOW_PLAYERS_ON_LB should be 1000 or 500
#define FETCH_NUM_PLAYERS 1000
#define SHOW_PLAYERS_ON_LB 1000

class LeaderBoardData
{
public:
	char ObjectID[SHOW_PLAYERS_ON_LB][32];
	char NamesArray[SHOW_PLAYERS_ON_LB][32];
	char CountryNameArray[SHOW_PLAYERS_ON_LB][32];

	int StrategyModeScoreArray[FETCH_NUM_PLAYERS];
	int TimerModeScoreArray[FETCH_NUM_PLAYERS];

	char UserName[32];
	char UserCountryName[32];

	int Rank;

	LeaderBoardData();
	void SetUserInfo();
};

#endif
