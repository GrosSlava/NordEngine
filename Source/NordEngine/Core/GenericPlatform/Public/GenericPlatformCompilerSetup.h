// Copyright Nord Engine. All Rights Reserved.
#pragma once




// clang-format off
#if WIN32 || WIN64
	#include "Windows/WindowsPlatformCompilerSetup.h"
#else
	#error "Undefined platform!"
#endif
// clang-format on
