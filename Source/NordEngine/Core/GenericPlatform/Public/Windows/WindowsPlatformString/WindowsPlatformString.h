
#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
	#error PLATFORM_WINDOWS not defined!
#endif

#include "GenericPlatformString.h"
#include "MicrosoftPlatformString.h"




/*	
	Windows string implementation.
*/ 
struct ENGINE_API FWindowsPlatformString : public FMicrosoftPlatformString
{
	
};


typedef FWindowsPlatformString FPlatformString;

