#ifndef MAINMENU_H
#define MAINMENU_H
#pragma once

#include "Sprite.h"
#include "Button.h"
#include "Page.h"
#include "RectPrim.h"
#include "MsgBox.h"
#include "AddData.h"
#include "ColorBlendBg.h"
#include "ColorRect.h"

class MainMenu : Page
{
private:
	Button* _playBtn;
	Button* _highScoresBtn;
	Button* _submitDetailsBtn;
	Button* _reviewBtn;
	Button* _shareOnGPlusBtn;
	Button* _exitBtn;
	Button* _musicOnOffBtn;
	Button* _sfxOnOffBtn;
	
	ColorBlendBg* _cBlendBg;	

	//Begin : RateMsgBox
	MsgBox* _rateMsgBox;
	ColorRect* _rateMsgBg;
	ColorRect* _yesBtn;
	ColorRect* _laterBtn;

	void RateDialogInit();
	void DrawRateDialog();
	void UpdateRateDialog();
	//End : RateMsgBox

public:
	MainMenu();
	~MainMenu();
	void Update();
	void Draw();

	bool IsSubmitDetailsBtnClicked();
	bool IsReviewBtnClicked();
	bool IsShareOnGPlusBtnClicked();
	bool IsShareOnFacebookBtnClicked();
	bool IsExitClicked();
};

#endif
