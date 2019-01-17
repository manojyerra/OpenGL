#ifndef UtilFuncs_H
#define UtilFuncs_H

#include "MathLib/MatrixLib/GLMat.h"
#include "Util/ImageBuffer.h"
#include <vector>
#include <string>
using namespace std;

class UtilFuncs
{
public:
	static int floatToIntBits(float val);
	static float nearVal(float x);
	static double Exp(double xVal, int nVal, double factor);
	static double factorial(int n);

	static float* AddFloat(float* arr, int arrSize, float addVal);
	static vector<string> OpenFileDialog(char* filter = "All Files (*.*)\0*.*\0", bool allowMultiSel=true);
	static string SaveFileDialog(char* filter = "All Files (*.*)\0*.*\0");
	static string OpenFolderDialer();

	static unsigned int GenerateGLTextureID(ImageBuffer* imgBuf);
	static unsigned int GenerateGLTextureID(int width, int height, bool hasAlpha, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};

class StringUtils
{
public:
	static vector<string> split(string str, char splitBy)
	{
		vector<string> splitVec;

		if(str.find(splitBy) == -1)
			return splitVec;

		int startIndex = 0;
		int len = str.length();
		bool justNowOneStrSplitted = false;

		for(int i=0;i<len;i++)
		{
			if(justNowOneStrSplitted)
			{
				if(str[i] != splitBy)
				{
					justNowOneStrSplitted = false;
					startIndex = i;
				}
			}

			if(str[i] == splitBy || i == len-1)
			{
				justNowOneStrSplitted = true;

				if(i == len-1)
					splitVec.push_back(str.substr(startIndex, i-startIndex+1));
				else
					splitVec.push_back(str.substr(startIndex, i-startIndex));

				startIndex = i;
			}
		}
		
		return splitVec;
	}

	static bool startsWith(string str, string checkStr)
	{
		int strLen = str.length();
		int checkStrLen = checkStr.length();

		if(strLen < checkStrLen)
			return false;

		for(int i=0;i<checkStrLen;i++)
		{
			if(str[i] != checkStr[i])
				return false;
		}

		return true;
	}

	static bool endsWith(string str, string checkStr)
	{
		int strLen = str.length();
		int checkStrLen = checkStr.length();

		if(strLen < checkStrLen)
			return false;

		int strLastIndex = str.length()-1;
		int checkStrLastIndex = checkStr.length()-1;

		for(int i=0;i<checkStrLen;i++)
		{
			if(str[strLastIndex-i] != checkStr[checkStrLastIndex-i])
				return false;
		}

		return true;
	}
};


#endif