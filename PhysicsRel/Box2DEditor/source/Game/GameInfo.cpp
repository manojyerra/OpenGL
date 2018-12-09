#include "GameInfo.h"

GameInfo* GameInfo::_ref = 0;
float GameInfo::deltaTime = 1.0f/30.0f;
bool GameInfo::gray = false;

GameInfo::GameInfo()
{
	drawX = 0;
	drawY = 0;

	drawW = 0;
	drawH = 0;

	_surfaceW = 0;
	_surfaceH = 0;

	_codedW = 0;
	_codedH = 0;

	_isGamePaused = false;
	_isSubmitClicked = false;
	_isTimeToShowAd = false;
	_isLeaderBoardOn = false;
	_isReviewClicked = false;
	_isShareClicked = false;

	_languageID = ENGLISH;
}

GameInfo::~GameInfo()
{
}

GameInfo* GameInfo::GetInstance()
{
	if(_ref == 0)
		_ref = new GameInfo();
	return _ref;
}

void GameInfo::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = 0;
	}
}

void GameInfo::SetSurfaceSize(float surfaceW, float surfaceH)
{
	_surfaceW = surfaceW;
	_surfaceH = surfaceH;
}

void GameInfo::SetCodedSize(float codedW, float codedH)
{
	_codedW = codedW;
	_codedH = codedH;
}

float GameInfo::CodedW()
{
	return _codedW;
}

float GameInfo::CodedH()
{
	return _codedH;
}

float GameInfo::SurfaceW()
{
	return _surfaceW;
}

float GameInfo::SurfaceH()
{
	return _surfaceH;
}

void GameInfo::CalcDrawArea()
{
	drawW = _surfaceW;
	drawH = _codedH * drawW / _codedW;

	if(drawH > _surfaceH)
	{
		drawH = _surfaceH;
		drawW = _codedW * drawH / _codedH;
	}

	drawX = (_surfaceW - drawW ) / 2.0f;
	drawY = (_surfaceH - drawH ) / 2.0f;
}


void GameInfo::SetGamePause(bool isGamePaused)			{	_isGamePaused = isGamePaused;			}
bool GameInfo::IsGamePaused()							{	return _isGamePaused;					}

void GameInfo::SetSubmitClicked(bool isSubmitClicked)	{	_isSubmitClicked = isSubmitClicked;		}
bool GameInfo::IsSubmitDetailsBtnClicked()				{	return _isSubmitClicked;				}

void GameInfo::SetTimeToShowAd(bool val)				{	_isTimeToShowAd = val;					}
bool GameInfo::IsTimeToShowAd()							{	return _isTimeToShowAd;					}

bool GameInfo::IsLeaderBoardOn()						{	return _isLeaderBoardOn;				}
void GameInfo::SetLeaderBoardOn(bool on)				{	_isLeaderBoardOn = on;					}

void GameInfo::SetReviewClicked(bool val)				{	_isReviewClicked = val;					}
bool GameInfo::IsReviewClicked()						{	return _isReviewClicked;				}

void GameInfo::SetShareClicked(bool val)				{	_isShareClicked = val;					}
bool GameInfo::IsShareClicked()							{	return _isShareClicked;					}

void GameInfo::SetLanguage(int languageID)				{	_languageID = languageID;				}
int GameInfo::GetLanguage()								{	return _languageID;						}


string GameInfo::GetTimeStr(int seconds, string preFixStr, string sufFixStr)
{
	int sec = seconds % 60;
	int min = seconds / 60;
	int hours = min / 60;
	min = min % 60;

	char secArr[6];
	char minArr[6];
	char hoursArr[6];

	if(sec < 10)	sprintf(secArr, "0%d", sec);
	else			sprintf(secArr, "%d", sec);

	if(min < 10)	sprintf(minArr, "0%d", min);
	else			sprintf(minArr, "%d", min);

	if(hours < 10)	sprintf(hoursArr, "0%d", hours);
	else			sprintf(hoursArr, "%d", hours);

	char timeArr[128];
	sprintf(timeArr, "%s%s:%s:%s%s",preFixStr.c_str(), hoursArr, minArr, secArr, sufFixStr.c_str()); 

	return timeArr;
}


bool GameInfo::IsIntersectsWithScreen(float rectX, float rectY, float rectWidth, float rectHeight)
{
	int tw = (int)CodedW();
	int th = (int)CodedH();
	int rw = (int)rectWidth;
	int rh = (int)rectHeight;

	if (rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) 
		return false; 

	int tx = (int)0;
	int ty = (int)0;
	int rx = (int)rectX;
	int ry = (int)rectY;

	rw += rx;
	rh += ry;
	tw += tx;
	th += ty;

	return ((rw < rx || rw > tx) &&	(rh < ry || rh > ty) &&	(tw < tx || tw > rx) && (th < ty || th > ry));	
}