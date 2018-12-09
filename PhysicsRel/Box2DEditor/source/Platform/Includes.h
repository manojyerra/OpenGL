#ifndef INCLUDES_H
#define INCLUDES_H


#ifdef _PLATFORM_ANDROID

//#define GL_GLEXT_PROTOTYPES

#include <android/log.h>
#include <GLES/gl.h>
//#include <GLES/glext.h>

#define printLog(...)	__android_log_print(ANDROID_LOG_INFO,"CPPNDK",__VA_ARGS__)

#define VK_LEFT 0
#define VK_UP 0
#define VK_RIGHT 0
#define VK_DOWN 0
#define VK_CONTROL 0
#define VK_ESCAPE 0
#define VK_TAB 0
#define VK_SHIFT 0
#define VK_DELETE 0
#define VK_OEM_PERIOD 0
#define VK_RETURN 0
#define MB_ICONINFORMATION 0
#define MB_OK 0
#define MB_ICONERROR 0
#define MessageBox(p1,p2,p3,p4) {}

#endif




#ifdef _PLATFORM_WIN32

#include <windows.h>
#include "Win32InputBox.h"
#include <gl/gl.h>
#include "stdio.h"

#define GL_CLAMP_TO_EDGE GL_CLAMP
#define glOrthof glOrtho

#define printLog Includes::print

class Includes
{
public:
	static void print(const char * pszFormat, ...)
	{
		const int MAX_LEN = 10000;
		char szBuf[MAX_LEN];

		va_list ap;
		va_start(ap, pszFormat);
		vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
		va_end(ap);

		WCHAR wszBuf[MAX_LEN] = {0};
		MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
		OutputDebugStringW(wszBuf);
		OutputDebugStringA("\n");

		//WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
		//printf("%s\n", szBuf);
	}
};

#endif



#endif //INCLUES_H
