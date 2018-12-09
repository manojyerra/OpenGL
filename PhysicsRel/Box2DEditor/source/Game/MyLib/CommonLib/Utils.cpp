#include "Utils.h"
#include "Includes.h"
#include "Platform.h"

unsigned int Utils::GetColor(unsigned int color, int colorID)
{
	unsigned char r	= (color >> 24) & 255;
	unsigned char g	= (color >> 16) & 255;
	unsigned char b	= (color >> 8) & 255;
	unsigned char a	= (color ) & 255;

	return GetColor(r, g, b, a, colorID);
}

unsigned int Utils::GetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, int colorID)
{
	if(colorID == RBG)
	{
		return (unsigned int)( (r<<24) + (b<<16) + (g<<8) + a );
	}
	else if(colorID == GRB)
	{
		return (unsigned int)( (g<<24) + (r<<16) + (b<<8) + a );
	}
	else if(colorID == GBR)
	{
		return (unsigned int)( (g<<24) + (b<<16) + (r<<8) + a );
	}
	else if(colorID == BRG)
	{
		return (unsigned int)( (b<<24) + (r<<16) + (g<<8) + a );
	}
	else if(colorID == BGR)
	{
		return (unsigned int)( (b<<24) + (g<<16) + (r<<8) + a );
	}

	return (unsigned int)( (r<<24) + (g<<16) + (b<<8) + a );
}

int Utils::GetNearInt(float val)
{
	if(val - (int)val < 0.5)
		val = (int)val;
	else
		val = (int)val + 1;

	return (int)val;
}

int Utils::RandInteger()
{
	static unsigned int x = Platform::GetPlatform()->GetTimeInMillis();
	static unsigned int y = 362436069;
	static unsigned int z = 521288629;
	static unsigned int w = 88675123;
	unsigned int t;

	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

int Utils::AbsInteger(int val)
{
	return (val < 0) ? -val : val;
}

float Utils::AbsFloat(float val)
{
	return (val < 0) ? -val : val;
}

bool Utils::Intersects(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	int tw = (int)w1;
	int th = (int)h1;
	int rw = (int)w2;
	int rh = (int)h2;

	if(rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) 
		return false; 

	int tx = (int)x1;
	int ty = (int)y1;
	int rx = (int)x2;
	int ry = (int)y2;

	rw += rx;
	rh += ry;
	tw += tx;
	th += ty;

	return ((rw < rx || rw > tx) &&	(rh < ry || rh > ty) &&	(tw < tx || tw > rx) && (th < ty || th > ry));
}

bool Utils::IsFileExists(char* fileAbsolutePath)
{
	FILE* fp = fopen(fileAbsolutePath, "rb");

	if(fp)
	{
		fclose(fp);
		return true;
	}

	return false;
}

unsigned int Utils::GetFileLength(char* fileAbsolutePath)
{
	if(IsFileExists(fileAbsolutePath) == false)
		return 0;

	FILE* fp = fopen(fileAbsolutePath, "rb");

	fseek(fp, 0, SEEK_END);
	unsigned int fileSize = (unsigned int)ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fclose(fp);

	return fileSize;
}

char* Utils::GetFileData(char* fileAbsolutePath)
{
	char* data = NULL;

	unsigned int fileSize = GetFileLength(fileAbsolutePath);

	if(fileSize > 0)
	{
		data = (char*)malloc(fileSize+1);
		memset(data, '\0', fileSize+1);

		FILE* fp = fopen(fileAbsolutePath, "rb");

		if(fread(data, 1, fileSize, fp) != fileSize)
		{
			free(data);
			data = NULL;
		}

		fclose(fp);
	}

	return data;
}

bool Utils::SetFileData(char* fileAbsolutePath, char* fileData, int size)
{
	FILE* fp = fopen(fileAbsolutePath,"wb");

	if(fwrite(fileData, 1, size, fp) == size)
	{
		fflush(fp);
		fclose(fp);
		return true;
	}
	return false;
}

void Utils::SaveFileAs(char* saveAsPath, char* filePath)
{
	char* fileData = Utils::GetFileData(filePath);
	int fileSize = Utils::GetFileLength(filePath);

	Utils::SetFileData(saveAsPath, fileData, fileSize);
	free(fileData);
}

void Utils::SetColorToBuffer(unsigned char* buf, int w, int h, int bytesPP, int colorID)
{
	int size = w*h*bytesPP;

	if(colorID == RBG)
	{
		for(int i=0; i<size; i+=bytesPP)
		{
			unsigned char r = buf[i+0];
			unsigned char g = buf[i+1];
			unsigned char b = buf[i+2];

			buf[i+0] = r;
			buf[i+1] = b;
			buf[i+2] = g;
		}
	}
	else if(colorID == GRB)
	{
		for(int i=0; i<size; i+=bytesPP)
		{
			unsigned char r = buf[i+0];
			unsigned char g = buf[i+1];
			unsigned char b = buf[i+2];

			buf[i+0] = g;
			buf[i+1] = r;
			buf[i+2] = b;
		}
	}
	else if(colorID == GBR)
	{
		for(int i=0; i<size; i+=bytesPP)
		{
			unsigned char r = buf[i+0];
			unsigned char g = buf[i+1];
			unsigned char b = buf[i+2];

			buf[i+0] = g;
			buf[i+1] = b;
			buf[i+2] = r;
		}
	}
	else if(colorID == BRG)
	{
		for(int i=0; i<size; i+=bytesPP)
		{
			unsigned char r = buf[i+0];
			unsigned char g = buf[i+1];
			unsigned char b = buf[i+2];

			buf[i+0] = b;
			buf[i+1] = r;
			buf[i+2] = g;
		}
	}
	else if(colorID == BGR)
	{
		for(int i=0; i<size; i+=bytesPP)
		{
			unsigned char r = buf[i+0];
			unsigned char g = buf[i+1];
			unsigned char b = buf[i+2];

			buf[i+0] = b;
			buf[i+1] = g;
			buf[i+2] = r;
		}
	}
}

#ifdef _PLATFORM_WIN32

vector<string> Utils::OpenFileDialog(char *filter, bool allowMultiSel) 
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

#endif


#ifdef _PLATFORM_WIN32

string Utils::SaveFileDialog(char *filter) 
{
	string fileNameStr = "";

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

	if(GetSaveFileNameA(&ofn))
		fileNameStr = fileName;

	return fileNameStr;
}

#endif


void Utils::ReplaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

string Utils::GetFileNameFromPath(string filePath)
{
	int index = filePath.find_last_of('/');

	if(index != -1)
	{
		return filePath.substr(index+1, filePath.length());
	}
	else
	{
		index = filePath.find_last_of('\\');
	
		if(index != -1)
			return filePath.substr(index+1, filePath.length());
	}

	return filePath;
}
