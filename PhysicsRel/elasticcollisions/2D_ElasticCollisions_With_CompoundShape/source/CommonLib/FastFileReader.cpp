#include "FastFileReader.h"

FastFileReader::FastFileReader()
{
	_pos = 0;
}

bool FastFileReader::IsExist(const char* filePath)
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

bool FastFileReader::Load(const char* filePath)
{
	Close();

	unsigned char* _arrData;
	_arrData = NULL;

	FILE* fp = fopen(filePath, "rb");
	fseek(fp, 0, SEEK_END);
	unsigned int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	_arrData = (unsigned char*)malloc(fileSize);

	if(fread(&_arrData[0], fileSize, 1, fp) == 0)
	{
		printf("Read Error...");
	}
	else
	{
		string temp((const char*)_arrData);
		_fileData = temp;
	}

	printf("\n File size : %d ",_fileData.size());
	
	if(fp)
		fclose(fp);

	return true;
}

string FastFileReader::ReadLine()
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

//string FastFileReader::ReadLine()
//{
//	if(_lineNum >= _linesVec.size())
//	{return "null";}
//
//	int num = _lineNum;
//	_lineNum++;
//	return _linesVec[num];
//}

string FastFileReader::ReadLineAndTrim()
{
	return (Trim(ReadLine()));
}

vector<string> FastFileReader::GetNextTagData(string tag)
{
	vector<string> vec;
	vec.clear();
	
	string beginTag = "<"+Trim(tag)+">";
	string endTag = "</"+Trim(tag)+">";

	if(GoTo(beginTag) == false)  { return vec; }

	string line="";

	do
	{
		line = ReadLine();

		if(line.compare(endTag) == 0) 
		{return vec; }

		vec.push_back(line);

	}while( line.compare("null") != 0 );

	vec.clear();
	return vec;
}


bool FastFileReader::GoTo(string tag)
{
	string line;
	do
	{
		line = ReadLine() ;
		if(line.compare(tag) == 0)
		{
			return true;
		}
	}while(line.compare("null") != 0);

	return false;
}


string FastFileReader::Trim(string line)
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


vector<string> FastFileReader::Split(string line, char ch)
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

void FastFileReader::Close()
{
	_fileData = "";
	_pos = 0;
	//_lineNum = 0;
}