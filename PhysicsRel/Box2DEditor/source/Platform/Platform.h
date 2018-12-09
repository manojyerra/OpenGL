#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>
using namespace std;

class Platform
{
private:
	static Platform* _ref;
	

public:
	static const int READ_FROM_ASSETS = 1;
	static const int READ_FROM_IM = 2;

	Platform();
	virtual ~Platform();

	static Platform* GetPlatform();
	static void DeletePlatform();

	virtual void onCreate() = 0;
	virtual void onSizeChange(int width, int height) = 0;
	virtual int Draw(int deltaTimeInMillis) = 0;
	virtual void onTouch(int x, int y, int eventID) = 0;
	virtual void onPause() = 0;
	virtual void onResume() = 0;
	virtual void onExit() = 0;

	virtual void SetSaveFolderPath(std::string saveFolderPath) = 0;
	virtual void SetDataFolderPath(std::string dataFolderPath) = 0;
	
	virtual string GetSaveFolderPath() = 0;
	virtual string GetDataFolderPath() = 0;

	virtual char* GetFileData(std::string filePath, int* fileSize, int readFrom) = 0;

	virtual bool SetSaveData(void* data, unsigned int size) = 0;
	virtual bool GetSaveData(void* data, unsigned int size) = 0;

	virtual void PlayMusic(bool play) = 0;
	virtual void PlaySFX(int sfxId, bool play, bool loop) = 0;
	virtual void OnAudioPause() = 0;
	virtual void OnAudioResume() = 0;
	virtual bool IsMusicPlaying() = 0;
	virtual float GetTimeInSeconds() = 0;
	virtual int GetTimeInMillis() = 0;

	virtual void ShowMsg(char* str) = 0;
	virtual void SetBackKeyPressed() = 0;
	virtual void SleepThread(int timeInMillis) = 0;

	virtual void SetWindowTitle(char* str) = 0;
	
	virtual bool IsNetworkAvailable() = 0;
};

#endif
