// Copyright Nord Engine. All Rights Reserved.
#pragma once




// clang-format off
// FPlatformFile will be defined.
#if WIN32 || WIN64
	#include "Windows/WindowsPlatformFile/WindowsPlatformFile.h"
#else
	#error "Undefined platform!"
#endif
// clang-format on