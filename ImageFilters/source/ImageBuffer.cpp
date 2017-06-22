#include "ImageBuffer.h"
#include "PNG/PNGReadWrite.h"
#include "DefinesAndIncludes.h"

ImageBuffer::ImageBuffer(string imagePath)
{
	_imagePath = imagePath;

	for(unsigned int i=0;i<_imagePath.length();i++)
	{
		if(_imagePath[i] == '\\')
			_imagePath[i] = '/';
	}
	_width = 0;
	_height = 0;
	_bytesPerPixel = 0;
	_numMipMapLevels = 0;
	_imgType = 0;
	_compressedFormatBufSize = 0;


	if(_imagePath.find(".png") != -1 || _imagePath.find(".PNG") != -1)
	{
		_buf = PNGReadWrite::Read(_imagePath, &_width, &_height, &_bytesPerPixel);
		_imgType = PNG;
	}
}

ImageBuffer::ImageBuffer(int width, int height, int bytesPerPixel)
{
	_width = width;
	_height = height;
	_bytesPerPixel = bytesPerPixel;
	_buf = (unsigned char*) malloc( width*height*bytesPerPixel );

	memset(_buf, 0, width*height*bytesPerPixel);
}

ImageBuffer::ImageBuffer(int width, int height, int bytesPerPixel, unsigned char* buffer)
{
	_width = width;
	_height = height;
	_bytesPerPixel = bytesPerPixel;
	_buf = buffer;
}

ImageBuffer* ImageBuffer::Clone()
{
	unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	memcpy(newBuf, _buf, _width*_height*_bytesPerPixel);
	return new ImageBuffer(_width, _height, _bytesPerPixel, newBuf);
}

bool ImageBuffer::Write(string savePath)
{
	return PNGReadWrite::Write(_buf, _width, _height, _bytesPerPixel*8, savePath.c_str());
}

//bool SuiImageBuffer::BeginWrite(string savePath)
//{
//	return PNGReadWrite::BeginWritePNG(_buf, _width, _height, _bytesPerPixel*8, savePath.c_str());
//}
//
//int SuiImageBuffer::WritePercent(float percent)
//{
//	return PNGReadWrite::WritePercentPNG(percent);
//}
//
//void SuiImageBuffer::EndWrite()
//{
//	PNGReadWrite::EndWritePNG();
//}

unsigned int ImageBuffer::GetBufferSize()
{
	if(_imgType == DDS_DXT1 || _imgType == DDS_DXT3 || _imgType == DDS_DXT5)
		return _compressedFormatBufSize;
	
	return _width*_height*_bytesPerPixel;
}

int ImageBuffer::GetNumMipMapLevels()
{
	return _numMipMapLevels;
}

bool ImageBuffer::IsPNG()		{	return (_imgType == PNG);		}
bool ImageBuffer::IsJPEG()		{	return (_imgType == JPEG);		}

bool ImageBuffer::IsDDS_DXT1()	{	return (_imgType == DDS_DXT1);	}
bool ImageBuffer::IsDDS_DXT3()	{	return (_imgType == DDS_DXT3);	}
bool ImageBuffer::IsDDS_DXT5()	{	return (_imgType == DDS_DXT5);	}
bool ImageBuffer::IsDDS()		{	return (IsDDS_DXT1() || IsDDS_DXT3() || IsDDS_DXT5());	}

int ImageBuffer::GetWidth()
{
	return _width;
}

int ImageBuffer::GetHeight()
{
	return _height;
}

int ImageBuffer::GetBytesPerPixel()
{
	return _bytesPerPixel;
}

unsigned char* ImageBuffer::GetBuffer()
{
	return _buf;
}

bool ImageBuffer::HasTransparency()
{
	return (_bytesPerPixel == 4);
}

unsigned int ImageBuffer::GetPixelColor(int x, int y)
{
	if( x < 0 || x >= _width || y < 0 || y >= _height)
		return 0;

	unsigned int pos = (_width*y + x) * _bytesPerPixel;

	unsigned char r = _buf[ pos + 0];
	unsigned char g = _buf[ pos + 1];
	unsigned char b = _buf[ pos + 2];
	unsigned char a = 255;

	if(_bytesPerPixel == 4)
		a = _buf[ pos + 3];

	return (unsigned int) ((r<<24) + (g<<16) + (b<<8) + a);
}

int ImageBuffer::GetR(int x, int y)
{
	if( x < 0 || x >= _width || y < 0 || y >= _height)
		return 0;

	unsigned int pos = (_width*y + x) * _bytesPerPixel;
	return _buf[ pos + 0];
}

int ImageBuffer::GetG(int x, int y)
{
	if( x < 0 || x >= _width || y < 0 || y >= _height)
		return 0;

	unsigned int pos = (unsigned int)((_width*y + x) * _bytesPerPixel);
	return _buf[ pos + 1];
}

int ImageBuffer::GetB(int x, int y)
{
	if( x < 0 || x >= _width || y < 0 || y >= _height)
		return 0;

	unsigned int pos = (unsigned int)((_width*y + x) * (float)_bytesPerPixel);
	return _buf[ pos + 2];
}

void ImageBuffer::SetPixelColor(int x, int y, unsigned int pixelColor)
{
	if( x < 0 || x >= _width || y < 0 || y >= _height)
		return;

	unsigned int pos = (unsigned int)((_width*y + x) * (float)_bytesPerPixel);

	_buf[ pos + 0]	= (pixelColor >> 24) & 255;
	_buf[ pos + 1]	= (pixelColor >> 16) & 255;
	_buf[ pos + 2]	= (pixelColor >> 8) & 255;

	if(_bytesPerPixel == 4)
		_buf[ pos + 3]	= (pixelColor ) & 255;
}



//*********************************Image manipulation functions ******************************************/



bool ImageBuffer::ScaleInPixels(int newWidth, int newHeight, int scaleType)
{
	float oldW = (float)_width;
	float oldH = (float)_height;

	float newW = (float)newWidth;
	float newH = (float)newHeight;

	if(oldW == 0 || oldH == 0)
		return false;
	
	if(newW == oldW && newH == oldH)
		return true;

	if(newW == 0) newW = 1;
	if(newH == 0) newH = 1;

	int numBytes = (int)(newW*newH*_bytesPerPixel);
	unsigned char* newBuf = (unsigned char*)malloc(numBytes);
	memset(newBuf, 0, numBytes);

	unsigned char* srcBuf = _buf;	
	unsigned char* destinBuf = newBuf;

	if(scaleType == NEAREST)
	{
		for(int j=0;j<newH;j++)
		{
			int jj = (int)(j * oldH / newH);

			for(int i=0;i<newW;i++)
			{
				int ii = (int)(i * oldW / newW);
					
				unsigned int destinPos = (unsigned int)((newW*j + i) * _bytesPerPixel);
				unsigned int sourcePos = (unsigned int)((oldW*jj + ii) * _bytesPerPixel);

				destinBuf[ destinPos + 0] = srcBuf[ sourcePos + 0];
				destinBuf[ destinPos + 1] = srcBuf[ sourcePos + 1];
				destinBuf[ destinPos + 2] = srcBuf[ sourcePos + 2];
		
				if(_bytesPerPixel == 4)
					destinBuf[ destinPos + 3] = srcBuf[ sourcePos + 3];
			}
		}
	}
	else if(scaleType == LINEAR)
	{
		for(int j=0;j<newH;j++)
		{
			float jj = oldH * (float)j / newH;

			float y1 = (int)jj;
			float y2 = y1-1;	if(y2<0) y2=0;
			float y3 = y1;
			float y4 = y1-1;	if(y4<0) y4=0;

			float y1Percent = (jj - y1)*100.0f;
			float y2Percent = 100.0f - y1Percent;
			float y3Percent = y1Percent;
			float y4Percent = 100.0f - y1Percent;

			for(int i=0;i<newW;i++)
			{
				float ii = oldW * (float)i / newW;

				float x1 = (int)ii;
				float x2 = x1;
				float x3 = x1-1;	if(x3<0) x3=0;
				float x4 = x1-1;	if(x4<0) x4=0;

				float x1Percent = (ii - x1)*100.0f;
				float x2Percent = x1Percent;
				float x3Percent = 100.0f-x1Percent;
				float x4Percent = 100.0f-x1Percent;

				float fP1 = x1Percent * y1Percent / 10000.0f;
				float fP2 = x2Percent * y2Percent / 10000.0f;
				float fP3 = x3Percent * y3Percent / 10000.0f;
				float fP4 = x4Percent * y4Percent / 10000.0f;

				unsigned int p1 = (unsigned int)((oldW*y1 + x1) * _bytesPerPixel);
				unsigned int p2 = (unsigned int)((oldW*y2 + x2) * _bytesPerPixel);
				unsigned int p3 = (unsigned int)((oldW*y3 + x3) * _bytesPerPixel);
				unsigned int p4 = (unsigned int)((oldW*y4 + x4) * _bytesPerPixel);
					
				unsigned int destinPos = (unsigned int)((newW*j + i) * _bytesPerPixel);

				destinBuf[ destinPos + 0] = srcBuf[p1+0]*fP1 + srcBuf[p2+0]*fP2 + srcBuf[p3+0]*fP3 + srcBuf[p4+0]*fP4;
				destinBuf[ destinPos + 1] = srcBuf[p1+1]*fP1 + srcBuf[p2+1]*fP2 + srcBuf[p3+1]*fP3 + srcBuf[p4+1]*fP4;
				destinBuf[ destinPos + 2] = srcBuf[p1+2]*fP1 + srcBuf[p2+2]*fP2 + srcBuf[p3+2]*fP3 + srcBuf[p4+2]*fP4;
		
				if(_bytesPerPixel == 4)
					destinBuf[ destinPos + 3] = srcBuf[p1+3]*fP1 + srcBuf[p2+3]*fP2 + srcBuf[p3+3]*fP3 + srcBuf[p4+3]*fP4;
			}
		}
	}

	if(_buf)
		free(_buf);

	_buf = newBuf;
	_width = newWidth;
	_height = newHeight;

	return true;
}

bool ImageBuffer::CropImage(int x, int y, int width, int height)
{
	if(x >= _width || y >= _height || width <= 0 || height <= 0)
		return false;

	if(x < 0)
		x = 0;
	if(y < 0)
		y = 0;
	if(x+width > _width)
		width = _width - x;
	if(y+height > _height)
		height = _height - y;

	unsigned char* newBuf = (unsigned char*) malloc( width*height*_bytesPerPixel);

	int newW = width;
	int newH = height;

	int oldW = _width;
	int oldH = _height;

	for(int j=y; j<y+newH; j++)
	{
		unsigned int newBufPos = (newW*(j-y) + 0) * _bytesPerPixel;
		unsigned int sourcePos = (oldW*j + x) * _bytesPerPixel;

		memcpy(&newBuf[newBufPos], &_buf[sourcePos], newW*_bytesPerPixel);

		//for(int i=x; i<x+newW; i++)
		//{
		//	unsigned int newBufPos = (newW*(j-y) + (i-x)) * _bytesPerPixel;
		//	unsigned int sourcePos = (oldW*j + i) * _bytesPerPixel;

		//	newBuf[ newBufPos + 0] = _buf[ sourcePos + 0];
		//	newBuf[ newBufPos + 1] = _buf[ sourcePos + 1];
		//	newBuf[ newBufPos + 2] = _buf[ sourcePos + 2];
	
		//	if(_bytesPerPixel == 4)
		//		newBuf[ newBufPos + 3] = _buf[ sourcePos + 3];
		//}
	}

	free(_buf);
	_buf = newBuf;
	_width = newW;
	_height = newH;

	return true;
}


bool ImageBuffer::AddAlphaLayer()
{
	if(HasTransparency())
		return false;
	
	int newBytesPerPixel = 4;
	int newBufSize = _width * _height * newBytesPerPixel;
	unsigned char* newBuf = (unsigned char*) malloc(newBufSize);
	memset(newBuf, 0, newBufSize);

	int oldBufPos = 0;

	for(int newBufPos=0; newBufPos<newBufSize; )
	{
		newBuf [newBufPos + 0] = _buf [oldBufPos + 0];
		newBuf [newBufPos + 1] = _buf [oldBufPos + 1];
		newBuf [newBufPos + 2] = _buf [oldBufPos + 2];
		newBuf [newBufPos + 3] = 255;

		oldBufPos += _bytesPerPixel;
		newBufPos += newBytesPerPixel;
	}	

	free(_buf);
	_buf = newBuf;
	_bytesPerPixel = newBytesPerPixel;

	return true;
}

bool ImageBuffer::RemoveAlphaLayerIfNoDataLose()
{
	if(!HasTransparency())
		return false;

	int bufSize = _width * _height * _bytesPerPixel;

	for(int pos=0; pos < bufSize; pos+=_bytesPerPixel)
	{
		if(_buf[ pos+3 ] < 255)
			return false;
	}	

	RemoveAlphaLayer();

	return true;
}

bool ImageBuffer::RemoveAlphaLayer()
{
	if(!HasTransparency())
		return false;
	
	int newBytesPerPixel = 3;
	int newBufSize = _width * _height * newBytesPerPixel;
	unsigned char* newBuf = (unsigned char*) malloc(newBufSize);
	memset(newBuf, 255, newBufSize);

	int currBufPos = 0;

	for(int newBufPos=0; newBufPos<newBufSize; newBufPos += newBytesPerPixel)
	{
		newBuf [newBufPos + 0] = _buf [currBufPos + 0];
		newBuf [newBufPos + 1] = _buf [currBufPos + 1];
		newBuf [newBufPos + 2] = _buf [currBufPos + 2];

		currBufPos += _bytesPerPixel;
	}	

	free(_buf);
	_buf = newBuf;
	_bytesPerPixel = newBytesPerPixel;

	/*
	int oldBufPos = 0;

	for(int newBufPos=0; newBufPos<newBufSize; )
	{
		unsigned char oldA = _buf [oldBufPos + 3];
		if(oldA == 255)
		{
			newBuf [newBufPos + 0] = _buf [oldBufPos + 0];
			newBuf [newBufPos + 1] = _buf [oldBufPos + 1];
			newBuf [newBufPos + 2] = _buf [oldBufPos + 2];
		}
		else if(oldA == 0)
		{
			newBuf [newBufPos + 0] = 255;
			newBuf [newBufPos + 1] = 255;
			newBuf [newBufPos + 2] = 255;
		}
		else
		{
			float oldAf = (float)oldA/255.0f;

			newBuf[ newBufPos + 0] = _buf[ oldBufPos + 0]*oldAf + 255 * (1.0f-oldAf);
			newBuf[ newBufPos + 1] = _buf[ oldBufPos + 1]*oldAf + 255 * (1.0f-oldAf);
			newBuf[ newBufPos + 2] = _buf[ oldBufPos + 2]*oldAf + 255 * (1.0f-oldAf);
		}

		oldBufPos += _bytesPerPixel;
		newBufPos += newBytesPerPixel;
	}	
	*/

	free(_buf);
	_buf = newBuf;
	_bytesPerPixel = newBytesPerPixel;

	return true;
}

bool ImageBuffer::SetCanvasSize(int newW, int newH)
{
	if(newW <= 0 || newH <= 0)
		return false;

	int newBufSize = newW * newH * _bytesPerPixel;
	unsigned char* newBuf = (unsigned char*) malloc(newBufSize);

	if(_bytesPerPixel == 3) memset(newBuf, 255, newBufSize);
	else					memset(newBuf, 0, newBufSize);

	int oldW = _width;
	int oldH = _height;

	for(int j=0; j<oldH && j<newH; j++)
	{
		for(int i=0; i<oldW && i<newW; i++)
		{
			unsigned int sourcePos = (unsigned int)((oldW*j + i) * _bytesPerPixel);
			unsigned int destinPos = (unsigned int)((newW*j + i) * _bytesPerPixel);

			newBuf[ destinPos + 0] = _buf[ sourcePos + 0];
			newBuf[ destinPos + 1] = _buf[ sourcePos + 1];
			newBuf[ destinPos + 2] = _buf[ sourcePos + 2];
	
			if(_bytesPerPixel == 4)
				newBuf[ destinPos + 3] = _buf[ sourcePos + 3];
		}
	}

	free(_buf);
	_buf = newBuf;
	_width = newW;
	_height = newH;

	return true;
}

bool ImageBuffer::AddCanvas(int left, int right , int top, int bottom)
{
	int oldW = _width;
	int oldH = _height;

	int newW = oldW + left + right;
	int newH = oldH + top + bottom;

	if(newW <= 0 || newH <= 0)
		return false;

	int newBufSize = newW * newH * _bytesPerPixel;
	unsigned char* newBuf = (unsigned char*) malloc(newBufSize);

	if(_bytesPerPixel == 3) memset(newBuf, 255, newBufSize);
	else					memset(newBuf, 0, newBufSize);

	for(int j=0;j<oldH;j++)
	{
		for(int i=0;i<oldW;i++)
		{
			unsigned int sourcePos = (unsigned int)((oldW*j + i) * _bytesPerPixel);
			unsigned int destinPos = (unsigned int)((newW*(j+top) + (i+left)) * _bytesPerPixel);

			newBuf[ destinPos + 0] = _buf[ sourcePos + 0];
			newBuf[ destinPos + 1] = _buf[ sourcePos + 1];
			newBuf[ destinPos + 2] = _buf[ sourcePos + 2];
	
			if(_bytesPerPixel == 4)
				newBuf[ destinPos + 3] = _buf[ sourcePos + 3];
		}
	}

	free(_buf);
	_buf = newBuf;
	_width = newW;
	_height = newH;

	return true;
}
//
//SuiRect ImageBuffer::AutoCrop()
//{
//	if(HasTransparency() == false)
//		return SuiRect(0,0,0,0);
//
//	int oldW = _width;
//	int oldH = _height;
//
//	int minX = _width;
//	int maxX = 0;
//
//	int minY = _height;
//	int maxY = 0;
//
//	for(int y=0; y<oldH; y++)
//	{
//		for(int x=0; x<oldW; x++)
//		{
//			unsigned int pos = (oldW*y + x) * _bytesPerPixel;
//			unsigned char alpha = _buf[ pos + 3];
//
//			if(alpha != 0)
//			{
//				if(x < minX)	minX = x;
//				if(x > maxX)	maxX = x;
//
//				if(y < minY)	minY = y;
//				if(y > maxY)	maxY = y;
//			}
//		}
//	}
//
//	int subW = (maxX-minX)+1;
//	int subH = (maxY-minY)+1;
//
//	if(subW > _width) subW = _width;
//	if(subH > _height) subH = _height;
//
//	if(subW != _width || subH != _height)
//	{
//		CropImage(minX, minY, subW, subH);
//		return SuiRect(minX, minY, subW, subH);
//	}
//
//	return SuiRect(0,0,0,0);
//}

void ImageBuffer::GrayScale()
{
	float r1 = 0.2126f;
	float g1 = 0.7152f;
	float b1 = 0.0722f;

	//float r1 = 0.3;
	//float g1 = 0.59;
	//float b1 = 0.11;

	for(int y=0;y<_height;y++)
	{
		for(int x=0;x<_width;x++)
		{
			unsigned int pos = (_width*y + x) * _bytesPerPixel;

			unsigned char r = _buf[ pos + 0];
			unsigned char g = _buf[ pos + 1];
			unsigned char b = _buf[ pos + 2];

			//int value = (unsigned char)(r1*(float)r + g1*(float)g + b1*(float)b);

			int value = (r+g+b)/3;

			if(value > 255)
				value = 255;

			_buf[ pos + 0] = value;
			_buf[ pos + 1] = value;
			_buf[ pos + 2] = value;
		}
	}
}

void ImageBuffer::BlackAndWhite(int level)
{
	if(level < 0)
		return;
	if(level > 100)
		level = 100;

	level = (int)((float)level * 255.0f / 100.0f);

	for(int y=0;y<_height;y++)
	{
		for(int x=0;x<_width;x++)
		{
			unsigned int pos = (_width*y + x) * _bytesPerPixel;

			unsigned char r = _buf[ pos + 0];
			unsigned char g = _buf[ pos + 1];
			unsigned char b = _buf[ pos + 2];

			unsigned char value = (unsigned char)(0.2126*(float)r + 0.7152*(float)g + 0.0722*(float)b);

			//unsigned char value = (r+g+b)/3;

			if(value > level)
				value = 255;
			else
				value = 0;

			_buf[ pos + 0] = value;
			_buf[ pos + 1] = value;
			_buf[ pos + 2] = value;
		}
	}
}

void ImageBuffer::Colorize(int rdx, int gdx, int bdx)
{
	for(int y=0;y<_height;y++)
	{
		for(int x=0;x<_width;x++)
		{
			unsigned int pos = (_width*y + x) * _bytesPerPixel;

			unsigned char r = _buf[ pos + 0];
			unsigned char g = _buf[ pos + 1];
			unsigned char b = _buf[ pos + 2];

			unsigned char value = (unsigned char)(0.2126*(float)r + 0.7152*(float)g + 0.0722*(float)b);

			_buf[ pos + 0] = (value+rdx > 255) ? 255 : (value+rdx);
			_buf[ pos + 1] = (value+gdx > 255) ? 255 : (value+gdx);
			_buf[ pos + 2] = (value+bdx > 255) ? 255 : (value+bdx);
		}
	}
}

void ImageBuffer::Sepia(int level)
{
	if(level < 0)
		return;
	if(level > 100)
		level = 100;

	float effectLevel = 0;

	if(level < 50)
	{
		effectLevel = (50-level) / 250.0f;
		effectLevel = -effectLevel;
	}
	else
	{
		effectLevel = (level-50) / 250.0f;
	}

	float r1 = 0.393f + effectLevel;
	float g1 = 0.769f + effectLevel;
	float b1 = 0.189f + effectLevel;

	float r2 = 0.349f + effectLevel;
	float g2 = 0.686f + effectLevel;
	float b2 = 0.168f + effectLevel;

	float r3 = 0.272f + effectLevel;
	float g3 = 0.534f + effectLevel;
	float b3 = 0.131f + effectLevel;

	for(int y=0;y<_height;y++)
	{
		for(int x=0;x<_width;x++)
		{
			unsigned int pos = (_width*y + x) * _bytesPerPixel;

			unsigned char r = _buf[ pos + 0];
			unsigned char g = _buf[ pos + 1];
			unsigned char b = _buf[ pos + 2];

			float valueR = r1*(float)r + g1*(float)g + b1*(float)b;
			float valueG = r2*(float)r + g2*(float)g + b2*(float)b;
			float valueB = r3*(float)r + g3*(float)g + b3*(float)b;

			_buf[ pos + 0] = (valueR > 255.0f) ? 255 : (unsigned char)valueR;
			_buf[ pos + 1] = (valueG > 255.0f) ? 255 : (unsigned char)valueG;
			_buf[ pos + 2] = (valueB > 255.0f) ? 255 : (unsigned char)valueB;
		}
	}
}


void ImageBuffer::BlurImage(int blurLevel)
{
	unsigned int size = _width*_height*_bytesPerPixel;

	unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	memcpy(newBuf, _buf, _width*_height*_bytesPerPixel);

	int blurLevelSQR = blurLevel * blurLevel;

	for(int j=0; j <_height; j++)
	{
		for(int i=0; i< _width; i++)
		{
			float sumR = 0;
			float sumG = 0;
			float sumB = 0;
			float sumA = 0;
			float count = 0;

			for(int n=-blurLevel; n<=blurLevel; n++)
			{
				for(int m=-blurLevel; m<=blurLevel; m++)
				{
					int xx = i+m;
					int yy = j+n;

					//float dist = ((xx-i)*(xx-i) + (yy-j)*(yy-j));

					//if(dist > blurLevelSQR)
					//	continue;

					if(xx < _width && yy < _height && xx >= 0 && yy >= 0)
					{
						unsigned int subPos = (_width*yy + xx) * _bytesPerPixel;

						if(subPos >= 0 && subPos+2 < size)
						{
							sumR += _buf[ subPos + 0];
							sumG += _buf[ subPos + 1];
							sumB += _buf[ subPos + 2];
							
							if(_bytesPerPixel == 4)
								sumA += _buf[ subPos + 3];
							
							count++;
						}
					}
				}
			}

			unsigned int pos = (_width*j + i) * _bytesPerPixel;

			if(pos >= 0 && pos+2 <size)
			{
				newBuf[pos+0] = (unsigned char)(sumR/count);
				newBuf[pos+1] = (unsigned char)(sumG/count);
				newBuf[pos+2] = (unsigned char)(sumB/count);
		
				if(_bytesPerPixel == 4)
					newBuf[pos+3] = (unsigned char)(sumA/count);
			}
		}
	}

	free(_buf);
	_buf = newBuf;
}

void ImageBuffer::Sharpen(int sharpLevel)
{
	if(sharpLevel < 0)
		return;

	if(sharpLevel > 100)
		sharpLevel = 100;

	int level = (sharpLevel <= 50) ? 1 : 2;
	int arrWidth = (sharpLevel <= 50) ? 3 : 5;

	int arrHalfW = arrWidth/2;

	float val3x3 = 0;
	float val5x5 = 0;

	if(sharpLevel <= 50)
	{
		val3x3 = (float)sharpLevel/50.0f;
		val5x5 = 0;
	}
	else
	{
		val3x3 = 1.0f;
		val5x5 = (float)(sharpLevel-50)/50.0f;
	}

	float center = 1.0f + val3x3 * 8 + val5x5 * 16;

	float subArr5x5[5][5] =
	{
		-val5x5,		-val5x5,		-val5x5,		-val5x5,		-val5x5,
		-val5x5,		-val3x3,		-val3x3,		-val3x3,		-val5x5,
		-val5x5,		-val3x3,		center,			-val3x3,		-val5x5,
		-val5x5,		-val3x3,		-val3x3,		-val3x3,		-val5x5,
		-val5x5,		-val5x5,		-val5x5,		-val5x5,		-val5x5
	};

	float subArr3x3[3][3] =
	{
		-val3x3,		-val3x3,		-val3x3,
		-val3x3,		center,			-val3x3,
		-val3x3,		-val3x3,		-val3x3
	};

	unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	memcpy(newBuf, _buf, _width*_height*_bytesPerPixel);

	for(int y = 0; y < _height; y++) 
	for(int x = 0; x < _width; x++) 
	{ 
		float sumR = 0.0f;
		float sumG = 0.0f;
		float sumB = 0.0f;
		float actualCenter = center;

		for(int subY=-level; subY<=level; subY++)
		for(int subX=-level; subX<=level; subX++)
		{
			int xx = x+subX;
			int yy = y+subY;

			if(xx < _width && yy < _height && xx >= 0 && yy >= 0)
			{
				if(subX == 0 && subY == 0)
					continue;

				unsigned int subPos = (_width*yy + xx) * _bytesPerPixel;

				if(level == 1)
				{
					sumR += _buf[ subPos + 0] * subArr3x3[subX+arrHalfW][subY+arrHalfW]; 
					sumG += _buf[ subPos + 1] * subArr3x3[subX+arrHalfW][subY+arrHalfW]; 
					sumB += _buf[ subPos + 2] * subArr3x3[subX+arrHalfW][subY+arrHalfW];
				}
				else if(level == 2)
				{
					sumR += _buf[ subPos + 0] * subArr5x5[subX+arrHalfW][subY+arrHalfW]; 
					sumG += _buf[ subPos + 1] * subArr5x5[subX+arrHalfW][subY+arrHalfW]; 
					sumB += _buf[ subPos + 2] * subArr5x5[subX+arrHalfW][subY+arrHalfW];
				}
			}
			else
			{
				if(level == 1)
					actualCenter += subArr3x3[subX+arrHalfW][subY+arrHalfW];
				else if(level == 2)
					actualCenter += subArr5x5[subX+arrHalfW][subY+arrHalfW];
			}
		}

		unsigned int subPos = (_width*y + x) * _bytesPerPixel;

		sumR += _buf[ subPos + 0] * actualCenter; 
		sumG += _buf[ subPos + 1] * actualCenter; 
		sumB += _buf[ subPos + 2] * actualCenter;

		unsigned int pos = (_width*y + x) * _bytesPerPixel;

		newBuf[ pos+0 ] = min(max(int(sumR), 0), 255); 
		newBuf[ pos+1 ] = min(max(int(sumG), 0), 255); 
		newBuf[ pos+2 ] = min(max(int(sumB), 0), 255);
	}

	free(_buf);
	_buf = newBuf;
}

void ImageBuffer::EdgeDetection(int effectLevel)
{
	if(effectLevel < 0)
		return;

	if(effectLevel > 100)
		effectLevel = 100;



	int level = (effectLevel <= 50) ? 1 : 2;
	int arrWidth = (effectLevel <= 50) ? 3 : 5;

	int arrHalfW = arrWidth/2;

	float val3x3 = 0;
	float val5x5 = 0;

	if(effectLevel <= 50)
	{
		val3x3 = (float)effectLevel/50.0f;
		val5x5 = 0;
	}
	else
	{
		val3x3 = 1.0f;
		val5x5 = (float)(effectLevel-50)/50.0f;
	}

	float center = val3x3 * 8 + val5x5 * 16;

	float subArr5x5[5][5] =
	{
		-val5x5,		-val5x5,		-val5x5,		-val5x5,		-val5x5,
		-val5x5,		-val3x3,		-val3x3,		-val3x3,		-val5x5,
		-val5x5,		-val3x3,		center,			-val3x3,		-val5x5,
		-val5x5,		-val3x3,		-val3x3,		-val3x3,		-val5x5,
		-val5x5,		-val5x5,		-val5x5,		-val5x5,		-val5x5
	};

	float subArr3x3[3][3] =
	{
		-val3x3,		-val3x3,		-val3x3,
		-val3x3,		center,			-val3x3,
		-val3x3,		-val3x3,		-val3x3
	};

	unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	memcpy(newBuf, _buf, _width*_height*_bytesPerPixel);

	for(int y = 0; y < _height; y++) 
	for(int x = 0; x < _width; x++) 
	{ 
		float sumR = 0.0f;
		float sumG = 0.0f;
		float sumB = 0.0f;
		float actualCenter = center;

		for(int subY=-level; subY<=level; subY++)
		for(int subX=-level; subX<=level; subX++)
		{
			int xx = x+subX;
			int yy = y+subY;

			if(xx < _width && yy < _height && xx >= 0 && yy >= 0)
			{
				if(subX == 0 && subY == 0)
					continue;

				unsigned int subPos = (_width*yy + xx) * _bytesPerPixel;

				if(level == 1)
				{
					sumR += _buf[ subPos + 0] * subArr3x3[subX+arrHalfW][subY+arrHalfW]; 
					sumG += _buf[ subPos + 1] * subArr3x3[subX+arrHalfW][subY+arrHalfW]; 
					sumB += _buf[ subPos + 2] * subArr3x3[subX+arrHalfW][subY+arrHalfW];
				}
				else if(level == 2)
				{
					sumR += _buf[ subPos + 0] * subArr5x5[subX+arrHalfW][subY+arrHalfW]; 
					sumG += _buf[ subPos + 1] * subArr5x5[subX+arrHalfW][subY+arrHalfW]; 
					sumB += _buf[ subPos + 2] * subArr5x5[subX+arrHalfW][subY+arrHalfW];
				}
			}
			else
			{
				if(level == 1)
					actualCenter += subArr3x3[subX+arrHalfW][subY+arrHalfW];
				else if(level == 2)
					actualCenter += subArr5x5[subX+arrHalfW][subY+arrHalfW];
			}
		}

		unsigned int subPos = (_width*y + x) * _bytesPerPixel;

		sumR += _buf[ subPos + 0] * actualCenter; 
		sumG += _buf[ subPos + 1] * actualCenter; 
		sumB += _buf[ subPos + 2] * actualCenter;

		unsigned int pos = (_width*y + x) * _bytesPerPixel;

		newBuf[ pos+0 ] = min(max(int(sumR), 0), 255); 
		newBuf[ pos+1 ] = min(max(int(sumG), 0), 255); 
		newBuf[ pos+2 ] = min(max(int(sumB), 0), 255);
	}

	free(_buf);
	_buf = newBuf;

	//int blurLevel = 1;

	//unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	//memcpy(newBuf, _buf, _width*_height*_bytesPerPixel);

	//for(int j=0; j <_height; j++)
	//{
	//	for(int i=0; i< _width; i++)
	//	{
	//		float sumR = 0;
	//		float sumG = 0;
	//		float sumB = 0;

	//		for(int n=-blurLevel; n<=blurLevel; n++)
	//		{
	//			for(int m=-blurLevel; m<=blurLevel; m++)
	//			{
	//				int xx = i+m;
	//				int yy = j+n;

	//				if(xx < _width && yy < _height && xx >= 0 && yy >= 0)
	//				{
	//					unsigned int subPos = (_width*yy + xx) * _bytesPerPixel;

	//					if(m == 0 && n == 0)
	//					{
	//						sumR += 8*_buf[ subPos + 0];
	//						sumG += 8*_buf[ subPos + 1];
	//						sumB += 8*_buf[ subPos + 2];
	//					}
	//					else
	//					{
	//						sumR -= _buf[ subPos + 0];
	//						sumG -= _buf[ subPos + 1];
	//						sumB -= _buf[ subPos + 2];
	//					}
	//				}
	//			}
	//		}

	//		if(sumR > 255) sumR = 255; else if(sumR < 0) sumR = 0;
	//		if(sumG > 255) sumG = 255; else if(sumG < 0) sumG = 0;
	//		if(sumB > 255) sumB = 255; else if(sumB < 0) sumB = 0;

	//		unsigned int pos = (_width*j + i) * _bytesPerPixel;

	//		newBuf[pos+0] = sumR;
	//		newBuf[pos+1] = sumG;
	//		newBuf[pos+2] = sumB;
	//	}
	//}

	//free(_buf);
	//_buf = newBuf;
}

void ImageBuffer::ColorOverlay(unsigned int color, int level)
{
	unsigned char r	= (color >> 24) & 255;
	unsigned char g	= (color >> 16) & 255;
	unsigned char b	= (color >> 8) & 255;

	ColorOverlay(r, g, b, level);
}

void ImageBuffer::ColorOverlay(unsigned char r, unsigned char g, unsigned char b, int level)
{
	unsigned char a	= level * 255/100;
	
	float alpha = a/255.0f;

	int buffSize = _width*_height*_bytesPerPixel;
	int pos = 0;

	for(int pos=0; pos<buffSize; )
	{
		_buf [pos + 0] = r*alpha + _buf [pos + 0] * (1.0f-alpha);
		_buf [pos + 1] = g*alpha + _buf [pos + 1] * (1.0f-alpha);
		_buf [pos + 2] = b*alpha + _buf [pos + 2] * (1.0f-alpha);

		if(_bytesPerPixel == 4)
		{
			if(_buf [pos + 3] + a > 255)
				_buf[pos+3] = 255;
			else
				_buf[pos+3] += a;
		}

		pos += _bytesPerPixel;
	}	
}

void ImageBuffer::Bright(int level)
{
	ColorOverlay(255,255,255,level);
}

void ImageBuffer::Dark(int level)
{
	ColorOverlay(0,0,0,level);
}

void ImageBuffer::Hue(int hueLevel)
{
	unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	memcpy(newBuf, _buf, _width*_height*_bytesPerPixel);

	for(int j=0; j <_height; j++)
	{
		for(int i=0; i< _width; i++)
		{
			unsigned int pos = (_width*j + i) * _bytesPerPixel;

			if(hueLevel >= 0 && hueLevel < 20)
			{
				float dr = _buf[pos+0] - _buf[pos+0];
				float dg = _buf[pos+2] - _buf[pos+1];
				float db = _buf[pos+1] - _buf[pos+2];

				float val = hueLevel - 0;

				newBuf[pos+0] = _buf[pos+0] + val * dr / 20;
				newBuf[pos+1] = _buf[pos+1] + val * dg / 20;
				newBuf[pos+2] = _buf[pos+2] + val * db / 20;
			}
			else if(hueLevel >= 20 && hueLevel < 40)
			{
				float dr = _buf[pos+1] - _buf[pos+0];
				float dg = _buf[pos+2] - _buf[pos+2];
				float db = _buf[pos+0] - _buf[pos+1];

				float val = hueLevel - 20;

				newBuf[pos+0] = _buf[pos+0] + val * dr / 20;
				newBuf[pos+1] = _buf[pos+2] + val * dg / 20;
				newBuf[pos+2] = _buf[pos+1] + val * db / 20;
			}
			else if(hueLevel >= 40 && hueLevel < 60)
			{
				float dr = _buf[pos+1] - _buf[pos+1];
				float dg = _buf[pos+0] - _buf[pos+2];
				float db = _buf[pos+2] - _buf[pos+0];

				float val = hueLevel - 40;

				newBuf[pos+0] = _buf[pos+1] + val * dr / 20;
				newBuf[pos+1] = _buf[pos+2] + val * dg / 20;
				newBuf[pos+2] = _buf[pos+0] + val * db / 20;
			}
			else if(hueLevel >= 60 && hueLevel < 80)
			{
				float dr = _buf[pos+2] - _buf[pos+1];
				float dg = _buf[pos+0] - _buf[pos+0];
				float db = _buf[pos+1] - _buf[pos+2];

				float val = hueLevel - 60;

				newBuf[pos+0] = _buf[pos+1] + val * dr / 20;
				newBuf[pos+1] = _buf[pos+0] + val * dg / 20;
				newBuf[pos+2] = _buf[pos+2] + val * db / 20;
			}
			else if(hueLevel >= 80 && hueLevel <= 100)
			{
				float dr = _buf[pos+2] - _buf[pos+2];
				float dg = _buf[pos+1] - _buf[pos+0];
				float db = _buf[pos+0] - _buf[pos+1];

				float val = hueLevel - 80;

				newBuf[pos+0] = _buf[pos+2] + val * dr / 20;
				newBuf[pos+1] = _buf[pos+0] + val * dg / 20;
				newBuf[pos+2] = _buf[pos+1] + val * db / 20;
			}
		}
	}

	free(_buf);
	_buf = newBuf;
}

bool ImageBuffer::SetTransparency(int level, bool add_AlphaLayer_if_not_exist)
{
	if(!HasTransparency() && !add_AlphaLayer_if_not_exist)
		return false;

	if(!HasTransparency() && add_AlphaLayer_if_not_exist)
		AddAlphaLayer();

	int bufSize = _width * _height * _bytesPerPixel;

	int pos = 0;

	unsigned char reduceA = level * 255 / 100;

	for(int pos=0; pos<bufSize; )
	{
		unsigned char currA = _buf [pos + 3];

		if(currA - reduceA < 0)
			_buf [pos + 3] = 0;
		else
			_buf [pos + 3] -= reduceA;

		pos += _bytesPerPixel;
	}
	return true;
}

void ImageBuffer::Invert()
{
	int bufSize = _width * _height * _bytesPerPixel;
	int pos = 0;

	for(int pos=0; pos<bufSize; pos += _bytesPerPixel)
	{
		_buf [pos + 0] = 255 - _buf [pos + 0];
		_buf [pos + 1] = 255 - _buf [pos + 1];
		_buf [pos + 2] = 255 - _buf [pos + 2];
	}
}

void ImageBuffer::MakeFeasibleToCompress(int compressionLevel)
{

	int bufSize = _width * _height * _bytesPerPixel;
	int pos = 0;

	float comLevel = 1;

	if(compressionLevel == 1)
		comLevel = 2;
	else if(compressionLevel == 2)
		comLevel = 4;
	else if(compressionLevel == 3)
		comLevel = 8;
	else if(compressionLevel == 4)
		comLevel = 16;
	else if(compressionLevel == 5)
		comLevel = 32;

	for(int pos=0; pos<bufSize; pos += _bytesPerPixel)
	{
		
		float rByComLevel = (float)(_buf [pos + 0]) / comLevel;
		int valR = (rByComLevel - (int)rByComLevel > 0.5) ? ((int)rByComLevel+1)*comLevel : (int)rByComLevel*comLevel;
		
		if(valR < 0)
			_buf[pos+0] = 0;
		else
			_buf[pos+0] = (valR > 255) ? 255 : valR;


		float gByComLevel = (float)(_buf [pos + 1]) / comLevel;
		int valG = (gByComLevel - (int)gByComLevel > 0.5) ? ((int)gByComLevel+1)*comLevel : (int)gByComLevel*comLevel;
		
		if(valG < 0)
			_buf[pos+1] = 0;
		else		
			_buf[pos+1] = (valG > 255) ? 255 : valG;

		float bByComLevel = (float)(_buf [pos + 2]) / comLevel;
		int valB = (bByComLevel - (int)bByComLevel > 0.5) ? ((int)bByComLevel+1)*comLevel : (int)bByComLevel*comLevel;

		if(valB < 0)
			_buf[pos+2] = 0;
		else		
			_buf[pos+2] = (valB > 255) ? 255 : valB;

		/*

		for(int i=0;i<3;i++)
		{
			int v = _buf[pos+i];

			if(v % 4 == 1)
				v--;
			else if(v % 4 == 3)
				v++;
			else if(v % 2 == 1)
				v--;

			if(v < 0)			v = 0;
			else if(v > 255)	v = 255;

			_buf[pos+i] = v;
		}

		*/

		//if(_bytesPerPixel == 4)
		//{
		//	if(_buf [pos + 3] <= 1)
		//	{
		//		_buf[pos+0] = 0;
		//		_buf[pos+1] = 0;
		//		_buf[pos+2] = 0;
		//		_buf[pos+3] = 0;
		//	}
		//	else
		//	{
		//		float aByComLevel = (float)(_buf [pos + 3] + 1) / comLevel;
		//		int valA = (aByComLevel - (int)aByComLevel > 0.5) ? ((int)aByComLevel+1)*comLevel : (int)aByComLevel*comLevel;
		//		_buf[pos+3] = (valA > 255) ? 255 : valA;
		//	}
		//}

		//_buf[pos+0] /= 4;
		//_buf[pos+1] /= 4;
		//_buf[pos+2] /= 4;

		//if(_bytesPerPixel == 4)
		//	_buf[pos+3] /= 4;
	}
}

void ImageBuffer::InvertValue()
{
	int bufSize = _width * _height * _bytesPerPixel;
	int pos = 0;

	int value;
	int value2;
	int min;
	int delta;

	for(int pos=0; pos<bufSize; pos += _bytesPerPixel)
	{
		int r = _buf [pos + 0];
		int g = _buf [pos + 1];
		int b = _buf [pos + 2];

		if (r > g)
		{
			value = r;
			min = g;

			if(b > r) value = b;
			if(b < g) min = b;
		}
		else
		{
			value = g;
			min = r;

			if(b > g) value = b;
			if(b < r) min = b;
		}

		delta = value - min;
		if(value == 0 || delta == 0)
		{
			r = 255 - value;
			g = 255 - value;
			b = 255 - value;
		}
		else
		{
			value2 = value / 2;

			if (r == value)
			{
				r = 255 - r;
				b = ((r * b) + value2) / value;
				g = ((r * g) + value2) / value;
			}
			else if (g == value)
			{
				g = 255 - g;
				r = ((g * r) + value2) / value;
				b = ((g * b) + value2) / value;
			}
			else
			{
				b = 255 - b;
				g = ((b * g) + value2) / value;
				r = ((b * r) + value2) / value;
			}
		}

		_buf [pos + 0] = r*(1.0f-0.1f);
		_buf [pos + 1] = g*(1.0f-0.1f);
		_buf [pos + 2] = b*(1.0f-0.1f);
	}
}

void ImageBuffer::InvertChannels(bool invertRedChannel, bool invertGreenChannel, bool invertBlueChannel)
{
	int bufSize = _width * _height * _bytesPerPixel;
	int pos = 0;

	int rCoff = 1;
	int gCoff = 1;
	int bCoff = 1;

	int rVal = 255;
	int gVal = 255;
	int bVal = 255;

	if(invertRedChannel == false)
	{
		rVal = 0;
		rCoff = -1;
	}

	if(invertGreenChannel == false)
	{
		gVal = 0;
		gCoff = -1;
	}

	if(invertBlueChannel == false)
	{
		bVal = 0;
		bCoff = -1;
	}

	for(int pos=0; pos<bufSize; )
	{
		_buf [pos + 0] = rVal - rCoff * _buf [pos + 0];
		_buf [pos + 1] = gVal - gCoff * _buf [pos + 1];
		_buf [pos + 2] = bVal - bCoff * _buf [pos + 2];

		pos += _bytesPerPixel;
	}
}

void ImageBuffer::FlipHorizantal()
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;

	for(int y=0;y<_height;y++)
	{
		for(int x=0;x<=_width/2;x++)
		{
			unsigned int pos = (_width*y + x) * _bytesPerPixel;
			unsigned int posFromLast = (_width*y + (_width-1-x)) * _bytesPerPixel;

			r = _buf[ pos + 0];
			g = _buf[ pos + 1];
			b = _buf[ pos + 2];

			_buf[ pos + 0] = _buf[ posFromLast + 0];
			_buf[ pos + 1] = _buf[ posFromLast + 1];
			_buf[ pos + 2] = _buf[ posFromLast + 2];

			_buf[ posFromLast + 0] = r;
			_buf[ posFromLast + 1] = g;
			_buf[ posFromLast + 2] = b;

			if(_bytesPerPixel == 4)
			{
				a = _buf[ pos + 3];
				_buf[ pos + 3] = _buf[ posFromLast + 3];
				_buf[ posFromLast + 3] = a;
			}
		}
	}
}

void ImageBuffer::FilpVertical()
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;

	unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	memset(newBuf, 0, _width*_height*_bytesPerPixel);

	for(int y=0;y<_height;y++)
	{
		unsigned int sourcePos = (_width*y + 0) * _bytesPerPixel;
		unsigned int newBufPos = (_width*(_height-1-y) + 0) * _bytesPerPixel;

		memcpy(&newBuf[newBufPos], &_buf[sourcePos], _width*_bytesPerPixel);
	}

	if(_buf)
	{
		free(_buf);
		_buf = NULL;
	}

	_buf = newBuf;
}

void ImageBuffer::Rotate90Degrees()
{
	int oldW = _width;
	int oldH = _height;

	int newW = _height;
	int newH = _width;

	int newBufSize = newW * newH * _bytesPerPixel;
	unsigned char* newBuf = (unsigned char*) malloc(newBufSize);

	for(int i=0;i<oldW;i++)
	{
		for(int j=oldH-1;j>=0;j--)
		{
			int sourcePos = (oldW*j + i) * _bytesPerPixel;
			int destinPos = (newW*i + oldH-1-j) * _bytesPerPixel;

			newBuf[ destinPos + 0] = _buf[ sourcePos + 0];
			newBuf[ destinPos + 1] = _buf[ sourcePos + 1];
			newBuf[ destinPos + 2] = _buf[ sourcePos + 2];
	
			if(_bytesPerPixel == 4)
				newBuf[ destinPos + 3] = _buf[ sourcePos + 3];
		}
	}

	free(_buf);
	_buf = newBuf;
	_width = newW;
	_height = newH;
}

void ImageBuffer::Rotate180Degrees()
{
	FlipHorizantal();
}

void ImageBuffer::Rotate270Degrees()
{
	int oldW = _width;
	int oldH = _height;

	int newW = _height;
	int newH = _width;

	int newBufSize = newW * newH * _bytesPerPixel;
	unsigned char* newBuf = (unsigned char*) malloc(newBufSize);

	for(int i=0;i<oldW;i++)
	{
		for(int j=0;j<oldH;j++)
		{
			int sourcePos = (oldW*j + i) * _bytesPerPixel;
			int destinPos = (newW*(oldW-1-i) + j) * _bytesPerPixel;

			newBuf[ destinPos + 0] = _buf[ sourcePos + 0];
			newBuf[ destinPos + 1] = _buf[ sourcePos + 1];
			newBuf[ destinPos + 2] = _buf[ sourcePos + 2];
	
			if(_bytesPerPixel == 4)
				newBuf[ destinPos + 3] = _buf[ sourcePos + 3];
		}
	}

	free(_buf);
	_buf = newBuf;
	_width = newW;
	_height = newH;
}

void ImageBuffer::PixelateNearest(int level)
{
	float effectLevel = (float)level;
	effectLevel *= 0.5f;
	effectLevel++;

	int actualW = _width;
	int actualH = _height;

	ScaleInPixels( _width/effectLevel, _height/effectLevel, NEAREST);
	ScaleInPixels( actualW, actualH, NEAREST);
}

void ImageBuffer::Contrast(int level)
{
	float effectLevel = (float)level / 100.0f;

	float color[3];

	for(int j=0; j <_height; j++)
	{
		for(int i=0; i< _width; i++)
		{
			unsigned int pos = (_width*j + i) * _bytesPerPixel;

			color[0] = _buf[ pos + 0];
			color[1] = _buf[ pos + 1];
			color[2] = _buf[ pos + 2];

			for(int channelIndex=0; channelIndex<3; channelIndex++)
			{
				if(color[channelIndex] > 128.0f)
				{
					color[channelIndex] *= 1 + effectLevel;
					if(color[channelIndex] > 255)
						color[channelIndex] = 255;
				}
				else
					color[channelIndex] *= 1 - effectLevel;
			}

			_buf[ pos + 0] = color[0];
			_buf[ pos + 1] = color[1];
			_buf[ pos + 2] = color[2];
		}
	}
}

void ImageBuffer::NegativePhoto()
{
	Invert();
	GrayScale();
}

void ImageBuffer::Explosion(int level)
{
	if(level <= 0)
		return;

	level++;

	for(int y=0; y <_height; y++)
	{
		for(int x=0; x< _width; x++)
		{
			int randX = rand()%level - level/2;
			int randY = rand()%level - level/2;

			int x2 = x+randX;
			int y2 = y+randY;

			if(x2 >=0 && x2< _width && y2 >=0 && y2 < _height)
			{
				unsigned int color1 = GetPixelColor( x, y);
				unsigned int color2 = GetPixelColor( x2, y2);

				SetPixelColor(x, y, color2);
				SetPixelColor(x2, y2, color1);
			}
		}
	}
}

void ImageBuffer::Emboss(int embossLevel)
{
	if(embossLevel < 0)
		return;

	if(embossLevel > 100)
		embossLevel = 100;

	int level = (embossLevel <= 50) ? 1 : 2;
	int arrWidth = (embossLevel <= 50) ? 3 : 5;

	int arrHalfW = arrWidth/2;

	float val3x3 = 0;
	float val5x5 = 0;

	if(embossLevel <= 50)
	{
		val3x3 = (float)embossLevel/50.0f;
		val5x5 = 0;
	}
	else
	{
		val3x3 = 1.0f;
		val5x5 = (float)(embossLevel-50)/50.0f;
	}

	float subArr5x5[5][5] =
	{
		-val5x5,		-val5x5,		-val5x5,		-val5x5,	0,
		-val5x5,		-val3x3,		-val3x3,		0,			val5x5,
		-val5x5,		-val3x3,		0,				val3x3,		val5x5,
		-val5x5,		0,				val3x3,			val3x3,		val5x5,
		0,				val5x5,			val5x5,			val5x5,		val5x5
	};

	float subArr3x3[3][3] =
	{
		-val3x3,		-val3x3,		0,
		-val3x3,		0,				val3x3,
		0,				val3x3,			val3x3
	};

	unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	memcpy(newBuf, _buf, _width*_height*_bytesPerPixel);

	for(int y = 0; y < _height; y++) 
	for(int x = 0; x < _width; x++) 
	{ 
		float sumR = 0.0f;
		float sumG = 0.0f;
		float sumB = 0.0f;

		for(int subY=-level; subY<=level; subY++)
		for(int subX=-level; subX<=level; subX++)
		{
			int xx = x+subX;
			int yy = y+subY;

			if(xx < _width && yy < _height && xx >= 0 && yy >= 0)
			{
				unsigned int subPos = (_width*yy + xx) * _bytesPerPixel;

				if(level == 1)
				{
					sumR += _buf[ subPos + 0] * subArr3x3[subX+arrHalfW][subY+arrHalfW]; 
					sumG += _buf[ subPos + 1] * subArr3x3[subX+arrHalfW][subY+arrHalfW]; 
					sumB += _buf[ subPos + 2] * subArr3x3[subX+arrHalfW][subY+arrHalfW];
				}
				else if(level == 2)
				{
					sumR += _buf[ subPos + 0] * subArr5x5[subX+arrHalfW][subY+arrHalfW]; 
					sumG += _buf[ subPos + 1] * subArr5x5[subX+arrHalfW][subY+arrHalfW]; 
					sumB += _buf[ subPos + 2] * subArr5x5[subX+arrHalfW][subY+arrHalfW];
				}
			}
		}

		unsigned int pos = (_width*y + x) * _bytesPerPixel;

		newBuf[ pos+0 ] = min(max(int(sumR + 128), 0), 255); 
		newBuf[ pos+1 ] = min(max(int(sumG + 128), 0), 255); 
		newBuf[ pos+2 ] = min(max(int(sumB + 128), 0), 255);
	}

	//int filterWidth = 3;
	//int filterHeight = 3;

	//float level = (float)embossLevel / 100.0f;

	//double filter[3][3] =
	//{
	//	-level,		-level,		0,
	//	-level,		0,			level,
	//	0,			level,		level
	//};

	//float factor = 1.0f;
	//float biasR = 128.0f;
	//float biasG = 128.0f;
	//float biasB = 128.0f;

	//unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	//memcpy(newBuf, _buf, _width*_height*_bytesPerPixel);

	//for(int y = 0; y < _height; y++) 
	//for(int x = 0; x < _width; x++) 
	//{ 
	//	float sumR = 0.0f;
	//	float sumG = 0.0f;
	//	float sumB = 0.0f;
	//	 
	//	for(int filterX = 0; filterX < filterWidth; filterX++) 
	//	for(int filterY = 0; filterY < filterHeight; filterY++) 
	//	{ 
	//		if(filter[filterX][filterY] == 0)
	//			continue;
	//		int xx = (x - filterWidth / 2 + filterX + _width) % _width; 
	//		int yy = (y - filterHeight / 2 + filterY + _height) % _height;

	//		unsigned int subPos = (_width*yy + xx) * _bytesPerPixel;

	//		sumR += _buf[ subPos + 0] * filter[filterX][filterY]; 
	//		sumG += _buf[ subPos + 1] * filter[filterX][filterY]; 
	//		sumB += _buf[ subPos + 2] * filter[filterX][filterY]; 
	//	}

	//	unsigned int pos = (_width*y + x) * _bytesPerPixel;

	//	newBuf[ pos+0 ] = min(max(int(factor * sumR + biasR), 0), 255); 
	//	newBuf[ pos+1 ] = min(max(int(factor * sumG + biasG), 0), 255); 
	//	newBuf[ pos+2 ] = min(max(int(factor * sumB + biasB), 0), 255);
	//}

	free(_buf);
	_buf = newBuf;

	GrayScale();
}

void ImageBuffer::MotionBlur()
{
	int filterWidth = 5;
	int filterHeight = 5;

	double filter[5][5] =
	{
		1, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 1, 0,
		0, 0, 0, 0, 1
	};

	float factor = 1.0f/5.0f;
	float bias = 0.0f;

	unsigned char* newBuf = (unsigned char*) malloc(_width*_height*_bytesPerPixel);
	memcpy(newBuf, _buf, _width*_height*_bytesPerPixel);

	for(int x = 0; x < _width; x++) 
	for(int y = 0; y < _height; y++) 
	{ 
		double sumR = 0.0f;
		double sumG = 0.0f;
		double sumB = 0.0f;
		 
		for(int filterX = 0; filterX < filterWidth; filterX++) 
		for(int filterY = 0; filterY < filterHeight; filterY++) 
		{ 
			if(filter[filterX][filterY] == 0)
				continue;

			int xx = (x - filterWidth / 2 + filterX + _width) % _width; 
			int yy = (y - filterHeight / 2 + filterY + _height) % _height;

			unsigned int subPos = (_width*yy + xx) * _bytesPerPixel;

			sumR += _buf[ subPos + 0] * filter[filterX][filterY]; 
			sumG += _buf[ subPos + 1] * filter[filterX][filterY]; 
			sumB += _buf[ subPos + 2] * filter[filterX][filterY]; 
		}

		unsigned int pos = (_width*y + x) * _bytesPerPixel;

		newBuf[ pos+0 ] = min(max(int(factor * sumR + bias), 0), 255); 
		newBuf[ pos+1 ] = min(max(int(factor * sumG + bias), 0), 255); 
		newBuf[ pos+2 ] = min(max(int(factor * sumB + bias), 0), 255);
	}

	free(_buf);
	_buf = newBuf;
}

void ImageBuffer::PencilSketch()
{
	RemoveAlphaLayer();

	ImageBuffer* uImg = this;
	ImageBuffer* lImg = new ImageBuffer(_imagePath.c_str());

	uImg->GrayScale();
	uImg->Invert();

	uImg->BlurImage(8);
	uImg->BlurImage(8);

	int w = uImg->GetWidth();
	int h = uImg->GetHeight();


	int size = w * h * 3;

	unsigned char* uBuf = uImg->GetBuffer();
	unsigned char* lBuf = lImg->GetBuffer();

	for(int i=0; i<size-3; i+=3)
	{
		for(int j=0;j<3;j++)
		{
			int val = 255;

			int L = lBuf[i+j];
			int U = uBuf[i+j];

			if(L == 0 && U == 255)
				val = 0;
			else if(L != 0 && U == 255)
				val = 255;
			else
			{
				val = (int)(255.0f * ((float)L/((float)255-(float)U)));
			}

			uBuf[i+j] = min(255, val);
		}
	}
	
	uImg->GrayScale();
	uImg->Contrast(10);

	delete lImg;
	lImg = NULL;
}

void ImageBuffer::RemoveWhiteBGFromGrayScaleImage()
{
	if(GetBytesPerPixel() == 3)
		AddAlphaLayer();

	int bytesPP = GetBytesPerPixel();
	int size = GetWidth() * GetHeight() * bytesPP;

	for(int i=0; i<size; i+=bytesPP)
	{
		int r = _buf[i+0];
		int g = _buf[i+1];
		int b = _buf[i+2];

		_buf[i] = _buf[i+1] = _buf[i+2] = 0;
		_buf[i+3] = 255-r;
	}	
}

//vector<Point> ImageBuffer::GetSimilarPixels(int x, int y, char* existArr)
//{
//	vector<Point> vec;
//
//	unsigned int pos = (_width*y + x);
//
//	if(existArr[pos] == 0 && IsInBoundry(x,y))
//	{
//		vec.push_back(Point(x,y));
//		existArr[pos] = 1;
//
//		for(int i=0;i<vec.size();i++)
//		{
//			int px = vec[i].x;
//			int py = vec[i].y;
//
//			unsigned int pos = (_width*py + px)*_bytesPerPixel;
//			unsigned char r = _buf[pos+0];
//			unsigned char g = _buf[pos+1];
//			unsigned char b = _buf[pos+2];
//
//			for(int j=0;j<4;j++)
//			{
//				int x = px;
//				int y = py;
//
//				if(j == 0)	x = px-1;
//				else if(j == 1)	x = px+1;
//				else if(j == 2)	y = py-1;
//				else if(j == 3)	y = py+1;
//
//				unsigned int pos = (_width*y + x)*_bytesPerPixel;
//				unsigned int arrPos = (_width*y + x);
//
//				if(IsInBoundry(x,y) && existArr[arrPos] == 0)
//				{
//					unsigned char r1 = _buf[pos+0];
//					unsigned char g1 = _buf[pos+1];
//					unsigned char b1 = _buf[pos+2];
//
//					if(r1 < r+3 && r1 > r-3   &&   g1 < g+3 && g1 > g-3   &&   b1 < b+3 && b1 > b-3)
//					//if(r1 != 0 && g1 != 0 && b1 != 0)
//					{
//						existArr[arrPos] = 1;
//						vec.push_back(Point(x,y));
//					}
//				}
//			}
//		}
//	}
//
//	return vec;
//}

bool ImageBuffer::IsInBoundry(int x, int y)
{
	return (x >=0 && x < _width && y >= 0 && y < _height);
}

ImageBuffer::~ImageBuffer()
{
	if(_buf)
	{
		free(_buf);
		_buf = NULL;
	}
}
