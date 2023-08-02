// Copyright Nord Engine. All Rights Reserved.
#pragma once

#ifndef WINDOWS_H_WRAPPER
#error Do not include PreWindowsAPI directly!
#endif

#if !PLATFORM_WINDOWS
#error PLATFORM_WINDOWS not defined!
#endif




// Disable all normal third party headers
THIRD_PARTY_INCLUDES_START

// Store the struct packing setting
PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING

// Save these macros for later; Windows redefines them
#pragma push_macro("TEXT")

// Undefine the TEXT macro for winnt.h to redefine it, unless it's already been included
#ifndef _WINNT_
#undef TEXT
#endif