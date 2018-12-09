#ifndef StringUtils_H
#define StringUtils_H

#include <vector>
#include <string>
using namespace std;

class StringUtils
{
public:
	static vector<string> split(string str, char splitBy);
	static bool startsWith(string str, string checkStr);
	static bool endsWith(string str, string checkStr);	
};

#endif