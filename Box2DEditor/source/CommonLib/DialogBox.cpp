#include "DialogBox.h"
#include "UIFont.h"
#include "Input.h"
#include "../Platform/Platform.h"

RectPrim* DialogBox::_bg = NULL;
RectPrim* DialogBox::_break = NULL;
RectPrim* DialogBox::_continue = NULL;
RectPrim* DialogBox::_ignoreAll = NULL;

vector<string> DialogBox::_fileNameVec;
vector<int>	DialogBox::_lineNumVec;

void DialogBox::Init()
{
	_bg = new RectPrim(100,100,100,100,0,0xffff00ff);
	_break = new RectPrim(0,0,0,0,0,0x00ffffff);
	_continue = new RectPrim(0,0,0,0,0,0xff00ffff);
	_ignoreAll = new RectPrim(0,0,0,0,0,0xff00abff);

	_fileNameVec.clear();
	_lineNumVec.clear();
}

int DialogBox::ShowDialog(char* msg, const char* filePath, unsigned int lineNum)
{
	for(int i=0;i<(int)_fileNameVec.size();i++)
	{
		if(_fileNameVec[i].compare(filePath) == 0 && _lineNumVec[i] == lineNum)
		{
			return IGNORE_ALL;
		}
	}

	DrawImgs(msg);

	Platform::Swap_Buffers();

	while(true)
	{
		Platform::DeviceYield(10);
		Input::Update(1.0f/30.0f);
		if(Input::IsMouseReleased())
		{
			if(_break->Contains((float)Input::MouseX, (float)Input::MouseY))
			{
				int a = 1;
				int b = 0;
				int c = a/b;
				return BREAK;
			}
			else if(_continue->Contains((float)Input::MouseX, (float)Input::MouseY))
			{
				ShowAnim(msg);
				return CONTINUE;
			}
			else if(_ignoreAll->Contains((float)Input::MouseX, (float)Input::MouseY))
			{
				ShowAnim(msg);

				_fileNameVec.push_back(filePath);
				_lineNumVec.push_back(lineNum);
				return IGNORE_ALL;
			}
		}
	}

	return BREAK;
}

void DialogBox::ShowAnim(char* msg)
{
	float viewPortValues[4];
	glGetFloatv( GL_VIEWPORT, viewPortValues);
	float x = viewPortValues[0];
	float y = viewPortValues[1];
	float w = viewPortValues[2];
	float h = viewPortValues[3];

	float n = w;
	if(n > h)
		n = h;

	for(int i=0;i<n/2;i+=20)
	{
		Platform::DeviceYield(5);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport( (GLint)(x+i), (GLint)(y+i), (GLsizei)(w-i*2), (GLsizei)(h-i*2) );
		DrawImgs(msg);
		Platform::Swap_Buffers();
	}

	glViewport( (GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h );
}

void DialogBox::DrawImgs(char* msg)
{
	float length = UIFont::GetInstance()->GetLength(msg, 18,18,-3);

	float w = length + 100.0f;
	float h = 100.0f;

	if(w < 450)
		w = 450;

	_bg->SetBounds((768.0f-w)/2.0f, (1024.0f-h)/2.0f, w, h);

	float x = _bg->GetX();
	float y = _bg->GetY();

	float btnWidth = 130;
	float btnHeight = 40;
	float borderGap = 10;

	_break->SetBounds(-btnWidth - borderGap + (768.0f-btnWidth)/2.0f, y+h-btnHeight-borderGap, btnWidth, btnHeight);
	_continue->SetBounds((768.0f-btnWidth)/2.0f, y+h-btnHeight-borderGap, btnWidth, btnHeight);
	_ignoreAll->SetBounds( btnWidth + borderGap + (768.0f-btnWidth)/2.0f, y+h-btnHeight-borderGap, btnWidth, btnHeight);

	_bg->Draw();
	_break->Draw();
	_continue->Draw();
	_ignoreAll->Draw();

	UIFont::GetInstance()->SetColor(0,0,0,255);
	UIFont::GetInstance()->Begin();

	UIFont::GetInstance()->Draw("Break", _break->GetX()+25, _break->GetY()+10, 18,18,-3);
	UIFont::GetInstance()->Draw("Continue", _continue->GetX()+3, _continue->GetY()+10, 18,18,-3);
	UIFont::GetInstance()->Draw("Ignore All", _ignoreAll->GetX()+2, _ignoreAll->GetY()+10, 18,18,-3);
	UIFont::GetInstance()->Draw(msg,(768 - length)/2.0f,_bg->GetY() + 20,18,18,-3);

	UIFont::GetInstance()->End();
}


int DialogBox::ShowFloatDialog(float number, const char* file, unsigned int line)
{
	char str[256];
	sprintf(str, "%.3f", number);
	return ShowDialog(str,file,line);
}

int DialogBox::ShowIntDialog(int number, const char* file, unsigned int line)
{
	char str[256];
	sprintf(str, "%d", number);
	return ShowDialog(str,file,line);
}

void DialogBox::Terminate()
{
	if(_bg)
	{
		delete _bg;
		_bg = NULL;
	}
	if(_break)
	{
		delete _break;
		_break = NULL;
	}
	if(_continue)
	{
		delete _continue;
		_continue = NULL;
	}
	if(_ignoreAll)
	{
		delete _ignoreAll;
		_ignoreAll = NULL;
	}

	//for(int i=0;i<(int)_fileNameVec.size();i++)
	//	_fileNameVec[i].clear();

	_fileNameVec.clear();
	_lineNumVec.clear();
}
