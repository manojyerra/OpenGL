#pragma once
#include "Sprite.h"
#include <vector>
using namespace std;

class AnimSprite
{
private:
	int _frameNum;
	float _time;
	float _frameRate;
	int _startFrame;
	int _endFrame;
	
	vector<Sprite*>_vec;
	vector<int>_startFrameVec;
	vector<int>_endFrameVec;

public:
	AnimSprite();
	void SetFrameRate(float frameRate);
	void SetFramesAsAnimation(int startFrame,int endFrame);
	void AddSprite(const char* fileName, float x, float y, float width, float height);
	void PlayAnimation(int animIndex);
	int  GetCurrFrameNum();
	void SetCurrFrameNum(int frameNum);
	void Draw(float deltaTime);
	~AnimSprite();
};