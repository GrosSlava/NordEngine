// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include <cassert>




/*....................Static assertion...................................*/

#define StaticCheck(assertion, msg) static_assert(assertion, msg);

#define StaticCheckTrue(assertion, msg) StaticCheck(!!(assertion), msg)
#define StaticCheckEqual(A, B, msg) StaticCheck((A) == (B), msg)
#define StaticCheckNotEqual(A, B, msg) StaticCheck((A) != (B), msg)

#define StaticCheckFloatEqual(A, B, msg) StaticCheck(((A) >= (B)) && ((B) >= (A)), msg)
#define StaticCheckFloatNotEqual(A, B, msg) StaticCheck(((A) < (B)) || ((A) > (B)), msg)

#define StaticCheckSameSize(A, B, msg) StaticCheck(sizeof(typeof(A)) == sizeof(typeof(B)), msg)
#define StaticCheckAlign(A, align, msg) StaticCheck(sizeof(A) % (align) == 0, msg)
#define StaticCheckSize(A, size, msg) StaticCheck(sizeof(A) == (size), msg)

/*.......................................................................*/

/*....................Runtime assertion..................................*/

#define Assert(Expr) assert(Expr)

#define AssertTrue(Expr) Assert(Expr)
#define AssertEqual(A, B) Assert((A) == (B))
#define AssertNotEqual(A, B) Assert((A) != (B))

#define AssertFloatEqual(A, B) Assert(((A) >= (B)) && ((B) >= (A)))
#define AssertFloatNotEqual(A, B) Assert(((A) < (B)) || ((A) > (B)))

/*.......................................................................*/



#if DO_CHECK

#define checkCode(Code) \
	do                  \
	{                   \
		Code;           \
	} while( false );

/** 
	Halts execution if Expression is false. 
*/
#define CHECK_IMPL(expr)                          \
	{                                             \
		if( UNLIKELY(!(expr)) )                   \
		{                                         \
			/* TODO DEBUG LOGG and stack trace */ \
			PLATFORM_BREAK();                     \
		}                                         \
	}
#define verify(expr) CHECK_IMPL(expr)
#define check(expr) CHECK_IMPL(expr)

/** 
	Halts execution if Expression is false.
*/
#define CHECK_F_IMPL(expr, format, ...)           \
	{                                             \
		if( UNLIKELY(!(expr)) )                   \
		{                                         \
			/* TODO DEBUG LOGG and stack trace */ \
			PLATFORM_BREAK();                     \
		}                                         \
	}
#define verifyf(expr, format, ...) CHECK_F_IMPL(expr, format, ##__VA_ARGS__)
#define checkf(expr, format, ...) CHECK_F_IMPL(expr, format, ##__VA_ARGS__)

struct FRecursionScopeMarker
{
public:

	FRecursionScopeMarker(uint16& InCounter) : Counter(InCounter) { ++Counter; }
	~FRecursionScopeMarker() { --Counter; }

private:

	uint16& Counter;
};

/**
	Denotes code paths that should never be reached.
*/
#define checkNoEntry() check(!"Enclosing block should never be called")
/**
	Denotes code paths that should not be executed more than once.
*/
#define checkNoReentry()                                                              \
	{                                                                                 \
		static bool s_beenHere##__LINE__ = false;                                     \
		check(!"Enclosing block was called more than once" || !s_beenHere##__LINE__); \
		s_beenHere##__LINE__ = true;                                                  \
	}
/**
	Denotes code paths that should never be called recursively.
*/
#define checkNoRecursion()                                                                \
	static uint16 RecursionCounter##__LINE__ = 0;                                         \
	check(!"Enclosing block was entered recursively" || RecursionCounter##__LINE__ == 0); \
	const FRecursionScopeMarker ScopeMarker##__LINE__(RecursionCounter##__LINE__);
/**
	Similar to checkNoEntry.
*/
#define unimplemented() check(!"Unimplemented function called")

#else //DO_CHECK

#define checkCode(...)
#define check(expr)
#define checkf(expr, format, ...)
#define checkNoEntry()
#define checkNoReentry()
#define checkNoRecursion()
#define verify(expr)
#define verifyf(expr, format, ...)
#define unimplemented()

#endif //DO_CHECK
