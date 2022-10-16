
#pragma once

#include "GenericPlatform.h"




/*
	Generic string implementation for most platforms.
*/
struct ENGINE_API FGenericPlatformString
{
	static int32 Stricmp(const ANSICHAR* String1, const ANSICHAR* String2);
	static int32 Stricmp(const WIDECHAR* String1, const WIDECHAR* String2);
	static int32 Stricmp(const UTF8CHAR* String1, const UTF8CHAR* String2);
	static int32 Stricmp(const UTF16CHAR* String1, const UTF16CHAR* String2);
	static int32 Stricmp(const UTF32CHAR* String1, const UTF32CHAR* String2);

	static int32 Stricmp(const ANSICHAR* String1, const WIDECHAR* String2);
	static int32 Stricmp(const ANSICHAR* String1, const UTF8CHAR* String2);
	static int32 Stricmp(const ANSICHAR* String1, const UTF16CHAR* String2);
	static int32 Stricmp(const ANSICHAR* String1, const UTF32CHAR* String2);
	static int32 Stricmp(const WIDECHAR* String1, const ANSICHAR* String2);
	static int32 Stricmp(const UTF8CHAR* String1, const ANSICHAR* String2);
	static int32 Stricmp(const UTF16CHAR* String1, const ANSICHAR* String2);
	static int32 Stricmp(const UTF32CHAR* String1, const ANSICHAR* String2);

	static int32 Strnicmp(const ANSICHAR* String1, const ANSICHAR* String2, SIZE_T Count);
	static int32 Strnicmp(const WIDECHAR* String1, const WIDECHAR* String2, SIZE_T Count);
	static int32 Strnicmp(const ANSICHAR* String1, const WIDECHAR* String2, SIZE_T Count);
	static int32 Strnicmp(const WIDECHAR* String1, const ANSICHAR* String2, SIZE_T Count);

	static int32 Strncmp(const ANSICHAR* String1, const ANSICHAR* String2, SIZE_T Count);
	static int32 Strncmp(const WIDECHAR* String1, const WIDECHAR* String2, SIZE_T Count);
	static int32 Strncmp(const ANSICHAR* String1, const WIDECHAR* String2, SIZE_T Count);
	static int32 Strncmp(const WIDECHAR* String1, const ANSICHAR* String2, SIZE_T Count);
};





//FPlatformString defined in platform string
#if WIN32 || WIN64
	#include "Windows/WindowsPlatformString/WindowsPlatformString.h"
#else
	#error "Undefined platform!"
#endif
