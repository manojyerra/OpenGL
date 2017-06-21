#ifndef PLATFORM_H
#define PLATFORM_H
#pragma once

#include "../MyDefines.h"

#ifdef  PLATFORM_MARMALADE
	#include "IwSoundSpec.h"
	#include "IwSoundInst.h"
	#include "IwResHandlerWAV.h"
#endif

//#include "../Sound/SoundsList.h"
#include <string>
using namespace std;

class Platform
{
private:
	#ifdef PLATFORM_MARMALADE
		static CIwSoundSpec* _soundSpecs[SoundsList::TOTAL_SOUNDS];
		static CIwSoundInst* _soundInst[SoundsList::TOTAL_SOUNDS];
	#endif

	#ifdef  PLATFORM_WIN32
		static HDC hDC;
		static bool _quit;
		static int MousePosX;
		static int MousePosY;
		static bool isMouseClicked;
		static bool isMousePressed;
		static bool isMouseReleased;
		static bool isRightMouseClicked;
		static bool isRightMousePressed;
		static bool isRightMouseReleased;
	#endif

public:

	#ifdef  PLATFORM_WIN32

	static HWND hWnd;
	static HINSTANCE hInstance;

	static int PrevMousePosX;
	static int PrevMousePosY;

	static bool PREV_LEFT_BUTTON_DOWN;
	static bool LEFT_BUTTON_DOWN;

	static bool PREV_RIGHT_BUTTON_DOWN;
	static bool RIGHT_BUTTON_DOWN;

	static int currKeyStates[256];
	static int prevKeyStates[256];
	static float timeCountForKeyPress[256];

	#define KEYS_CLEAR          0x0C
	#define KEYS_RETURN         0x0D

	#define KEYS_SHIFT          0x10
	#define KEYS_CONTROL        0x11
	#define KEYS_MENU           0x12
	#define KEYS_PAUSE          0x13
	#define KEYS_CAPITAL        0x14

	#define KEYS_KANA           0x15
	#define KEYS_HANGEUL        0x15 
	#define KEYS_HANGUL         0x15
	#define KEYS_JUNJA          0x17
	#define KEYS_FINAL          0x18
	#define KEYS_HANJA          0x19
	#define KEYS_KANJI          0x19

	#define KEYS_ESCAPE         0x1B

	#define KEYS_CONVERT        0x1C
	#define KEYS_NONCONVERT     0x1D
	#define KEYS_ACCEPT         0x1E
	#define KEYS_MODECHANGE     0x1F

	#define KEYS_SPACE          0x20
	#define KEYS_PRIOR          0x21
	#define KEYS_NEXT           0x22
	#define KEYS_END            0x23
	#define KEYS_HOME           0x24
	#define KEYS_LEFT           0x25
	#define KEYS_UP             0x26
	#define KEYS_RIGHT          0x27
	#define KEYS_DOWN           0x28
	#define KEYS_SELECT         0x29
	#define KEYS_PRINT          0x2A
	#define KEYS_EXECUTE        0x2B
	#define KEYS_SNAPSHOT       0x2C
	#define KEYS_INSERT         0x2D
	#define KEYS_DELETE         0x2E
	#define KEYS_HELP           0x2F

	#define KEYS_NUMPAD0        0x60
	#define KEYS_NUMPAD1        0x61
	#define KEYS_NUMPAD2        0x62
	#define KEYS_NUMPAD3        0x63
	#define KEYS_NUMPAD4        0x64
	#define KEYS_NUMPAD5        0x65
	#define KEYS_NUMPAD6        0x66
	#define KEYS_NUMPAD7        0x67
	#define KEYS_NUMPAD8        0x68
	#define KEYS_NUMPAD9        0x69
	#define KEYS_MULTIPLY       0x6A
	#define KEYS_ADD            0x6B
	#define KEYS_SEPARATOR      0x6C
	#define KEYS_SUBTRACT       0x6D
	#define KEYS_DECIMAL        0x6E
	#define KEYS_DIVIDE         0x6F
	#define KEYS_F1             0x70
	#define KEYS_F2             0x71
	#define KEYS_F3             0x72
	#define KEYS_F4             0x73
	#define KEYS_F5             0x74
	#define KEYS_F6             0x75
	#define KEYS_F7             0x76
	#define KEYS_F8             0x77
	#define KEYS_F9             0x78
	#define KEYS_F10            0x79
	#define KEYS_F11            0x7A
	#define KEYS_F12            0x7B

	#endif

	//Dialog
	static const int CONTINUE = 0;
	static const int BREAK = 1;
	static const int IGNORE_ALL = 2;

	//Result
	static const int RESULT_SUCCESS = 0;
	static const int RESULT_ERROR = 1;

	//SoundProperty
	static const int SOUND_OUTPUT_FREQ		= 1;
	static const int SOUND_DEFAULT_FREQ		= 2;
	static const int SOUND_NUM_CHANNELS		= 3;
	static const int SOUND_USED_CHANNELS	= 4;
	static const int SOUND_AVAILABLE		= 5;
	static const int SOUND_VOLUME_DEFAULT	= 6;
	static const int SOUND_STEREO_ENABLED	= 7;

	//AudioProperty
    static const int AUDIO_STATUS			= 1;
    static const int AUDIO_POSITION			= 2;
    static const int AUDIO_VOLUME_DEFAULT	= 3;
    static const int AUDIO_CHANNEL			= 4;
    static const int AUDIO_NUM_CHANNELS		= 5;
    static const int AUDIO_AVAILABLE		= 6;
    static const int AUDIO_MUTES_S3ESOUND	= 7;

	static void Init();
	static void Swap_Buffers();
	static unsigned long TimerGetMs();
	static int GetWindowWidth();
	static int GetWindowHeight();
	static int DebugAssertShow(const char* string);
	static int SecureStorageGet(void* data, unsigned int size);
	static int SecureStoragePut(void* data, unsigned int size);

	static void UpdateMouse();
	static int MouseX();
	static int MouseY();
	static bool IsMouseClicked();
	static bool IsMousePressed();
	static bool IsMouseReleased();
	static bool IsRightMousePressed();
	static bool IsRightMouseReleased();
	static bool IsRightMouseClicked();

	static void UpdateKeyBoard(float deltaTime);
	static bool IsKeyTyped(int key);
	static bool IsKeyPressed(int key);
	static bool IsKeyReleased(int key);
	static bool IsKeyPressedStill(int key, float time);

	static void SoundInit();
	static void SoundUpdate();
	static void SoundTerminate();
	static void PlaySFX(const int soundID);
	static void PauseSFX(int soundID);
	static void ResumeSFX(int soundID);
	static void StopSFX(int soundID);
	static bool IsPlayingSFX(int soundID);
	static void SetSfxVolume(int soundID, float sfxVal);
	static float GetSfxVolume(int soundID);

	static void SetAllSfxVolume(float volume);
	static float GetAllSFXVolume();
	static void ResumeAllSFX();
	static void PauseAllSFX();
	static void StopAllSFX();

	static void AudioPlay(string filePath, int repeatCount);
	static void AudioPause();
	static void AudioResume();
	static void AudioStop();
	static bool AudioIsPlaying();
	static void SetAudioVolume(float volume);
	static float GetAudioVolume();

	//static void SoundSetInt(int soundProperty, int value);
	//static void AudioSetInt(int audioProperty, int value);

	static void SetWindowTitle(string title);

	static void DeviceYield(int milliSeconds);
	static bool DeviceCheckQuitRequest();
	static void DeviceExit();

	static void Terminate();
};

#endif
