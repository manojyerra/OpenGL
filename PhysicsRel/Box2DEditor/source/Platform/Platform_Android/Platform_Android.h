#ifdef _PLATFORM_ANDROID

#ifndef PLATFORM_ANDROID_H
#define PLATFORM_ANDROID_H

#include "Platform.h"
#include "GameLoop.h"
#include <string>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class Platform_Android : public Platform
{
private:
	std::string _saveFolderPath;
	std::string _dataFolderPath;
	
	AAssetManager* _assetManager;

	GameLoop* _gameLoop;

public:

	Platform_Android();
	~Platform_Android();

	void SetAssetManager(AAssetManager* assetManager);
	void SetLanguageID(int languageID);
	
	// Implementation of Platform pure virtual functions...
	void onCreate();
	void onSizeChange(int width, int height);
	int Draw(int deltaTimeInMillis);
	void onTouch(int x, int y, int eventID);
	void onPause();
	void onResume();
	void onExit();
	void SetBackKeyPressed();
	void SetAccMeterMoved(int dir);

	void SetSaveFolderPath(std::string saveFolderPath);
	void SetDataFolderPath(std::string dataFolderPath);

	string GetSaveFolderPath();
	string GetDataFolderPath();

	char* GetFileData(std::string filePath, int* fileSize, int readFrom);
	bool SetSaveData(void* data, unsigned int size);
	bool GetSaveData(void* data, unsigned int size);

	void PlayMusic(bool play);
	void PlaySFX(int sfxId, bool play, bool loop);
	void OnAudioPause();
	void OnAudioResume();
	bool IsMusicPlaying();
	float GetTimeInSeconds();
	int GetTimeInMillis();

	void ShowMsg(char* str);
	void SleepThread(int timeInMillis);

	void SetWindowTitle(char* str);
	bool IsNetworkAvailable();	
};

#endif


#endif
