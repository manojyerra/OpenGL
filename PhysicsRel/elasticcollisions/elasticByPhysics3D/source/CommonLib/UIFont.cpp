#include "UIFont.h"
#include "FileReader.h"
//#include "MyGL.h"
#include "../CommonLib/MemTrace.h"

UIFont* UIFont::_ref = NULL;


UIFont::UIFont()
{
	_isInit = false;
	_unitWidth = 0;
	_horGap = 0;
	_verGap = 0;
	_width = _unitWidth;
	_height = 0;
}


UIFont::~UIFont()
{
}


UIFont* UIFont::GetInstance()
{
	if(_ref == NULL)
		_ref = (UIFont*)GLNew(new UIFont(), sizeof(UIFont));
	return _ref;
}


void UIFont::Init()
{
	if(_isInit )
		return;

	for(int i=0; i<=126; i++)
		_normalFontVec.push_back((Sprite*)GLNew(new Sprite(), sizeof(Sprite)));

	FileReader fr;
	fr.Load("data/font/UIFont/LettersPos.txt");

	vector<string> lettersVec = fr.GetTagData("ObjectsList", false);

	int vecSize = lettersVec.size();

	string imageOrder = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890`!\"$%^&*()-_=+[{]};:'@#~\\|,<.>/?";

	//int y = 31;
	//int h = 19;

	int y = 279;
	int h = 19;

	for(unsigned int i=0;i<lettersVec.size();i++)
	{
		string xStr = fr.GetAttribute("x", lettersVec[i]);
		string wStr = fr.GetAttribute("width", lettersVec[i]);

		int x = atoi(xStr.c_str());
		int w = atoi(wStr.c_str());

		char ch = imageOrder[i];
		if(ch != ' ')
		{
			_normalFontVec[(int)ch]->Init("data/font/UIFont/actualLetters.png",0,0,w,h);
			_normalFontVec[(int)ch]->SetUVBounds_InPixels(x,y,w,h);
		}

		if(ch == 'Z')
		{
			y = 304;
			h = 19;
		}
		else if(ch == 'z')
		{
			y = 332;
			h = 19;
		}
		else if(ch == '0')
		{
			y = 361;		//previous value is 357
			h = 19;
		}
	}

	_unitWidth = (int)_normalFontVec[(int)'A']->GetWidth();
	_horGap = 0;
	_verGap = 0;
	_width = _unitWidth;
	_height = _normalFontVec[(int)'A']->GetHeight();

	_cx = 0;
	_cy = 0;
	_cw = 1900;
	_ch = 1200;

	SetColor(255,255,255,255);
	SetBorderColor(0,0,0,10);

	_isInit = true;
}


void UIFont::SetWidth(float width)
{
	_width = width;
}


void UIFont::SetHeight(float height)
{
	_height = height;
}

void UIFont::SetColor(int r, int g, int b, int a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void UIFont::SetBorderColor(int borderR, int borderG, int borderB, int borderA)
{
	_borderR = borderR;
	_borderG = borderG;
	_borderB = borderB;
	_borderA = borderA;
}

void UIFont::SetColor(unsigned int color)
{
	_r	= (color >> 24) & 255;
	_g	= (color >> 16) & 255;
	_b	= (color >> 8) & 255;
	_a	= (color ) & 255;
}

void UIFont::SetBorderColor(unsigned int borderColor)
{
	_borderR	= (borderColor >> 24) & 255;
	_borderG	= (borderColor >> 16) & 255;
	_borderB	= (borderColor >> 8) & 255;
	_borderA	= (borderColor ) & 255;
}

void UIFont::SetHorGap(float horGap)
{
	_horGap = horGap;
}


void UIFont::SetVerGap(float verGap)
{
	_verGap = verGap;
}


void UIFont::SetClipArea(int cx, int cy, int cw, int ch)
{
	_cx = cx;
	_cy = cy;
	_cw = cw;
	_ch = ch;
}

void UIFont::RemoveClipArea()
{
	_cx = 0;
	_cy = 0;
	_cw = 480;
	_ch = 320;
}

int UIFont::GetLength(string text, int y, float width, float height, float horGap)
{
	float x=0;
	
	_width = width;
	_height = height;
	_horGap = horGap;

	for(unsigned int i=0;i<text.length();i++)
	{
		int index = (int)text[i];
		
		if(index >=33 && index <= 126)
		{
			x += (_normalFontVec[index]->GetWidth() * _width /_unitWidth)+_horGap;
		}
		//else if(index == (int)'\n')
		//{
		//	x = 0;
		//	y += _verGap + _height;
		//}
		else if(index == (int)' ')
			x += _horGap + _width/2;
		else if( index == (int)'\t')
			x += _horGap + 2*_width;
	}

	return (x);
}


//void UIFont::DrawFromCenter(string text, int x, int y, float width, float height, int border)
//{
//	DrawFromCenter(text, x, y, width, height, 0, border);
//}


void UIFont::DrawFromCenter(string text, int x, int y, float width, float height, float horGap, int border)
{
	int totalHeight = height;

	for(unsigned int i=0;i<text.length();i++)
	{
		int index = (int)text[i];

		if(index == (int)'\n')
			totalHeight += _verGap + height;
	}
	y = y-totalHeight/2;


	text += "\n";

	string token = "";
	int start = 0;

	
	//MyGL::GetInstance()->StartAddingToStream();

	for(unsigned int i=0;i<text.length();i++)
	{
		int index = (int)text[i];

		if(index == ((int)'\n'))
		{
			token = text.substr(start, i-start);
			start = i+1;

			float len = (float)GetLength(token, y, width, height, horGap);
			Draw(token, (int)(x-len/2), y, width, height, horGap, border);
			y += _verGap + height;
			token = "";
		}
	}

	//MyGL::GetInstance()->FinishAddingToStream();
}


void UIFont::Draw(string text, int x, int y, float width, float height, float horGap, int border)//, bool playSinWave)
{
	float initX = x;
	float initY = y;
	
	_width = width;
	_height = height;
	_horGap = horGap;

	int num = 1;

	if(border == CUSTOM_BORDER)
		num = 2;


	for(int loop = 0; loop<num; loop++)		//uncomment for FPS
	{
		if(loop == 1)
		{
			x = initX;
			y = initY;
		}

		//static float length = (float)text.length();
		//static float angle = 360/length;
		//static float angleCount = 0;
		//static float offSetY = 0;
		//static float waveContinue = 1;

		//if(angleCount >= 360)
		//{
		//	angleCount = waveContinue;
		//	waveContinue +=1;
		//	if(waveContinue >= 360)
		//		waveContinue = waveContinue-360;
		//}

		for(unsigned int i=0;i<text.length();i++)
		{
			//if(playSinWave)
			//{
			//	offSetY = sin(angleCount)*50;
			//	angleCount += angle;
			//}

			int index = (int)text[i];
			
			if(index >=33 && index <= 126)
			{
				float actualWidth = _normalFontVec[index]->GetWidth();

				if(loop == 0)
				{
					if(border == BLACK_BORDER)
						_normalFontVec[index]->SetV_InPixels(_normalFontVec[index]->GetV_InPixels()-270);
					else if(border == TRANS_BORDER)
						_normalFontVec[index]->SetV_InPixels(_normalFontVec[index]->GetV_InPixels()+118);
					else if(border == NO_BORDER || border == CUSTOM_BORDER)
						_normalFontVec[index]->SetV_InPixels(_normalFontVec[index]->GetV_InPixels()-135);
					
					_normalFontVec[index]->SetColor(_r,_g,_b,_a);
				}
				else
					_normalFontVec[index]->SetColor(_borderR,_borderG,_borderB,_borderA);

				_normalFontVec[index]->SetClipArea(_cx, _cy, _cw, _ch);
				//_normalFontVec[index]->SetBounds(x, y+offSetY, actualWidth * _width /_unitWidth, _height);
				_normalFontVec[index]->SetBounds(x, y, actualWidth * _width /_unitWidth, _height);
				_normalFontVec[index]->Draw();

				if(loop == 0)
				{
					if(border == BLACK_BORDER)
						_normalFontVec[index]->SetV_InPixels(_normalFontVec[index]->GetV_InPixels()+270);
					else if(border == TRANS_BORDER)
						_normalFontVec[index]->SetV_InPixels(_normalFontVec[index]->GetV_InPixels()-118);
					else if(border == NO_BORDER || border == CUSTOM_BORDER)
						_normalFontVec[index]->SetV_InPixels(_normalFontVec[index]->GetV_InPixels()+135);
				}


				x += _normalFontVec[index]->GetWidth()+_horGap;
				_normalFontVec[index]->SetWidth(actualWidth);
			}
			else if(index == (int)'\n')
			{
				x = initX;
				y += _verGap + _height;
			}
			else if(index == (int)' ')
				x += _horGap + _width/2;
			else if( index == (int)'\t')
				x += _horGap + 2*_width;
		}
	}
}


void UIFont::Draw(int number, int x, int y, float width, float height, float horGap, int border)
{
	char str[64];
	sprintf(str,"%d",number);
	Draw(str, x, y, width, height, horGap, border);
}


void UIFont::Draw(char ch, int x, int y, float w, float h, float horGap, int border)
{
	string str="";
	str += ch;
	Draw(str, x, y, w, h, horGap, border);
}


void UIFont::Terminate()
{
	for( int i=0; i<(int)_normalFontVec.size(); i++ )
	{
		if( _normalFontVec[i] )
		{
			_normalFontVec[i]->Terminate();
			GLDelete( _normalFontVec[i]);
			_normalFontVec[i] = NULL;
		}
	}
	_normalFontVec.clear();


	if(_ref)
	{
		GLDelete( _ref);
		_ref =NULL;
	}
}