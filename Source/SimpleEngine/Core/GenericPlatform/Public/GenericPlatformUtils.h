
#pragma once




/*
	@param VERSION The release number in which the feature was marked deprecated.
	@param MESSAGE A message containing upgrade notes.
*/
#define DEPRECATED(Version, Message) [[deprecated(Message " Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.")]]

/* Hints compiler that expression is likely to be true, allows (penalized by worse performance) expression to be false */
#if( defined(__clang__) || defined(__GNUC__) ) && (PLATFORM_UNIX)
	#define LIKELY(x) __builtin_expect(!!(x), 1)
#else
	#define LIKELY(x) (x)
#endif

/* Hints compiler that expression is unlikely to be true, allows (penalized by worse performance) expression to be true */
#if( defined(__clang__) || defined(__GNUC__) ) && (PLATFORM_UNIX)
	#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
	#define UNLIKELY(x) (x)
#endif

// If we don't have a platform-specific define for the TEXT macro, define it now.
#ifndef TEXT
#if _UNICODE
	#define TEXT_PASTE(x) L##x
#else
	#define TEXT_PASTE(x) x
#endif
	#define TEXT(x) TEXT_PASTE(x)
#endif

#define MACRO_TEXT(x) TEXT(x)
#define STRINGIFY(x) #x
#define STRINGIFY_MACRO(x) STRINGIFY(x)
#define CONCAT(x, y) x##y
#define CONCAT_MACRO_TEXT(x, y) CONCAT(x, y)





#define PRAGMA_DISABLE_DEPRECATION_WARNINGS \
			__pragma (warning(push)) \
			__pragma (warning(disable: 4995)) /* 'function': name was marked as #pragma deprecated */ \
			__pragma (warning(disable: 4996)) /* The compiler encountered a deprecated declaration. */

#define PRAGMA_ENABLE_DEPRECATION_WARNINGS \
			__pragma (warning(pop))



#define PRAGMA_DISABLE_SHADOW_VARIABLE_WARNINGS \
			__pragma (warning(push)) \
			__pragma (warning(disable: 4456)) /* declaration of 'LocalVariable' hides previous local declaration */ \
			__pragma (warning(disable: 4457)) /* declaration of 'LocalVariable' hides function parameter */ \
			__pragma (warning(disable: 4458)) /* declaration of 'LocalVariable' hides class member */ \
			__pragma (warning(disable: 4459)) /* declaration of 'LocalVariable' hides global declaration */ \
			__pragma (warning(disable: 6244)) /* local declaration of <variable> hides previous declaration at <line> of <file> */

#define PRAGMA_ENABLE_SHADOW_VARIABLE_WARNINGS \
			__pragma(warning(pop))



#define PRAGMA_DISABLE_UNSAFE_TYPECAST_WARNINGS \
			__pragma (warning(push)) \
			__pragma (warning(disable: 4244)) /* 'argument': conversion from 'type1' to 'type2', possible loss of data */

#define PRAGMA_ENABLE_UNSAFE_TYPECAST_WARNINGS \
			__pragma(warning(pop))



#define PRAGMA_DISABLE_UNDEFINED_IDENTIFIER_WARNINGS \
			__pragma(warning(push)) \
			__pragma(warning(disable: 4668)) /* 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives' */

#define PRAGMA_ENABLE_UNDEFINED_IDENTIFIER_WARNINGS \
			__pragma(warning(pop))



#define PRAGMA_DISABLE_MISSING_VIRTUAL_DESTRUCTOR_WARNINGS \
			__pragma(warning(push)) \
			__pragma(warning(disable: 4265)) /* class' : class has virtual functions, but destructor is not virtual */

#define PRAGMA_ENABLE_MISSING_VIRTUAL_DESTRUCTOR_WARNINGS \
			__pragma(warning(pop))



#define PRAGMA_DISABLE_REORDER_WARNINGS \
			__pragma(warning(push)) \
			__pragma(warning(disable: 5038)) /* data member 'member1' will be initialized after data member 'member2' data member 'member' will be initialized after base class 'base_class' */

#define PRAGMA_ENABLE_REORDER_WARNINGS \
			__pragma(warning(pop))



#define PRAGMA_DISABLE_REGISTER_WARNINGS \
			__pragma(warning(push)) \
			__pragma(warning(disable: 5033)) /* 'register' is no longer a supported storage class */

#define PRAGMA_ENABLE_REGISTER_WARNINGS \
			__pragma(warning(pop))





// Disable common CA warnings around SDK includes
#define THIRD_PARTY_INCLUDES_START \
			__pragma(warning(push)) \
			__pragma(warning(disable: 4510))  /* '<class>': default constructor could not be generated. */ \
			__pragma(warning(disable: 4610))  /* object '<class>' can never be instantiated - user-defined constructor required. */ \
			__pragma(warning(disable: 4800))  /* Implicit conversion from '<type>' to bool. Possible information loss. */ \
			__pragma(warning(disable: 4946))  /* reinterpret_cast used between related classes: '<class1>' and '<class1>' */ \
			__pragma(warning(disable: 4996))  /* '<obj>' was declared deprecated. */ \
			__pragma(warning(disable: 6011))  /* Dereferencing NULL pointer '<ptr>'. */ \
			__pragma(warning(disable: 6101))  /* Returning uninitialized memory '<expr>'.  A successful path through the function does not set the named _Out_ parameter. */ \
			__pragma(warning(disable: 6287))  /* Redundant code:  the left and right sub-expressions are identical. */ \
			__pragma(warning(disable: 6308))  /* 'realloc' might return null pointer: assigning null pointer to 'X', which is passed as an argument to 'realloc', will cause the original memory block to be leaked. */ \
			__pragma(warning(disable: 6326))  /* Potential comparison of a constant with another constant. */ \
			__pragma(warning(disable: 6340))  /* Mismatch on sign: Incorrect type passed as parameter in call to function. */ \
			__pragma(warning(disable: 6385))  /* Reading invalid data from '<ptr>':  the readable size is '<num1>' bytes, but '<num2>' bytes may be read. */ \
			__pragma(warning(disable: 6386))  /* Buffer overrun while writing to '<ptr>':  the writable size is '<num1>' bytes, but '<num2>' bytes might be written. */ \
			__pragma(warning(disable: 28182)) /* Dereferencing NULL pointer. '<ptr1>' contains the same NULL value as '<ptr2>' did. */ \
			__pragma(warning(disable: 28251)) /* Inconsistent annotation for '<func>': this instance has no annotations. */ \
			__pragma(warning(disable: 28252)) /* Inconsistent annotation for '<func>': return/function has '<annotation>' on the prior instance. */ \
			__pragma(warning(disable: 28253)) /* Inconsistent annotation for '<func>': _Param_(<num>) has '<annotation>' on the prior instance. */ \
			__pragma(warning(disable: 28301)) /* No annotations for first declaration of '<func>'. */ \
			PRAGMA_DISABLE_REORDER_WARNINGS \
			PRAGMA_DISABLE_UNDEFINED_IDENTIFIER_WARNINGS \
			PRAGMA_DISABLE_SHADOW_VARIABLE_WARNINGS \
			PRAGMA_DISABLE_MISSING_VIRTUAL_DESTRUCTOR_WARNINGS \
			PRAGMA_DISABLE_DEPRECATION_WARNINGS


#define THIRD_PARTY_INCLUDES_END \
			PRAGMA_ENABLE_DEPRECATION_WARNINGS \
			PRAGMA_ENABLE_MISSING_VIRTUAL_DESTRUCTOR_WARNINGS \
			PRAGMA_ENABLE_SHADOW_VARIABLE_WARNINGS \
			PRAGMA_ENABLE_UNDEFINED_IDENTIFIER_WARNINGS \
			PRAGMA_ENABLE_REORDER_WARNINGS \
			__pragma(warning(pop))






#if WIN32 || WIN64
	#include "Windows/WindowsPlatformUtils.h"
#else
	#error "Undefined platform!"
#endif





#ifndef PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING
	#error PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING should be defined for current platform!
#endif





#ifndef PLATFORM_BREAK
	#define PLATFORM_BREAK()
#endif
#ifndef PLATFORM_CODE_SECTION
	// Tells the compiler to put the decorated function in a certain section (aka. segment) of the executable.
	#define PLATFORM_CODE_SECTION(Name)
#endif

#ifndef DLLEXPORT
	#define DLLEXPORT
#endif
#ifndef DLLIMPORT
	#define DLLIMPORT
#endif

#ifndef VARARGS
	#define VARARGS				/* Functions with variable arguments */
#endif
#ifndef CDECL
	#define CDECL				/* Standard C function */
#endif
#ifndef STDCALL
	#define STDCALL				/* Standard calling convention */
#endif
#ifndef FORCEINLINE
	#define FORCEINLINE			/* Force code to be inline */
#endif
#ifndef INLINE
	#define INLINE 				/* Compiler choose to inline code or not */
#endif
#ifndef NOINLINE
	#define NOINLINE			/* Force code to NOT be inline */
#endif
#ifndef RESTRICT
	#define RESTRICT			/* no alias hint */
#endif
#ifndef CONSTEXPR
	#define CONSTEXPR 
#endif
#ifndef ABSTRACT
	#define ABSTRACT
#endif

#ifndef LINE_TERMINATOR
	#define LINE_TERMINATOR TEXT("\n")
#endif
#ifndef LINE_TERMINATOR_ANSI
	#define LINE_TERMINATOR_ANSI "\n"
#endif

#ifndef MS_ALIGN
	#define MS_ALIGN(n)
#endif
#ifndef MSVC_PRAGMA
	#define MSVC_PRAGMA(Pragma)
#endif

