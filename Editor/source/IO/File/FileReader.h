#ifndef FILE_READER_H
#define FILE_READER_H
#pragma once

#include<string>
#include<vector>
using namespace std;

class FileReader
{
private:
	string _fileData;
	unsigned int _fileSize;
	unsigned int _pos;

public:
	FileReader();
	FileReader(const char* fileName);
	bool LoadFile(const char* fileName);

	void Reset();
	int Read();
	string ReadLine();
	string ReadLineAndTrim();
	vector<string> ReadLine(char ch);
	string Trim(string line);
	string GetAttribute(string attribute, string line);

	int GetTagIntData(string tag);
	float GetTagFloatData(string tag);
	string GetTagStringData(string tag, bool trim);
	vector<string> GetTagData(string tag, bool trim);

	vector<string> Split(string line, char ch);
	bool GoTo(string tag);

	void Close();
	~FileReader();
};

#endif