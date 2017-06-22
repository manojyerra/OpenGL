#include "StringUtils.h"

vector<string> StringUtils::split(string str, char splitBy)
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

bool StringUtils::startsWith(string str, string checkStr)
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

bool StringUtils::endsWith(string str, string checkStr)
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
