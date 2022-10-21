
#pragma once

#include "CoreMinimal.h"

#if !PLATFORM_WINDOWS
	#error PLATFORM_WINDOWS not defined!
#endif




/*
	Entry point for engine.
	Use it in your main after custom initialization.
*/
extern int ENGINE_API WINAPI wWinMain_Internal();
