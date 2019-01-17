//#include "WinUtil.h"
//#include <windows.h>
//#include "WGL_ARB_multisample.h"
//#include <sstream>
//#include "AppInfo.h"
//
//// GL_EXT_texture_filter_anisotropic
//#define GL_TEXTURE_MAX_ANISOTROPY_EXT       0x84FE
//#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT   0x84FF
//
//bool _enableVerticalSync;
//bool _enableAlphaToCoverage;
//bool _supportsGenerateMipMap;
//int _maxAnisotrophy;
//
//extern HDC _hDC;
//extern HWND _hWnd;
//HGLRC _hRC;
//
//void EnableVerticalSync(bool enableVerticalSync)
//{
//    // WGL_EXT_swap_control.
//
//    typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(GLint);
//
//    static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
//        reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(
//        wglGetProcAddress("wglSwapIntervalEXT"));
//
//    if (wglSwapIntervalEXT)
//    {
//        wglSwapIntervalEXT(enableVerticalSync ? 1 : 0);
//        _enableVerticalSync = enableVerticalSync;
//    }
//}
//
//bool ExtensionSupported(const char *pszExtensionName)
//{
//    static const char *pszGLExtensions = 0;
//    static const char *pszWGLExtensions = 0;
//
//    if (!pszGLExtensions)
//        pszGLExtensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
//
//    if (!pszWGLExtensions)
//    {
//        // WGL_ARB_extensions_string.
//
//        typedef const char *(WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC);
//
//        PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
//            reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(
//            wglGetProcAddress("wglGetExtensionsStringARB"));
//
//        if (wglGetExtensionsStringARB)
//            pszWGLExtensions = wglGetExtensionsStringARB(_hDC);
//    }
//
//    if (!strstr(pszGLExtensions, pszExtensionName))
//    {
//        if (!strstr(pszWGLExtensions, pszExtensionName))
//            return false;
//    }
//
//    return true;
//}
//
//bool InitGL()
//{
//    if (!(_hDC = GetDC(_hWnd)))
//	{
//        //throw std::runtime_error("GetDC() failed.");
//		return false;
//	}
//
//    int pf = 0;
//    PIXELFORMATDESCRIPTOR pfd = {0};
//    OSVERSIONINFO osvi = {0};
//
//    pfd.nSize = sizeof(pfd);
//    pfd.nVersion = 1;
//    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
//    pfd.iPixelType = PFD_TYPE_RGBA;
//    pfd.cColorBits = 24;
//    pfd.cDepthBits = 24;
//    pfd.iLayerType = PFD_MAIN_PLANE;
//
//    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//
//    if (!GetVersionEx(&osvi))
//        throw std::runtime_error("GetVersionEx() failed.");
//
//    // When running under Windows Vista or later support desktop composition.
//    if (osvi.dwMajorVersion > 6 || (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion >= 0))
//        pfd.dwFlags |=  PFD_SUPPORT_COMPOSITION;
//
//    // Setting up your Windows application to use multisample anti-aliasing
//    // (MSAA) is platform dependent. The actual process varies between
//    // operating systems. Under Windows we use the WGL_ARB_pixel_format and the
//    // WGL_ARB_multisample OpenGL extensions to enumerate an extended pixel
//    // format that supports MSAA.
//    //
//    // For further details check out the source code for these functions
//    // (found in the WGL_ARB_multisample.cpp file):
//    //  ChooseBestAntiAliasingPixelFormat()
//    //  ChooseAntiAliasingPixelFormat()
//
//	bool needMultiSample = true;
//    
//	if(needMultiSample)
//	{
//		bool selectBestMultiSample = false;
//		// Here's an example of how to select a pixel format that supports a
//		// specific number of samples. Here we have chosen 4x MSAA.
//		if(selectBestMultiSample)
//		{
//			// Select a pixel format supporting the highest quality MSAA.
//			ChooseBestAntiAliasingPixelFormat(pf);
//			_enableAlphaToCoverage = true;
//
//			AppInfo::GetInstance()->_pixelFormatSupportMSAA = true;
//			AppInfo::GetInstance()->_pixelFormatHasHighestQualityMSAA = true;
//		}
//		else
//		{
//			int samples = 4;
//			ChooseAntiAliasingPixelFormat(pf, samples);
//			if (pf)
//				_enableAlphaToCoverage = true;
//
//			AppInfo::GetInstance()->_pixelFormatSupportMSAA = true;
//			AppInfo::GetInstance()->_pixelFormatHasHighestQualityMSAA = false;
//			AppInfo::GetInstance()->_numberOfSamples = samples;
//		}
//
//		// It's always a good idea to fall back to a standard pixel format in case
//		// MSAA is not supported. Most modern video cards should support some form
//		// of MSAA - one exception is older embedded video cards.
//		if (!pf)
//		{
//			AppInfo::GetInstance()->_pixelFormatSupportMSAA = false;
//			pf = ChoosePixelFormat(_hDC, &pfd);
//			_enableAlphaToCoverage = false;
//		}
//	}
//	else
//	{
//		AppInfo::GetInstance()->_pixelFormatSupportMSAA = false;
//		pf = ChoosePixelFormat(_hDC, &pfd);
//		_enableAlphaToCoverage = false;
//	}
//
//    if (!SetPixelFormat(_hDC, pf, &pfd))
//        throw std::runtime_error("SetPixelFormat() failed.");
//
//    if (!(_hRC = wglCreateContext(_hDC)))
//        throw std::runtime_error("wglCreateContext() failed.");
//
//    if (!wglMakeCurrent(_hDC, _hRC))
//        throw std::runtime_error("wglMakeCurrent() failed.");
//
//    EnableVerticalSync(false);
//
//    // Check for GL_EXT_texture_filter_anisotropic support.
//    if (ExtensionSupported("GL_EXT_texture_filter_anisotropic"))
//        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &_maxAnisotrophy);
//    else
//        _maxAnisotrophy = 1;
//
//    // Check for GL_SGIS_generate_mipmap support.
//    if (ExtensionSupported("GL_SGIS_generate_mipmap"))
//        _supportsGenerateMipMap = true;
//    else
//        _supportsGenerateMipMap = false;
//}
//
//void SetProcessorAffinity()
//{
//    // Assign the current thread to one processor. This ensures that timing
//    // code runs on only one processor, and will not suffer any ill effects
//    // from power management.
//    //
//    // Based on DXUTSetProcessorAffinity() function from the DXUT framework.
//
//    DWORD_PTR dwProcessAffinityMask = 0;
//    DWORD_PTR dwSystemAffinityMask = 0;
//    HANDLE hCurrentProcess = GetCurrentProcess();
//
//    if (!GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask))
//        return;
//
//    if (dwProcessAffinityMask)
//    {
//        // Find the lowest processor that our process is allowed to run against.
//
//        DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));
//
//        // Set this as the processor that our thread must always run against.
//        // This must be a subset of the process affinity mask.
//
//        HANDLE hCurrentThread = GetCurrentThread();
//
//        if (hCurrentThread != INVALID_HANDLE_VALUE)
//        {
//            SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
//            CloseHandle(hCurrentThread);
//        }
//    }
//    CloseHandle(hCurrentProcess);
//}
//
//
//void DeleteContext()
//{
//    if (_hRC)
//    {
//        wglMakeCurrent(_hDC, 0);
//        wglDeleteContext(_hRC);
//        _hRC = 0;
//    }
//}