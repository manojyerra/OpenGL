#include "SoundManager.h"
#include "Platform.h"
#include "SaveManager.h"

SoundManager* SoundManager::_ref = 0;

SoundManager::SoundManager()
{
	Platform::GetPlatform()->PlayMusic(true);
	Platform::GetPlatform()->SleepThread(100);
	Platform::GetPlatform()->PlayMusic(SaveManager::GetInstance()->IsMusicOn());
}

SoundManager* SoundManager::GetInstance()
{
	if(_ref == 0)
		_ref = new SoundManager();
	return _ref;
}

void SoundManager::SetMusicOn(bool play)
{
	Platform::GetPlatform()->PlayMusic(play);
	SaveManager::GetInstance()->SetMusicOn(play);
	SaveManager::GetInstance()->SaveGame();
}

void SoundManager::SetSFXOn(bool play)
{
	SaveManager::GetInstance()->SetSFXOn(play);
	SaveManager::GetInstance()->SaveGame();
}

void SoundManager::PlaySFX(int sfxId)
{
	if(SaveManager::GetInstance()->IsSFXOn())
		Platform::GetPlatform()->PlaySFX(sfxId, true, false);
}

void SoundManager::PlaySFX(int sfxId, bool play, bool loop)
{
	if(SaveManager::GetInstance()->IsSFXOn())
		Platform::GetPlatform()->PlaySFX(sfxId, play, loop);
}

void SoundManager::OnPause()
{
	Platform::GetPlatform()->OnAudioPause();
}

void SoundManager::OnResume()
{
	Platform::GetPlatform()->OnAudioResume();
}

SoundManager::~SoundManager()
{
}

void SoundManager::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}
