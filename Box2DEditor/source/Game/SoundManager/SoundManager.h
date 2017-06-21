#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#pragma once

#include "SoundsList.h"

class SoundManager
{
	static SoundManager* _ref;
	SoundManager();
	~SoundManager();

public:
	static SoundManager* GetInstance();
	static void DeleteInstance();

	void SetMusicOn(bool play);
	void SetSFXOn(bool play);

	void PlaySFX(int sfxId);
	void PlaySFX(int sfxId, bool play, bool loop);

	void OnPause();
	void OnResume();
};
#endif
