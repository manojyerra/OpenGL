#include "UIFont.h"
#include "AddData.h"
#include "Platform.h"

UIFont* UIFont::_ref = NULL;

UIFont* UIFont::GetInstance()
{
	if(_ref == NULL)
		_ref = new UIFont();
	return _ref;
}

UIFont::UIFont()
{
	_addData = new AddData(128, true, true, false);

	for(int i=0; i<=128; i++)
	{
		_normalFontVec.push_back(new LetterInfo());
		_newFontVec.push_back(new LetterInfo());
	}

	_textureInfo = TextureManager::GetInstance()->CreateTexture("../assets/font/fontimage.png", Platform::READ_FROM_IM);
	_textureInfoNew = TextureManager::GetInstance()->CreateTexture("../assets/font/fontimagenew.png", Platform::READ_FROM_IM);

	FillUVInfo();

	float imgW = _textureInfo->width;
	float imgH = _textureInfo->height;

	for(int i=0;i<=126;i++)
	{
		if(cw[i] != 0)
		{
			_normalFontVec[i]->SetBounds(0,0,(float)cw[i],(float)ch[i]);
			_normalFontVec[i]->SetUVBounds( ((float)u[i])/imgW, ((float)v[i])/imgH, ((float)cw[i])/imgW, ((float)ch[i])/imgH);
		}
	}

	imgW = _textureInfoNew->width;
	imgH = _textureInfoNew->height;

	for(int i=0;i<=126;i++)
	{
		if(cw[i] != 0)
		{
			_newFontVec[i]->SetBounds(0,0,(float)cw1[i],(float)ch1[i]);
			_newFontVec[i]->SetUVBounds( ((float)u1[i])/imgW, ((float)v1[i])/imgH, ((float)cw1[i])/imgW, ((float)ch1[i])/imgH);
		}
	}

	SetColor(255,255,25,255);

	_fontType = 0;
}

void UIFont::SetColor(int r, int g, int b, int a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void UIFont::SetColor(unsigned int color)
{
	_r	= (color >> 24) & 255;
	_g	= (color >> 16) & 255;
	_b	= (color >> 8) & 255;
	_a	= (color ) & 255;
}

float UIFont::GetLength(string text, float fontSize, float horGap)
{
	float x=0;

	for(unsigned int i=0;i<text.length();i++)
	{
		int index = (int)text[i];

		if(index >=33 && index <= 126)
		{
			float ww = fontSize * _normalFontVec[index]->w / _normalFontVec[index]->h;

			if(_fontType == FONT_NEW)
				ww = fontSize * _newFontVec[index]->w / _newFontVec[index]->h;

			x += ww + horGap;
		}
		else if(index == (int)' ')
		{
			x += horGap + fontSize/3.5;
		}
		else if( index == (int)'\t')
		{
			x += horGap + 2*fontSize;
		}
	}

	return (x);
}

void UIFont::Draw(int number, float xPos, float yPos, float fontSize, float horGap)
{
	char str[64];
	sprintf(str,"%d",number);
	Draw(str, xPos, yPos, fontSize, horGap, 0);
}

void UIFont::Draw(char ch, float xPos, float yPos, float fontSize, float horGap)
{
	string str="";
	str += ch;
	Draw(str, xPos, yPos, fontSize, horGap, 0);
}

void UIFont::Begin()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureInfo->textureID);

	//AddData functions
	_addData->glBegin(GL_TRIANGLES);
}

void UIFont::Begin(int fontType)
{
	_fontType = fontType;

	glEnable(GL_TEXTURE_2D);

	if(_fontType == FONT_NEW)
		glBindTexture(GL_TEXTURE_2D, _textureInfoNew->textureID);
	else
		glBindTexture(GL_TEXTURE_2D, _textureInfo->textureID);

	//AddData functions
	_addData->glBegin(GL_TRIANGLES);
}

void UIFont::DrawFromCenter(string text, float xPos, float yPos, float fontSize, float horGap)
{
	float length = (float)GetLength(text, fontSize, horGap);
	Draw(text, xPos-length/2, yPos-fontSize/2, fontSize, horGap, 0);
}

void UIFont::Draw(string text, float xPos, float yPos, float fontSize, float horGap, float verGap)
{
	_addData->glColor4ub(_r,_g,_b,_a);

	float x = xPos;
	float y = yPos;

	for(unsigned int i=0;i<text.length();i++)
	{
		int index = (int)text[i];

		if(index >= 33 && index <= 126)
		{
			float xx = x;
			float yy = y;
			float hh = fontSize;
			float ww = fontSize * _normalFontVec[index]->w / _normalFontVec[index]->h;

			LetterInfo* lf = _normalFontVec[index];

			if(_fontType == FONT_NEW)
			{
				ww = fontSize * _newFontVec[index]->w / _newFontVec[index]->h;
				lf = _newFontVec[index];
			}

			_addData->glTexCoord2f(lf->u,			(lf->v));					_addData->glVertex3f(xx,	yy,		0);
			_addData->glTexCoord2f(lf->u+lf->cw,	(lf->v));					_addData->glVertex3f(xx+ww,	yy,		0);
			_addData->glTexCoord2f(lf->u,			(lf->v+lf->ch));			_addData->glVertex3f(xx,	yy+hh,	0);
			_addData->glTexCoord2f(lf->u+lf->cw,	(lf->v));					_addData->glVertex3f(xx+ww,	yy,		0);
			_addData->glTexCoord2f(lf->u,			(lf->v+lf->ch));			_addData->glVertex3f(xx,	yy+hh,	0);
			_addData->glTexCoord2f(lf->u+lf->cw,	(lf->v+lf->ch));			_addData->glVertex3f(xx+ww,	yy+hh,	0);

			x += ww + horGap;
		}
		else if(index == (int)'\n')
		{
			x = xPos;
			y += verGap + fontSize;
		}
		else if(index == (int)' ')
		{
			x += fontSize/3.5 + horGap;
		}
		else if( index == (int)'\t')
		{
			x += 2*fontSize + horGap;
		}
	}
}

void UIFont::End()
{
	_addData->glEnd();
	//End using AddDataFunctions
	glDisable(GL_TEXTURE_2D);

	_fontType = 0;
}

UIFont::~UIFont()
{
	for( int i=0; i<(int)_normalFontVec.size(); i++ )
	{
		if( _normalFontVec[i] )
		{
			delete _normalFontVec[i];
			_normalFontVec[i] = NULL;
		}
	}
	_normalFontVec.clear();

	for( int i=0; i<(int)_newFontVec.size(); i++ )
	{
		if( _newFontVec[i] )
		{
			delete _newFontVec[i];
			_newFontVec[i] = NULL;
		}
	}
	_newFontVec.clear();

	if(_addData)
		delete _addData;

	TextureManager::GetInstance()->DeleteTexture( _textureInfo );
	TextureManager::GetInstance()->DeleteTexture( _textureInfoNew );
}


void UIFont::DeleteInstance()
{
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
		ch[i] = 0;

		u1[i] = 0;
		v1[i] = 0;
		cw1[i] = 0;
		ch1[i] = 0;
	}

u[(int)'A'] = 17; v[(int)'A'] = 1; cw[(int)'A'] = 20; ch[(int)'A'] = 33;
u[(int)'B'] = 54; v[(int)'B'] = 1; cw[(int)'B'] = 19; ch[(int)'B'] = 33;
u[(int)'C'] = 90; v[(int)'C'] = 1; cw[(int)'C'] = 19; ch[(int)'C'] = 33;
u[(int)'D'] = 126; v[(int)'D'] = 1; cw[(int)'D'] = 19; ch[(int)'D'] = 33;
u[(int)'E'] = 162; v[(int)'E'] = 1; cw[(int)'E'] = 19; ch[(int)'E'] = 33;
u[(int)'F'] = 198; v[(int)'F'] = 1; cw[(int)'F'] = 19; ch[(int)'F'] = 33;
u[(int)'G'] = 234; v[(int)'G'] = 1; cw[(int)'G'] = 19; ch[(int)'G'] = 33;
u[(int)'H'] = 270; v[(int)'H'] = 1; cw[(int)'H'] = 19; ch[(int)'H'] = 33;
u[(int)'I'] = 306; v[(int)'I'] = 1; cw[(int)'I'] = 19; ch[(int)'I'] = 33;
u[(int)'J'] = 342; v[(int)'J'] = 1; cw[(int)'J'] = 19; ch[(int)'J'] = 33;
u[(int)'K'] = 378; v[(int)'K'] = 1; cw[(int)'K'] = 19; ch[(int)'K'] = 33;
u[(int)'L'] = 414; v[(int)'L'] = 1; cw[(int)'L'] = 19; ch[(int)'L'] = 33;
u[(int)'M'] = 450; v[(int)'M'] = 1; cw[(int)'M'] = 19; ch[(int)'M'] = 33;
u[(int)'N'] = 486; v[(int)'N'] = 1; cw[(int)'N'] = 19; ch[(int)'N'] = 33;
u[(int)'O'] = 17; v[(int)'O'] = 31; cw[(int)'O'] = 20; ch[(int)'O'] = 33;
u[(int)'P'] = 54; v[(int)'P'] = 31; cw[(int)'P'] = 19; ch[(int)'P'] = 33;
u[(int)'Q'] = 90; v[(int)'Q'] = 31; cw[(int)'Q'] = 19; ch[(int)'Q'] = 33;
u[(int)'R'] = 126; v[(int)'R'] = 31; cw[(int)'R'] = 19; ch[(int)'R'] = 33;
u[(int)'S'] = 162; v[(int)'S'] = 31; cw[(int)'S'] = 19; ch[(int)'S'] = 33;
u[(int)'T'] = 198; v[(int)'T'] = 31; cw[(int)'T'] = 19; ch[(int)'T'] = 33;
u[(int)'U'] = 234; v[(int)'U'] = 31; cw[(int)'U'] = 19; ch[(int)'U'] = 33;
u[(int)'V'] = 270; v[(int)'V'] = 31; cw[(int)'V'] = 19; ch[(int)'V'] = 33;
u[(int)'W'] = 306; v[(int)'W'] = 31; cw[(int)'W'] = 19; ch[(int)'W'] = 33;
u[(int)'X'] = 342; v[(int)'X'] = 31; cw[(int)'X'] = 19; ch[(int)'X'] = 33;
u[(int)'Y'] = 378; v[(int)'Y'] = 31; cw[(int)'Y'] = 19; ch[(int)'Y'] = 33;
u[(int)'Z'] = 414; v[(int)'Z'] = 31; cw[(int)'Z'] = 19; ch[(int)'Z'] = 33;
u[(int)'a'] = 450; v[(int)'a'] = 31; cw[(int)'a'] = 19; ch[(int)'a'] = 33;
u[(int)'b'] = 486; v[(int)'b'] = 31; cw[(int)'b'] = 19; ch[(int)'b'] = 33;
u[(int)'c'] = 17; v[(int)'c'] = 61; cw[(int)'c'] = 20; ch[(int)'c'] = 33;
u[(int)'d'] = 54; v[(int)'d'] = 61; cw[(int)'d'] = 19; ch[(int)'d'] = 33;
u[(int)'e'] = 90; v[(int)'e'] = 61; cw[(int)'e'] = 19; ch[(int)'e'] = 33;
u[(int)'f'] = 126; v[(int)'f'] = 61; cw[(int)'f'] = 19; ch[(int)'f'] = 33;
u[(int)'g'] = 162; v[(int)'g'] = 61; cw[(int)'g'] = 19; ch[(int)'g'] = 33;
u[(int)'h'] = 198; v[(int)'h'] = 61; cw[(int)'h'] = 19; ch[(int)'h'] = 33;
u[(int)'i'] = 234; v[(int)'i'] = 61; cw[(int)'i'] = 19; ch[(int)'i'] = 33;
u[(int)'j'] = 270; v[(int)'j'] = 61; cw[(int)'j'] = 19; ch[(int)'j'] = 33;
u[(int)'k'] = 306; v[(int)'k'] = 61; cw[(int)'k'] = 19; ch[(int)'k'] = 33;
u[(int)'l'] = 342; v[(int)'l'] = 61; cw[(int)'l'] = 19; ch[(int)'l'] = 33;
u[(int)'m'] = 378; v[(int)'m'] = 61; cw[(int)'m'] = 19; ch[(int)'m'] = 33;
u[(int)'n'] = 414; v[(int)'n'] = 61; cw[(int)'n'] = 19; ch[(int)'n'] = 33;
u[(int)'o'] = 450; v[(int)'o'] = 61; cw[(int)'o'] = 19; ch[(int)'o'] = 33;
u[(int)'p'] = 486; v[(int)'p'] = 61; cw[(int)'p'] = 19; ch[(int)'p'] = 33;
u[(int)'q'] = 17; v[(int)'q'] = 91; cw[(int)'q'] = 20; ch[(int)'q'] = 33;
u[(int)'r'] = 54; v[(int)'r'] = 91; cw[(int)'r'] = 19; ch[(int)'r'] = 33;
u[(int)'s'] = 90; v[(int)'s'] = 91; cw[(int)'s'] = 19; ch[(int)'s'] = 33;
u[(int)'t'] = 126; v[(int)'t'] = 91; cw[(int)'t'] = 19; ch[(int)'t'] = 33;
u[(int)'u'] = 162; v[(int)'u'] = 91; cw[(int)'u'] = 19; ch[(int)'u'] = 33;
u[(int)'v'] = 198; v[(int)'v'] = 91; cw[(int)'v'] = 19; ch[(int)'v'] = 33;
u[(int)'w'] = 234; v[(int)'w'] = 91; cw[(int)'w'] = 19; ch[(int)'w'] = 33;
u[(int)'x'] = 270; v[(int)'x'] = 91; cw[(int)'x'] = 19; ch[(int)'x'] = 33;
u[(int)'y'] = 306; v[(int)'y'] = 91; cw[(int)'y'] = 19; ch[(int)'y'] = 33;
u[(int)'z'] = 342; v[(int)'z'] = 91; cw[(int)'z'] = 19; ch[(int)'z'] = 33;
u[(int)'0'] = 378; v[(int)'0'] = 91; cw[(int)'0'] = 19; ch[(int)'0'] = 33;
u[(int)'1'] = 414; v[(int)'1'] = 91; cw[(int)'1'] = 19; ch[(int)'1'] = 33;
u[(int)'2'] = 450; v[(int)'2'] = 91; cw[(int)'2'] = 19; ch[(int)'2'] = 33;
u[(int)'3'] = 486; v[(int)'3'] = 91; cw[(int)'3'] = 19; ch[(int)'3'] = 33;
u[(int)'4'] = 17; v[(int)'4'] = 121; cw[(int)'4'] = 20; ch[(int)'4'] = 33;
u[(int)'5'] = 54; v[(int)'5'] = 121; cw[(int)'5'] = 19; ch[(int)'5'] = 33;
u[(int)'6'] = 90; v[(int)'6'] = 121; cw[(int)'6'] = 19; ch[(int)'6'] = 33;
u[(int)'7'] = 126; v[(int)'7'] = 121; cw[(int)'7'] = 19; ch[(int)'7'] = 33;
u[(int)'8'] = 162; v[(int)'8'] = 121; cw[(int)'8'] = 19; ch[(int)'8'] = 33;
u[(int)'9'] = 198; v[(int)'9'] = 121; cw[(int)'9'] = 19; ch[(int)'9'] = 33;
u[(int)'~'] = 234; v[(int)'~'] = 121; cw[(int)'~'] = 19; ch[(int)'~'] = 33;
u[(int)'!'] = 270; v[(int)'!'] = 121; cw[(int)'!'] = 19; ch[(int)'!'] = 33;
u[(int)'@'] = 306; v[(int)'@'] = 121; cw[(int)'@'] = 19; ch[(int)'@'] = 33;
u[(int)'#'] = 342; v[(int)'#'] = 121; cw[(int)'#'] = 19; ch[(int)'#'] = 33;
u[(int)'$'] = 378; v[(int)'$'] = 121; cw[(int)'$'] = 19; ch[(int)'$'] = 33;
u[(int)'%'] = 414; v[(int)'%'] = 121; cw[(int)'%'] = 19; ch[(int)'%'] = 33;
u[(int)'^'] = 450; v[(int)'^'] = 121; cw[(int)'^'] = 19; ch[(int)'^'] = 33;
u[(int)'&'] = 486; v[(int)'&'] = 121; cw[(int)'&'] = 19; ch[(int)'&'] = 33;
u[(int)'*'] = 17; v[(int)'*'] = 151; cw[(int)'*'] = 20; ch[(int)'*'] = 33;
u[(int)'('] = 54; v[(int)'('] = 151; cw[(int)'('] = 19; ch[(int)'('] = 33;
u[(int)')'] = 90; v[(int)')'] = 151; cw[(int)')'] = 19; ch[(int)')'] = 33;
u[(int)'_'] = 126; v[(int)'_'] = 151; cw[(int)'_'] = 19; ch[(int)'_'] = 33;
u[(int)'+'] = 162; v[(int)'+'] = 151; cw[(int)'+'] = 19; ch[(int)'+'] = 33;
u[(int)'{'] = 198; v[(int)'{'] = 151; cw[(int)'{'] = 19; ch[(int)'{'] = 33;
u[(int)'}'] = 234; v[(int)'}'] = 151; cw[(int)'}'] = 19; ch[(int)'}'] = 33;
u[(int)'|'] = 270; v[(int)'|'] = 151; cw[(int)'|'] = 19; ch[(int)'|'] = 33;
u[(int)':'] = 306; v[(int)':'] = 151; cw[(int)':'] = 19; ch[(int)':'] = 33;
u[(int)'"'] = 342; v[(int)'"'] = 151; cw[(int)'"'] = 19; ch[(int)'"'] = 33;
u[(int)'<'] = 378; v[(int)'<'] = 151; cw[(int)'<'] = 19; ch[(int)'<'] = 33;
u[(int)'>'] = 414; v[(int)'>'] = 151; cw[(int)'>'] = 19; ch[(int)'>'] = 33;
u[(int)'?'] = 450; v[(int)'?'] = 151; cw[(int)'?'] = 19; ch[(int)'?'] = 33;
u[(int)'`'] = 486; v[(int)'`'] = 151; cw[(int)'`'] = 19; ch[(int)'`'] = 33;
u[(int)'-'] = 17; v[(int)'-'] = 181; cw[(int)'-'] = 20; ch[(int)'-'] = 33;
u[(int)'='] = 54; v[(int)'='] = 181; cw[(int)'='] = 19; ch[(int)'='] = 33;
u[(int)'['] = 90; v[(int)'['] = 181; cw[(int)'['] = 19; ch[(int)'['] = 33;
u[(int)']'] = 126; v[(int)']'] = 181; cw[(int)']'] = 19; ch[(int)']'] = 33;
u[(int)'\\'] = 162; v[(int)'\\'] = 181; cw[(int)'\\'] = 19; ch[(int)'\\'] = 33;
u[(int)';'] = 198; v[(int)';'] = 181; cw[(int)';'] = 19; ch[(int)';'] = 33;
u[(int)'\''] = 234; v[(int)'\''] = 181; cw[(int)'\''] = 19; ch[(int)'\''] = 33;
u[(int)','] = 270; v[(int)','] = 181; cw[(int)','] = 19; ch[(int)','] = 33;
u[(int)'.'] = 306; v[(int)'.'] = 181; cw[(int)'.'] = 19; ch[(int)'.'] = 33;
u[(int)'/'] = 342; v[(int)'/'] = 181; cw[(int)'/'] = 19; ch[(int)'/'] = 33;

for(int i=0;i<127;i++)
{
	ch[i] = ch[i]-1;
}



u1[(int)'A'] = 23; v1[(int)'A'] = 345; cw1[(int)'A'] = 32; ch1[(int)'A'] = 49;
u1[(int)'B'] = 65; v1[(int)'B'] = 345; cw1[(int)'B'] = 26; ch1[(int)'B'] = 49;
u1[(int)'C'] = 102; v1[(int)'C'] = 345; cw1[(int)'C'] = 28; ch1[(int)'C'] = 49;
u1[(int)'D'] = 140; v1[(int)'D'] = 345; cw1[(int)'D'] = 28; ch1[(int)'D'] = 49;
u1[(int)'E'] = 180; v1[(int)'E'] = 345; cw1[(int)'E'] = 26; ch1[(int)'E'] = 49;
u1[(int)'F'] = 216; v1[(int)'F'] = 345; cw1[(int)'F'] = 24; ch1[(int)'F'] = 49;
u1[(int)'G'] = 247; v1[(int)'G'] = 345; cw1[(int)'G'] = 28; ch1[(int)'G'] = 49;
u1[(int)'H'] = 287; v1[(int)'H'] = 345; cw1[(int)'H'] = 30; ch1[(int)'H'] = 49;
u1[(int)'I'] = 328; v1[(int)'I'] = 345; cw1[(int)'I'] = 11; ch1[(int)'I'] = 49;
u1[(int)'J'] = 346; v1[(int)'J'] = 345; cw1[(int)'J'] = 25; ch1[(int)'J'] = 49;
u1[(int)'K'] = 383; v1[(int)'K'] = 345; cw1[(int)'K'] = 27; ch1[(int)'K'] = 49;
u1[(int)'L'] = 420; v1[(int)'L'] = 345; cw1[(int)'L'] = 24; ch1[(int)'L'] = 49;
u1[(int)'M'] = 454; v1[(int)'M'] = 345; cw1[(int)'M'] = 35; ch1[(int)'M'] = 49;
u1[(int)'N'] = 501; v1[(int)'N'] = 345; cw1[(int)'N'] = 30; ch1[(int)'N'] = 49;
u1[(int)'O'] = 541; v1[(int)'O'] = 345; cw1[(int)'O'] = 30; ch1[(int)'O'] = 49;
u1[(int)'P'] = 582; v1[(int)'P'] = 345; cw1[(int)'P'] = 27; ch1[(int)'P'] = 49;
u1[(int)'Q'] = 618; v1[(int)'Q'] = 345; cw1[(int)'Q'] = 30; ch1[(int)'Q'] = 49;
u1[(int)'R'] = 660; v1[(int)'R'] = 345; cw1[(int)'R'] = 28; ch1[(int)'R'] = 49;
u1[(int)'S'] = 696; v1[(int)'S'] = 345; cw1[(int)'S'] = 26; ch1[(int)'S'] = 49;
u1[(int)'T'] = 728; v1[(int)'T'] = 345; cw1[(int)'T'] = 33; ch1[(int)'T'] = 49;
u1[(int)'U'] = 767; v1[(int)'U'] = 345; cw1[(int)'U'] = 27; ch1[(int)'U'] = 49;
u1[(int)'V'] = 804; v1[(int)'V'] = 345; cw1[(int)'V'] = 32; ch1[(int)'V'] = 49;
u1[(int)'W'] = 843; v1[(int)'W'] = 345; cw1[(int)'W'] = 39; ch1[(int)'W'] = 49;
u1[(int)'X'] = 889; v1[(int)'X'] = 345; cw1[(int)'X'] = 31; ch1[(int)'X'] = 49;
u1[(int)'Y'] = 927; v1[(int)'Y'] = 345; cw1[(int)'Y'] = 31; ch1[(int)'Y'] = 49;
u1[(int)'Z'] = 963; v1[(int)'Z'] = 345; cw1[(int)'Z'] = 31; ch1[(int)'Z'] = 49;
u1[(int)'a'] = 24; v1[(int)'a'] = 412; cw1[(int)'a'] = 23; ch1[(int)'a'] = 49;
u1[(int)'b'] = 57; v1[(int)'b'] = 412; cw1[(int)'b'] = 22; ch1[(int)'b'] = 49;
u1[(int)'c'] = 88; v1[(int)'c'] = 412; cw1[(int)'c'] = 19; ch1[(int)'c'] = 49;
u1[(int)'d'] = 117; v1[(int)'d'] = 412; cw1[(int)'d'] = 26; ch1[(int)'d'] = 49;
u1[(int)'e'] = 152; v1[(int)'e'] = 412; cw1[(int)'e'] = 21; ch1[(int)'e'] = 49;
u1[(int)'f'] = 183; v1[(int)'f'] = 412; cw1[(int)'f'] = 17; ch1[(int)'f'] = 49;
u1[(int)'g'] = 202; v1[(int)'g'] = 412; cw1[(int)'g'] = 28; ch1[(int)'g'] = 49;
u1[(int)'h'] = 238; v1[(int)'h'] = 412; cw1[(int)'h'] = 26; ch1[(int)'h'] = 49;
u1[(int)'i'] = 273; v1[(int)'i'] = 412; cw1[(int)'i'] = 11; ch1[(int)'i'] = 49;
u1[(int)'j'] = 286; v1[(int)'j'] = 412; cw1[(int)'j'] = 21; ch1[(int)'j'] = 49;
u1[(int)'k'] = 316; v1[(int)'k'] = 412; cw1[(int)'k'] = 24; ch1[(int)'k'] = 49;
u1[(int)'l'] = 349; v1[(int)'l'] = 412; cw1[(int)'l'] = 11; ch1[(int)'l'] = 49;
u1[(int)'m'] = 370; v1[(int)'m'] = 412; cw1[(int)'m'] = 37; ch1[(int)'m'] = 49;
u1[(int)'n'] = 417; v1[(int)'n'] = 412; cw1[(int)'n'] = 26; ch1[(int)'n'] = 49;
u1[(int)'o'] = 452; v1[(int)'o'] = 412; cw1[(int)'o'] = 20; ch1[(int)'o'] = 49;
u1[(int)'p'] = 482; v1[(int)'p'] = 412; cw1[(int)'p'] = 26; ch1[(int)'p'] = 49;
u1[(int)'q'] = 517; v1[(int)'q'] = 412; cw1[(int)'q'] = 23; ch1[(int)'q'] = 49;
u1[(int)'r'] = 550; v1[(int)'r'] = 412; cw1[(int)'r'] = 23; ch1[(int)'r'] = 49;
u1[(int)'s'] = 578; v1[(int)'s'] = 412; cw1[(int)'s'] = 18; ch1[(int)'s'] = 49;
u1[(int)'t'] = 604; v1[(int)'t'] = 412; cw1[(int)'t'] = 22; ch1[(int)'t'] = 49;
u1[(int)'u'] = 634; v1[(int)'u'] = 412; cw1[(int)'u'] = 23; ch1[(int)'u'] = 49;
u1[(int)'v'] = 666; v1[(int)'v'] = 412; cw1[(int)'v'] = 24; ch1[(int)'v'] = 49;
u1[(int)'w'] = 698; v1[(int)'w'] = 412; cw1[(int)'w'] = 34; ch1[(int)'w'] = 49;
u1[(int)'x'] = 739; v1[(int)'x'] = 412; cw1[(int)'x'] = 23; ch1[(int)'x'] = 49;
u1[(int)'y'] = 769; v1[(int)'y'] = 412; cw1[(int)'y'] = 23; ch1[(int)'y'] = 49;
u1[(int)'z'] = 799; v1[(int)'z'] = 412; cw1[(int)'z'] = 24; ch1[(int)'z'] = 49;
u1[(int)'1'] = 26; v1[(int)'1'] = 479; cw1[(int)'1'] = 27; ch1[(int)'1'] = 49;
u1[(int)'2'] = 62; v1[(int)'2'] = 479; cw1[(int)'2'] = 29; ch1[(int)'2'] = 49;
u1[(int)'3'] = 101; v1[(int)'3'] = 479; cw1[(int)'3'] = 26; ch1[(int)'3'] = 49;
u1[(int)'4'] = 137; v1[(int)'4'] = 479; cw1[(int)'4'] = 30; ch1[(int)'4'] = 49;
u1[(int)'5'] = 178; v1[(int)'5'] = 479; cw1[(int)'5'] = 30; ch1[(int)'5'] = 49;
u1[(int)'6'] = 216; v1[(int)'6'] = 479; cw1[(int)'6'] = 27; ch1[(int)'6'] = 49;
u1[(int)'7'] = 254; v1[(int)'7'] = 479; cw1[(int)'7'] = 25; ch1[(int)'7'] = 49;
u1[(int)'8'] = 294; v1[(int)'8'] = 479; cw1[(int)'8'] = 22; ch1[(int)'8'] = 49;
u1[(int)'9'] = 332; v1[(int)'9'] = 479; cw1[(int)'9'] = 24; ch1[(int)'9'] = 49;
u1[(int)'0'] = 365; v1[(int)'0'] = 479; cw1[(int)'0'] = 30; ch1[(int)'0'] = 49;
u1[(int)'`'] = 23; v1[(int)'`'] = 546; cw1[(int)'`'] = 14; ch1[(int)'`'] = 49;
u1[(int)'!'] = 47; v1[(int)'!'] = 546; cw1[(int)'!'] = 11; ch1[(int)'!'] = 49;
u1[(int)'"'] = 69; v1[(int)'"'] = 546; cw1[(int)'"'] = 16; ch1[(int)'"'] = 49;
u1[(int)'$'] = 92; v1[(int)'$'] = 546; cw1[(int)'$'] = 23; ch1[(int)'$'] = 49;
u1[(int)'%'] = 125; v1[(int)'%'] = 546; cw1[(int)'%'] = 28; ch1[(int)'%'] = 49;
u1[(int)'^'] = 162; v1[(int)'^'] = 546; cw1[(int)'^'] = 23; ch1[(int)'^'] = 49;
u1[(int)'&'] = 196; v1[(int)'&'] = 546; cw1[(int)'&'] = 32; ch1[(int)'&'] = 49;
u1[(int)'*'] = 236; v1[(int)'*'] = 546; cw1[(int)'*'] = 22; ch1[(int)'*'] = 49;
u1[(int)'('] = 266; v1[(int)'('] = 546; cw1[(int)'('] = 17; ch1[(int)'('] = 49;
u1[(int)')'] = 290; v1[(int)')'] = 546; cw1[(int)')'] = 15; ch1[(int)')'] = 49;
u1[(int)'-'] = 318; v1[(int)'-'] = 546; cw1[(int)'-'] = 20; ch1[(int)'-'] = 49;
u1[(int)'_'] = 347; v1[(int)'_'] = 546; cw1[(int)'_'] = 21; ch1[(int)'_'] = 49;
u1[(int)'='] = 376; v1[(int)'='] = 546; cw1[(int)'='] = 22; ch1[(int)'='] = 49;
u1[(int)'+'] = 409; v1[(int)'+'] = 546; cw1[(int)'+'] = 22; ch1[(int)'+'] = 49;
u1[(int)'['] = 442; v1[(int)'['] = 546; cw1[(int)'['] = 16; ch1[(int)'['] = 49;
u1[(int)'{'] = 465; v1[(int)'{'] = 546; cw1[(int)'{'] = 20; ch1[(int)'{'] = 49;
u1[(int)']'] = 490; v1[(int)']'] = 546; cw1[(int)']'] = 18; ch1[(int)']'] = 49;
u1[(int)'}'] = 516; v1[(int)'}'] = 546; cw1[(int)'}'] = 19; ch1[(int)'}'] = 49;
u1[(int)';'] = 542; v1[(int)';'] = 546; cw1[(int)';'] = 13; ch1[(int)';'] = 49;
u1[(int)':'] = 567; v1[(int)':'] = 546; cw1[(int)':'] = 9; ch1[(int)':'] = 49;
u1[(int)'\''] = 587; v1[(int)'\''] = 546; cw1[(int)'\''] = 9; ch1[(int)'\''] = 49;
u1[(int)'@'] = 606; v1[(int)'@'] = 546; cw1[(int)'@'] = 31; ch1[(int)'@'] = 49;
u1[(int)'#'] = 646; v1[(int)'#'] = 546; cw1[(int)'#'] = 32; ch1[(int)'#'] = 49;
u1[(int)'\\'] = 685; v1[(int)'\\'] = 546; cw1[(int)'\\'] = 23; ch1[(int)'\\'] = 49;
u1[(int)'|'] = 720; v1[(int)'|'] = 546; cw1[(int)'|'] = 8; ch1[(int)'|'] = 49;
u1[(int)','] = 740; v1[(int)','] = 546; cw1[(int)','] = 13; ch1[(int)','] = 49;
u1[(int)'<'] = 763; v1[(int)'<'] = 546; cw1[(int)'<'] = 20; ch1[(int)'<'] = 49;
u1[(int)'.'] = 793; v1[(int)'.'] = 546; cw1[(int)'.'] = 8; ch1[(int)'.'] = 49;
u1[(int)'>'] = 813; v1[(int)'>'] = 546; cw1[(int)'>'] = 20; ch1[(int)'>'] = 49;
u1[(int)'/'] = 841; v1[(int)'/'] = 546; cw1[(int)'/'] = 26; ch1[(int)'/'] = 49;
u1[(int)'?'] = 877; v1[(int)'?'] = 546; cw1[(int)'?'] = 20; ch1[(int)'?'] = 49;


	for(int i=0;i<127;i++)
	{
		v1[i] += 310;
	}

	for(int i=0;i<127;i++)
	{
		cw1[i] += 2;
	}



}

