#ifdef _PLATFORM_ANDROID

#include "Platform_Android.h"
#include "Includes.h"
#include <sys/stat.h>


void PlayMusic_native(bool play);
void PlaySound_native(int soundId, bool play, bool loop);
void OnAudioPause_native();
void OnAudioResume_native();
bool IsMusicPlaying_native();
float GetTimeInSeconds_native();
int GetTimeInMillis_native();
void ShowMsg_native(char* str);
void SleepThread_native(int timeInMillis);
bool IsNetworkAvailable_native();

Platform_Android::Platform_Android()
{
	_assetManager = NULL;
	_gameLoop = NULL;
	_dataFolderPath = "";
}

Platform_Android::~Platform_Android(){}

void Platform_Android::SetAssetManager(AAssetManager* assetManager)		{ _assetManager = assetManager; }

void Platform_Android::onCreate()							{ _gameLoop = new GameLoop(); 				}
void Platform_Android::onSizeChange(int width, int height)	{ _gameLoop->onSizeChange(width, height);	}
int Platform_Android::Draw(int deltaTimeInMillis)			{ return _gameLoop->Draw(deltaTimeInMillis);}
void Platform_Android::onTouch(int x, int y, int eventID)
{
	//0 for down
	//1 for up
	//2 for drag

	if(eventID == 0 || eventID == 2)
		_gameLoop->onTouch(x, y, 1);
	else if(eventID == 1)
		_gameLoop->onTouch(x, y, 2);
}
void Platform_Android::onPause()							{ _gameLoop->onPause();						}
void Platform_Android::onResume()							{ _gameLoop->onResume();					}
void Platform_Android::onExit()
{
	if(_gameLoop)
	{
		delete _gameLoop;
		_gameLoop = NULL;
	}
}

void Platform_Android::SetSaveFolderPath(std::string saveFolderPath)	{ _saveFolderPath = saveFolderPath; 	}
void Platform_Android::SetDataFolderPath(std::string dataFolderPath)	{	_dataFolderPath = dataFolderPath;	}

string Platform_Android::GetSaveFolderPath()
{
	return _saveFolderPath;
}

string Platform_Android::GetDataFolderPath()
{
	return _dataFolderPath;
}

char* Platform_Android::GetFileData(std::string filePath, int* fileMemSize, int readFrom)
{
	if(readFrom == READ_FROM_IM)
	{
		std::string finalPath = _saveFolderPath;
		finalPath.append("/");
		finalPath.append(filePath.c_str());

		FILE* fp = fopen(finalPath.c_str(), "rb");

		if(fp)
		{
			fseek(fp, 0, SEEK_END);
			unsigned int fileSize = (unsigned int)ftell(fp);
			fileMemSize[0] = fileSize;

			fseek(fp, 0, SEEK_SET);
			char* arrData = (char*)malloc(fileSize+1);
			memset(arrData, (int)'\0', fileSize+1);

			if(fread(arrData, 1, fileSize, fp) != fileSize)
			{
				free(arrData);
				fclose(fp);
				fileMemSize[0] = 0;
				return NULL;
			}

			fclose(fp);
			return arrData;
		}		
	}
	else
	{
		if(_assetManager)
		{
			for(int i=0;i<filePath.length();i++)
			{
				char ch = filePath[i];

				if(ch>='A' && ch <='Z')
					filePath[i] = (int)ch - ((int)'Z'- (int)'z');
			}

			//printLog("\nFilePath <%s>", filePath.c_str());

			std::string finalPath = _dataFolderPath;
			finalPath.append("/");
			finalPath.append(filePath.c_str());
		
			AAsset* asset = AAssetManager_open(_assetManager, (const char*)filePath.c_str(), AASSET_MODE_UNKNOWN);

			if(asset == NULL)
			{
				//printLog("Asset object is NULL for ", filePath.c_str());
				return NULL;
			}

			long size = AAsset_getLength(asset);
			if(size == 0)
			{
				//printLog("File Size is Zero for %s", filePath.c_str());
				return NULL;
			}

			char* buffer = (char*) malloc (sizeof(char)*size);
			AAsset_read (asset,buffer,size);
			AAsset_close(asset);
			fileMemSize[0] = (int)size;

			return buffer;
		}
	}

	return NULL;
}

bool Platform_Android::SetSaveData(void* data, unsigned int size)
{
	std::string filePath = _saveFolderPath+"/s1.sda";

	FILE* fp = fopen(filePath.c_str(),"wb");

	if(fwrite(data, 1, size, fp) == size)
	{
		fclose(fp);
		return true;
	}
	return false;
}

bool Platform_Android::GetSaveData(void* data, unsigned int size)
{
	std::string filePath = _saveFolderPath+"/s1.sda";

	FILE* fp = fopen(filePath.c_str(),"rb");

	if(fp == NULL)
		return false;

	if(fread(data, 1, size, fp) == size)
	{
		fclose(fp);
		return true;
	}
	return false;
}

void Platform_Android::SetLanguageID(int languageID)
{
	_gameLoop->SetLanguageID(languageID);
}

void Platform_Android::SetBackKeyPressed()
{
	_gameLoop->SetBackKeyPressed();
}

void Platform_Android::SetAccMeterMoved(int dir)
{
	_gameLoop->SetAccMeterMoved(dir);
}

//calling native functions...

void Platform_Android::PlayMusic(bool play)
{
	PlayMusic_native(play);
}

void Platform_Android::PlaySFX(int sfxId, bool play, bool loop)
{
	PlaySound_native(sfxId, play, loop);
}

void Platform_Android::OnAudioPause()
{
	OnAudioPause_native();
}

void Platform_Android::OnAudioResume()
{
	OnAudioResume_native();
}

bool Platform_Android::IsMusicPlaying()
{
	return IsMusicPlaying_native();
}

float Platform_Android::GetTimeInSeconds()
{
	return GetTimeInSeconds_native();
}

int Platform_Android::GetTimeInMillis()
{
	return GetTimeInMillis_native();
}

void Platform_Android::ShowMsg(char* str)
{
	ShowMsg_native(str);
}

void Platform_Android::SleepThread(int timeInMillis)
{
	SleepThread_native(timeInMillis);
}

void Platform_Android::SetWindowTitle(char* str)
{
}

bool Platform_Android::IsNetworkAvailable()
{
	return IsNetworkAvailable_native();
}


#endif
