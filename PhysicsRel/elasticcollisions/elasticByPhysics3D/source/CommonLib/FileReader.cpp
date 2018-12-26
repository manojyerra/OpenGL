#include "FileReader.h"
#include "MemTrace.h"
#include "stdio.h"
#include "conio.h"


FileReader::FileReader()
{
	_pos = 0;
	g_FileHandle = NULL;
}

bool FileReader::IsExist(const char* filePath)
{
	bool exist = false;
	FILE* fp = fopen( filePath, "r");

	if(fp)
	{
		fclose(fp);
		exist = true;
	}

	return exist;
}

bool FileReader::Load(const char* filePath)
{
	Close();

	unsigned char* _arrData;
	_arrData = NULL;

	g_FileHandle = fopen(filePath, "rb");
	fseek(g_FileHandle, 0, SEEK_END);
	unsigned int fileSize = ftell(g_FileHandle);
	fseek(g_FileHandle, 0, SEEK_SET);

	_arrData = (unsigned char*)GLMalloc(fileSize);

	if(fread(&_arrData[0], fileSize, 1, g_FileHandle) == 0)
	{
		printf("Read Error...");
	}
	else
	{
		string temp((const char*)_arrData);
		_fileData = temp;
	}

	printf("\n File size : %d ",_fileData.size());
	
	if (g_FileHandle)
		fclose(g_FileHandle);

	return true;
}


void FileReader::Reset()
{
	_pos = 0;
}


int FileReader::Read()
{
	if(_pos < (int)_fileData.length())
	{
		int charValue = (int)_fileData[_pos];
		_pos++;
		return charValue;
	}

	return -1;
}

string FileReader::ReadLine()
{
	if(_pos < (int)_fileData.length())
	{
		std::string line="";

		for(int i = _pos; i<(int)_fileData.length(); i++)
		{
			if(_fileData[i] == '\n')
			{ 
				_pos++;
				if( (int)line[line.length()-1] == 13 )	line = line.substr(0, line.length()-1);
				return line;
			}
			line += _fileData[i];
			_pos++;
		}

		return line;
	}
	return "null";
}

string FileReader::ReadLineAndTrim()
{
	return (Trim(ReadLine()));
}


vector<string> FileReader::ReadLine(char ch)
{
	vector<string> vec;

	string line = ReadLine();
	
	int arrCount = -1;
	bool enteredNow = false;

	for(int i=0;i<(int)line.length();i++)
	{
		char strChar = line[i];

		if(strChar != ch) 
		{
			if(enteredNow == false)			
			{
				enteredNow = true;
				arrCount++;
				string word;
				vec.push_back(word);
			}

			vec[arrCount] += strChar;
		}
		else { enteredNow = false; }

	}

	return vec;
}

string FileReader::GetAttribute(string attribute, string line)
{
	int index = -1;
	int count = 0;
	do
	{
		if(count > 4) { return ""; }
		if(count == 0) attribute.append("=");
		else if(count == 1) attribute.append(" =");
		else if(count == 2) attribute.append("  =");
		else if(count == 3) attribute.append("   =");
		else if(count == 4) attribute.append("    =");

		index =(int) line.find(attribute);
		count++;

	}while(index == -1);

	//write("\nfound index = %d", index);


	int start = -1;
	int end = -1;

	char ch = '\"';

	for(unsigned int i=index;i<line.length();i++)
	{
		if(line[i] == ch)
		{
			if(start == -1)
			{
				start = i;
			}
			else
			{
				end = i;
				break;
			}
		}
	}

	//write("\n start : %d, end = %d", start, end);

	start++;

	string returnStr = line.substr(start, end-start);

	return returnStr;
}

vector<string> FileReader::GetTagData(string tag, bool trim)
{
	vector<string> vec;
	Reset();
	
	string beginTag = "<";
	beginTag += Trim(tag);
	beginTag += ">";

	string endTag = "</";
	endTag += Trim(tag);
	endTag += ">";

	if(GoTo(beginTag) == false)
	{
		return vec;
	}

	std::string line="";

	do
	{
		if(trim) { line = ReadLineAndTrim(); } else { line = ReadLine(); }

		if(line.find(endTag) != -1)
		{
			//s3eDebugTracePrintf("\nFound end tag  %s", endTag.c_str());
			return vec;
		}

		vec.push_back(line);

	}while( line.compare("null") != 0 );

	//s3eDebugTracePrintf("\n %s end tag not found ", endTag.c_str());

	vec.clear();

	return vec;
}


bool FileReader::GoTo(string tag)
{
	Reset();
	string line;

	//write("\nTag is !!%s!!", tag.c_str());
	
	//write("\nLines::");

	do
	{
		line = Trim(ReadLine()) ;

		if( line.find(tag) != -1)
		{
			//s3eDebugTracePrintf("\n%s tag found ", tag.c_str());
			return true;
		}
	}while(line.compare("null") != 0);

	//write("\n:%s: tag not found", tag.c_str());
	return false;
}


string FileReader::Trim(string line)
{
	string retStr="";

	int start = 0;
	int end = 0;

	for(int i=0; i<(int)line.length(); i++) { if( line[i] != ' ')  {	start = i; break; } }

	for(int j= line.length()-1; j>=0; j--) { if( line[j] != ' ')  {	end = j; break; } }

	for(int k=start; k<=end; k++)
	{
		retStr += line[k];
	}
	return retStr;
}


vector<string> FileReader::Split(string line, char ch)
{
	vector<string> vec;
	
	int arrCount = -1;
	bool enteredNow = false;

	for(int i=0;i<(int)line.length();i++)
	{
		char strChar = line[i];

		if(strChar != ch) 
		{
			if(enteredNow == false)			
			{
				enteredNow = true;
				arrCount++;
				string word;
				vec.push_back(word);
			}
			vec[arrCount] += strChar;
		}
		else { enteredNow = false; }
	}

	return vec;
}

void FileReader::Close()
{
	_fileData = "";
	_pos = 0;

	if(g_FileHandle)
	{
		fclose(g_FileHandle);
		g_FileHandle = NULL;
	}
}
