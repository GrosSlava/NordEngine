
#pragma once




#if WIN32 || WIN64
	#include "Windows/WindowsPlatformFile/WindowsPlatformFile.h"
#else
	#error "Undefined platform!"
#endif