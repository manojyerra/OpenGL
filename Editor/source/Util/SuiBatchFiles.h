#ifndef SUI_BATCH_FILES_H
#define SUI_BATCH_FILES_H

#include "Sui/SuiIncludes.h"
#include "SuiFile.h"
#include <vector>
#include <string>
using namespace std;

class SuiBatchFiles
{
private:
	string _sourcePath;
	string _destinPath;

	void PrintRecursive(SuiFile suiFile);
	void CreateFolderDump(SuiFile suiFile);
	string SubtractPath(string superSetStr, string subSetStr);

	void AddRecursive(SuiFile suiFile, vector<string>* vec, string filter);

public:
	SuiBatchFiles(string sourcePath);
	bool PrintFolderStructure();
	bool DumpFolderStructure();

	vector<string> GetAllFilePaths(string filter);

	void DestinPath(string destinPath);
	string GetDestinPath();
};

#endif