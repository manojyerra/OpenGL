#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "Button.h"
#include "GameInit.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Editor/Editor.h"

class GameLoop
{
private:
	int _textureID;
	bool _init;
	bool _onResume;

	GameInit* _gameInit;
	Button* _sfxBtn;
	Button* _bgmBtn;

	//Begin : Common to all games.
	int _frameCountForFPS;
	int _timeCountForFPS;
	int _fps;

	void DisplayFPS(int deltaTimeInMillis);
	//End : Common to all games.

	Editor* _editor;

public:
	GameLoop();
	~GameLoop();
	void Init();

	void onCreate();
	void onSizeChange(int surfaceW, int surfaceH);
	void Update(int deltaTimeInMillis);
	int Draw(int deltaTimeInMillis);
	void onTouch(int x, int y, int eventID);
	void onPause();
	void onResume();

	void SetLanguageID(int languageID);
	void SetBackKeyPressed();
	void SetAccMeterMoved(int dir);
};

#endif
