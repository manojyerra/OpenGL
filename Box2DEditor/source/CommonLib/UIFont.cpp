#include "UIFont.h"
#include "../MyDefines.h"
#include "FileReader.h"
#include "math.h"
#include "AddData.h"

UIFont* UIFont::_ref = NULL;

UIFont::UIFont() {}
UIFont::~UIFont() {}

UIFont* UIFont::GetInstance()
{
	if(_ref == NULL)
		_ref = new UIFont();
	return _ref;
}

void UIFont::Init()
{
	for(int i=0; i<=126; i++)
		_normalFontVec.push_back(new LetterInfo());

	_imageInfo = Ref::GetInstance()->GetTexture("font/UIFont/actualLetters.png", false, false);

	FillUVInfo();

	for(int i=0;i<=126;i++)
	{
		if(cw[i] != 0)
		{
			_normalFontVec[i]->SetBounds(0,0,(float)cw[i],(float)ch[i]);
			_normalFontVec[i]->SetUVBounds( ((float)u[i])/512.0f, ((float)v[i])/128.0f, ((float)cw[i])/512.0f, ((float)ch[i])/128.0f);
		}
	}

	//FileReader fr;
	//fr.Load("font/UIFont/LettersPos.txt");

	//string line = fr.ReadLine();
	//line = fr.ReadLine();

	//int numLines = 0;
	//sscanf( line.c_str(), "%d", &numLines);

	//string imageOrder = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890`!\"$%^&*()-_=+[{]};:'@#~\\|,<.>/?";

	//float y = 279.0f;
	//float h = 19.0f;

	//for(int i=0;i<numLines;i++)
	//{
	//	int x,w;
	//	char ch;

	//	line = fr.ReadLine();
	//	sscanf( line.c_str(),"%c,%d,%d",&ch,&x,&w);

	//	if(ch != ' ')
	//	{
	//		_normalFontVec[(int)ch]->SetBounds(0,0,(float)w,(float)h);
	//		_normalFontVec[(int)ch]->SetUVBounds(((float)x)/512.0f,(y-135)/512.0f,((float)w)/512.0f,h/512.0f);
	//	}

	//	if(ch == 'Z')
	//	{
	//		y = 304;
	//	}
	//	else if(ch == 'z')
	//	{
	//		y = 332;
	//	}
	//	else if(ch == '0')
	//	{
	//		y = 361;		//previous value is 357
	//	}
	//}

	_unitWidth = _normalFontVec[(int)'A']->w;

	//_cx = 0;
	//_cy = 0;
	//_cw = 1900;
	//_ch = 1200;

	SetColor(255,255,25,255);

	_rotX = 0.0f;
	_rotY = 0.0f;
	_rotZ = 0.0f;
}


void UIFont::SetWidth(float width)
{
	//_width = width;
}


void UIFont::SetHeight(float height)
{
	//_height = height;
}

void UIFont::SetColor(int r, int g, int b, int a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void UIFont::SetRotation(float angleX, float angleY, float angleZ)
{
	_rotX = angleX;
	_rotY = angleY;
	_rotZ = angleZ;
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
//	_horGap = horGap;
}


void UIFont::SetVerGap(float verGap)
{
//	_verGap = verGap;
}


//void UIFont::SetClipArea(int cx, int cy, int cw, int ch)
//{
//	_cx = cx;
//	_cy = cy;
//	_cw = cw;
//	_ch = ch;
//}
//
//void UIFont::RemoveClipArea()
//{
//	_cx = 0;
//	_cy = 0;
//	_cw = 1024;
//	_ch = 1024;
//}

float UIFont::GetLength(string text, float width, float height, float horGap)
{
	float x=0;

	for(unsigned int i=0;i<text.length();i++)
	{
		int index = (int)text[i];

		if(index >=33 && index <= 126)
			x += (_normalFontVec[index]->w * width /_unitWidth)+horGap;
		else if(index == (int)' ')
			x += horGap + width/2;
		else if( index == (int)'\t')
			x += horGap + 2*width;
	}

	return (x);
}


void UIFont::Draw(int number, float xPos, float yPos, float width, float height, float horGap)
{
	char str[64];
	sprintf_s(str,"%d",number);
	Draw(str, xPos, yPos, width, height, horGap);
}


void UIFont::Draw(char ch, float xPos, float yPos, float w, float h, float horGap)
{
	string str="";
	str += ch;
	Draw(str, xPos, yPos, w, h, horGap);
}

void UIFont::Begin()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _imageInfo->textureID);

	//start using AddData functions
	AddData::GetInstance()->glBegin(GL_TRIANGLES);
}

void UIFont::DrawAndFit(string text, float xPos, float yPos, float fitInLength, float height, float horGap)
{
	float defaultOneLetterWidth = 18;
	float numChars =(float)text.length();
	float lengthByDefaults = GetLength( text, defaultOneLetterWidth, height, horGap);

	AddData* addData = AddData::GetInstance();

	addData->glColor4ub(_r,_g,_b,_a);

	float x = xPos;
	float y = yPos;
	//horGap = 0;

	for(unsigned int i=0;i<text.length();i++)
	{
		int index = (int)text[i];

		if(index >= 33 && index <= 126)
		{
			float xx = x;
			float yy = y;
			float ww = _normalFontVec[index]->w * defaultOneLetterWidth /(_unitWidth);
			ww = ww*fitInLength/lengthByDefaults;
			float hh = height;

			LetterInfo* lf = _normalFontVec[index];

			addData->glTexCoord2f(lf->u,			1-(lf->v));					addData->glVertex3f(xx,		yy,		0);
			addData->glTexCoord2f(lf->u+lf->cw,	1-(lf->v));						addData->glVertex3f(xx+ww,	yy,		0);
			addData->glTexCoord2f(lf->u,			1-(lf->v+lf->ch));			addData->glVertex3f(xx,		yy+hh,	0);
			addData->glTexCoord2f(lf->u+lf->cw,	1-(lf->v));						addData->glVertex3f(xx+ww,	yy,		0);
			addData->glTexCoord2f(lf->u,			1-(lf->v+lf->ch));			addData->glVertex3f(xx,		yy+hh,	0);
			addData->glTexCoord2f(lf->u+lf->cw,	1-(lf->v+lf->ch));				addData->glVertex3f(xx+ww,	yy+hh,	0);

			x += ww+horGap; //_normalFontVec[index]->w*18 + _horGap;
		}
		else if(index == (int)'\n')
		{
			x = 0;
			//y += _verGap + _height;
		}
		else if(index == (int)' ')
			x += horGap + (fitInLength * defaultOneLetterWidth / lengthByDefaults)/2;
		else if( index == (int)'\t')
			x += horGap + 2*(fitInLength * defaultOneLetterWidth / lengthByDefaults);
	}
}

void UIFont::DrawFromCenter(string text, float xPos, float yPos, float width, float height, float horGap)
{
	float length = (float)GetLength(text, width, height, horGap);
	Draw(text, xPos-length/2, yPos-height/2, width, height, horGap);
}

void UIFont::Draw(string text, float xPos, float yPos, float width, float height, float horGap)
{
	AddData* addData = AddData::GetInstance();

	//_width = width;
	//_height = height;
	//_horGap = horGap;

	addData->glColor4ub(_r,_g,_b,_a);

	float x = xPos;
	float y = yPos;

	for(unsigned int i=0;i<text.length();i++)
	{
		int index = (int)text[i];

		if(index >= 33 && index <= 126)
		{
			float xx = x;
			float yy = y;
			float ww = _normalFontVec[index]->w * width /_unitWidth;
			float hh = height;

			LetterInfo* lf = _normalFontVec[index];

			addData->glTexCoord2f(lf->u,			1-(lf->v));					addData->glVertex3f(xx,		yy,		0);
			addData->glTexCoord2f(lf->u+lf->cw,	1-(lf->v));						addData->glVertex3f(xx+ww,	yy,		0);
			addData->glTexCoord2f(lf->u,			1-(lf->v+lf->ch));			addData->glVertex3f(xx,		yy+hh,	0);
			addData->glTexCoord2f(lf->u+lf->cw,	1-(lf->v));						addData->glVertex3f(xx+ww,	yy,		0);
			addData->glTexCoord2f(lf->u,			1-(lf->v+lf->ch));			addData->glVertex3f(xx,		yy+hh,	0);
			addData->glTexCoord2f(lf->u+lf->cw,	1-(lf->v+lf->ch));				addData->glVertex3f(xx+ww,	yy+hh,	0);

			x += ww + horGap; //_normalFontVec[index]->w*18 + _horGap;
		}
		else if(index == (int)'\n')
		{
			x = 0;
			//y += verGap + height;
		}
		else if(index == (int)' ')
		{
			x += width/2;
			x += horGap;
		}
		else if( index == (int)'\t')
		{
			x += 2*width;
			x += horGap;
		}
	}
}


void UIFont::End()
{
	AddData::GetInstance()->glEnd();
	//End using AddDataFunctions
	glDisable(GL_TEXTURE_2D);
}

void UIFont::Terminate()
{
	for( int i=0; i<(int)_normalFontVec.size(); i++ )
	{
		if( _normalFontVec[i] )
		{
			delete _normalFontVec[i];
			_normalFontVec[i] = NULL;
		}
	}

	Ref::GetInstance()->UnLoad( _imageInfo );

	if(_ref)
	{
		delete _ref;
		_ref =NULL;
	}
}


void UIFont::FillUVInfo()
{
	for(int i=0;i<127;i++)
	{
		u[i] = 0;
		v[i] = 0;
		cw[i] = 0;
		ch[i] = 19;
	}

	u[(int)'A'] = 3;	cw[(int)'A'] = 18;	v[(int)'A'] = 0;
	u[(int)'B'] = 26;	cw[(int)'B'] = 14;	v[(int)'B'] = 0;
	u[(int)'C'] = 45;	cw[(int)'C'] = 14;	v[(int)'C'] = 0;
	u[(int)'D'] = 64;	cw[(int)'D'] = 15;	v[(int)'D'] = 0;
	u[(int)'E'] = 84;	cw[(int)'E'] = 14;	v[(int)'E'] = 0;
	u[(int)'F'] = 103;	cw[(int)'F'] = 13;	v[(int)'F'] = 0;
	u[(int)'G'] = 120;	cw[(int)'G'] = 16;	v[(int)'G'] = 0;
	u[(int)'H'] = 141;	cw[(int)'H'] = 16;	v[(int)'H'] = 0;
	u[(int)'I'] = 162;	cw[(int)'I'] = 9;	v[(int)'I'] = 0;
	u[(int)'J'] = 174;	cw[(int)'J'] = 13;	v[(int)'J'] = 0;
	u[(int)'K'] = 192;	cw[(int)'K'] = 16;	v[(int)'K'] = 0;
	u[(int)'L'] = 212;	cw[(int)'L'] = 13;	v[(int)'L'] = 0;
	u[(int)'M'] = 229;	cw[(int)'M'] = 18;	v[(int)'M'] = 0;
	u[(int)'N'] = 252;	cw[(int)'N'] = 16;	v[(int)'N'] = 0;
	u[(int)'O'] = 273;	cw[(int)'O'] = 17;	v[(int)'O'] = 0;
	u[(int)'P'] = 295;	cw[(int)'P'] = 14;	v[(int)'P'] = 0;
	u[(int)'Q'] = 313;	cw[(int)'Q'] = 18;	v[(int)'Q'] = 0;
	u[(int)'R'] = 335;	cw[(int)'R'] = 15;	v[(int)'R'] = 0;
	u[(int)'S'] = 354;	cw[(int)'S'] = 12;	v[(int)'S'] = 0;
	u[(int)'T'] = 371;	cw[(int)'T'] = 15;	v[(int)'T'] = 0;
	u[(int)'U'] = 390;	cw[(int)'U'] = 15;	v[(int)'U'] = 0;
	u[(int)'V'] = 409;	cw[(int)'V'] = 16;	v[(int)'V'] = 0;
	u[(int)'W'] = 430;	cw[(int)'W'] = 21;	v[(int)'W'] = 0;
	u[(int)'X'] = 454;	cw[(int)'X'] = 16;	v[(int)'X'] = 0;
	u[(int)'Y'] = 474;	cw[(int)'Y'] = 15;	v[(int)'Y'] = 0;
	u[(int)'Z'] = 492;	cw[(int)'Z'] = 15;	v[(int)'Z'] = 0;

	u[(int)'a'] = 3;	cw[(int)'a'] = 13;	v[(int)'a'] = 25;
	u[(int)'b'] = 21;	cw[(int)'b'] = 14;	v[(int)'b'] = 25;
	u[(int)'c'] = 39;	cw[(int)'c'] = 12;	v[(int)'c'] = 25;
	u[(int)'d'] = 55;	cw[(int)'d'] = 15;	v[(int)'d'] = 25;
	u[(int)'e'] = 74;	cw[(int)'e'] = 13;	v[(int)'e'] = 25;
	u[(int)'f'] = 91;	cw[(int)'f'] = 11;	v[(int)'f'] = 25;
	u[(int)'g'] = 106;	cw[(int)'g'] = 14;	v[(int)'g'] = 25;
	u[(int)'h'] = 125;	cw[(int)'h'] = 14;	v[(int)'h'] = 25;
	u[(int)'i'] = 143;	cw[(int)'i'] = 9;	v[(int)'i'] = 25;
	u[(int)'j'] = 154;	cw[(int)'j'] = 11;	v[(int)'j'] = 25;
	u[(int)'k'] = 169;	cw[(int)'k'] = 14;	v[(int)'k'] = 25;
	u[(int)'l'] = 187;	cw[(int)'l'] = 9;	v[(int)'l'] = 25;
	u[(int)'m'] = 200;	cw[(int)'m'] = 19;	v[(int)'m'] = 25;
	u[(int)'n'] = 223;	cw[(int)'n'] = 14;	v[(int)'n'] = 25;
	u[(int)'o'] = 242;	cw[(int)'o'] = 14;	v[(int)'o'] = 25;
	u[(int)'p'] = 260;	cw[(int)'p'] = 15;	v[(int)'p'] = 25;
	u[(int)'q'] = 279;	cw[(int)'q'] = 14;	v[(int)'q'] = 25;
	u[(int)'r'] = 297;	cw[(int)'r'] = 11;	v[(int)'r'] = 25;
	u[(int)'s'] = 312;	cw[(int)'s'] = 11;	v[(int)'s'] = 25;
	u[(int)'t'] = 328;	cw[(int)'t'] = 11;	v[(int)'t'] = 25;
	u[(int)'u'] = 343;	cw[(int)'u'] = 14;	v[(int)'u'] = 25;
	u[(int)'v'] = 361;	cw[(int)'v'] = 14;	v[(int)'v'] = 25;
	u[(int)'w'] = 378;	cw[(int)'w'] = 19;	v[(int)'w'] = 25;
	u[(int)'x'] = 401;	cw[(int)'x'] = 14;	v[(int)'x'] = 25;
	u[(int)'y'] = 419;	cw[(int)'y'] = 14;	v[(int)'y'] = 25;
	u[(int)'z'] = 437;	cw[(int)'z'] = 12;	v[(int)'z'] = 25;

	u[(int)'1'] = 5;	cw[(int)'1'] = 12;	v[(int)'1'] = 53;
	u[(int)'2'] = 22;	cw[(int)'2'] = 14;	v[(int)'2'] = 53;
	u[(int)'3'] = 40;	cw[(int)'3'] = 15;	v[(int)'3'] = 53;
	u[(int)'4'] = 59;	cw[(int)'4'] = 14;	v[(int)'4'] = 53;
	u[(int)'5'] = 77;	cw[(int)'5'] = 14;	v[(int)'5'] = 53;
	u[(int)'6'] = 95;	cw[(int)'6'] = 15;	v[(int)'6'] = 53;
	u[(int)'7'] = 114;	cw[(int)'7'] = 14;	v[(int)'7'] = 53;
	u[(int)'8'] = 132;	cw[(int)'8'] = 15;	v[(int)'8'] = 53;
	u[(int)'9'] = 150;	cw[(int)'9'] = 16;	v[(int)'9'] = 53;
	u[(int)'0'] = 169;	cw[(int)'0'] = 15;	v[(int)'0'] = 53;

	u[(int)'\''] = 4;	cw[(int)'\''] = 9;	v[(int)'\''] = 82;
	u[(int)'!'] = 20;	cw[(int)'!'] = 8;	v[(int)'!'] = 82;
	u[(int)'"'] = 40;	cw[(int)'"'] = 10;	v[(int)'"'] = 82;
	u[(int)'$'] = 49;	cw[(int)'$'] = 12;	v[(int)'$'] = 82;
	u[(int)'%'] = 66;	cw[(int)'%'] = 19;	v[(int)'%'] = 82;
	u[(int)' '] = 85;	cw[(int)' '] = 19;	v[(int)' '] = 82;
	u[(int)'&'] = 105;	cw[(int)'&'] = 16;	v[(int)'&'] = 82;
	u[(int)'*'] = 126;	cw[(int)'*'] = 10;	v[(int)'*'] = 82;
	u[(int)'('] = 142;	cw[(int)'('] = 8;	v[(int)'('] = 82;
	u[(int)')'] = 155;	cw[(int)')'] = 9;	v[(int)')'] = 82;
	u[(int)'-'] = 168;	cw[(int)'-'] = 9;	v[(int)'-'] = 82;
	u[(int)'_'] = 181;	cw[(int)'_'] = 12;	v[(int)'_'] = 82;
	u[(int)'='] = 197;	cw[(int)'='] = 14;	v[(int)'='] = 82;
	u[(int)'+'] = 216;	cw[(int)'+'] = 14;	v[(int)'+'] = 82;
	u[(int)'['] = 234;	cw[(int)'['] = 10;	v[(int)'['] = 82;
	u[(int)'{'] = 248;	cw[(int)'{'] = 10;	v[(int)'{'] = 82;
	u[(int)']'] = 262;	cw[(int)']'] = 10;	v[(int)']'] = 82;
	u[(int)'}'] = 277;	cw[(int)'}'] = 10;	v[(int)'}'] = 82;
	u[(int)';'] = 291;	cw[(int)';'] = 9;	v[(int)';'] = 82;
	u[(int)':'] = 305;	cw[(int)':'] = 8;	v[(int)':'] = 82;
	u[(int)'\''] = 317;	cw[(int)'\''] = 8;	v[(int)'\''] = 82;
	u[(int)'@'] = 329;	cw[(int)'@'] = 18;	v[(int)'@'] = 82;
	u[(int)'#'] = 351;	cw[(int)'#'] = 15;	v[(int)'#'] = 82;
	u[(int)'~'] = 489;	cw[(int)'~'] = 20;	v[(int)'~'] = 82;
	u[(int)'\\'] = 369;	cw[(int)'\\'] = 13;	v[(int)'\\'] = 82;
	u[(int)'|'] = 386;	cw[(int)'|'] = 8;	v[(int)'|'] = 82;
	u[(int)','] = 400;	cw[(int)','] = 9;	v[(int)','] = 82;
	u[(int)'<'] = 413;	cw[(int)'<'] = 12;	v[(int)'<'] = 82;
	u[(int)'.'] = 429;	cw[(int)'.'] = 8;	v[(int)'.'] = 82;
	u[(int)'>'] = 442;	cw[(int)'>'] = 12;	v[(int)'>'] = 82;
	u[(int)'/'] = 455;	cw[(int)'/'] = 14;	v[(int)'/'] = 82;
	u[(int)'?'] = 473;	cw[(int)'?'] = 12;	v[(int)'?'] = 82;

}
