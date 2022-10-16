
#pragma once

#include "CoreMinimal.h"

#if !PLATFORM_WINDOWS
	#error PLATFORM_WINDOWS not defined!
#endif




int ENGINE_API WINAPI wWinMain_Internal(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
int ENGINE_API WINAPI wWinMain_Internal();
