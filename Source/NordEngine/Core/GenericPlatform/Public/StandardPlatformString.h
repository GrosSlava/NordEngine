// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "GenericPlatformString.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>
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
	template<typename CharType>
	static FORCEINLINE CharType* Strlwr(CharType* Dest, SIZE_T DestCount)
	{
		for( CharType* Char = Dest; *Char && DestCount > 0; ++Char, --DestCount )
		{
			*Char = TChar<CharType>::ToLower(*Char);
		}
		return Dest;
	}

	//........................... Wide character implementation..............................//

	static FORCEINLINE void Strcpy(WIDECHAR* Dest, SIZE_T DestCount, const WIDECHAR* Src) { wcscpy_s(Dest, DestCount, Src); }
	static FORCEINLINE void Strncpy(WIDECHAR* Dest, const WIDECHAR* Src, SIZE_T MaxLen)
	{
		wcsncpy(Dest, Src, MaxLen - 1);
		Dest[MaxLen - 1] = 0;
	}
	static FORCEINLINE void Strcat(WIDECHAR* Dest, SIZE_T DestCount, const WIDECHAR* Src) { wcscat_s(Dest, DestCount, Src); }
	static FORCEINLINE int32 Strcmp(const WIDECHAR* String1, const WIDECHAR* String2) { return wcscmp(String1, String2); }
	static FORCEINLINE int32 Strncmp(const WIDECHAR* String1, const WIDECHAR* String2, SIZE_T Count) { return wcsncmp(String1, String2, Count); }
	static FORCEINLINE int32 Strlen(const WIDECHAR* String) { return (int32)wcslen(String); }
	static FORCEINLINE int32 Strnlen(const WIDECHAR* String, SIZE_T StringSize) { return (int32)wcsnlen_s(String, StringSize); }
	static FORCEINLINE const WIDECHAR* Strstr(const WIDECHAR* String, const WIDECHAR* Find) { return wcsstr(String, Find); }
	static FORCEINLINE const WIDECHAR* Strchr(const WIDECHAR* String, WIDECHAR C) { return wcschr(String, C); }
	static FORCEINLINE const WIDECHAR* Strrchr(const WIDECHAR* String, WIDECHAR C) { return wcsrchr(String, C); }
	static FORCEINLINE int32 Atoi(const WIDECHAR* String) { return (int32)wcstol(String, nullptr, 0); }
	static FORCEINLINE int64 Atoi64(const WIDECHAR* String) { return wcstoll(String, nullptr, 0); }
	static FORCEINLINE float Atof(const WIDECHAR* String) { return wcstof(String, nullptr); }
	static FORCEINLINE double Atod(const WIDECHAR* String) { return wcstod(String, nullptr); }
	static FORCEINLINE int32 Strtoi(const WIDECHAR* Start, WIDECHAR** End, int32 Base) { return (int32)wcstol(Start, End, Base); }
	static FORCEINLINE int64 Strtoi64(const WIDECHAR* Start, WIDECHAR** End, int32 Base) { return wcstoll(Start, End, Base); }
	static FORCEINLINE uint64 Strtoui64(const WIDECHAR* Start, WIDECHAR** End, int32 Base) { return wcstoull(Start, End, Base); }
	static FORCEINLINE WIDECHAR* Strtok(WIDECHAR* StrToken, const WIDECHAR* Delim, WIDECHAR** Context) { return wcstok(StrToken, Delim, Context); }
	static FORCEINLINE WIDECHAR* Strset(WIDECHAR* Dest, SIZE_T DestCount, WIDECHAR C) { return (WIDECHAR*)wmemset(Dest, C, DestCount); }
	static FORCEINLINE int32 GetVarArgs(WIDECHAR* Dest, SIZE_T DestSize, const WIDECHAR*& Fmt, va_list ArgPtr)
	{
		int32 Result = vswprintf(Dest, DestSize, Fmt, ArgPtr);
		va_end(ArgPtr);
		return Result;
	}
	template<typename... Args>
	static FORCEINLINE int32 Printf(WIDECHAR* Buffer, SIZE_T BufferCount, WIDECHAR* Format, Args... args)
	{
		return swprintf(Buffer, BufferCount, Format, args...);
	}
	template<typename... Args>
	static FORCEINLINE int32 FPrintf(FILE* Stream, WIDECHAR* Format, Args... args)
	{
		return fwprintf(Stream, Format, args...);
	}
	//.......................................................................................//

	//..................................Ansi implementation..................................//

	static FORCEINLINE void Strcpy(ANSICHAR* Dest, SIZE_T DestCount, const ANSICHAR* Src) { strcpy_s(Dest, DestCount, Src); }
	static FORCEINLINE void Strncpy(ANSICHAR* Dest, const ANSICHAR* Src, int32 MaxLen)
	{
		strncpy(Dest, Src, MaxLen);
		Dest[MaxLen - 1] = 0;
	}
	static FORCEINLINE void Strcat(ANSICHAR* Dest, SIZE_T DestCount, const ANSICHAR* Src) { strcat_s(Dest, DestCount, Src); }
	static FORCEINLINE int32 Strcmp(const ANSICHAR* String1, const ANSICHAR* String2) { return strcmp(String1, String2); }
	static FORCEINLINE int32 Strncmp(const ANSICHAR* String1, const ANSICHAR* String2, SIZE_T Count) { return strncmp(String1, String2, Count); }
	static FORCEINLINE int32 Strlen(const ANSICHAR* String) { return (int32)strlen(String); }
	static FORCEINLINE int32 Strnlen(const ANSICHAR* String, SIZE_T StringSize) { return (int32)strnlen_s(String, StringSize); }
	static FORCEINLINE const ANSICHAR* Strstr(const ANSICHAR* String, const ANSICHAR* Find) { return strstr(String, Find); }
	static FORCEINLINE const ANSICHAR* Strchr(const ANSICHAR* String, ANSICHAR C) { return strchr(String, C); }
	static FORCEINLINE const ANSICHAR* Strrchr(const ANSICHAR* String, ANSICHAR C) { return strrchr(String, C); }
	static FORCEINLINE int32 Atoi(const ANSICHAR* String) { return atoi(String); }
	static FORCEINLINE int64 Atoi64(const ANSICHAR* String) { return strtoll(String, nullptr, 0); }
	static FORCEINLINE float Atof(const ANSICHAR* String) { return (float)atof(String); }
	static FORCEINLINE double Atod(const ANSICHAR* String) { return atof(String); }
	static FORCEINLINE int32 Strtoi(const ANSICHAR* Start, ANSICHAR** End, int32 Base) { return (int32)strtol(Start, End, Base); }
	static FORCEINLINE int64 Strtoi64(const ANSICHAR* Start, ANSICHAR** End, int32 Base) { return strtoll(Start, End, Base); }
	static FORCEINLINE uint64 Strtoui64(const ANSICHAR* Start, ANSICHAR** End, int32 Base) { return strtoull(Start, End, Base); }
	static FORCEINLINE ANSICHAR* Strtok(ANSICHAR* StrToken, const ANSICHAR* Delim, ANSICHAR** Context) { return strtok(StrToken, Delim); }
	static FORCEINLINE ANSICHAR* Strset(ANSICHAR* Dest, SIZE_T DestCount, ANSICHAR C) { return (ANSICHAR*)memset(Dest, C, DestCount); }
	static FORCEINLINE int32 GetVarArgs(ANSICHAR* Dest, SIZE_T DestSize, const ANSICHAR*& Fmt, va_list ArgPtr)
	{
		int32 Result = vsnprintf(Dest, DestSize, Fmt, ArgPtr);
		va_end(ArgPtr);
		return (Result != -1 && Result < (int32)DestSize) ? Result : -1;
	}
	template<typename... Args>
	static FORCEINLINE int32 Printf(ANSICHAR* Buffer, SIZE_T BufferCount, ANSICHAR* Format, Args... args)
	{
		return snprintf(Buffer, BufferCount, Format, args...);
	}
	template<typename... Args>
	static FORCEINLINE int32 FPrintf(FILE* Stream, ANSICHAR* Format, Args... args)
	{
		return fprintf(Stream, Format, args...);
	}
	//.......................................................................................//
};