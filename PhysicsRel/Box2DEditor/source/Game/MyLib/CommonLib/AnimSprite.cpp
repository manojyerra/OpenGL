#include "AnimSprite.h"
#include "GameInfo.h"

AnimSprite::AnimSprite()
{
	_playType = PLAY_LOOP;
	_frameNum = 0;
	_time = 0.0f;
	_totTime = 0.0f;
	_timeLimitToPlay = -1.0f;
	_frameRate = 30.0f;
	_startFrame = 0;
	_endFrame = 0;
	_rot = 0;
	_playAnim = false;
}

AnimSprite::AnimSprite(int playType)
{
	_playType = playType;
	_frameNum = 0;
	_time = 0.0f;
	_totTime = 0.0f;
	_timeLimitToPlay = -1.0f;
	_frameRate = 30.0f;
	_startFrame = 0;
	_endFrame = 0;
	_rot = 0;
	_playAnim = false;
}

void AnimSprite::SetTimeLimit(float time)
{
	_timeLimitToPlay = time;
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

void AnimSprite::SetBounds(float x, float y, float w, float h)
{
	for(int i =0;i <(int)_vec.size();i++)
		_vec[i]->SetBounds(x,y,w,h);
}

void AnimSprite::SetPos(float x, float y)
{
	for(int i =0;i <(int)_vec.size();i++)
		_vec[i]->SetPos(x,y);
}

void AnimSprite::AddSprite(const char* fileName,float x,float y,float width,float height)
{
	AddSprite(fileName, x, y, width, height, 1.0f/_frameRate);
}

void AnimSprite::AddSprite(const char* fileName,float x,float y,float w,float h, float u, float v, float cropW, float cropH)
{
	AddSprite(fileName, x,y,w,h, u,v,cropW,cropH, 1.0f/_frameRate);
}

void AnimSprite::AddSprite_uv01(const char* fileName, float x, float y, float w, float h, float u, float v, float cropW, float cropH)
{
	 Sprite* sprite = new Sprite(fileName,x,y,w,h);
	 sprite->SetUVBounds0To1(u,v,cropW,cropH);
	 _vec.push_back(sprite);
	 _timeVec.push_back(1.0f/_frameRate);
	 _endFrame = _vec.size() - 1;
}

void AnimSprite::AddSprite(const char* fileName, float x, float y, float w, float h, float time)
{
	 Sprite* sprite = new Sprite(fileName,x,y,w,h);
	 _vec.push_back(sprite);
	 _timeVec.push_back(time);
	 _endFrame = _vec.size() - 1;
}

void AnimSprite::AddSprite(const char* fileName, float x, float y, float w, float h, float u, float v, float cropW, float cropH, float time)
{
	 Sprite* sprite = new Sprite(fileName,x,y,w,h, u,v,cropW,cropH);
	 _vec.push_back(sprite);
	 _timeVec.push_back(time);
	 _endFrame = _vec.size() - 1;
}

void AnimSprite::PlayAnimation(int animIndex)
{
	_startFrame = _startFrameVec[animIndex - 1];
	_endFrame = _endFrameVec[animIndex - 1];
	_frameNum = _startFrame ;
	_playAnim = true;
}

float AnimSprite::X(int frameIndex)
{
	if(frameIndex <= _vec.size()) 
		return _vec[frameIndex-1]->GetX();

	return 0;
}

float AnimSprite::Y(int frameIndex)
{
	if(frameIndex <= _vec.size()) 
		return _vec[frameIndex-1]->GetY();

	return 0;
}

float AnimSprite::W(int frameIndex)
{
	if(frameIndex <= _vec.size()) 
		return _vec[frameIndex-1]->GetWidth();

	return 0;
}

float AnimSprite::H(int frameIndex)
{
	if(frameIndex <= _vec.size()) 
		return _vec[frameIndex-1]->GetHeight();

	return 0;
}

void AnimSprite::SetRotation(float angleInDegrees)
{
	_rot = (int)angleInDegrees % 360;

	for(int i =0;i <(int)_vec.size();i++)
		_vec[i]->SetRotation(_rot);
}

float AnimSprite::GetRotation()
{
	return _rot;
}

bool AnimSprite::IsPlayingAnim()
{
	return _playAnim;
}

void AnimSprite::PlayAnimation()
{
	_playAnim = true;
}

int AnimSprite::GetCurrFrameNum()
{
	return _frameNum+1;
}

void AnimSprite::SetCurrFrameNum(int frameNum)
{
	_frameNum = frameNum-1;
}

void AnimSprite::Draw(bool visible)
{
	if(_playAnim && _frameNum >= 0 && _frameNum < (int)_vec.size())
	{
		float deltaTime = GameInfo::deltaTime;

		_time += deltaTime;
		_totTime += deltaTime;

		if(_timeLimitToPlay > 0 && _totTime >= _timeLimitToPlay)
		{
			_timeLimitToPlay = -1;
			_totTime = 0;
			_playAnim = false;
		}

		if(visible)
			_vec[_frameNum]->Draw();

		if(_time >= (1/(float)_frameRate))
		{
			_time = 0;
			_frameNum++;
			if(_frameNum > _endFrame)
			{
				_frameNum = _startFrame;
				if(_playType == PLAY_ONCE)
					_playAnim = false;
			}
		}
	}
}

AnimSprite::~AnimSprite()
{
	for(int i =0;i <(int)_vec.size();i++)
	{
		if(_vec[i])
		{
			delete _vec[i];
			_vec[i] = NULL;
		}
	}
	_vec.clear();

	_startFrameVec.clear();
	_endFrameVec.clear();
}