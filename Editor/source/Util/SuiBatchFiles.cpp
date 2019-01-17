#include "SuiBatchFiles.h"
#include "UtilFuncs.h"

SuiBatchFiles::SuiBatchFiles(string sourcePath)
{
	_sourcePath = sourcePath;
	_destinPath = sourcePath;
	_destinPath.append("_new");
}

void SuiBatchFiles::DestinPath(string destinPath)
{
	_destinPath = destinPath;
}

string SuiBatchFiles::GetDestinPath()
{
	return _destinPath;
}

bool SuiBatchFiles::DumpFolderStructure()
{
	SuiFile suiFile(_sourcePath);
	if(!suiFile.isDirectory())
		return false;

	SuiFile createDir(_destinPath);
	createDir.makeDirectory();

	CreateFolderDump(suiFile);
	return true;
}

void SuiBatchFiles::CreateFolderDump(SuiFile suiFile)
{
	vector<SuiFile> filesVec = suiFile.listFiles();

	for(int i=0; i<(int)filesVec.size(); i++)
	{
		SuiFile suiFile = filesVec[i];

		if(suiFile.isDirectory() && suiFile.isHidden() == false)
		{
			string remaining = SubtractPath(suiFile.getAbsolutePath(), _sourcePath);

			if(remaining.length() > 0)
			{
				string newPath = _destinPath;
				newPath.append(remaining);

				SuiFile newSuiFile(newPath);
				newSuiFile.makeDirectory();

				CreateFolderDump(suiFile);
			}
		}
	}
}

string SuiBatchFiles::SubtractPath(string superSetStr, string subSetStr)
{
	if(superSetStr.length() <= subSetStr.length())
		return "";

	if(superSetStr.find_first_of(subSetStr) == -1)
		return "";

	return superSetStr.substr(subSetStr.length(), superSetStr.length());
}


/////////////////////////// Print Folder Structure //////////////////////////////////////

bool SuiBatchFiles::PrintFolderStructure()
{
	SuiFile suiFile(_sourcePath);
	if(!suiFile.isDirectory())
		return false;

	PrintRecursive(suiFile);
	return true;
}

void SuiBatchFiles::PrintRecursive(SuiFile suiFile)
{
	long folderCount = 0;
	long fileCount = 0;
	char path[1024];

	vector<SuiFile> filesVec = suiFile.listFiles();

	for(int i=0; i<(int)filesVec.size(); i++)
	{
		SuiFile suiFile = filesVec[i];
		string fullPath = suiFile.getAbsolutePath();

		if(suiFile.isDirectory())
		{
			folderCount++;
			sprintf(path,"\nFolder : %s Count = %d",suiFile.getAbsolutePath().c_str(), folderCount);
			OutputDebugString(path);

			PrintRecursive(suiFile);
		}
		else
		{
			fileCount++;
			sprintf(path,"\nFile : %s Count = %d",suiFile.getAbsolutePath().c_str(), fileCount);
			OutputDebugString(path);
		}
	}
}

vector<string> SuiBatchFiles::GetAllFilePaths(string filter)
{
	vector<string> vec;

	SuiFile suiFile(_sourcePath);
	if(!suiFile.isDirectory())
		return vec;

	AddRecursive(suiFile, &vec, filter);
	return vec;
}

void SuiBatchFiles::AddRecursive(SuiFile suiFile, vector<string>* vec, string filter)
{
	vector<SuiFile> filesVec = suiFile.listFiles();

	for(int i=0; i<(int)filesVec.size(); i++)
	{
		SuiFile suiFile = filesVec[i];
		string fullPath = suiFile.getAbsolutePath();

		if(suiFile.isDirectory())
		{
			AddRecursive(suiFile, vec, filter);
		}
		else
		{
			if(StringUtils::endsWith(suiFile.getAbsolutePath(), filter))
				vec->push_back(suiFile.getAbsolutePath());
		}
	}
}