#pragma once

#include "../CommonLib/Sprite.h"
#include "../CommonLib/UIFont.h"
#include "../Primitives/RectPrim.h"
#include "MyButton.h"

#include <string>
#include <vector>
using namespace std;

class MyWindow
{
private:
	vector<MyButton*> _componentVec;
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
	MyButton* _prevOnCom;
	MyButton* _justNowOnBtn;

	bool _isMoving;

public:
	MyWindow(float x, float y);
	~MyWindow();

	MyButton* Update();
	MyButton* AddButton(string label, int type = MyButton::PROPERTY);
	void NextRow();

	MyButton* GetActiveDialog();
	MyButton* GetActiveModeCom();
	MyButton* GetCom(int x, int y);

	void MovePos(float dx, float dy);
	bool Contains(float x, float y);

	void Draw();
};
