#include "Sprite.h"
#include "Ref.h"
//#include "MyGL.h"
#include "math.h"
#include "RectPrim.h"

Sprite::Sprite()
{
	_texture = NULL;
}

Sprite::~Sprite(){}

void Sprite::Init(float x, float y, float drawWidth, float drawHeight, int bpp)
{
	FilePath = "";
	_texture = Ref::GetInstance()->GetTexture(drawWidth, drawHeight, bpp);
	Reset(x, y, drawWidth, drawHeight, 0, 0, 1, 1);
}

void Sprite::Init(const char *filePath, float x, float y, float drawWidth, float drawHeight, bool filterNearest)
{
	FilePath = filePath;
	_texture = Ref::GetInstance()->GetTexture(filePath, filterNearest);
	Reset(x, y, drawWidth, drawHeight, 0, 0, 1, 1);
}

void Sprite::Reset(float x, float y, float drawWidth, float drawHeight, float u, float v, float cropWidth, float cropHeight)
{
	_x = x;
	_y = y;
	_drawWidth = drawWidth;
	_drawHeight = drawHeight;

	_midX = _x + _drawWidth/2;
	_midY = _y + _drawHeight/2;

	_minVertexX = _x;
	_maxVertexX = _x + _drawWidth;

	_u = u;
	_v = v;
	_cropWidth = cropWidth;
	_cropHeight = cropHeight;

	_visible = true;
	_red = 255;
	_green = 255;
	_blue = 255;
	_alpha = 255;
	_color = 0xffffffff;

	_cX = 0;
	_cY = 0;
	_cW = 2000;
	_cH = 1400;

	_isClipped = false;
	_flip = false;
	_horizontalFlip = false;
	_rotateBasedOnCenter = false;

	_angle = 0.0f;
	_centerX = _x+_drawWidth/2;
	_centerY = _y+_drawHeight/2;

	_cellWidth = 48;
	_cellHeight = 48;
	_actionPerformedForSelection = false;
}

ImageInfo* Sprite::GetTexture()
{
	return _texture;
}

void Sprite::SetTexture(ImageInfo* imageInfo)
{
	_texture = imageInfo;
}

void Sprite::DrawInLocalAxis()
{
	if(_texture->textureID != 0 && _visible)
	{
		DrawImageRegion( _midX-_drawWidth/2, _midY-_drawHeight/2, _drawWidth, _drawHeight, _u,_v,  _cropWidth,_cropHeight);
	}
}

void Sprite::Draw()
{
	if(_texture->textureID != 0 && _visible)
	{
		//if(!_isClipped)
			DrawImageRegion( _x, _y, _drawWidth, _drawHeight, _u,_v,  _cropWidth,_cropHeight);
		//else
		//	ClipImage(_cX, _cY, _cW, _cH);
	}
}


void Sprite::DrawFromCenter()
{
	if(_texture->textureID != 0 && _visible)
	{
		float x = _x;
		float y = _y;

		SetPos(_x-_drawWidth/2, _y-_drawHeight/2);

		//if(!_isClipped)
			DrawImageRegion( _x,_y, _drawWidth, _drawHeight, _u,_v,  _cropWidth,_cropHeight);
		//else
		//	ClipImage(_cX, _cY, _cW, _cH);

		SetPos(x, y);
	}
}

bool Sprite::HitTest(float hitX, float hitY)
{
	Posf newPos = Posf::RotatePoint(hitX, hitY, _midX, _midY, -_angle);

	float hitX1 = newPos.x;        //applying anti rotation.
	float hitY1 = newPos.y;

	float xx = GetX();
	float yy = GetY();

	float hitX2 = hitX1 - GetX();    //applying anti translation.
	float hitY2 = hitY1 - GetY();

	if(hitX2 < 0 || hitY2 < 0 || hitX2 > GetWidth() || hitY2 > GetHeight())
		return false;

	float hitX3 = GetCropWidth_InPixels() * hitX2 / GetWidth();			//applying anti scaling
	float hitY3 = GetCropHeight_InPixels() * hitY2 / GetHeight() ;

	float hitX4 = hitX3 + _u*_texture->width;		//adding uv values.
	float hitY4 = hitY3 + _v*_texture->height;

   int arr[4];

   GetPixelColor( hitX4, hitY4, arr);

   return (arr[3]>1);
}

void Sprite::GetPixelColor(int x, int y, int* colorsArr)
{
	if(x <= _texture->width && y <= _texture->height)
	{
		y = _texture->height - y;

		int pos = (_texture->width*y + x) * 4;

		colorsArr[0] = _texture->imageRawData[ pos + 0];
		colorsArr[1] = _texture->imageRawData[ pos + 1];
		colorsArr[2] = _texture->imageRawData[ pos + 2];
		colorsArr[3] = _texture->imageRawData[ pos + 3];
	}
	else
	{
		colorsArr[0] = 0;
		colorsArr[1] = 0;
		colorsArr[2] = 0;
		colorsArr[3] = 0;
	}
}

void Sprite::SetPixelColorForRawData(int x, int y, int r, int g, int b, int a)
{
	if(x <= _texture->width && y <= _texture->height && x >= 0 && y >= 0)
	{
		y = _texture->height - y;

		int pos = (_texture->width*y + x) * 4;

		_texture->imageRawData[ pos + 0] = r;
		_texture->imageRawData[ pos + 1] = g;
		_texture->imageRawData[ pos + 2] = b;
		_texture->imageRawData[ pos + 3] = a;
	}
}

void Sprite::DeleteAndCreateWithRawData()
{
	glDeleteTextures(1,&_texture->textureID);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D,0, _texture->format, _texture->width, _texture->height, 0,
				_texture->format, GL_UNSIGNED_BYTE, (GLvoid*)_texture->imageRawData);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	_texture->textureID = textureID;
}

void Sprite::SetPixelColorFromBottom( int x, int y, int r, int g, int b, int a )
{
	unsigned char data[4];
	data[0] = r;
	data[1] = g;
	data[2] = b;
	data[3] = a;

	glBindTexture(GL_TEXTURE_2D, _texture->textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, _texture->format, GL_UNSIGNED_BYTE, data);
}

void Sprite::SetPixelColor( int x, int y, int r, int g, int b, int a )
{
	unsigned char data[4];
	data[0] = r;
	data[1] = g;
	data[2] = b;
	data[3] = a;

	glBindTexture(GL_TEXTURE_2D, _texture->textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, _texture->height-(y+1), 1, 1, _texture->format, GL_UNSIGNED_BYTE, data);
}


bool Sprite::ClipImage(int x, int y, int w, int h)
{
	if(Intersects(x, y, w, h))
	{
		int x1 = _x;
		int y1 = _y;
		int x2 = _x+_drawWidth;
		int y2 = _y+_drawHeight;

		if( x1 < x ) x1 = x;
		if( y1 < y ) y1 = y;
		if( x2 > x+w ) x2 = x+w;
		if( y2 > y+h ) y2 = y+h;

		float uInPixels = _u*(float)_texture->width;
		float vInPixels = _v*(float)_texture->height;
		float cropWidthInPixels = _cropWidth*(float)_texture->width;
		float cropHeightInPixels = _cropHeight*(float)_texture->height;

		int nCropWidth = (x2-x1)* cropWidthInPixels / _drawWidth;
		int nCropHeight = (y2-y1)* cropHeightInPixels / _drawHeight;

		int nU = 0;
		int nV = 0;

		if(_x < x)
			nU = (x-_x) * cropWidthInPixels / _drawWidth;

		if(_y < y)
			nV = (y-_y) * cropWidthInPixels / _drawHeight;

		DrawImageRegion(x1,y1,(x2-x1),(y2-y1),	_u+((float)nU/(float)_texture->width),
												_v+((float)nV/(float)_texture->height),
												(float)nCropWidth/(float)_texture->width,
												(float)nCropHeight/(float)_texture->height);
	}

	return false;
}


void Sprite::DrawImageRegion(float x, float y, float drawWidth, float drawHeight, float u, float v, float cropWidth, float cropHeight)
{
	GLfloat ver[12];
	GLfloat uvs[12];

	ver[0] = _midX -drawWidth/2;	ver[1] = _midY -drawHeight/2;
	ver[2] = _midX +drawWidth/2;	ver[3] = _midY -drawHeight/2;
	ver[4] = _midX -drawWidth/2;	ver[5] = _midY +drawHeight/2;
	ver[6] = _midX +drawWidth/2;	ver[7] = _midY -drawHeight/2;
	ver[8] = _midX -drawWidth/2;	ver[9] = _midY +drawHeight/2;
	ver[10] = _midX +drawWidth/2;	ver[11] = _midY +drawHeight/2;

	float u1 = u;
	float v1 = v;
	float u2 = u+cropWidth;
	float v2 = v+cropHeight;

	if(_texture->isMyImage)
	{
		uvs[0] = u1;	uvs[1] = v1;
		uvs[2] = u2;	uvs[3] = v1;
		uvs[4] = u1;	uvs[5] = v2;
		uvs[6] = u2;	uvs[7] = v1;
		uvs[8] = u1;	uvs[9] = v2;
		uvs[10] = u2;	uvs[11] = v2;
	}
	else
	{
		if(_flip)
		{
			uvs[0] = u1;	uvs[1] = 1.0f-v2;
			uvs[2] = u2;	uvs[3] = 1.0f-v2;
			uvs[4] = u1;	uvs[5] = 1.0f-v1;
			uvs[6] = u2;	uvs[7] = 1.0f-v2;
			uvs[8] = u1;	uvs[9] = 1.0f-v1;
			uvs[10] = u2;	uvs[11] = 1.0f-v1;
		}
		else if(_horizontalFlip)
		{
			uvs[0] = u2;	uvs[1] = 1.0f-v1;
			uvs[2] = u1;	uvs[3] = 1.0f-v1;
			uvs[4] = u2;	uvs[5] = 1.0f-v2;
			uvs[6] = u1;	uvs[7] = 1.0f-v1;
			uvs[8] = u2;	uvs[9] = 1.0f-v2;
			uvs[10] = u1;	uvs[11] = 1.0f-v2;
		}
		else
		{
			uvs[0] = u1;	uvs[1] = 1.0f-v1;
			uvs[2] = u2;	uvs[3] = 1.0f-v1;
			uvs[4] = u1;	uvs[5] = 1.0f-v2;
			uvs[6] = u2;	uvs[7] = 1.0f-v1;
			uvs[8] = u1;	uvs[9] = 1.0f-v2;
			uvs[10] = u2;	uvs[11] = 1.0f-v2;
		}
	}

	if(_angle != 0.0f)
	{
		for(int i=0;i<12;i+=2)
		{
			Posf newpos = Posf::RotatePoint(ver[i], ver[i+1], _midX, _midY, _angle);
			ver[i] = newpos.x;
			ver[i+1] = newpos.y;
		}
	}

	//glEnable(GL_SCISSOR_TEST);
	//glScissor(_cX, _cY, _cW, _cH);

	glColor4ub(_red, _green, _blue, _alpha);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, _texture->textureID);
	glVertexPointer(2, GL_FLOAT, 0, ver);
	glTexCoordPointer (2, GL_FLOAT, 0, uvs);

	glDrawArrays(GL_TRIANGLES,0,6);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_SCISSOR_TEST);

	//RectPrim::Draw(_midX -drawWidth/2, _midY -drawHeight/2, drawWidth, drawHeight, 255,0,0,255,true);
}


bool Sprite::Intersects(float rectX, float rectY, float rectWidth, float rectHeight)
{
	int tw = _drawWidth;
	int th = _drawHeight;
	int rw = rectWidth;
	int rh = rectHeight;

	if (rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) 
	{  
		return false; 
	}

	int tx = _midX-_drawWidth/2;
	int ty = _midY-_drawHeight/2;
	int rx = rectX;
	int ry = rectY;

	rw += rx;
	rh += ry;
	tw += tx;
	th += ty;

	return ((rw < rx || rw > tx) &&	(rh < ry || rh > ty) &&	(tw < tx || tw > rx) && (th < ty || th > ry));	
}


bool Sprite::Contains(float X, float Y, float addOffSet) 
{
	float x = _midX - _drawWidth/2;
	float y = _midY - _drawHeight/2;

	return (X >= x && X <= x+_drawWidth && Y >= y && Y <= y+_drawHeight);
}

void Sprite::SetClipArea(int x, int y, int w, int h)
{
	_isClipped = true;
	_cX = x;
	_cY = y;
	_cW = w;
	_cH = h;
}

void Sprite::DisableClipArea()
{
	_isClipped = false;
}

void Sprite::SetFlip(bool flip)
{
	_flip = flip;
}

void Sprite::SetHorizantalFlip(bool horizantalFlip)
{
	_horizontalFlip = horizantalFlip;
}

void Sprite::SetMidX(float midX)
{
	_midX = midX;
}

void Sprite::SetMidY(float midY)
{
	_midY = midY;
}

float Sprite::GetMidX()
{
	return _midX;
}

float Sprite::GetMidY()
{
	return _midY;
}


void Sprite::MoveXY(float dx, float dy)
{
	float prevMidX = _midX;
	float prevMidY = _midY;

	_midX += dx;
	_midY += dy;

	Posf newpos = Posf::RotatePoint(_midX, _midY, prevMidX, prevMidY, _angle);

	_midX = newpos.x;
	_midY = newpos.y;
}

void Sprite::SetX(float x)
{
	_midX = x+_drawWidth/2;
}
void Sprite::SetY(float y)
{
	_midY = y+_drawHeight/2;
}
void Sprite::SetPos(float x, float y)
{
	_midX = x+_drawWidth/2;
	_midY = y+_drawHeight/2;
}
void Sprite::SetWidth(float drawWidth)
{
	MoveXY( (drawWidth - _drawWidth)/2, 0 );
	_drawWidth = drawWidth;
}
void Sprite::SetHeight(float drawHeight)
{
	MoveXY( 0, (drawHeight - _drawHeight)/2 );
	_drawHeight = drawHeight;
}
void Sprite::SetSize(float drawWidth, float drawHeight)
{
	_drawWidth = drawWidth;
	_drawHeight = drawHeight;
}
void Sprite::SetBounds(float x, float y, float drawWidth, float drawHeight)
{
	_midX = x+_drawWidth/2;
	_midY = y+_drawHeight/2;

	SetWidth(drawWidth);
	SetHeight(drawHeight);
}

float Sprite::GetX() { return _midX -_drawWidth/2; }
float Sprite::GetY()	{ return _midY -_drawHeight/2; }

float Sprite::GetWidth() { return _drawWidth; }
float Sprite::GetHeight() { return _drawHeight; }

void  Sprite::SetRotation(float angle)
{
	_angle = ((int)angle%360) + (angle - (int)angle);
}

void Sprite::AddRotation(float angle, int pivotX, int pivotY)
{
	_angle += angle;

	if(_angle > 360)
		_angle = _angle - 360;

	Posf newpos = Posf::RotatePoint(_midX, _midY, pivotX, pivotY, angle);
	_midX = newpos.x;
	_midY = newpos.y;
}

float Sprite::GetRotation()
{
	return _angle;
}

float Sprite::GetU_0To1() { return _u; }
float Sprite::GetV_0To1() { return _v; }
float Sprite::GetCropWidth_0To1() { return _cropWidth; }
float Sprite::GetCropHeight_0To1() { return _cropHeight; }

int Sprite::GetU_InPixels() { return (int)(_u*(float)_texture->width); }
int Sprite::GetV_InPixels() { return (int)(_v*(float)_texture->height); }
int Sprite::GetCropWidth_InPixels() { return (int)(_cropWidth*(float)_texture->width); }
int Sprite::GetCropHeight_InPixels() { return (int)(_cropHeight*(float)_texture->height); }

void Sprite::SetU_0To1(float u) { _u = u; }
void Sprite::SetV_0To1(float v) { _v = v; }
void Sprite::SetCropWidth_0To1(float cw) { _cropWidth = cw; }
void Sprite::SetCropHeight_0To1(float ch) { _cropHeight = ch; }
void Sprite::SetUVBounds_0To1(float u, float v, float cropWidth, float cropHeight)
{
	_u = u;
	_v = v;
	_cropWidth = cropWidth;
	_cropHeight = cropHeight;
}

void Sprite::SetU_InPixels(int u) {	_u = (float)(u*1.0f) / _texture->width*1.0f; }
void Sprite::SetV_InPixels(int v) {	_v = (float)(v*1.0f) / _texture->height*1.0f; }
void Sprite::SetCropWidth_InPixels(int cw) { _cropWidth = (float)(cw*1.0f) / _texture->width*1.0f; }
void Sprite::SetCropHeight_InPixels(int ch) { _cropHeight = (float)(ch*1.0f) / _texture->height*1.0f; }
void Sprite::SetUVBounds_InPixels(int u, int v, int cropWidth, int cropHeight)
{
	_u = (float)(u*1.0f) / _texture->width*1.0f;
	_v = (float)(v*1.0f) / _texture->height*1.0f;
	_cropWidth = (float)(cropWidth*1.0f) / _texture->width*1.0f;
	_cropHeight = (float)(cropHeight*1.0f) / _texture->height*1.0f;
}

void Sprite::SetVisible(bool visible) { _visible = visible; }
bool Sprite::IsVisible() { return _visible; }

void Sprite::SetColor(int red, int green, int blue, int alpha)
{	if(red<0)	_red	= 0; else if(red	> 255)	_red	= 255; else _red	= red;
	if(green<0) _green	= 0; else if(green	> 255)	_green	= 255; else _green	= green;
	if(blue<0)	_blue	= 0; else if(blue	> 255)	_blue	= 255; else _blue	= blue;
	if(alpha<0) _alpha	= 0; else if(alpha	> 255)	_alpha	= 255; else _alpha	= alpha;

	_color = (unsigned int)( (_red<<24) + (_green<<16) + (_blue<<8) + _alpha );
}

void Sprite::SetColor(unsigned int color)
{
	_color	= color;
	_red	= (_color >> 24) & 255;
	_green	= (_color >> 16) & 255;
	_blue	= (_color >> 8) & 255;
	_alpha	= (_color ) & 255;
}

void Sprite::SetRed(int red) { SetColor( red, GetGreen(), GetBlue(), GetAlpha()); }
void Sprite::SetGreen(int green) { SetColor( GetRed(), green, GetBlue(), GetAlpha()); }
void Sprite::SetBlue(int blue) { SetColor( GetRed(), GetGreen(), blue, GetAlpha()); }
void Sprite::SetAlpha(int alpha) {	SetColor( GetRed(), GetGreen(), GetBlue(), alpha ); }


int Sprite::GetColor() { return _color; }
int Sprite::GetRed() { return _red; };
int Sprite::GetGreen() { return _green; };
int Sprite::GetBlue() { return _blue; };
int Sprite::GetAlpha() { return _alpha; };

Sprite* Sprite::Clone()
{
	Sprite* cloneSprite = new Sprite();

	cloneSprite->Init(FilePath.c_str(), GetX(), GetY(), GetWidth(), GetHeight());

	cloneSprite->SetU_0To1(GetU_0To1());
	cloneSprite->SetV_0To1(GetV_0To1());
	cloneSprite->SetCropWidth_0To1(GetCropWidth_0To1());
	cloneSprite->SetCropHeight_0To1(GetCropHeight_0To1());

	cloneSprite->SetRotation(GetRotation());
	cloneSprite->_horizontalFlip = _horizontalFlip;
	cloneSprite->SetColor(GetColor());
	cloneSprite->SetColor(GetRed(), GetGreen(), GetBlue(), GetAlpha());


	return cloneSprite;
}

void Sprite::Terminate()
{
	if(_texture)
		Ref::GetInstance()->UnLoad(_texture->textureID);
}
