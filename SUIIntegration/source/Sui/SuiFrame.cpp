#include "SuiFrame.h"
#include "SuiIncludes.h"

#include "SuiInput.h"
#include "SuiFont.h"
#include "SuiBox.h"
#include "SuiManager.h"

SuiFrame::SuiFrame(float x, float y, float w, float h, int alignment) : SuiComponent(FRAME)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_titleBarRect.SetBounds(_x, _y, _w, 22);
	_dragRect.SetBounds((float)(_x+_w-SLIDER_WIDTH), (float)(_y+_h-SLIDER_WIDTH), (float)SLIDER_WIDTH, (float)SLIDER_WIDTH);

	_alignment = alignment;
	_elementVec.clear();
	_isBgVisible = true;
	_isBorderVisible = true;
	_focusGain = false;

	_closeRect.SetColor(255,0,100,255);
	_minRect.SetColor(150,150,255,255);
	_maxRect.SetColor(150,150,255,255);

	_removeCloseOption = false;

	SetBoundsToMinMaxClose();

	_isMinimized = false;
	_isMaximized = false;
	_miniIconRect.SetBounds(0,0,0,0);
	_beforeMaxRect.SetBounds(0,0,0,0);

	_bgR = 120;
	_bgG = 120;
	_bgB = 120;
	_bgA = 255;

	_activeBar = NONE;
	_dxOnActive = 0.0f;
	_dyOnActive = 0.0f;

	//Scroller realted..
	_scrollAmount = 0;
	_totViewHeight = 0;
	_scroller = new SuiScroller(_x+_w-(float)SLIDER_WIDTH, _y+_titleBarRect.h, (float)SLIDER_WIDTH, _h-_titleBarRect.h-SLIDER_WIDTH);
	////////////////////////

	SuiManager::GetInstance()->AddFrame(this);

	_resetBoundsCalled = false;
}

SuiFrame::~SuiFrame()
{
	if(_scroller)
	{
		delete _scroller;
		_scroller = NULL;
	}

	for(int i=0; i<(int)_elementVec.size(); i++)
	{
		delete _elementVec[i];
		_elementVec[i] = NULL;
	}

	SuiManager::GetInstance()->RemoveFrame(this);
}

void SuiFrame::SetFocusOn()
{
	SuiManager::GetInstance()->SetFocusOn(this);	
}

void SuiFrame::SetFocusOnFromManager(bool focus)
{
	_focusGain = focus;

	if(!_focusGain)
	{
		_activeBar = NONE;
		_dxOnActive = 0.0f;
		_dyOnActive = 0.0f;
	}
}

bool SuiFrame::IsMinimized()
{
	return _isMinimized;
}

bool SuiFrame::IsMaximized()
{
	return _isMaximized;
}

void SuiFrame::SetMinimized(bool val)
{
	_isMinimized = val;
}

void SuiFrame::SetMaximized(bool val)
{
	_isMaximized = val;

	if(_isMaximized)
	{
		_beforeMaxRect.SetBounds(_x,_y,_w,_h);

		_x = 0;
		_y = 0;
		_w = SuiManager::GetInstance()->GetWindowWidth();
		_h = SuiManager::GetInstance()->GetWindowHeightWithoutStatusBar();
	}
	else
	{
		_x = _beforeMaxRect.x;
		_y = _beforeMaxRect.y;
		_w = _beforeMaxRect.w;
		_h = _beforeMaxRect.h;
	}
}

void SuiFrame::SetRemoveCloseOption(bool remove)
{
	_removeCloseOption = remove;
	SetBoundsToMinMaxClose();
}

void SuiFrame::Add(SuiComponent* com)
{
	_elementVec.push_back(com);
}

SuiComponent* SuiFrame::getComponent(int index)
{
	if(index >=0 && index < _elementVec.size())
		return _elementVec[index];
	return NULL;
}

int SuiFrame::GetNumComponents()
{
	return _elementVec.size();
}

void SuiFrame::SetBounds(float x, float y, float w, float h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

SuiComponent* SuiFrame::getComponentAt(float x, float y)
{
	if(_activeBar != NONE || _titleBarRect.Contains(x,y) 
		|| (_scroller->IsScrollEnabled() && _scroller->Contains(x, y)) || _dragRect.Contains(x, y))
		return this;

	if(!Contains(x,y))
		return NULL;

	for(int i=0; i<(int)_elementVec.size(); i++)
	{
		if(_elementVec[i]->IsVisible() == false)
				continue;

		SuiComponent* com = _elementVec[i];

		if(com->GetComponentID() == SuiComponent::BOX)
		{
			SuiComponent* returnCom = ((SuiBox*)_elementVec[i])->getComponentAt(x,y);
			if(returnCom != NULL)
				return returnCom;
		}
		else if(com->Contains(x,y))
			return com;
	}
	return this;
}

SuiEvents SuiFrame::UpdateByInput()
{
	SuiEvents eventsVec;

	if(IsVisible() == false)
		eventsVec;

	float mx = (float)SuiInput::MX;
	float my = (float)SuiInput::MY;

	if(SuiInput::IsMouseClicked())
	{
		if(_dragRect.Contains(mx, my))
		{
			if(!_isMaximized)
			{
				_activeBar = DRAGGER;
				_dxOnActive = SuiInput::MX - _dragRect.x;
				_dyOnActive = SuiInput::MY - _dragRect.y;
			}
		}
		else if(_scroller->IsScrollEnabled() && _scroller->Contains(mx, my))
		{
			_activeBar = SCROLL_BAR;
			_dxOnActive = SuiInput::MX - _scroller->GetX();
			_dyOnActive = SuiInput::MY - _scroller->GetY();
		}
		else if(TitleBarContains(mx, my))
		{
			_activeBar = TITLE_BAR;
			_dxOnActive = SuiInput::MX - _titleBarRect.x;
			_dyOnActive = SuiInput::MY - _titleBarRect.y;
		}
		else
		{
			_activeBar = NONE;
			_dxOnActive = 0.0f;
			_dyOnActive = 0.0f;
		}
	}
	else if(SuiInput::IsMousePressed())
	{
		if(_activeBar == DRAGGER)
		{
			if(!_isMaximized)
			{
				float draggerNewX = SuiInput::MX - _dxOnActive;
				float draggerNewY = SuiInput::MY - _dyOnActive;

				float draggerRBX = draggerNewX + _dragRect.w;	// RBX = Right Bottom X position.
				float draggerRBY = draggerNewY + _dragRect.h;

				float frameNewW = draggerRBX - _x;
				float frameNewH = draggerRBY - _y;

				SetBounds(_x, _y, frameNewW, frameNewH);
			}
		}
		else if(_activeBar == TITLE_BAR)
		{
			_x = SuiInput::MX - _dxOnActive;
			_y = SuiInput::MY - _dyOnActive;
		}
		else if(_activeBar == SCROLL_BAR)
		{
			_scroller->Update();
			if(_scroller->IsScrollEnabled())
				Scroll(_scroller->GetScrollPercent());
		}
	}
	else if(SuiInput::IsMouseReleased())
	{
		if(_maxRect.Contains(mx, my))
		{
			SetMaximized( !IsMaximized() );
		}
		else if(_minRect.Contains(mx, my))
		{
			_isMinimized = true;

			_activeBar = NONE;
			_dxOnActive = -1;
			_dyOnActive = -1;
		}
		else
		{
			_activeBar = NONE;
			_dxOnActive = -1;
			_dyOnActive = -1;
		}
	}

	ResetBounds();

	return eventsVec;
}

bool SuiFrame::TitleBarContains(float x, float y)
{
	return (_titleBarRect.Contains(x, y) && !IsMinMaxCloseContains(x, y));
}

void SuiFrame::Scroll(float percent)
{
	float comAreaH = _h - _titleBarRect.h;
	float scrollH = (_totViewHeight - comAreaH) * percent / 100.0f;
	
	float moveInY = -(scrollH - _scrollAmount);
	MoveElements(0, moveInY);

	_scrollAmount -= moveInY;
}

void SuiFrame::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;

	_titleBarRect.Move(dx, dy);
	_scroller->Move(dx, dy);
	_dragRect.Move(dx, dy);
	_closeRect.Move(dx, dy);

	MoveElements(dx, dy);
}

void SuiFrame::MoveElements(float dx, float dy)
{
	for(int i=0; i<(int)_elementVec.size(); i++)
		_elementVec[i]->Move(dx, dy);
}

void SuiFrame::SetBoundsToMinMaxClose()
{
	float startX = (float)(_x+_w-TITLE_BAR_HEIGHT);
	float startY = (float)(_y+2);
	float width = (float)(TITLE_BAR_HEIGHT*0.9f);
	float height = (float)(TITLE_BAR_HEIGHT*0.75f);

	_closeRect.SetBounds(startX,					startY, width, height);
	
	if(_removeCloseOption)
		_closeRect.w = 0;

	_maxRect.SetBounds(_closeRect.x-_closeRect.w,	startY, width, height);
	_minRect.SetBounds(_maxRect.x-_maxRect.w,		startY, width, height);
}

bool SuiFrame::IsMinMaxCloseContains(float x, float y)
{
	return (_closeRect.Contains(x,y) || _maxRect.Contains(x,y) || _minRect.Contains(x,y));
}

void SuiFrame::RectifyFrameBounds()
{
	if(_activeBar == TITLE_BAR)
	{
		if(_x+_w-SLIDER_WIDTH < 0)
			_x = -_w+SLIDER_WIDTH;

		float screenWidth = SuiManager::GetInstance()->GetWindowWidth();

		if(_x+SLIDER_WIDTH > screenWidth)
			_x = screenWidth-SLIDER_WIDTH;
	}

	if(_y < 0)
		_y = 0;

	if(_w < TITLE_BAR_HEIGHT*4)
		_w = TITLE_BAR_HEIGHT*4;

	if(_h < TITLE_BAR_HEIGHT*2 + _dragRect.h)
		_h = TITLE_BAR_HEIGHT*2 + _dragRect.h;
}

bool SuiFrame::IsResetBoundsCalled()
{
	return _resetBoundsCalled;
}

void SuiFrame::ResetBounds()
{
	_resetBoundsCalled = true;

	//START : Reset title slider and components which are attached to SuiFrame with scroll amount.

	RectifyFrameBounds();

	_titleBarRect.SetBounds(_x, _y, _w, (float)TITLE_BAR_HEIGHT);
	SetBoundsToMinMaxClose();

	ResetBounds(_x, _y+_titleBarRect.h-_scrollAmount, _w);

	_scroller->SetBounds(_x + _w - (float)SLIDER_WIDTH, _y+_titleBarRect.h, (float)SLIDER_WIDTH, _h-_titleBarRect.h-SLIDER_WIDTH);
	_scroller->SetTotalViewLength(_totViewHeight);
	_scroller->SetViewLength(_h-_titleBarRect.h);
	_scroller->SetScrollPercent(0);
	_scroller->Calc();

	_dragRect.SetBounds((float)(_x+_w-SLIDER_WIDTH), (float)(_y+_h-SLIDER_WIDTH), (float)SLIDER_WIDTH, (float)SLIDER_WIDTH);

	//END : //////////////////////////////////////////////////////////////////////////////////////



	//START : Calculate ScrollPercent based on _scrollAmount and Set it Slider.

	float totMovable = _totViewHeight - (_h-_titleBarRect.h);
	float scrollPercent = _scrollAmount * 100.0f / totMovable;
	if(totMovable < 0)
	{
		scrollPercent = 0;
		_scrollAmount = 0;
	}
	_scroller->SetScrollPercent(scrollPercent);
	_scroller->Calc();

	//END : //////////////////////////////////////////////////////////////////////////////////////


	if(_scroller->IsScrollEnabled())
		ResetBounds(_x, _y+_titleBarRect.h-_scrollAmount, _w- SLIDER_WIDTH);
}

void SuiFrame::ResetBounds(float x, float y, float w)
{
	x += _leftMargin;
	w -= _leftMargin + _rightMargin;
	y += _topMargin;

	_totViewHeight = 0;

	if(_alignment == H_ALIGNMENT)
	{
		float eachBoxW = w / _elementVec.size();

		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			if(_elementVec[i]->IsVisible() == false)
				continue;

			float xx = x+i*eachBoxW + _elementVec[i]->LeftMargin();
			float yy = y			+ _elementVec[i]->TopMargin();
			float ww = eachBoxW		- (_elementVec[i]->LeftMargin() + _elementVec[i]->RightMargin());

			_elementVec[i]->SetPos(xx, yy);
			_elementVec[i]->SetWidth(ww);
			_elementVec[i]->ResetBounds();

			float endY = _elementVec[i]->GetH()+_elementVec[i]->TopMargin()+_elementVec[i]->BottomMargin();

			if(endY > _totViewHeight)
				_totViewHeight = endY;
		}
	}
	else if(_alignment == V_ALIGNMENT)
	{
		for(int i=0; i<(int)_elementVec.size(); i++)
		{
			if(_elementVec[i]->IsVisible() == false)
				continue;

			float xx = x + _elementVec[i]->LeftMargin();
			float yy = y + _elementVec[i]->TopMargin();
			float ww = w - (_elementVec[i]->LeftMargin() + _elementVec[i]->RightMargin());

			_elementVec[i]->SetPos(xx,yy+_totViewHeight);
			_elementVec[i]->SetWidth(ww);
			_elementVec[i]->ResetBounds();

			_totViewHeight += _elementVec[i]->GetH()+_elementVec[i]->TopMargin()+_elementVec[i]->BottomMargin();
		}
	}

	_totViewHeight += _topMargin + _bottomMargin;
}

void SuiFrame::Draw()
{
	GLboolean isScissorEnable = false;
	GLint scissorBox[4];

	glGetBooleanv(GL_SCISSOR_TEST, &isScissorEnable);
	glGetIntegerv(GL_SCISSOR_BOX, scissorBox);

	int scX = _x-1;
	int scY = _y;
	int scW = _w+1;
	int scH = _h+1;

	glEnable(GL_SCISSOR_TEST);

	//AppInfo* appInfo = AppInfo::GetInstance();
	//int newScX = appInfo->viewX + appInfo->viewW * scX / appInfo->baseWindowW;
	//int newScY = appInfo->viewY + appInfo->viewH * scY / appInfo->baseWindowH;
	//int newScW = appInfo->viewW * scW / appInfo->baseWindowW;
	//int newScH = appInfo->viewH * scH / appInfo->baseWindowH;

	float viewX = 0;
	float viewY = 0;
	float viewW = 800;
	float viewH = 600;

	float baseWindowW = 800;
	float baseWindowH = 600;

	float currWindowH = 800;

	int newScX =viewX + viewW * scX / baseWindowW;
	int newScY =viewY + viewH * scY / baseWindowH;
	int newScW =viewW * scW / baseWindowW;
	int newScH =viewH * scH / baseWindowH;

	glScissor(newScX, (GLint)(currWindowH-newScY-newScH), newScW, newScH);

	//glScissor(scX, (GLint)(SuiManager::GetInstance()->GetWindowHeight()-scY-scH), scW, scH);

	SuiFont::GetInstance()->EnableBatch();

	if(_isBgVisible)
		DrawBackground();

	for(int i=0; i<(int)_elementVec.size(); i++)
	{
		if(_elementVec[i]->IsVisible())
			_elementVec[i]->Draw();
	}

	if(!_isMaximized)
		DrawDragger();

	_scroller->Draw();

	DrawTitleBar();

	if(_isBorderVisible)
		DrawBorder();

	SuiFont::GetInstance()->DrawBatch();

	//Reset Scissor state

	if(isScissorEnable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);

	glScissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]);
}

void SuiFrame::DrawTitleBar()
{
	if(_focusGain)
		_titleBarRect.SetColor(200,200,255,255);
	else
		_titleBarRect.SetColor(130,130,255,255);

	_titleBarRect.DrawWithoutBorder();

	float titleH = _titleBarRect.h;

	SuiFont::GetInstance()->SetColor(0xffffffff);
	SuiFont::GetInstance()->Begin();

	SuiFont::GetInstance()->HorBorder(_x, _x+_w);

	float horGap = -0.5;
	float fontSize = SuiFont::GetInstance()->GetFontSize();

	if(_nameAlignment == LEFT)			SuiFont::GetInstance()->DrawFromLeft(_name, _x+1, _y+titleH/2, fontSize);
	else if(_nameAlignment == RIGHT)	SuiFont::GetInstance()->DrawFromRight(_name, _x+_w+1, _y+titleH/2, fontSize);
	else								SuiFont::GetInstance()->DrawFromCenter(_name, _x+1+_w/2, _y+titleH/2, fontSize);
	SuiFont::GetInstance()->End();

	DrawMinMaxClose();
}

void SuiFrame::DrawMinMaxClose()
{
	_minRect.DrawWithoutBorder();
	SuiRect::Draw(_minRect.x+_minRect.w*0.2f, _minRect.y+_minRect.h*0.55f, _minRect.w - _minRect.w*0.4f, 3.0f, 255,255,255,255, false);

	_maxRect.DrawWithoutBorder();

	float xx = _maxRect.x + _maxRect.w*0.2f;
	float yy = _minRect.y + _minRect.h*0.2f;
	float ww = _minRect.w - _minRect.w*0.4f;
	float hh = _minRect.h - _minRect.h*0.4f;

	if(_isMaximized)
	{
		xx += 1.0f;
		yy += 1.0f;
		ww -= 2.0f;
		hh -= 2.0f;
	}

	SuiRect::Draw(xx,yy,ww,hh, 255,255,255,255, false);
	SuiRect::Draw(xx+2.0f, yy+2.0f, ww-4.0f ,hh-4.0f, _minRect.color[0],_minRect.color[1],_minRect.color[2],_minRect.color[3], false);

	if(_removeCloseOption == false)
	{
		_closeRect.DrawWithoutBorder();

		glColor4ub(255,255,255,255);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glVertex2f((float)(int)(_closeRect.x + 4.0f),					(float)(int)(_closeRect.y + 3.0f)					);
		glVertex2f((float)(int)(_closeRect.x + _closeRect.w - 5.0f),	(float)(int)(_closeRect.y + _closeRect.h - 3.0f)	);
		glVertex2f((float)(int)(_closeRect.x + _closeRect.w - 5.0f),	(float)(int)(_closeRect.y + 3.0f)					);
		glVertex2f((float)(int)(_closeRect.x + 4.0f),					(float)(int)(_closeRect.y + _closeRect.h - 3.0f)	);
		glEnd();
		glLineWidth(1.0f);
	}

	glLineWidth(1.0f);
	_minRect.DrawBorder();
	_maxRect.DrawBorder();
	
	if(_removeCloseOption == false)
		_closeRect.DrawBorder();
}

void SuiFrame::DrawDragger()
{
	//_dragRect.Draw();

	glColor4ub(0,0,0,255);
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	for(float i=0; i<1.0f; i+=0.20f)
	{
		glVertex2f(_dragRect.x+_dragRect.w,			_dragRect.y+_dragRect.h*i);
		glVertex2f(_dragRect.x+_dragRect.w*i,		_dragRect.y+_dragRect.h);
	}
	glEnd();
}
