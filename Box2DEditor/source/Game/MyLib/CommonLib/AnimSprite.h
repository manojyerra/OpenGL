#pragma once
#include "Sprite.h"
#include <vector>
using namespace std;

class AnimSprite
{
private:
	int _frameNum;
	float _time;
	float _totTime;
	float _timeLimitToPlay;
	float _frameRate;
	int _startFrame;
	int _endFrame;

	vector<int>_startFrameVec;
	vector<int>_endFrameVec;
	int _playType;
	bool _playAnim;
	bool _playAllFrames;

	float _rot;

public:
	vector<Sprite*>_vec;
	vector<float> _timeVec;

	static const int PLAY_ONCE = 1;
	static const int PLAY_LOOP = 2;

	AnimSprite();
	~AnimSprite();

	AnimSprite(int playType);

	void SetTimeLimit(float time);
	void SetFrameRate(float frameRate);
	void SetFramesAsAnimation(int startFrame,int endFrame);

	void AddSprite(const char* fileName, float x, float y, float width, float height);
	void AddSprite(const char* fileName, float x, float y, float width, float height, float time);
	void AddSprite(const char* fileName, float x, float y, float w, float h, float u, float v, float cropW, float corpH);
	void AddSprite(const char* fileName, float x, float y, float w, float h, float u, float v, float cropW, float corpH, float time);

	void AddSprite_uv01(const char* fileName, float x, float y, float w, float h, float u, float v, float cropW, float corpH);

	void SetPos(float x, float y);
	void SetBounds(float x, float y, float w, float h);
	void PlayAnimation();
	void PlayAnimation(int animIndex);
	bool IsPlayingAnim();

	float X(int frameIndex);
	float Y(int frameIndex);
	float W(int frameIndex);
	float H(int frameIndex);

	void SetRotation(float angleInDegrees);
	float GetRotation();

	int  GetCurrFrameNum();
	void SetCurrFrameNum(int frameNum);
	void Draw(bool visible = true);
};