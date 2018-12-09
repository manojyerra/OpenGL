#include "MsgBox.h"
#include "GameInfo.h"
#include "UIFont.h"
#include "RectPrim.h"
#include "Input.h"

MsgBox::MsgBox(string msg)
{
	int ow = GameInfo::GetInstance()->CodedW();
	int oh = GameInfo::GetInstance()->CodedH();

	_isActive = false;

	int bgW = 100;
	int bgH = 100;
	
	_bg = new ColorRect((ow-bgW)/2, (oh-bgH)/2, bgW, bgH);

	_btnW = 135;
	_btnH = 50;

	_buttonIndex = -1;

	_msg = msg;

	_inActiveOnBackPress = false;
}

MsgBox::~MsgBox()
{
	if(_bg)
	{
		delete _bg;
		_bg = NULL;
	}

	for(int i=0;i<_btnVec.size();i++)
	{
		delete _btnVec[i];
		_btnVec[i] = NULL;
	}

	_btnVec.clear();
}

void MsgBox::SetMsg(string msg)
{
	_msg = msg;
}

void MsgBox::SetBgSize(int w, int h)
{
	int bgW = w;
	int bgH = h;
	
	int ow = GameInfo::GetInstance()->CodedW();
	int oh = GameInfo::GetInstance()->CodedH();

	_bg->SetBounds((ow-bgW)/2, (oh-bgH)/2, bgW, bgH);
}

void MsgBox::SetBtnSize(int w, int h)
{
	_btnW = w;
	_btnH = h;
}

void MsgBox::SetActive(bool isActive)
{
	_isActive = isActive;
}

bool MsgBox::IsActive()
{
	return _isActive;
}

int MsgBox::GetButtonIndex_OnClick()
{
	return _buttonIndex;
}

void MsgBox::SetInActiveOnBackPress(bool inActive)
{
	_inActiveOnBackPress = inActive;
}

void MsgBox::Update()
{
	_buttonIndex = -1;

	if(_isActive)
	{
		for(int i=0;i<_btnVec.size();i++)
		{
			//_btnVec[i]->Update();

			if(_btnVec[i]->IsClicked())
			{
				_buttonIndex = i;
				SetActive(false);
				return;
			}
		}

		if(_inActiveOnBackPress && Input::IsBackPressed())
			SetActive(false);
	}
}

void MsgBox::AddButton(string text)
{
	ColorRect* btn = new ColorRect(0,_bg->GetY()+_bg->GetH()-1.5*_btnH,_btnW, _btnH);
	_btnVec.push_back(btn);
}

void MsgBox::Draw()
{
	if(_isActive)
	{
		int ow = GameInfo::GetInstance()->CodedW();
		int oh = GameInfo::GetInstance()->CodedH();

		RectPrim::Draw(0,0,ow,oh, 0, 0x00000088);

		_bg->Draw();

		int bgX = _bg->GetX();
		int bgY = _bg->GetY();
		int bgW = _bg->GetW();
		int bgH = _bg->GetH();

		UIFont::GetInstance()->SetColor(255,255,255,255);
		UIFont::GetInstance()->Begin(1);
		UIFont::GetInstance()->DrawFromCenter(_msg, bgX+bgW/2, bgY+bgH/4, 30, -1);
		UIFont::GetInstance()->End();

		float numBtns = _btnVec.size();
		float gap = 20;
		float totW = (numBtns * _btnW) + ((numBtns-1) * gap);
		float startX = _bg->GetX() + _bg->GetW()/2 - totW/2;

		for(int i=0;i<_btnVec.size();i++)
		{
			_btnVec[i]->SetX(startX + i*(_btnW+gap));
			_btnVec[i]->Draw();
		}
	}
}