#ifndef MY_DEFINES_H
#define MY_DEFINES_H
#pragma once

#ifdef  PLATFORM_WIN32
	#include <windows.h>
	#include <gl/gl.h>
	#include <gl/glu.h>

	#define glOrthof glOrtho
#endif

#endif
