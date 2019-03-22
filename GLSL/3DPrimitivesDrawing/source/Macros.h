#ifndef Defines_H
#define Defines_H

#include <windows.h>
#include <GL/glew.h>
#include <stdio.h>

#define glColorui(c) glColor4ub((c >> 24) & 255, (c >> 16) & 255, (c >> 8) & 255, (c) & 255)
#define glColorA(c,a) glColor4ub((c >> 24) & 255, (c >> 16) & 255, (c >> 8) & 255, a)
#define DEG_RAD 0.0174532925194f
#define RAD_DEG 57.295780490443f
#define PI_VAL 3.1415926f

#define writeConsole Macros::write

//Begin : MessageBox related

#define MESSAGE_OK						0x00000000L
#define MESSAGE_OKCANCEL				0x00000001L
#define MESSAGE_ABORTRETRYIGNORE		0x00000002L
#define MESSAGE_YESNOCANCEL				0x00000003L
#define MESSAGE_YESNO					0x00000004L
#define MESSAGE_RETRYCANCEL				0x00000005L

#define ShowMessageBox					MessageBox

//End : MessageBox related


//#ifdef _DEBUG
//   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
//#else
//   #define DEBUG_CLIENTBLOCK
//#endif // _DEBUG
//
//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#endif
//
//#ifdef _DEBUG
//#define new DEBUG_CLIENTBLOCK
//#endif


class Macros
{
public:
	static void write(const char * pszFormat, ...);
};


#endif
