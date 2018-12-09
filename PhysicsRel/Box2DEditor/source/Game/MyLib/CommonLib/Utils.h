#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
using namespace std;

#define randInt Utils::RandInteger
#define absInt Utils::AbsInteger
#define absFloat Utils::AbsFloat

class Utils
{
public:
	static const int RGB = 0;
	static const int RBG = 1;
	static const int GRB = 2;
	static const int GBR = 3;
	static const int BRG = 4;
	static const int BGR = 5;

	static unsigned int GetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, int colorID);
	static unsigned int GetColor(unsigned int color, int colorID);
	static void SetColorToBuffer(unsigned char* buf, int w, int h, int bytesPP, int colorID);
	static int GetNearInt(float val);
	static int RandInteger();
	static int AbsInteger(int val);
	static float AbsFloat(float val);
	
	static bool Intersects(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
	static bool IsFileExists(char* fileAbsolutePath);
	static unsigned int GetFileLength(char* fileAbsolutePath);
	static char* GetFileData(char* fileAbsolutePath);
	static bool SetFileData(char* fileAbsolutePath, char* fileData, int size);
	static void SaveFileAs(char* saveAsPath, char* filePath);

#ifdef _PLATFORM_WIN32

	static vector<string> OpenFileDialog(char* filter = "All Files (*.*)\0*.*\0", bool allowMultiSel=true);
	static string SaveFileDialog(char* filter = "All Files (*.*)\0*.*\0");
	
#endif

	static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
	static string GetFileNameFromPath(string filePath);
};

#endif