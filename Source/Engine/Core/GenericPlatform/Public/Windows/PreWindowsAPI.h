
#pragma once

#include "GenericPlatform.h"

#ifndef WINDOWS_H_WRAPPER
	#error Do not include PreWindowsAPI directly!
#endif

#if !PLATFORM_WINDOWS
	#error PLATFORM_WINDOWS not defined!
#endif



PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING

// Save these macros for later; Windows redefines them
#pragma push_macro("MAX_uint8")
#pragma push_macro("MAX_uint16")
#pragma push_macro("MAX_uint32")
#pragma push_macro("MAX_int32")
#pragma push_macro("TEXT")
#pragma push_macro("TRUE")
#pragma push_macro("FALSE")

// Undefine the TEXT macro for winnt.h to redefine it, unless it's already been included
#ifndef _WINNT_
	#undef TEXT
#endif

// Disable all normal third party headers
THIRD_PARTY_INCLUDES_START