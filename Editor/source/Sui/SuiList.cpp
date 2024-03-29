#include "SuiList.h"
#include "SuiIncludes.h"
#include "SuiInput.h"
#include "SuiEvents.h"
#include "SuiManager.h"
#include "AppInfo.h"

SuiList::SuiList(int elementsToShow) : SuiComponent(SUI_LIST)
{
	_elementsToShow = elementsToShow;
	_elementsVec.clear();

	_isBorderVisible = true;
	_isBgVisible = true;
	_selectedIndex = -1;

	_selectedColor[0] = 180;
	_selectedColor[1] = 190;
	_selectedColor[2] = 255;
	_selectedColor[3] = 255;
	
	_nonSelectedColor[0] = 200;
	_nonSelectedColor[1] = 200;
	_nonSelectedColor[2] = 200;
	_nonSelectedColor[3] = 255;

	_bgR = 255;
	_bgG = 3;
	_bgB = 98;
	_bgA = 255;

	_actionListener = NULL;
	_mouseListener = NULL;
	_isDisableFrameScissor = false;

	//Scroller related...
	_scrollAmount = 0;
	_totViewHeight = 0;
	_scroller = new SuiScroller(_x+_w-(float)SLIDER_WIDTH, _y, (float)SLIDER_WIDTH, _h-SLIDER_WIDTH);
	_scrollActive = false;
	////////////////////
}

SuiList::~SuiList()
{
	for(int i=0; i<(int)_elementsVec.size(); i++)
	{
		delete _elementsVec[i];
		_elementsVec[i] = NULL;
	}

	if(_scroller)
	{
		delete _scroller;
		_scroller = NULL;
	}
}

void SuiList::SetSelectColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_selectedColor[0] = r;
	_selectedColor[1] = g;
	_selectedColor[2] = b;
	_selectedColor[3] = a;

	for(int i=0;i<_elementsVec.size();i++)
		_elementsVec[i]->SetBgColor(_nonSelectedColor);

	if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
		_elementsVec[_selectedIndex]->SetBgColor(_selectedColor);
}

void SuiList::SetNonSelectColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_nonSelectedColor[0] = r;
	_nonSelectedColor[1] = g;
	_nonSelectedColor[2] = b;
	_nonSelectedColor[3] = a;

	for(int i=0;i<_elementsVec.size();i++)
		_elementsVec[i]->SetBgColor(_nonSelectedColor);

	if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
		_elementsVec[_selectedIndex]->SetBgColor(_selectedColor);
}

void SuiList::Add(string name)
{
	SuiLabel* label = new SuiLabel(name, SuiLabel::LEFT);
	label->SetBgColor(_nonSelectedColor);
	label->SetBgVisible(true);
	label->SetBorderVisible(true);
	label->SetFontColor(0x000000ff);

	_elementsVec.push_back(label);

	if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
		_elementsVec[_selectedIndex]->SetBgColor(_selectedColor);
}

int SuiList::Size()
{
	return _elementsVec.size();
}

void SuiList::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;

	_scroller->Move(dx, dy);
	MoveElements(dx, dy);
}

void SuiList::MoveElements(float dx, float dy)
{
	for(int i=0; i<(int)_elementsVec.size(); i++)
		_elementsVec[i]->Move(dx, dy);
}

void SuiList::Scroll(float percent)
{
	float comAreaH = _h;
	float scrollH = (_totViewHeight - comAreaH) * percent / 100.0f;
	
	float moveInY = -(scrollH - _scrollAmount);
	MoveElements(0, moveInY);

	_scrollAmount -= moveInY;
}

void SuiList::ResetBounds()
{
	ResetBounds(_x, _y-_scrollAmount, _w);

	_scroller->SetBounds(_x + _w - (float)SLIDER_WIDTH, _y, (float)SLIDER_WIDTH, _h);
	_scroller->SetTotalViewLength(_totViewHeight);
	_scroller->SetViewLength(_h);
	_scroller->SetScrollPercent(0);
	_scroller->Calc();

	//START : Calculate ScrollPercent based on _scrollAmount and Set it Slider.

	float totMovable = _totViewHeight - _h;

	if(totMovable > 0)
	{
		float scrollPercent = _scrollAmount * 100.0f / totMovable;
		if(totMovable < 0)
		{
			scrollPercent = 0;
			_scrollAmount = 0;
		}
		_scroller->SetScrollPercent(scrollPercent);
		_scroller->Calc();
	}

	//END : //////////////////////////////////////////////////////////////////////////////////////

	if(_scroller->IsScrollEnabled())
		ResetBounds(_x, _y-_scrollAmount, _w- SLIDER_WIDTH);
}

void SuiList::ResetBounds(float x, float y, float w)
{
	_h = 0;
	_totViewHeight = 0;

	for(int i=0; i<(int)_elementsVec.size(); i++)
	{
		_elementsVec[i]->SetPos(x,y+_totViewHeight);
		_elementsVec[i]->SetWidth(w);
		_elementsVec[i]->ResetBounds();

		if(i < _elementsToShow)
			_h += _elementsVec[i]->GetH();

		_totViewHeight += _elementsVec[i]->GetH();
	}
}

bool SuiList::Contains(float x, float y)
{
	if(_scrollActive)
		return true;
	return (x >= _x && x <= _x+_w && y >= _y && y <= _y+_h);
}

SuiEvents SuiList::UpdateByInput()
{
	SuiEvents eventsVec;

	float mx = (float)SuiInput::MX;
	float my = (float)SuiInput::MY;

	if(SuiInput::IsMouseClicked())
	{
		if(_scroller->Contains(mx, my))
		{
			_scrollActive = true;
			return eventsVec;
		}
		else
			_scrollActive = false;
	}
	else if(SuiInput::IsMousePressed())
	{
		if(_scrollActive)
		{
			_scroller->Update();
			if(_scroller->IsScrollEnabled())
				Scroll(_scroller->GetScrollPercent());

			return eventsVec;
		}
	}
	else if(SuiInput::IsMouseReleased())
	{
		_scrollActive = false;
		return eventsVec;
	}

	if(!Contains((float)SuiInput::MX, (float)SuiInput::MY))
		return eventsVec;

	if(SuiInput::IsMouseClicked())
	{
		for(int i=0; i<(int)_elementsVec.size(); i++)
		{
			if(_elementsVec[i]->Contains((float)SuiInput::MX, (float)SuiInput::MY) && _selectedIndex != i)
			{
				if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
					_elementsVec[_selectedIndex]->SetBgColor(_nonSelectedColor);
				
				_elementsVec[i]->SetBgColor(_selectedColor);
				_selectedIndex = i;

				if(_actionListener)
					eventsVec.ACTION_PERFORMED = true;
				break;
			}
		}
	}

	return eventsVec;
}

void SuiList::SetSelect(int index)
{
	if(index >= 0 && index < (int)_elementsVec.size())
	{
		if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
			_elementsVec[_selectedIndex]->SetBgColor(_nonSelectedColor);
		
		_selectedIndex = index;
		_elementsVec[_selectedIndex]->SetBgColor(_selectedColor);
	}
	else
	{
		if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
			_elementsVec[_selectedIndex]->SetBgColor(_nonSelectedColor);

		_selectedIndex = -1;
	}
}

int SuiList::GetSelectedIndex()
{
	return _selectedIndex;
}

string SuiList::GetSelectedStr()
{
	return GetItem(_selectedIndex);
}

string SuiList::GetItem(int index)
{
	if(index >= 0 && index < (int)_elementsVec.size())
		return _elementsVec[index]->GetName();

	string emptyStr = "";
	return emptyStr;
}

void SuiList::DisableScissor()
{
	_isDisableFrameScissor = true;
}

void SuiList::Draw()
{
	GLboolean isScissorEnable = false;
	GLint scissorBox[4];

	glGetBooleanv(GL_SCISSOR_TEST, &isScissorEnable);
	glGetIntegerv(GL_SCISSOR_BOX, scissorBox);

	float localY1 = _y;
	float localY2 = localY1 + (GLsizei)_h+1;

	if(!_isDisableFrameScissor)
	{
		float frameScissorY1 = SuiManager::GetInstance()->GetWindowHeight() - (scissorBox[1] + scissorBox[3]);
		float frameScissorY2 = frameScissorY1 + scissorBox[3];

		if(frameScissorY2 < localY1)
			return;

		if(localY1 < frameScissorY1)
			localY1 = frameScissorY1;

		if(localY2 > frameScissorY2)
			localY2 = frameScissorY2;
	}

	glEnable(GL_SCISSOR_TEST);

	AppInfo* appInfo = AppInfo::GetInstance();
	int newScX = appInfo->viewX + appInfo->viewW * (_x-1) / appInfo->baseWindowW;
	int newScY = appInfo->viewY + appInfo->viewH * localY1 / appInfo->baseWindowH;
	int newScW = appInfo->viewW * (_w+2) / appInfo->baseWindowW;
	int newScH = appInfo->viewH * (localY2-localY1) / appInfo->baseWindowH;

	glScissor(newScX, (GLint)(appInfo->currWindowH-newScY-newScH), newScW, newScH);

	//glScissor((GLint)(_x-1), (GLint)(SuiManager::GetInstance()->GetWindowHeight() - localY2), (GLsizei)(_w+2), localY2-localY1);

	if(_isBgVisible)
		DrawBackground();

	for(int i=0; i<(int)_elementsVec.size(); i++)
	{
		_elementsVec[i]->Draw();
	}

	if(_scroller)
		_scroller->Draw();

	if(_isBorderVisible)
		DrawBorder();


	//Reset Scissor state

	if(isScissorEnable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);

	glScissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]);
}