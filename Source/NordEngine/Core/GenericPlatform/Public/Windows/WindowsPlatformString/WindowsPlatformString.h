// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
#error PLATFORM_WINDOWS not defined!
#endif

#include "GenericPlatformString.h"
#include "StandardPlatformString.h"




/**
	Windows string implementation.
*/
struct ENGINE_API FWindowsPlatformString : public FStandardPlatformString
{
};

using FPlatformString = FWindowsPlatformString;
