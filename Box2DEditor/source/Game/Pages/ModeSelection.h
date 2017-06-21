#ifndef MODESELECTION_H
#define MODESELECTION_H
#pragma once

#include "Page.h"

#include "ColorRect.h"
#include "BlendTransBg.h"

//#include "Button.h"
//#include "ColorBlendBg.h"

class ModeSelection : Page
{
private:
	/*
	Button* _proModeBtn;
	Button* _surModeBtn;
	Button* _backBtn;
	
	ColorBlendBg* _cBlendBg;
	*/

	ColorRect* _proModeBtn;
	ColorRect* _surModeBtn;
	ColorRect* _backBtn;
	
	BlendTransBg* _cBlendBg;

public:
	ModeSelection();
	~ModeSelection();
	void Update();
	void Draw();
};

#endif
