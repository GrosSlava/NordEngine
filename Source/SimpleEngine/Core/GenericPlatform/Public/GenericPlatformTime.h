
#pragma once




#if WIN32 || WIN64
	#include "Windows/WindowsPlatformTime/WindowsPlatformTime.h"
#else
	#error "Undefined platform!"
#endif