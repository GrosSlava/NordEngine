// Copyright Nord Engine. All Rights Reserved.
#pragma once




// clang-format off
// FPlatformMisc will be defined.
#if WIN32 || WIN64
	#include "Windows/WindowsPlatformMisc/WindowsPlatformMisc.h"
#else
	#error "Undefined platform!"
#endif
// clang-format on