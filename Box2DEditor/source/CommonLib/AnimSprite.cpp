#include "AnimSprite.h"

AnimSprite::AnimSprite()
{
	_frameNum = 0;
	_time = 0.0f;
	_frameRate = 30.0f;
	_startFrame = 0;
	_endFrame = 0;
}

void AnimSprite::SetFrameRate(float frameRate)
{
	_frameRate = frameRate;
}

void AnimSprite::SetFramesAsAnimation(int startFrame,int endFrame)
{
	_startFrameVec.push_back(startFrame - 1);
	_endFrameVec.push_back (endFrame - 1);
}

void AnimSprite::AddSprite(const char* fileName,float x,float y,float width,float height)
{
	 Sprite* sprite = new Sprite(fileName,x,y,width,height);
	 _vec.push_back(sprite);
	 _endFrame = _vec.size() - 1;
}


void AnimSprite::PlayAnimation(int animIndex)
{
	_startFrame = _startFrameVec[animIndex - 1];
	_endFrame = _endFrameVec[animIndex - 1];
	_frameNum = _startFrame ;
}


int AnimSprite::GetCurrFrameNum()
{
	return _frameNum+1;
}


void AnimSprite::SetCurrFrameNum(int frameNum)
{
	_frameNum = frameNum-1;
}


void AnimSprite::Draw(float deltaTime)
{
	if(_frameNum >= 0 && _frameNum < (int)_vec.size())
	{
		_time += deltaTime;
		
		_vec[_frameNum]->Draw();

		if(_time >= (1/(float)_frameRate))
		{
			_time = 0;
			_frameNum++;
			if(_frameNum > _endFrame)
				_frameNum = _startFrame;
		}
	}

}

AnimSprite::~AnimSprite()
{
	for(int i =0;i <(int)_vec.size();i++)
	{
		if(_vec[i])
		{
			_vec[i]->~Sprite();
			delete _vec[i];
			_vec[i] = NULL;
		}
	}
	_vec.clear();

	_startFrameVec.clear();
	_endFrameVec.clear();
}