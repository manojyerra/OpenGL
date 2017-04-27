#include "SuiManager.h"
#include "SuiInput.h"
#include "SuiFont.h"
#include "SuiIncludes.h"
#include "SuiEvents.h"
#include "SuiChoice.h"

SuiManager* SuiManager::_ref = 0;

void SuiManager::Setup(int windowWidth, int windowHeight)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;

	_statusBarRect.SetColor(0,0,255,120);
	_statusBarRect.SetBounds(0,_windowHeight*0.92,_windowWidth,_windowHeight*0.08);
}

SuiManager::SuiManager()
{
	//_nextSuiCycle = NULL;
	//_currSuiCycle = NULL;
	_activeFrame = NULL;
	_windowWidth = 0;
	_windowHeight = 0;
	_dialogCom = NULL;
	_statusBarRect.SetBounds(0,0,0,0);
	_newFrameAdded = false;
}

SuiManager* SuiManager::GetInstance()
{
	if(_ref == 0)
		_ref = new SuiManager();
	return _ref;
}

float SuiManager::GetWindowWidth()						{	return _windowWidth;					}
float SuiManager::GetWindowHeight()						{	return _windowHeight;					}
float SuiManager::GetWindowHeightWithoutStatusBar()		{	return _windowHeight-_statusBarRect.h;	}

void SuiManager::Run()
{
	Update();
	Draw();
}

unsigned int SuiManager::GetTimeInMilliSeconds()
{
	return GetTickCount();
}


///***********************************************************************************************************/////////



void SuiManager::AddFrame(SuiFrame* frame)
{
	_framesVec.push_back(frame);
	_newFrameAdded = true;
}

void SuiManager::RemoveFrame(SuiFrame* frame)
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

void SuiManager::ActiveFrame(int frameIndex)
{
	if(_activeFrame)
		_activeFrame->SetFocusOnFromManager(false);

	_activeFrame = _framesVec[frameIndex];
	_framesVec.erase(_framesVec.begin() + frameIndex);
	_framesVec.push_back(_activeFrame);

	if(_activeFrame)
		_activeFrame->SetFocusOnFromManager(true);
}

void SuiManager::SetFocusOn(SuiFrame* frame)
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

void SuiManager::SetDialogCom(SuiComponent* dialogCom)
{
	_dialogCom = dialogCom;
}

void SuiManager::CheckForResetBounds()
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

bool SuiManager::Contains(float mx, float my)
{
	for(int i=_framesVec.size()-1; i>=0; i--)
		if(_framesVec[i]->IsVisible() && (_framesVec[i]->IsMinimized()==false) && _framesVec[i]->Contains(mx, my))
			return true;

	return false;
}

void SuiManager::Update()
{
	CheckForResetBounds();

	float mx = (float)SuiInput::MX;
	float my = (float)SuiInput::MY;

	if(_dialogCom != NULL)
	{
		if(_dialogCom->IsDialogMode())
			_dialogCom->UpdateDialog();
		else
		{
			SuiEvents eventsVec = _dialogCom->UpdateByInput();
			FireEvent(_dialogCom, eventsVec);
			_dialogCom = NULL;
		}

		return;
	}

	if(SuiInput::IsMouseClicked())
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
					//return;
				}
				break;
			}
		}
	}

	if(_activeFrame && !_activeFrame->IsMinimized())
	{
		if(SuiInput::IsMouseClicked() || SuiInput::IsMousePressed() || SuiInput::IsMouseReleased())
		{
			SuiComponent* com = _activeFrame->getComponentAt(mx, my);
		
			if(com)
			{
				FireEvent(com, com->UpdateByInput());
				_activeFrame->ResetBounds();
			}
		}
	}
}

void SuiManager::FireEvent(SuiComponent* com, SuiEvents &eventsVec)
{
	if(eventsVec.ACTION_PERFORMED == false)
		return;

	int id = com->GetComponentID();

	if(id == SuiComponent::BUTTON)
	{
		SuiActionListener* actionListener = ((SuiButton*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SuiActionEvent(com));
	}
	else if(id == SuiComponent::CHECKBOX)
	{
		SuiActionListener* actionListener = ((SuiCheckBox*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SuiActionEvent(com));
	}
	else if(id == SuiComponent::RADIO_BUTTON)
	{
		SuiActionListener* actionListener = ((SuiRadioButton*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SuiActionEvent(com));
	}
	else if(id == SuiComponent::SLIDER)
	{
		SuiActionListener* actionListener = ((SuiSlider*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SuiActionEvent(com));
	}
	else if(id == SuiComponent::SUI_CHOICE)
	{
		SuiActionListener* actionListener = ((SuiChoice*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SuiActionEvent(com));
	}
	else if(id == SuiComponent::SUI_LIST)
	{
		SuiActionListener* actionListener = ((SuiList*)com)->GetActionListener();
		if(actionListener)
			actionListener->actionPerformed(SuiActionEvent(com));
	}
}


void SuiManager::Draw()
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

			SuiRect::Draw(x, y, w, h, 250,100,200,255, false);
			SuiRect::Draw(x, y, w, h, 0,0,0,255, true);
			SuiRect::Draw(x+2, y+2, w-4, h-4, 0,0,0,255, true);
			count++;

			SuiFont::GetInstance()->SetColor(0xffffffff);
			SuiFont::GetInstance()->Begin();
			SuiFont::GetInstance()->HorBorder(x, x+w);
			float horGap = -0.5f;
			float fontSize = SuiFont::GetInstance()->GetFontSize();
			if(_framesVec[i]->GetName().size() <= 5)
				SuiFont::GetInstance()->DrawFromLeft(_framesVec[i]->GetName(), x+4, y+h/2, fontSize);
			else
			{
				string name = _framesVec[i]->GetName().substr(0,5);
				name.append("...");
				SuiFont::GetInstance()->DrawFromLeft(name, x+4, y+h/2, fontSize);
			}
			
			SuiFont::GetInstance()->End();
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

			if(SuiInput::IsMouseClicked())
			{
				float mx = (float)SuiInput::MX;
				float my = (float)SuiInput::MY;

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


void SuiManager::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

SuiManager::~SuiManager()
{
	SuiFont::DeleteInstance();
}
