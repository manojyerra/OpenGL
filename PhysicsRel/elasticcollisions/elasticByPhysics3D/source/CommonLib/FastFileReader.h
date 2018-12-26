#include<string>
#include<vector>
using namespace std;

class FastFileReader
{
private:
	string _fileData;
	int _pos;

	//vector<string> _linesVec;
	//int _lineNum;
	//string ReadLineByPos();

public:
	FastFileReader();	
	static bool IsExist(const char* filePath);
	bool Load(const char* fileName);

	string ReadLine();
	string ReadLineAndTrim();
	vector<string> GetNextTagData(string tag);

	static string Trim(string line);
	static vector<string> Split(string line, char ch);

	bool GoTo(string tag);
	void Close();
};
