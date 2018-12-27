#pragma once

#include<string>
#include <vector>
using namespace std;

class FileReader
{
private:
	FILE* g_FileHandle;
	string _fileData;
	int _pos;
public:
	FileReader();
	static bool IsExist(const char* filePath);
	bool Load(const char* filePath);	
	void Reset();
	int Read();
	string ReadLine();
	string ReadLineAndTrim();
	vector<string> ReadLine(char ch);
	string Trim(string line);

	string GetAttribute(string attribute, string line);
	vector<string> GetTagData(string tag, bool trim );
	vector<string> Split(string line, char ch);

	bool GoTo(string tag);
	void Close();
};
