// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
#error PLATFORM_WINDOWS not defined!
#endif

#include "GenericPlatformMiscInfo.h"

#include "Array.h"




/**
	Helper struct used to get stack frames.
*/
struct ENGINE_API FWindowsStackTraceHelper
{
	static bool GetStackFrames(TArray<FStackFrame>& OutStackFrames);
};