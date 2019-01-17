#ifndef SUI_FILE_H
#define SUI_FILE_H

#include "Sui/SuiIncludes.h"
#include <vector>
#include <string>
using namespace std;

class SuiFile
{
private:
	string _filePath;
	long _fileSize;

public:
	 SuiFile(string filePath);

	 long getSize();
	 string getName();
	 string getParentFolder();
	 string getPath();
	 string getAbsolutePath();

	 bool isExists();
	 bool isDirectory();
	 bool isFile();
	 bool isReadOnly();
	 bool isHidden();
	 bool isCompressed();
	 bool isArchive();

	 bool makeDirectory();
	 bool createNewFile();
	 bool deleteFile();

	 vector<string> list();
	 vector<SuiFile> listFiles();

	 //bool canRead();
	 //bool canWrite();
	 //long lastModified();
	 //bool renameTo(string);
	 //bool setReadOnly();
	 //bool setWritable(bool);
	 //bool setReadable(bool);
	 //bool setExecutable(bool);
	 //bool canExecute();
	 //int equals(SuiFile* suiFile);
};

#endif
