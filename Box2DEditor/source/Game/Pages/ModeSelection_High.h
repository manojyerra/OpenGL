#ifndef MODESELECTION_HIGH_H
#define MODESELECTION_HIGH_H
#pragma once

#include "Sprite.h"
#include "Button.h"
#include "Page.h"
#include "LeaderBoard.h"
#include "ColorBlendBg.h"

class ModeSelection_High : Page
{
private:

	Button* _proModeBtn;
	Button* _surModeBtn;
	Button* _backBtn;

	LeaderBoard* _leaderBoard;

	ColorBlendBg* _cBlendBg;

public:
	ModeSelection_High();
	~ModeSelection_High();
	void Update();
	void Draw();
};

#endif
