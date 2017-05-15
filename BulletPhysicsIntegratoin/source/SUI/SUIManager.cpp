#include "SUIManager.h"
#include "SUIInput.h"
#include "SUIFont.h"
#include "SUIIncludes.h"
#include "SUIEvents.h"
#include "SUIChoice.h"

SUIManager* SUIManager::_ref = 0;

void SUIManager::Setup(int windowWidth, int windowHeight)
{
	_windowWidth = (float)windowWidth;
	_windowHeight = (float)windowHeight;

	_statusBarRect.SetColor(0,0,255,120);
	_statusBarRect.SetBounds(0,_windowHeight*0.92f,_windowWidth,_windowHeight*0.08f);

	SUIInput::Init();
}

SUIManager::SUIManager()
{
	_activeFrame = NULL;
	_windowWidth = 0;
	_windowHeight = 0;
	_dialogCom = NULL;
	_statusBarRect.SetBounds(0,0,0,0);
	_newFrameAdded = false;
}

SUIManager* SUIManager::GetInstance()
{
	if(_ref == 0)
		_ref = new SUIManager();
	return _ref;
}

float SUIManager::GetWindowWidth()						{	return _windowWidth;					}
float SUIManager::GetWindowHeight()						{	return _windowHeight;					}
float SUIManager::GetWindowHeightWithoutStatusBar()		{	return _windowHeight-_statusBarRect.h;	}

void SUIManager::Run()
{
	Update();
	Draw();
}

unsigned int SUIManager::GetTimeInMilliSeconds()
{
	return GetTickCount();
}


///***********************************************************************************************************/////////



void SUIManager::AddFrame(SUIFrame* frame)
{
	_framesVec.push_back(frame);
	_newFrameAdded = true;
}

void SUIManager::RemoveFrame(SUIFrame* frame)
{
	for(int i=0;i<(int)_framesVec.size();i++)
	{
		if(_framesVec[i] == frame)
		{
			_framesVec.erase(_framesVec.begin() + i);
			break;
		}
	}
}

void SUIManager::ActiveFrame(int frameIndex)
{
	if(_activeFrame)
		_activeFrame->SetFocusOnFromManager(false);

	_activeFrame = _framesVec[frameIndex];
	_framesVec.erase(_framesVec.begin() + frameIndex);
	_framesVec.push_back(_activeFrame);

	if(_activeFrame)
		_activeFrame->SetFocusOnFromManager(true);
}

void SUIManager::SetFocusOn(SUIFrame* frame)
{
	for(int i=0;i<(int)_framesVec.size();i++)
	{
		if(_framesVec[i] == frame)
		{
			ActiveFrame(i);
			return;
		}
	}
}

void SUIManager::SetDialogCom(SUIComponent* dialogCom)
{
	_dialogCom = dialogCom;
}

void SUIManager::CheckForResetBounds()
{
	if(_newFrameAdded)
	{
		_newFrameAdded = false;
		
		for(int i=0;i<(int)_framesVec.size();i++)
		{
			if(_framesVec[i]->IsResetBoundsCalled() == false)
				_framesVec[i]->ResetBounds();
		}
	}
}

bool SUIManager::Contains(float mx, float my)
{
	for(int i=_framesVec.size()-1; i>=0; i--)
	{
		if(_framesVec[i]->IsVisible() && (_framesVec[i]->IsMinimized()==false) && _framesVec[i]->Contains(mx, my))
		{
			return true;
		}
	}

	float iConY = _statusBarRect.y;
	float iConH = _statusBarRect.h;
	float iConW = iConH*1.2f;

	float marginH = iConW*0.05f;
	float marginV = iConW*0.05f;

	int count = 0;

	for(int i=0;i<(int)_framesVec.size();i++)
	{
		if(_framesVec[i]->IsMinimized())
		{
			float x = (count*iConW)+marginH;
			float y = iConY+marginV;
			float w = iConW-2*marginH;
			float h = iConH-2*marginV;

			if(mx >= x && mx <= x+w && my >= y && my <= y+h)
				return true;

			count++;
		}
	}

	return false;
}

void SUIManager::Update()
{
	CheckForResetBounds();

	float mx = (float)SUIInput::MX;
	float my = (float)SUIInput::MY;

	if(_dialogCom != NULL)
	{
		if(_dialogCom->IsDialogMode())
			_dialogCom->UpdateDialog();
		else
		{
			SUIEvents eventsVec = _dialogCom->UpdateByInput();
			FireEvent(_dialogCom, eventsVec);
			_dialogCom = NULL;
		}

		return;
	}

	if(SUIInput::IsMouseClicked() || SUIInput::IsScrolled())
	{
		for(int i=_framesVec.size()-1; i>=0; i--)
		{
			if(_framesVec[i]->IsVisible() && (_framesVec[i]->IsMinimized()==false) && _framesVec[i]->Contains(mx, my))
			{
				if(_framesVec[i] != _activeFrame)
				{
					ActiveFrame(i);

					if(_activeFrame)
						_activeFrame->UpdateByInput();
				}

				break;
			}
		}
	}

	if(_activeFrame && !_activeFrame->IsMinimized())
	{
		if(	SUIInput::IsMouseClicked()  || SUIInput::IsMousePressed() || SUIInput::IsMouseReleased())
		{
			SUIComponent* com = _activeFrame->getComponentAt(mx, my);
		
			if(com)
			{
				FireEvent(com, com->UpdateByInput());
				_activeFrame->ResetBounds();
			}
		}
		else if(SUIInput::IsScrolled() && _activeFrame->Contains(mx, my))
		{
			_activeFrame->UpdateByInput();
		}
	}
}

void SUIManager::FireEvent(SUIComponent* com, SUIEvents &eventsVec)
{
	if(eventsVec.ACTION_PERFORMED == false)
		return;

	int id = com->GetComponentID();

	if(id == SUIComponent::BUTTON)
	{
		SUIActionListener* actionListener = ((SUIButton*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SUIActionEvent(com));
	}
	else if(id == SUIComponent::CHECKBOX)
	{
		SUIActionListener* actionListener = ((SUICheckBox*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SUIActionEvent(com));
	}
	else if(id == SUIComponent::RADIO_BUTTON)
	{
		SUIActionListener* actionListener = ((SUIRadioButton*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SUIActionEvent(com));
	}
	else if(id == SUIComponent::SLIDER)
	{
		SUIActionListener* actionListener = ((SUISlider*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SUIActionEvent(com));
	}
	else if(id == SUIComponent::SUI_CHOICE)
	{
		SUIActionListener* actionListener = ((SUIChoice*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SUIActionEvent(com));
	}
	else if(id == SUIComponent::SUI_LIST)
	{
		SUIActionListener* actionListener = ((SUIList*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SUIActionEvent(com));
	}
}


void SUIManager::Draw()
{
	GLboolean isDepthTestEnable = false;
	GLboolean isLightEnable = false;
	GLboolean isBlendEnable = false;
	GLboolean isLineSmoothEnable = false;
	GLboolean isTexture2DEnable = false;
	GLboolean isCullFaceEnable = false;
	int blendSRC = 0;
	int blendDST = 0;

	float lineWidth = 0.0f;
	int shadeModel = 0;

	GLfloat color[4];
	GLint viewPort[4];
	float projection[16];
	float modelView[16];
	
	glGetBooleanv(GL_DEPTH_TEST, &isDepthTestEnable);
	glGetBooleanv(GL_LIGHTING, &isLightEnable);
	glGetBooleanv(GL_BLEND, &isBlendEnable);
	glGetBooleanv(GL_LINE_SMOOTH, &isLineSmoothEnable);
	glGetBooleanv(GL_TEXTURE_2D, &isTexture2DEnable);
	glGetBooleanv(GL_CULL_FACE, &isCullFaceEnable);
	
	glGetIntegerv(GL_BLEND_SRC, &blendSRC);
	glGetIntegerv(GL_BLEND_DST, &blendDST);
	glGetIntegerv(GL_SHADE_MODEL, &shadeModel);
	glGetFloatv(GL_CURRENT_COLOR, color);
	glGetFloatv(GL_LINE_WIDTH, &lineWidth);

	glGetIntegerv(GL_VIEWPORT, viewPort);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);


	//Setting GLSetting 

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, _windowWidth, _windowHeight, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	for(int i=0;i<(int)_framesVec.size();i++)
	{
		if(!_framesVec[i]->IsMinimized() && _framesVec[i]->IsVisible())
			_framesVec[i]->Draw();
	}

	if(_dialogCom != NULL)
		_dialogCom->DrawDialog();

	_statusBarRect.DrawWithBorder();

	float iConY = _statusBarRect.y;
	float iConH = _statusBarRect.h;
	float iConW = iConH*1.2f;

	float marginH = iConW*0.05f;
	float marginV = iConW*0.05f;

	glLineWidth(1.0f);
	int count = 0;
	for(int i=0;i<(int)_framesVec.size();i++)
	{
		if(_framesVec[i]->IsMinimized())
		{
			float x = (count*iConW)+marginH;
			float y = iConY+marginV;
			float w = iConW-2*marginH;
			float h = iConH-2*marginV;

			SUIRect::Draw(x, y, w, h, 250,100,200,255, false);
			SUIRect::Draw(x, y, w, h, 0,0,0,255, true);
			SUIRect::Draw(x+2, y+2, w-4, h-4, 0,0,0,255, true);
			count++;

			SUIFont::GetInstance()->SetColor(0xffffffff);
			SUIFont::GetInstance()->Begin();
			SUIFont::GetInstance()->HorBorder(x, x+w);
			float horGap = -0.5f;
			float fontSize = SUIFont::GetInstance()->GetFontSize();
			if(_framesVec[i]->GetName().size() <= 5)
				SUIFont::GetInstance()->DrawFromLeft(_framesVec[i]->GetName(), x+4, y+h/2, fontSize);
			else
			{
				string name = _framesVec[i]->GetName().substr(0,5);
				name.append("...");
				SUIFont::GetInstance()->DrawFromLeft(name, x+4, y+h/2, fontSize);
			}
			
			SUIFont::GetInstance()->End();
		}
	}

	count = 0;
	for(int i=0;i<(int)_framesVec.size();i++)
	{
		if(_framesVec[i]->IsMinimized())
		{
			float x = (count*iConW)+marginH;
			float y = iConY+marginV;
			float w = iConW-2*marginH;
			float h = iConH-2*marginV;

			count++;

			if(SUIInput::IsMouseClicked())
			{
				float mx = (float)SUIInput::MX;
				float my = (float)SUIInput::MY;

				if(mx >= x && mx <= x+w && my >= y && my <= y+h)
				{
					_framesVec[i]->SetMinimized(false);
					ActiveFrame(i);
					break;
				}
			}
		}
	}


	//Resetting GL States back...

	if(isDepthTestEnable)	glEnable(GL_DEPTH_TEST);	else	glDisable(GL_DEPTH_TEST);
	if(isLightEnable)		glEnable(GL_LIGHTING);		else	glDisable(GL_LIGHTING);
	if(isBlendEnable)		glEnable(GL_BLEND);			else	glDisable(GL_BLEND);
	if(isLineSmoothEnable)	glEnable(GL_LINE_SMOOTH);	else	glDisable(GL_LINE_SMOOTH);
	if(isTexture2DEnable)	glEnable(GL_TEXTURE_2D);	else	glDisable(GL_TEXTURE_2D);
	if(isCullFaceEnable)	glEnable(GL_CULL_FACE);		else	glDisable(GL_CULL_FACE);

	glLineWidth(lineWidth);
	glColor4f(color[0], color[1], color[2], color[3]);
	glBlendFunc(blendSRC, blendDST);
	glShadeModel(shadeModel);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(projection);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(modelView);
}


void SUIManager::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

SUIManager::~SUIManager()
{
	SUIFont::DeleteInstance();
}
