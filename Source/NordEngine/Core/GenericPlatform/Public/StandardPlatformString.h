// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "GenericPlatformString.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "Char.h"




/**
	Standard implementation.
*/
struct ENGINE_API FStandardPlatformString : public FGenericPlatformString
{
	using FGenericPlatformString::Stricmp;
	using FGenericPlatformString::Strncmp;
	using FGenericPlatformString::Strnicmp;


	template<typename CharType>
	static FORCEINLINE CharType* Strupr(CharType* Dest, SIZE_T DestCount)
	{
		for( CharType* Char = Dest; *Char && DestCount > 0; ++Char, --DestCount )
		{
			*Char = TChar<CharType>::ToUpper(*Char);
		}
		return Dest;
	}

	//........................... Wide character implementation..............................//

	static FORCEINLINE WIDECHAR* Strcpy(WIDECHAR* Dest, SIZE_T DestCount, const WIDECHAR* Src) { return wcscpy(Dest, Src); }
	static FORCEINLINE WIDECHAR* Strncpy(WIDECHAR* Dest, const WIDECHAR* Src, SIZE_T MaxLen)
	{
		wcsncpy(Dest, Src, MaxLen - 1);
		Dest[MaxLen - 1] = 0;
		return Dest;
	}
	static FORCEINLINE WIDECHAR* Strcat(WIDECHAR* Dest, SIZE_T DestCount, const WIDECHAR* Src) { return (WIDECHAR*)wcscat(Dest, Src); }
	static FORCEINLINE int32 Strcmp(const WIDECHAR* String1, const WIDECHAR* String2) { return wcscmp(String1, String2); }
	static FORCEINLINE int32 Strncmp(const WIDECHAR* String1, const WIDECHAR* String2, SIZE_T Count) { return wcsncmp(String1, String2, Count); }
	static FORCEINLINE int32 Strlen(const WIDECHAR* String) { return (int32)wcslen(String); }
	static FORCEINLINE int32 Strnlen(const WIDECHAR* String, SIZE_T StringSize) { return (int32)wcsnlen_s(String, StringSize); }
	static FORCEINLINE const WIDECHAR* Strstr(const WIDECHAR* String, const WIDECHAR* Find) { return wcsstr(String, Find); }
	static FORCEINLINE const WIDECHAR* Strchr(const WIDECHAR* String, WIDECHAR C) { return wcschr(String, C); }
	static FORCEINLINE const WIDECHAR* Strrchr(const WIDECHAR* String, WIDECHAR C) { return wcsrchr(String, C); }
	static FORCEINLINE int32 Atoi(const WIDECHAR* String) { return (int32)wcstol(String, NULL, 10); }
	static FORCEINLINE int64 Atoi64(const WIDECHAR* String) { return wcstoll(String, NULL, 10); }
	static FORCEINLINE float Atof(const WIDECHAR* String) { return wcstof(String, NULL); }
	static FORCEINLINE double Atod(const WIDECHAR* String) { return wcstod(String, NULL); }
	static FORCEINLINE int32 Strtoi(const WIDECHAR* Start, WIDECHAR** End, int32 Base) { return (int32)wcstol(Start, End, Base); }
	static FORCEINLINE int64 Strtoi64(const WIDECHAR* Start, WIDECHAR** End, int32 Base) { return wcstoll(Start, End, Base); }
	static FORCEINLINE uint64 Strtoui64(const WIDECHAR* Start, WIDECHAR** End, int32 Base) { return wcstoull(Start, End, Base); }
	static FORCEINLINE WIDECHAR* Strtok(WIDECHAR* StrToken, const WIDECHAR* Delim, WIDECHAR** Context) { return wcstok(StrToken, Delim, Context); }
	static FORCEINLINE int32 GetVarArgs(WIDECHAR* Dest, SIZE_T DestSize, const WIDECHAR*& Fmt, va_list ArgPtr)
	{
		int32 Result = vswprintf(Dest, DestSize, Fmt, ArgPtr);
		va_end(ArgPtr);
		return Result;
	}

	//.......................................................................................//

	//..................................Ansi implementation..................................//

	static FORCEINLINE ANSICHAR* Strcpy(ANSICHAR* Dest, SIZE_T DestCount, const ANSICHAR* Src) { return strcpy(Dest, Src); }
	static FORCEINLINE ANSICHAR* Strncpy(ANSICHAR* Dest, const ANSICHAR* Src, int32 MaxLen)
	{
		strncpy(Dest, Src, MaxLen);
		Dest[MaxLen - 1] = 0;
		return Dest;
	}
	static FORCEINLINE ANSICHAR* Strcat(ANSICHAR* Dest, SIZE_T DestCount, const ANSICHAR* Src) { return strcat(Dest, Src); }
	static FORCEINLINE int32 Strcmp(const ANSICHAR* String1, const ANSICHAR* String2) { return strcmp(String1, String2); }
	static FORCEINLINE int32 Strncmp(const ANSICHAR* String1, const ANSICHAR* String2, SIZE_T Count) { return strncmp(String1, String2, Count); }
	static FORCEINLINE int32 Strlen(const ANSICHAR* String) { return (int32)strlen(String); }
	static FORCEINLINE int32 Strnlen(const ANSICHAR* String, SIZE_T StringSize) { return (int32)strnlen_s(String, StringSize); }
	static FORCEINLINE const ANSICHAR* Strstr(const ANSICHAR* String, const ANSICHAR* Find) { return strstr(String, Find); }
	static FORCEINLINE const ANSICHAR* Strchr(const ANSICHAR* String, ANSICHAR C) { return strchr(String, C); }
	static FORCEINLINE const ANSICHAR* Strrchr(const ANSICHAR* String, ANSICHAR C) { return strrchr(String, C); }
	static FORCEINLINE int32 Atoi(const ANSICHAR* String) { return atoi(String); }
	static FORCEINLINE int64 Atoi64(const ANSICHAR* String) { return strtoll(String, NULL, 10); }
	static FORCEINLINE float Atof(const ANSICHAR* String) { return (float)atof(String); }
	static FORCEINLINE double Atod(const ANSICHAR* String) { return atof(String); }
	static FORCEINLINE int32 Strtoi(const ANSICHAR* Start, ANSICHAR** End, int32 Base) { return (int32)strtol(Start, End, Base); }
	static FORCEINLINE int64 Strtoi64(const ANSICHAR* Start, ANSICHAR** End, int32 Base) { return strtoll(Start, End, Base); }
	static FORCEINLINE uint64 Strtoui64(const ANSICHAR* Start, ANSICHAR** End, int32 Base) { return strtoull(Start, End, Base); }
	static FORCEINLINE ANSICHAR* Strtok(ANSICHAR* StrToken, const ANSICHAR* Delim, ANSICHAR** Context) { return strtok(StrToken, Delim); }
	static FORCEINLINE int32 GetVarArgs(ANSICHAR* Dest, SIZE_T DestSize, const ANSICHAR*& Fmt, va_list ArgPtr)
	{
		int32 Result = vsnprintf(Dest, DestSize, Fmt, ArgPtr);
		va_end(ArgPtr);
		return (Result != -1 && Result < (int32)DestSize) ? Result : -1;
	}

	//.......................................................................................//
};