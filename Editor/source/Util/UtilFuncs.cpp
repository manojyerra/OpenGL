#include "UtilFuncs.h"
#include "Sui/Sui.h"
#include "math.h"

int UtilFuncs::floatToIntBits( float val )
{
	int* intVal = (int*)&val;
	return intVal[0];
}

float* UtilFuncs::AddFloat(float* arr, int arrSize, float addVal)
{
	float* newArr = (float*)malloc((arrSize+1) * sizeof(float));
	memcpy(newArr, arr, arrSize);
	newArr[arrSize] = addVal;
	free(arr);
	return newArr;
}

float UtilFuncs::nearVal(float x)
{
	float dx = x - (int)x;
	
	if(x > 0)
	{
		if(dx > 0.5f)	return (int)x + 1;
		else			return (int)x;
	}
	else
	{
		if(dx < -0.5f)	return (int)x - 1;
		else			return (int)x;
	}

	return x;
}

double UtilFuncs::Exp(double xVal, int nVal, double factor)
{
	xVal *= factor;

	double sum = 0;

	for(int i=0;i<=nVal;i++)
	{
		double up = pow((double)xVal, (int)i);
		double down = factorial(i);
		sum += up/down;
	}

	return sum;
}

double UtilFuncs::factorial(int n)
{
	double sum = 1;
	for(int i=1;i<=n;i++)
		sum *= i;
	return sum;
}

vector<string> UtilFuncs::OpenFileDialog(char *filter, bool allowMultiSel) 
{
	vector<string> filePaths;

	HWND owner = NULL;
	OPENFILENAME ofn;
	#define _MAX_PATH 30000
	char path[_MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	 
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = path;
	ofn.nMaxFile = _MAX_PATH;
	
	if(allowMultiSel)
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT;
	else
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

	ofn.lpstrDefExt = "";

	string folderPath;

	if(GetOpenFileName(&ofn))
		folderPath = path;

    char buf[_MAX_PATH + 1];
    char *p = path + strlen(path) + 1;

	do
    {
      strcpy(buf, path);
      if (*p)   // The case of multiple file selection
      {
        strcat(buf, "\\");
        strcat(buf, p);
	  }

	  string filePath(folderPath);
	  filePath.append("\\");
	  filePath.append(p);
	  filePaths.push_back(filePath);

      // go to next file name
      if (*p)
        p = p + strlen(p) + 1;

    }  while (*p);

	for(int i=0;i<filePaths.size();i++)
	{
		string str = filePaths[i];
		if(str[str.length()-1] == '\\')
		{
			str = str.substr(0, str.length()-1);
			filePaths[i] = str;
		}
	}

	return filePaths;
}

string UtilFuncs::SaveFileDialog(char *filter) 
{
	HWND owner = NULL;

	OPENFILENAME ofn;
	#define _MAX_PATH 30000
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	 
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	string fileNameStr;

	if(GetSaveFileNameA(&ofn))
		fileNameStr = fileName;

	return fileNameStr;
}

//string UtilFuncs::OpenFolderDialer()
//{
//	BROWSEINFO bi = { 0 };
//    bi.lpszTitle = _T("Pick a Directory");
//    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
//    if ( pidl != 0 )
//    {
//        // get the name of the folder
//        TCHAR path[MAX_PATH];
//        if ( SHGetPathFromIDList ( pidl, path ) )
//        {
//            _tprintf ( _T("Selected Folder: %s\n"), path );
//        }
//
//        // free memory used
//        IMalloc * imalloc = 0;
//        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
//        {
//            imalloc->Free ( pidl );
//            imalloc->Release ( );
//        }
//
//		return string(path);
//    }
//
//	return string();
//}

unsigned int UtilFuncs::GenerateGLTextureID(ImageBuffer* imgBuf)
{
	int width = imgBuf->GetWidth();
	int height = imgBuf->GetHeight();
	int bytesPerPixel = imgBuf->GetBytesPerPixel();
	void* buffer = imgBuf->GetBuffer();

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(imgBuf->IsDDS())
	{
		unsigned int format = 0;

		if(imgBuf->IsDDS_DXT1())
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		else if(imgBuf->IsDDS_DXT3())
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		else if(imgBuf->IsDDS_DXT5())
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

		glCompressedTexImage2DARB( GL_TEXTURE_2D, 0, format, width, height, 0, imgBuf->GetBufferSize(), imgBuf->GetBuffer());	
	}
	else
	{
		if(bytesPerPixel == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		else if(bytesPerPixel == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
		else if(bytesPerPixel == 1)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_RGB8, GL_UNSIGNED_BYTE, buffer);
		else
			return 0;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	return textureID;
}

unsigned int UtilFuncs::GenerateGLTextureID(int width, int height, bool hasAlpha, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	int bytesPerPixel = (hasAlpha == true) ? 4 : 3;
	int size = width * height * bytesPerPixel;
	unsigned char* buffer = (unsigned char*)malloc(size);

	for(int i=0; i<size; i+=bytesPerPixel)
	{
		buffer[i+0] = r;
		buffer[i+1] = g;
		buffer[i+2] = b;

		if(bytesPerPixel == 4)
			buffer[i+3] = a;
	}

	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(bytesPerPixel == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if(bytesPerPixel == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	free(buffer);

	return textureID;
}

