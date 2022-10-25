
#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
	#error PLATFORM_WINDOWS not defined!
#endif

#include "intrin.h"





/* 
	We use a struct packing of 4 for Win32 and 8 for Win64,
	and the default packing is 8 for Win32 and 16 for Win64.
*/
#ifdef WIN64
	#define PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING \
		__pragma(pack(push)) \
		__pragma(pack(16))
#else 
	#define PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING \
		__pragma(pack(push)) \
		__pragma(pack(8))
#endif 
	
#define PRAGMA_POP_PLATFORM_DEFAULT_PACKING \
	__pragma(pack(pop))






#define PLATFORM_BREAK() (__nop(), __debugbreak())

#define PLATFORM_CODE_SECTION(Name) __declspec(code_seg(Name)) 

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)


#define VARARGS     __cdecl									/* Functions with variable arguments */
#define CDECL	    __cdecl									/* Standard C function */
#define STDCALL		__stdcall								/* Standard calling convention */
#define FORCEINLINE _forceinline							/* Force code to be inline */
#define INLINE		inline									/* Compiler choose to inline code or not */
#define NOINLINE	_declspec(noinline)						/* Force code to NOT be inline */
#define RESTRICT	__restrict								/* no alias hint */
#define CONSTEXPR	constexpr
#define ABSTRACT	abstract


#define LINE_TERMINATOR TEXT("\r\n")
#define LINE_TERMINATOR_ANSI "\r\n"


#define MS_ALIGN(n) __declspec(align(n))
#define MSVC_PRAGMA(Pragma) __pragma(Pragma)