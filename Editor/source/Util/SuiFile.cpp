#include "SuiFile.h"
#include <direct.h>
#include <strsafe.h>
#include <stdlib.h>

SuiFile::SuiFile(string filePath)
{
	_fileSize = 0;
	_filePath = filePath;

	//for(int i=0;i<_filePath.length();i++)
	//	if(_filePath[i] == '\\')
	//		_filePath[i] = '/';

	if(isFile())
	{
		FILE* fp = fopen(_filePath.c_str(), "r");
		if(fp)
		{
			fseek(fp, 0, SEEK_END);
			_fileSize = ftell(fp);
			fclose(fp);
		}
	}
}

long SuiFile::getSize()			
{
	return _fileSize;
}

string SuiFile::getPath()
{
	return _filePath;
}

string SuiFile::getName()
{
	char drive[_MAX_DRIVE]; 
	char dir[_MAX_DIR]; 
	char fname[_MAX_FNAME]; 
	char ext[_MAX_EXT]; 

	_splitpath(_filePath.c_str(), drive, dir, fname, ext); 

	string fileName(fname);
	fileName.append(ext);
	return fileName;
}

string SuiFile::getParentFolder()
{
	char drive[_MAX_DRIVE]; 
	char dir[_MAX_DIR]; 
	char fname[_MAX_FNAME]; 
	char ext[_MAX_EXT]; 

	_splitpath(_filePath.c_str(), drive, dir, fname, ext); 

	string fileName(drive);
	fileName.append(dir);
	return fileName;
}

string SuiFile::getAbsolutePath()
{
	int BUFSIZE = 4096;
	char buf[4096];
	TCHAR** lppPart={NULL};

	if(isExists())
	{
		BOOL success = GetFullPathName(_filePath.c_str(), BUFSIZE, buf, lppPart);
		if(success)	
			return string(buf);
	}

	return "";
}


bool SuiFile::isExists()			
{
	DWORD dwAttrib = GetFileAttributes(_filePath.c_str());
	return(dwAttrib != INVALID_FILE_ATTRIBUTES);
}

bool SuiFile::isDirectory()
{
	string str = getAbsolutePath();
	DWORD dwAttrib = GetFileAttributes(getAbsolutePath().c_str());
	if(dwAttrib == INVALID_FILE_ATTRIBUTES)
		return false;

	if(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

bool SuiFile::isFile()
{
	DWORD dwAttrib = GetFileAttributes(_filePath.c_str());
	if(dwAttrib == INVALID_FILE_ATTRIBUTES)
		return false;
	return !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool SuiFile::isReadOnly()
{
	DWORD dwAttrib = GetFileAttributes(_filePath.c_str());
	return(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_READONLY));
}

bool SuiFile::isHidden()
{
	DWORD dwAttrib = GetFileAttributes(_filePath.c_str());
	return(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_HIDDEN));
}

bool SuiFile::isCompressed()
{
	DWORD dwAttrib = GetFileAttributes(_filePath.c_str());
	return(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_COMPRESSED));
}

bool SuiFile::isArchive()
{
	DWORD dwAttrib = GetFileAttributes(_filePath.c_str());
	return(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_ARCHIVE));
}



bool SuiFile::makeDirectory()
{
	mkdir(_filePath.c_str());
	return isExists();
}

bool SuiFile::createNewFile()
{
	bool success = false;

	FILE* fp = fopen(_filePath.c_str(),"wb");
	if(fp)
	{
		success = true;
		fclose(fp);
	}

	return success;
}

bool SuiFile::deleteFile()
{
	return (DeleteFile(_filePath.c_str()) != 0);
}


vector<string> SuiFile::list()
{
	vector<string> fileNamesVec;

	if(!isDirectory())
		fileNamesVec;

	TCHAR szDir[MAX_PATH];

	sprintf(szDir, "%s", _filePath.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	WIN32_FIND_DATA wfd;
	HANDLE fHandle = FindFirstFile(szDir, &wfd);
	if(INVALID_HANDLE_VALUE == fHandle) 
		return fileNamesVec;

	do
	{
		if(strcmp(wfd.cFileName, ".") == 0 ||  strcmp(wfd.cFileName, "..") == 0)
			continue;

		string fullPath = getAbsolutePath();
		fullPath.append("\\");
		fullPath.append(wfd.cFileName);

		SuiFile suiFile(fullPath);

		if(suiFile.getAbsolutePath().find(wfd.cFileName) == -1)
			continue;

		fileNamesVec.push_back(suiFile.getAbsolutePath());
	}
	while(FindNextFile(fHandle, &wfd));

	return fileNamesVec;
}

vector<SuiFile> SuiFile::listFiles()
{
	vector<SuiFile> filesVec;

	if(!isDirectory())
		filesVec;

	TCHAR szDir[MAX_PATH];

	sprintf(szDir, "%s", _filePath.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	WIN32_FIND_DATA wfd;
	HANDLE fHandle = FindFirstFile(szDir, &wfd);
	if(INVALID_HANDLE_VALUE == fHandle) 
		return filesVec;

	do
	{
		if(strcmp(wfd.cFileName, ".") == 0 ||  strcmp(wfd.cFileName, "..") == 0)
			continue;

		string fullPath = getAbsolutePath();
		fullPath.append("\\");
		fullPath.append(wfd.cFileName);

		SuiFile suiFile(fullPath);

		if(suiFile.getAbsolutePath().find(wfd.cFileName) == -1)
			continue;

		filesVec.push_back(suiFile);
	}
	while(FindNextFile(fHandle, &wfd));

	return filesVec;
}


//long SuiFile::lastModified()			{	return 0;		}
//bool SuiFile::canRead()					{	return false;	}
//bool SuiFile::canWrite()				{	return false;	}
//bool SuiFile::renameTo(string)			{	return false;	}
//bool SuiFile::setReadOnly()				{	return false;	}
//bool SuiFile::setWritable(bool)			{	return false;	}
//bool SuiFile::setReadable(bool)			{	return false;	}
//bool SuiFile::setExecutable(bool)		{	return false;	}
//bool SuiFile::canExecute()				{	return false;	}
//int SuiFile::equals(SuiFile* suiFile)	{	return 0;		}


/*
	//int MAX_PATH = 1000;
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	sprintf(szDir, "C:/Program Files (x86)");
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	//szDir = "C:/ProgramData";

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		int a = 10;
		//DisplayErrorBox(TEXT("FindFirstFile"));
		//return dwError;
	} 
   
   // List all the files in the directory with some info about them.

	char path[256];
   do
   {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
			sprintf(path,"  %s   <DIR>\n", ffd.cFileName);
			OutputDebugString(path);
      }
      else
      {
			sprintf(path,"  %s   <FILE>\n", ffd.cFileName);
			OutputDebugString(path);
      }
   }
   while (FindNextFile(hFind, &ffd) != 0);

*/

/*
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define BUFSIZE 4096
#define LONG_DIR_NAME TEXT("c:\\longdirectoryname")

void _tmain(int argc, TCHAR *argv[])
{
    DWORD  retval=0;
    BOOL   success; 
    TCHAR  buffer[BUFSIZE]=TEXT(""); 
    TCHAR  buf[BUFSIZE]=TEXT(""); 
    TCHAR** lppPart={NULL};

   if( argc != 2 )
   {
      _tprintf(TEXT("Usage: %s [file]\n"), argv[0]);
      return;
   }

// Retrieve the full path name for a file. 
// The file does not need to exist.

    retval = GetFullPathName(argv[1],
                 BUFSIZE,
                 buffer,
                 lppPart);
    
    if (retval == 0) 
    {
        // Handle an error condition.
        printf ("GetFullPathName failed (%d)\n", GetLastError());
        return;
    }
    else 
    {
        _tprintf(TEXT("The full path name is:  %s\n"), buffer);
        if (lppPart != NULL && *lppPart != 0)
        {
            _tprintf(TEXT("The final component in the path name is:  %s\n"), *lppPart);
        }
    }


// Create a long directory name for use with the next two examples.

    success = CreateDirectory(LONG_DIR_NAME,
                              NULL);

    if (!success)
    {
        // Handle an error condition.
        printf ("CreateDirectory failed (%d)\n", GetLastError());
        return;
    }


// Retrieve the short path name.  

    retval = GetShortPathName(LONG_DIR_NAME,
                  buf,
                  BUFSIZE);

    if (retval == 0) 
    {
        // Handle an error condition.
         printf ("GetShortPathName failed (%d)\n", GetLastError());
         return;
    }
    else _tprintf(TEXT("The short name for %s is %s\n"), 
                  LONG_DIR_NAME, buf);


// Retrieve the long path name.  

    retval = GetLongPathName(buf,
              buffer,
              BUFSIZE);

    if (retval == 0) 
    {
        // Handle an error condition.
         printf ("GetLongPathName failed (%d)\n", GetLastError());
         return;
    }
    else _tprintf(TEXT("The long name for %s is %s\n"), buf, buffer);

// Clean up the directory.

    success = RemoveDirectory(LONG_DIR_NAME);
    if (!success)
    {
        // Handle an error condition.
        printf ("RemoveDirectory failed (%d)\n", GetLastError());
        return;
    }
}
*/



	//LARGE_INTEGER GetMyFileSize(CString strFileName)
	//{
	//	LARGE_INTEGER nLargeInteger = { 0 } ;
	//	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
	//												OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	//	if (hFile != INVALID_HANDLE_VALUE)
	//	{
	//		BOOL bSuccess = GetFileSizeEx(hFile, &nLargeInteger) ;
	//	}

	//	return nLargeInteger ;
	//}



	//HANDLE hFile = CreateFile("C:/1work/asdf.txt",						// file to open
 //                      GENERIC_READ,          // open for reading
 //                      FILE_SHARE_READ,       // share for reading
 //                      NULL,                  // default security
 //                      OPEN_EXISTING,         // existing file only
 //                      FILE_ATTRIBUTE_NORMAL, // normal file
 //                      NULL);                 // no attr. template