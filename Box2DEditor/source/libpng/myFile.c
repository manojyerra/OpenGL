#include "myFile.h"

FILE* myFileOpen(const char *path, const char *mode)
{
	pos = 0;

	myFP = fopen(path, mode);

	///////////////////////////////////////////////
	//fseek( myFP, 0, SEEK_END);
	//fileSize = ftell( myFP );
	//fseek( myFP, 0, SEEK_SET );

	//buf = (unsigned char*) malloc( fileSize);
	//fread( buf, 1, fileSize, myFP);
	//fseek( myFP, 0, SEEK_SET );
	//////////////////////////////////////////////

	return myFP;
}

size_t myFileRead(unsigned char* dest, size_t size, size_t nmemb, FILE *stream)
{

	//////////////////////////////////////////
	//size_t count = 0;

	//if(size > 1)
	//	printf("may be this is the problem");

	//while(count < nmemb)
	//{
	//	dest[count] = buf[pos];
	//	count++;
	//	pos++;
	//}

	//return count;
	///////////////////////////////////////////////

	return fread( dest, size, nmemb, stream);
}

void myFileClose(FILE* fp)
{
	fclose(fp);

	/////////////////////////////
	//free( buf );
	/////////////////////////////
}
