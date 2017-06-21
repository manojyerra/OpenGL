#ifndef GAME_INFO
#define GAME_INFO
#pragma once

#include <string>
using namespace std;

//Add Timer mode.
//Take force rating.

//Remove external storage permission.
//Add Identity permission.
//Change game icon to better one.
//Change icons in res folder.
//Add Music
//Change flags non-2power.

class GameInfo
{
private:
	static GameInfo* _ref;

	GameInfo();
	~GameInfo();

	float _surfaceW;
	float _surfaceH;

	float _codedW;
	float _codedH;

	bool _isGamePaused;
	bool _isSubmitClicked;
	bool _isTimeToShowAd;
	bool _isReviewClicked;
	bool _isShareClicked;

	bool _isLeaderBoardOn;
	int _languageID;

public:
	static const int ENGLISH = 1;
	//static const int GERMAN = 2;

	static float deltaTime;
	static bool gray;
	
	float drawX;
	float drawY;

	float drawW;
	float drawH;

	static GameInfo* GetInstance();
	static void DeleteInstance();

	void SetSurfaceSize(float surfaceW, float surfaceH);
	void SetCodedSize(float codedW, float codedH);
	void CalcDrawArea();

	float CodedW();
	float CodedH();

	float SurfaceW();
	float SurfaceH();

	void SetGamePause(bool isGamePaused);
	bool IsGamePaused();

	void SetSubmitClicked(bool isSubmitClicked);
	bool IsSubmitDetailsBtnClicked();

	void SetTimeToShowAd(bool val);
	bool IsTimeToShowAd();

	bool IsLeaderBoardOn();
	void SetLeaderBoardOn(bool on);

	void SetReviewClicked(bool val);
	bool IsReviewClicked();

	void SetShareClicked(bool val);
	bool IsShareClicked();

	void SetLanguage(int languageID);
	int GetLanguage();

	string GetTimeStr(int seconds, string preFixStr, string sufFixStr);
	bool IsIntersectsWithScreen(float x, float y, float w, float h);
};

#endif
