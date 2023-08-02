// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
#error PLATFORM_WINDOWS not defined!
#endif

#include "FString.h"




/**
	Helper struct used to get the string version of the Windows version.
*/
struct ENGINE_API FWindowsOSVersionHelper
{
	enum ErrorCodes
	{
		SUCCEEDED = 0,
		ERROR_UNKNOWNVERSION = 1,
		ERROR_GETPRODUCTINFO_FAILED = 2,
		ERROR_GETVERSIONEX_FAILED = 4,
		ERROR_GETWINDOWSGT62VERSIONS_FAILED = 8,
	};

	static int32 GetOSVersions(FString& OutOSVersion, FString& OutOSSubVersion);
	static bool GetOSVersion(FString& OutOSVersion);
};
