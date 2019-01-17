#ifndef Def_H
#define Def_H

#include "windows.h"
#include <string>
using namespace std;

typedef unsigned char byte;

//#define glColor(c) glColor4ub((c >> 24) & 255, (c >> 16) & 255, (c >> 8) & 255, (c) & 255)

#define DEG_RAD 0.0174532925194f
#define RAD_DEG 57.295780490443f
#define PI_VAL 3.1415926
#define D_ double


static void Exception()
{
	int a = 1;
	int b = 0;
	int c = a/b;
}

static void pr(const char *s, D_ p0=0, D_ p1=0, D_ p2=0, D_ p3=0, D_ p4=0, D_ p5=0, D_ p6=0, D_ p7=0, D_ p8=0, D_ p9=0)
{
	string str(s);

	int count = 0;
	double val = 0;
	char arr[128];

	while(true)
	{
		if(count == 0)		val = p0;
		else if(count == 1)	val = p1;
		else if(count == 2)	val = p2;
		else if(count == 3)	val = p3;
		else if(count == 4)	val = p4;
		else if(count == 5) val = p5;
		else if(count == 6) val = p6;
		else if(count == 7) val = p7;
		else if(count == 8) val = p8;
		else if(count == 9) val = p9;

		int intI = str.find("%d");
		int floatI = str.find("%f");
		int charI = str.find("%c");

		if(intI == -1)		intI = 100000;
		if(floatI == -1)	floatI = 100000;
		if(charI == -1)		charI = 100000;

		if(intI < floatI && intI < charI)
		{
			sprintf(arr,"%d", (int)val);
			str.replace(str.find("%d"),2, arr);
		}
		else if(floatI < intI && floatI < charI)
		{
			sprintf(arr,"%f", (float)val);
			str.replace(str.find("%f"),2, arr);
		}
		else if(charI < floatI && charI < intI)
		{
			sprintf(arr,"%c", (int)val);
			str.replace(str.find("%c"),2, arr);
		}
		else
			break;

		count++;
	}

	OutputDebugString(str.c_str());
}


#endif
