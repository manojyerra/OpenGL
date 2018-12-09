#include "Platform.h"
#include "Platform_Android.h"
#include "Platform_Win32.h"

Platform* Platform::_ref = NULL;

Platform::Platform()
{
}

Platform::~Platform()
{
}

Platform* Platform::GetPlatform()
{
	if(_ref == NULL)
	{
#ifdef _PLATFORM_ANDROID
		_ref = new Platform_Android();
#endif

#ifdef _PLATFORM_WIN32
		_ref = new Platform_Win32();
#endif

	}

	return _ref;
}

void Platform::DeletePlatform()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

