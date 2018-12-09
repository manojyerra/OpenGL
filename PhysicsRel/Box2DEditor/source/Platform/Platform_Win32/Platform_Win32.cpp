#ifdef _PLATFORM_WIN32

#include "Platform_Win32.h"
#include "Win32Window.h"
#include "Includes.h"
#include <sys/stat.h>

Platform_Win32::Platform_Win32()
{
	_gameLoop = NULL;
	_musicPlayer = new MusicPlayer();
}

Platform_Win32::~Platform_Win32()
{
}

void Platform_Win32::onCreate()								{ _gameLoop = new GameLoop(); 				}
void Platform_Win32::onSizeChange(int width, int height)	{ _gameLoop->onSizeChange(width, height);	}
int Platform_Win32::Draw(int deltaTimeInMillis)				{ return _gameLoop->Draw(deltaTimeInMillis);}
void Platform_Win32::onTouch(int x, int y, int eventID)		{ _gameLoop->onTouch(x, y, eventID);		}
void Platform_Win32::onPause()								{ _gameLoop->onPause();						}
void Platform_Win32::onResume()								{ _gameLoop->onResume();					}
void Platform_Win32::onExit()
{
	if(_musicPlayer)
	{
		delete _musicPlayer;
		_musicPlayer = NULL;
	}

	if(_gameLoop)
	{
		delete _gameLoop;
		_gameLoop = NULL;
	}
}

void Platform_Win32::SetSaveFolderPath(std::string saveFolderPath)		{ _saveFolderPath = saveFolderPath; 	}
void Platform_Win32::SetDataFolderPath(std::string dataFolderPath)		
{	
	_dataFolderPath = dataFolderPath;	
}

string Platform_Win32::GetSaveFolderPath()
{
	return _saveFolderPath;
}

string Platform_Win32::GetDataFolderPath()
{
	return _dataFolderPath;
}

char* Platform_Win32::GetFileData(std::string filePath, int* fileMemSize, int readFrom)
{
	std::string finalPath = "";

	if(readFrom == READ_FROM_IM)
	{
		finalPath = _saveFolderPath;
	}
	else if(readFrom == READ_FROM_ASSETS)
	{
		finalPath = _dataFolderPath;
	}

	finalPath.append("/");
	finalPath.append(filePath.c_str());


	FILE  *fp = fopen(finalPath.c_str(), "rb");

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
	return NULL;
}

bool Platform_Win32::SetSaveData(void* data, unsigned int size)
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

bool Platform_Win32::GetSaveData(void* data, unsigned int size)
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

void Platform_Win32::PlayMusic(bool play)
{
	_musicPlayer->Play(play);
}

void Platform_Win32::PlaySFX(int sfxId, bool play, bool loop)
{
}

void Platform_Win32::OnAudioPause()
{
}

void Platform_Win32::OnAudioResume()
{
}


bool Platform_Win32::IsMusicPlaying()
{
	return _musicPlayer->IsPlaying();
}

float Platform_Win32::GetTimeInSeconds()
{	
	return (float)GetTickCount() / 1000.0f;
}

int Platform_Win32::GetTimeInMillis()
{
	return GetTickCount();
}

void Platform_Win32::ShowMsg(char* str)
{
	printLog("\n%s", str);
}

void Platform_Win32::SetBackKeyPressed()
{
	_gameLoop->SetBackKeyPressed();
}

void Platform_Win32::SleepThread(int timeInMillis)
{
	Sleep(timeInMillis);
}

void Platform_Win32::SetWindowTitle(char* str)
{
	SetWindowText(Win32Window::hWnd1, str);
}

bool Platform_Win32::IsNetworkAvailable()
{
	return false;
}

#endif
