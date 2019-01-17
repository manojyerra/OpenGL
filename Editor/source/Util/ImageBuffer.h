#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

//#include "SuiIncludes.h"
#include "Sui/SuiRect.h"
#include "MathLib/Point.h"

#include <string>
#include <vector>
#include<algorithm> 

using namespace std;

class ImageBuffer
{
private:
	unsigned char* _buf;
	int _width;
	int _height;
	int _bytesPerPixel;
	int _numMipMapLevels;
	int _compressedFormatBufSize;

	int _imgType;
	string _imagePath;

public:
	static const int LINEAR = 1;
	static const int NEAREST = 2;

	static const int PNG = 1;
	static const int JPEG = 2;
	static const int DDS_DXT1 = 3;
	static const int DDS_DXT3 = 4;
	static const int DDS_DXT5 = 5;

	ImageBuffer(string imagePath);
	ImageBuffer(int width, int height, int bytesPerPixel);
	ImageBuffer(int width, int height, int bytesPerPixel, unsigned char* buffer);
	~ImageBuffer();


	//****************************************** Setter Getter functions ***********************************/
	ImageBuffer* Clone();
	unsigned char* GetBuffer();
	unsigned int GetBufferSize();
	int GetNumMipMapLevels();
	bool IsPNG();
	bool IsJPEG();
	bool IsDDS();
	bool IsDDS_DXT1();
	bool IsDDS_DXT3();
	bool IsDDS_DXT5();

	int GetWidth();
	int GetHeight();
	int GetBytesPerPixel();
	bool HasTransparency();
	unsigned int GetPixelColor(int x, int y);
	int GetR(int x, int y);
	int GetG(int x, int y);
	int GetB(int x, int y);
	void SetPixelColor(int x, int y, unsigned int pixelColor);
	//******************************************************************************************************/

	//*********************************For Compression *****************************************************/
	void MakeFeasibleToCompress(int compressionLevel);
	//******************************************************************************************************/	

	//********************************* Writing with intervals *********************************************/
	bool Write(string savePath);
	//bool BeginWrite(string savePath);
	//int WritePercent(float percent);
	//void EndWrite();
	//******************************************************************************************************/


	//*********************************Image manipulation functions ******************************************/
	bool AddAlphaLayer();
	bool RemoveAlphaLayer();
	bool RemoveAlphaLayerIfNoDataLose();
	bool CropImage(int x, int y, int width, int height);
	bool ScaleInPixels(int width, int height, int scaleType);
	bool SetCanvasSize(int width, int height);
	bool AddCanvas(int left, int right , int top, int bottom);
	SuiRect AutoCrop();
	void Rotate90Degrees();
	void Rotate180Degrees();
	void Rotate270Degrees();

	void FlipHorizantal();
	void FilpVertical();
	void ColorOverlay(unsigned int color, int level);
	void ColorOverlay(unsigned char r, unsigned char g, unsigned char b, int level);
	void Invert();
	void InvertValue();
	void NegativePhoto();

	void BlurImage(int effectLevel);				//1 to 5
	void Sharpen(int effectLevel);					//1 to 100
	void Emboss(int effectLevel);					//1 to 100
	void EdgeDetection(int effectLevel);			//1 to 100
	void Hue(int hueLevel);							//1 to 100
	void Sepia(int level);							//1 to 100
	void Colorize(int r, int g, int b);				//
	void Contrast(int level);						//1 to 100	
	void Bright(int level);							//1 to 100
	void Dark(int level);							//1 to 100
	void PixelateNearest(int level);				//1 to 100
	void Explosion(int level);						//1 to 100
	void BlackAndWhite(int level);					//1 to 100
	void MotionBlur();								//
	bool SetTransparency(int level, bool add_AlphaLayer_if_not_exist);								//1 to 100
	void InvertChannels(bool invertRedChannel, bool invertGreenChannel, bool invertBlueChannel);	//
	void GrayScale();
	void PencilSketch();
	void RemoveWhiteBGFromGrayScaleImage();
	//**********************************************************************************************************/

	//*********************************Image utility functions ******************************************/

	vector<Point> GetSimilarPixels(int x, int y, char* existArr);
	bool IsInBoundry(int x, int y);

	//**********************************************************************************************************/
};

#endif
