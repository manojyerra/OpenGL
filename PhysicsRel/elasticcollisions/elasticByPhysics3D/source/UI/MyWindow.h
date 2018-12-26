#pragma once

#include "../CommonLib/Sprite.h"
#include "../CommonLib/Button.h"
#include "../CommonLib/UIFont.h"
#include "../CommonLib/RectPrim.h"

#include "MyEvent.h"

#include <string>
#include <vector>
using namespace std;

class MyWindow
{
private:
	vector<Button*> _componentVec;
	float _x;
	float _y;
	float _w;
	float _h;

	float _startX;
	float _startY;
	float _windowX2;
	float _windowY2;
	
	float _horGapBtwComponents;
	float _VerGapBtwComponents;

	float _btnDefaultHeight;
	bool _goToNextRow;
	Button* _prevOnCom;
	Button* _justNowOnBtn;

	bool _isMoving;
	bool _isOn;

public:
	MyWindow();
	~MyWindow();

	void Init(float x, float y);
	Button* Update();

	void On();
	void Off();
	bool IsOn();

	Button* AddButton(string label, int type = Button::PROPERTY);
	void NextRow();

	Button* GetActiveDialog();
	Button* GetActiveModeCom();
	Button* GetCom(int x, int y);

	void MovePos(float dx, float dy);
	bool Contains(float x, float y);

	void Draw();
	void Terminate();
};
