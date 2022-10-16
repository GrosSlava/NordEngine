
#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
	#error PLATFORM_WINDOWS not defined!
#endif



#define WINDOWS_H_WRAPPER

#include "Windows/PreWindowsAPI.h"
#include "Windows/MinWindows.h"
#include "Windows/PostWindowsAPI.h"

#undef WINDOWS_H_WRAPPER