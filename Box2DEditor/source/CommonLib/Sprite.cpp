#include "Sprite.h"
#include "Ref.h"
#include "math.h"
#include "AddData.h"

Sprite::Sprite(const char *texture, float x, float y, float drawWidth, float drawHeight, bool filterNearest, bool holdImageData)
{
	_texture = Ref::GetInstance()->GetTexture(texture, filterNearest, holdImageData);
	Reset(x, y, drawWidth, drawHeight, 0, 0, _texture->width, _texture->height);
}

Sprite::Sprite(const char *texture, float x, float y, float drawWidth, float drawHeight, int u, int v, int cropWidth, int cropHeight, bool filterNearest, bool holdImageData)
{
	_texture = Ref::GetInstance()->GetTexture(texture, filterNearest, holdImageData);
	Reset(x, y, drawWidth, drawHeight, u, v, cropWidth, cropHeight);
}

void Sprite::Reset(float x, float y, float drawWidth, float drawHeight, int u, int v, int cropWidth, int cropHeight)
{
	_x = x;
	_y = y;
	_drawWidth = drawWidth;
	_drawHeight = drawHeight;

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

	_isClipped = false;
	_flip = false;
	_horizontalFlip = false;
	_rotateBasedOnCenter = false;

	_angle = 0.0f;
	_centerX = _x+_drawWidth/2;
	_centerY = _y+_drawHeight/2;

	_isWaveOn = false;
	_waveSpeed = 5;
	_waveHeight = _drawHeight/10.0f;
	_waveInitAngle = 0;

	_phyBox = NULL;
}

void Sprite::SetAsPhysicsBody(bool val)
{
	if(val)
	{
		if(_phyBox == NULL)
			_phyBox = new PhyBox(_x, _y, _drawWidth, _drawHeight, 0, PhyBox::DYNAMIC_BODY);
	}
	else
	{
		if(_phyBox)
		{
			delete _phyBox;
			_phyBox = NULL;
		}
	}
}


ImageInfo* Sprite::GetTexture()
{
	return _texture;
}

void Sprite::Draw()
{
	if(_texture->textureID != 0 && _visible)
	{
		if(!_isClipped)
			DrawImageRegion( _x,_y, _drawWidth, _drawHeight, _u,_v,  _cropWidth,_cropHeight);
		else
			ClipImage(_cX, _cY, _cW, _cH);
	}
}


void Sprite::DrawFromCenter()
{
	if(_texture->textureID != 0 && _visible)
	{
		float x = _x;
		float y = _y;

		SetPos(_x-_drawWidth/2, _y-_drawHeight/2);

		if(!_isClipped)
			DrawImageRegion( _x,_y, _drawWidth, _drawHeight, _u,_v,  _cropWidth,_cropHeight);
		else
			ClipImage(_cX, _cY, _cW, _cH);

		SetPos(x, y);
	}
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
	if(_texture->imageRawData == NULL)
	{
		//Exception...
		int a = 0;
		int b = 1/a;
	}

	glDeleteTextures(1,&_texture->textureID);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D,0, _texture->format, _texture->width, _texture->height, 0,
				_texture->format, GL_UNSIGNED_BYTE, (GLvoid*)_texture->imageRawData);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	#ifdef PLATFORM_MARMALADE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	#endif

	#ifdef PLATFORM_WIN32
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	#endif

	_texture->textureID = textureID;
}



bool Sprite::ClipImage(int x, int y, int w, int h)
{
	if(Intersects((float)x, (float)y, (float)w, (float)h))
	{
		float x1 = _x;
		float y1 = _y;
		float x2 = _x+_drawWidth;
		float y2 = _y+_drawHeight;

		if( x1 < (float)x ) x1 = (float)x;
		if( y1 < (float)y ) y1 = (float)y;
		if( x2 > (float)(x+w) ) x2 = (float)(x+w);
		if( y2 > (float)(y+h) ) y2 = (float)(y+h);

		float nCropWidth = (x2-x1)* _cropWidth / _drawWidth;
		float nCropHeight = (y2-y1)* _cropHeight / _drawHeight;

		float nU = 0;
		float nV = 0;

		if(_x < x)
			nU = (x-_x) * _cropWidth / _drawWidth;

		if(_y < y)
			nV = (y-_y) * _cropHeight / _drawHeight;

		DrawImageRegion( x1,y1, (x2-x1),(y2-y1), (int)(_u+nU),(int)(_v+nV), (int)nCropWidth, (int)nCropHeight);
	}

	return false;
}


void Sprite::SetWaveEffect(bool isWaveOn)
{
	_isWaveOn = isWaveOn;
}

void Sprite::SetWaveSpeed(float waveSpeed)
{
	_waveSpeed = waveSpeed;
}

void Sprite::SetWaveHeight(float waveHeight)
{
	_waveHeight = waveHeight;
}


void Sprite::DrawImageRegion(float x, float y, float drawWidth, float drawHeight, int u, int v, int cropWidth, int cropHeight)
{
	if(_phyBox)
	{
		x = _phyBox->GetX();
		y = _phyBox->GetY();
		drawWidth = _phyBox->GetWidth();
		drawHeight = _phyBox->GetHeight();
		_angle = _phyBox->GetAngle();
		_rotateBasedOnCenter = true;
		_phyBox->Draw();
	}

	GLfloat ver[12];
	GLfloat uvs[12];

	ver[0] = x;				ver[1] = y;
	ver[2] = x+drawWidth;	ver[3] = y;
	ver[4] = x;				ver[5] = y+drawHeight;
	ver[6] = x+drawWidth;	ver[7] = y;
	ver[8] = x;				ver[9] = y+drawHeight;
	ver[10] = x+drawWidth;	ver[11] = y+drawHeight;

	float u1 = (float)u/(float)_texture->width;
	float v1 = (float)v/(float)_texture->height;
	float u2 = ((float)(u+cropWidth))/(float)_texture->width;
	float v2 = ((float)(v+cropHeight))/(float)_texture->height;

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

	if(_rotateBasedOnCenter)
	{
		_centerX = x+drawWidth/2;
		_centerY = y+drawHeight/2;
	}

	if(_isWaveOn == false)
	{
		if(_angle != 0.0f)
		{
			for(int i=0;i<12;i+=2)
			{
				Posf newpos = Posf::RotatePoint(ver[i], ver[i+1], _centerX, _centerY, _angle);
				ver[i] = newpos.x;
				ver[i+1] = newpos.y;
			}
		}

		glColor4f(((float)_red)/255.0f, ((float)_green)/255.0f, ((float)_blue)/255.0f, ((float)_alpha)/255.0f);

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
	}
	else
	{
		AddData* addData = AddData::GetInstance();
		addData->glColor4ub(_red,_green,_blue,_alpha);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _texture->textureID);

		addData->glBegin(GL_TRIANGLES);

		float imgX = GetX();
		float imgY = GetY();

		float imgH = GetHeight();
		float imgW = GetWidth();

		float sinWaveHeight = _waveHeight;
		float speed = _waveSpeed;

		float gap = 50;
		
		_waveInitAngle += speed;

		if(_waveInitAngle > 360)
			_waveInitAngle = _waveInitAngle - 360;

		for(float x = 0; x+gap <= 360; x+=gap)
		{
			float x1 = x;
			float x2 = x+gap;

			float y1 = imgY+sinf( (_waveInitAngle+x1)*(22.0f/7.0f) / 180.0f )*sinWaveHeight;
			float y2 = imgY+sinf( (_waveInitAngle+x2)*(22.0f/7.0f) / 180.0f )*sinWaveHeight;

			if(_angle != 0.0f)
			{
				Posf newpos1 = Posf::RotatePoint( imgX+(x1*imgW/360.0f), y1,		_centerX, _centerY, _angle);
				Posf newpos2 = Posf::RotatePoint( imgX+(x2*imgW/360.0f), y2,		_centerX, _centerY, _angle);
				Posf newpos3 = Posf::RotatePoint( imgX+(x1*imgW/360.0f), y1+imgH,	_centerX, _centerY, _angle);
				Posf newpos4 = Posf::RotatePoint( imgX+(x2*imgW/360.0f), y2,		_centerX, _centerY, _angle);
				Posf newpos5 = Posf::RotatePoint( imgX+(x1*imgW/360.0f), y1+imgH,	_centerX, _centerY, _angle);
				Posf newpos6 = Posf::RotatePoint( imgX+(x2*imgW/360.0f), y2+imgH,	_centerX, _centerY, _angle);

				addData->glTexCoord2f( x1/360.0f, 1-0.0f);	addData->glVertex3f( newpos1.x, newpos1.y, 0);
				addData->glTexCoord2f( x2/360.0f, 1-0.0f);	addData->glVertex3f( newpos2.x, newpos2.y, 0);
				addData->glTexCoord2f( x1/360.0f, 1-1.0f);	addData->glVertex3f( newpos3.x, newpos3.y, 0);
				addData->glTexCoord2f( x2/360.0f, 1-0.0f);	addData->glVertex3f( newpos4.x, newpos4.y, 0);
				addData->glTexCoord2f( x1/360.0f, 1-1.0f);	addData->glVertex3f( newpos5.x, newpos5.y, 0);
				addData->glTexCoord2f( x2/360.0f, 1-1.0f);	addData->glVertex3f( newpos6.x, newpos6.y, 0);
			}
			else
			{
				addData->glTexCoord2f( x1/360.0f,	1-0.0f);	addData->glVertex3f( imgX+(x1*imgW/360.0f), y1,		0);
				addData->glTexCoord2f( x2/360.0f,	1-0.0f);	addData->glVertex3f( imgX+(x2*imgW/360.0f), y2,		0);
				addData->glTexCoord2f( x1/360.0f,	1-1.0f);	addData->glVertex3f( imgX+(x1*imgW/360.0f), y1+imgH,0);
				addData->glTexCoord2f( x2/360.0f,	1-0.0f);	addData->glVertex3f( imgX+(x2*imgW/360.0f), y2,		0);
				addData->glTexCoord2f( x1/360.0f,	1-1.0f);	addData->glVertex3f( imgX+(x1*imgW/360.0f), y1+imgH,0);
				addData->glTexCoord2f( x2/360.0f,	1-1.0f);	addData->glVertex3f( imgX+(x2*imgW/360.0f), y2+imgH,0);
			}
		}

		addData->glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}


bool Sprite::Intersects(float rectX, float rectY, float rectWidth, float rectHeight)
{
	int tw = (int)_drawWidth;
	int th = (int)_drawHeight;
	int rw = (int)rectWidth;
	int rh = (int)rectHeight;

	if (rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) 
		return false; 

	int tx = (int)_x;
	int ty = (int)_y;
	int rx = (int)rectX;
	int ry = (int)rectY;

	rw += rx;
	rh += ry;
	tw += tx;
	th += ty;

	return ((rw < rx || rw > tx) &&	(rh < ry || rh > ty) &&	(tw < tx || tw > rx) && (th < ty || th > ry));	
}


bool Sprite::Contains(float X, float Y) 
{
	return ( X >= _x && X <= _x+_drawWidth && Y >= _y && Y <= _y+_drawHeight );
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

void Sprite::SetX(float x) 
{
	SetBounds(x, _y, _drawWidth, _drawHeight);
}
void Sprite::SetY(float y)
{
	SetBounds(_x, y, _drawWidth, _drawHeight);
}
void Sprite::SetPos(float x, float y)
{
	SetBounds(x, y, _drawWidth, _drawHeight);
}
void Sprite::SetWidth(float drawWidth)
{
	SetBounds(_x, _y, drawWidth, _drawHeight);
}
void Sprite::SetHeight(float drawHeight)
{
	SetBounds(_x, _y, _drawWidth, drawHeight);
}
void Sprite::SetSize(float drawWidth, float drawHeight)
{
	SetBounds(_x, _y, drawWidth, drawHeight);
}
void Sprite::SetBounds(float x, float y, float drawWidth, float drawHeight)
{
	_x = x;
	_y = y;
	_drawWidth = drawWidth;
	_drawHeight = drawHeight;

	if(_rotateBasedOnCenter)
		SetRotation(_angle);
}

void  Sprite::SetRotation(float angle)
{
	_rotateBasedOnCenter = true;
	_angle = ((int)angle%360) + (angle - (int)angle);
}

void Sprite::SetRotation(float angle, int centerX, int centerY)
{
	_rotateBasedOnCenter = false;
	_angle = ((int)angle%360) + (angle - (int)angle);
	_centerX = (float)centerX;
	_centerY = (float)centerY;
}

float Sprite::GetRotation()
{
	return _angle;
}

float Sprite::GetX() { return _x; }
float Sprite::GetY() { return _y; }
float Sprite::GetWidth() { return _drawWidth; }
float Sprite::GetHeight() { return _drawHeight; }


void Sprite::SetUVBounds(int u, int v, int cropWidth, int cropHeight)
{
	_u = u;
	_v = v;
	_cropWidth = cropWidth;
	_cropHeight = cropHeight;
}

int Sprite::GetU() { return _u; }
int Sprite::GetV() { return _v; }
void Sprite::SetU(int u) {	_u = u; }
void Sprite::SetV(int v) {	_v = v; }


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
void Sprite::SetAlpha(int alpha) 
{
	if(alpha < 0)
		alpha = 0;
	SetColor( GetRed(), GetGreen(), GetBlue(), alpha ); 
}


int Sprite::GetColor() { return _color; }
int Sprite::GetRed() { return _red; };
int Sprite::GetGreen() { return _green; };
int Sprite::GetBlue() { return _blue; };
int Sprite::GetAlpha() { return _alpha; };


Sprite::~Sprite()
{
	if(_texture)
		Ref::GetInstance()->UnLoad(_texture);
}
