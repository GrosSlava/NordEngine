// Copyright Nord Engine. All Rights Reserved.
#pragma once




// FPlatformTime will be defined.
// clang-format off
#if WIN32 || WIN64
	#include "Windows/WindowsPlatformTime/WindowsPlatformTime.h"
#else
	#error "Undefined platform!"
#endif
// clang-format on