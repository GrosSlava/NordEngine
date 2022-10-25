
#pragma once




#if WIN32 || WIN64
	#include "Windows/WindowsPlatformMisc/WindowsPlatformMisc.h"
#else
	#error "Undefined platform!"
#endif