#ifdef _PLATFORM_WIN32

#include "MusicPlayer.h"

MusicPlayer::MusicPlayer()
{
	_state = PAUSE;
	_prevState = PAUSE;
	_musicFilePath = "../../res/raw/bgm.mp3";

	//Play(true);
}

MusicPlayer::~MusicPlayer()
{
}

void MusicPlayer::SetMusicFilePath(string filePath)
{
	_musicFilePath = filePath;
}

void MusicPlayer::Play(bool play)
{
	if(play)
	{
		//if(_musicFilePath.length() > 0)
		//	PlaySound(TEXT(_musicFilePath.c_str()), NULL, SND_ASYNC | SND_LOOP);

		_prevState = _state;
		_state = PLAY;
	}
	else
	{
		PlaySound(NULL, 0, 0);
		_prevState = _state;
		_state = PAUSE;
	}
}

bool MusicPlayer::IsPlaying()
{
	return (_state == PLAY);
}

#endif