
#pragma once

#include "GenericPlatform.h"
#include "GenericPlatformString.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <tchar.h>




PRAGMA_DISABLE_DEPRECATION_WARNINGS


struct ENGINE_API FMicrosoftPlatformString : public FGenericPlatformString
{
	using FGenericPlatformString::Stricmp;
	using FGenericPlatformString::Strncmp;
	using FGenericPlatformString::Strnicmp;


	//........................... Wide character implementation..............................//
	/*
	static FORCEINLINE WIDECHAR* Strcpy(WIDECHAR* Dest, const WIDECHAR* Src) noexcept
	{
		return (WIDECHAR*)_tcscpy(Dest, Src);
	}

	static FORCEINLINE WIDECHAR* Strncpy(WIDECHAR* Dest, const WIDECHAR* Src, size_t MaxLen) noexcept
	{
		_tcsncpy(Dest, Src, MaxLen - 1);
		Dest[MaxLen - 1] = 0;
		return Dest;
	}

	static FORCEINLINE WIDECHAR* Strcat(WIDECHAR* Dest, const WIDECHAR* Src) noexcept
	{
		return (WIDECHAR*)_tcscat(Dest, Src);
	}

	static FORCEINLINE WIDECHAR* Strupr(WIDECHAR* Dest) noexcept
	{
		return (WIDECHAR*)_tcsupr(Dest);
	}

	static FORCEINLINE int32 Strcmp(const WIDECHAR* String1, const WIDECHAR* String2) noexcept
	{
		return _tcscmp(String1, String2);
	}

	static FORCEINLINE int32 Stricmp(const WIDECHAR* String1, const WIDECHAR* String2) noexcept
	{
		return _tcsicmp(String1, String2);
	}

	static FORCEINLINE int32 Strncmp(const WIDECHAR* String1, const WIDECHAR* String2, size_t Count) noexcept
	{
		return _tcsncmp(String1, String2, Count);
	}

	static FORCEINLINE int32 Strnicmp(const WIDECHAR* String1, const WIDECHAR* String2, size_t Count) noexcept
	{
		return _tcsnicmp(String1, String2, Count);
	}

	static FORCEINLINE int32 Strlen(const WIDECHAR* String) noexcept
	{
		return (int32)_tcslen(String);
	}

	static FORCEINLINE int32 Strnlen(const WIDECHAR* String, size_t StringSize) noexcept
	{
		return (int32)_tcsnlen(String, StringSize);
	}

	static FORCEINLINE const WIDECHAR* Strstr(const WIDECHAR* String, const WIDECHAR* Find) noexcept
	{
		return _tcsstr(String, Find);
	}

	static FORCEINLINE const WIDECHAR* Strchr(const WIDECHAR* String, WIDECHAR C) noexcept
	{
		return _tcschr(String, C);
	}

	static FORCEINLINE const WIDECHAR* Strrchr(const WIDECHAR* String, WIDECHAR C) noexcept
	{
		return _tcsrchr(String, C);
	}

	static FORCEINLINE int32 Atoi(const WIDECHAR* String) noexcept
	{
		return _tstoi(String);
	}

	static FORCEINLINE int64 Atoi64(const WIDECHAR* String) noexcept
	{
		return _tstoi64(String);
	}

	static FORCEINLINE float Atof(const WIDECHAR* String) noexcept
	{
		return (float)_tstof(String);
	}

	static FORCEINLINE double Atod(const WIDECHAR* String) noexcept
	{
		return _tcstod(String, NULL);
	}

	static FORCEINLINE int32 Strtoi(const WIDECHAR* Start, WIDECHAR** End, int32 Base) noexcept
	{
		return _tcstoul(Start, End, Base);
	}

	static FORCEINLINE int64 Strtoi64(const WIDECHAR* Start, WIDECHAR** End, int32 Base) noexcept
	{
		return _tcstoi64(Start, End, Base);
	}

	static FORCEINLINE uint64 Strtoui64(const WIDECHAR* Start, WIDECHAR** End, int32 Base) noexcept
	{
		return _tcstoui64(Start, End, Base);
	}

	static FORCEINLINE WIDECHAR* Strtok(WIDECHAR* StrToken, const WIDECHAR* Delim, WIDECHAR** Context) noexcept
	{
		return _tcstok_s(StrToken, Delim, Context);
	}


	static FORCEINLINE int32 GetVarArgs(WIDECHAR* Dest, size_t DestSize, const WIDECHAR*& Fmt, va_list ArgPtr) noexcept
	{
		int32 Result = vswprintf(Dest, DestSize, Fmt, ArgPtr);
		va_end(ArgPtr);
		return Result;
	}
	*/
	//.......................................................................................//



	//..................................Ansi implementation..................................//
	
	static FORCEINLINE ANSICHAR* Strcpy(ANSICHAR* Dest, const ANSICHAR* Src) noexcept
	{
		return (ANSICHAR*)strcpy(Dest, Src);
	}

	static FORCEINLINE void Strncpy(ANSICHAR* Dest, const ANSICHAR* Src, size_t MaxLen) noexcept
	{
		strncpy(Dest, Src, MaxLen);
		Dest[MaxLen - 1] = 0;
	}

	static FORCEINLINE ANSICHAR* Strcat(ANSICHAR* Dest, const ANSICHAR* Src) noexcept
	{
		return (ANSICHAR*)strcat(Dest, Src);
	}

	static FORCEINLINE ANSICHAR* Strupr(ANSICHAR* Dest) noexcept
	{
		return (ANSICHAR*)_strupr(Dest);
	}

	static FORCEINLINE int32 Strcmp(const ANSICHAR* String1, const ANSICHAR* String2) noexcept
	{
		return strcmp(String1, String2);
	}

	static FORCEINLINE int32 Stricmp(const ANSICHAR* String1, const ANSICHAR* String2) noexcept
	{
		return stricmp(String1, String2);
	}

	static FORCEINLINE int32 Strncmp(const ANSICHAR* String1, const ANSICHAR* String2, size_t Count) noexcept
	{
		return strncmp(String1, String2, Count);
	}

	static FORCEINLINE int32 Strnicmp(const ANSICHAR* String1, const ANSICHAR* String2, size_t Count) noexcept
	{
		return strnicmp(String1, String2, Count);
	}

	static FORCEINLINE int32 Strlen(const ANSICHAR* String) noexcept
	{
		return (int32)strlen(String);
	}

	static FORCEINLINE int32 Strnlen(const ANSICHAR* String, size_t StringSize) noexcept
	{
		return (int32)strnlen(String, StringSize);
	}

	static FORCEINLINE const ANSICHAR* Strstr(const ANSICHAR* String, const ANSICHAR* Find) noexcept
	{
		return strstr(String, Find);
	}

	static FORCEINLINE const ANSICHAR* Strchr(const ANSICHAR* String, ANSICHAR C) noexcept
	{
		return strchr(String, C);
	}

	static FORCEINLINE const ANSICHAR* Strrchr(const ANSICHAR* String, ANSICHAR C) noexcept
	{
		return strrchr(String, C);
	}

	static FORCEINLINE int32 Atoi(const ANSICHAR* String) noexcept
	{
		return atoi(String);
	}

	static FORCEINLINE int64 Atoi64(const ANSICHAR* String) noexcept
	{
		return _strtoi64(String, NULL, 10);
	}

	static FORCEINLINE float Atof(const ANSICHAR* String) noexcept
	{
		return (float)atof(String);
	}

	static FORCEINLINE double Atod(const ANSICHAR* String) noexcept
	{
		return atof(String);
	}

	static FORCEINLINE int32 Strtoi(const ANSICHAR* Start, ANSICHAR** End, int32 Base) noexcept
	{
		return strtol(Start, End, Base);
	}

	static FORCEINLINE int64 Strtoi64(const ANSICHAR* Start, ANSICHAR** End, int32 Base) noexcept
	{
		return _strtoi64(Start, End, Base);
	}

	static FORCEINLINE uint64 Strtoui64(const ANSICHAR* Start, ANSICHAR** End, int32 Base) noexcept
	{
		return _strtoui64(Start, End, Base);
	}

	static FORCEINLINE ANSICHAR* Strtok(ANSICHAR* StrToken, const ANSICHAR* Delim, ANSICHAR** Context)
	{
		return strtok_s(StrToken, Delim, Context);
	}

	static FORCEINLINE int32 GetVarArgs(ANSICHAR* Dest, size_t DestSize, const ANSICHAR*& Fmt, va_list ArgPtr) noexcept
	{
		int32 Result = vsnprintf(Dest, DestSize, Fmt, ArgPtr);
		va_end(ArgPtr);
		return (Result != -1 && Result < (int32)DestSize) ? Result : -1;
	}

	//.......................................................................................//



	//.......UCS2CHAR implementation - this is identical to WIDECHAR for Windows platforms...//
	
	/*static FORCEINLINE int32 Strlen(const UCS2CHAR* String) noexcept
	{
		return (int32)(_tcslen((const WIDECHAR*)String));
	}

	static FORCEINLINE int32 Strnlen(const UCS2CHAR* String, size_t StringSize) noexcept
	{
		return (int32)(_tcsnlen((const WIDECHAR*)String, StringSize));
	}*/

	//.......................................................................................//
}; 


PRAGMA_ENABLE_DEPRECATION_WARNINGS