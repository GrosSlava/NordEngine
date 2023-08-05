// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "CommonMacros.h"
#include <cstdio>




#define TEST_INTERNAL(Expr)                         \
	{                                               \
		if( !(Expr) )                               \
		{                                           \
			printf("Fail on line: %d\n", __LINE__); \
			return PROGRAM_EXIT_FAILURE;            \
		}                                           \
	}
#define TEST_INTERNAL_MSG(Expr, msg)       \
	{                                      \
		if( !(Expr) )                      \
		{                                  \
			if( msg ) printf("%s\n", msg); \
			return PROGRAM_EXIT_FAILURE;   \
		}                                  \
	}



#define Test(Expr) TEST_INTERNAL(!!(Expr))
#define TestMsg(Expr, msg) TEST_INTERNAL_MSG(!!(Expr), msg)

#define TestTrue(Expr) Test(Expr)
#define TestTrueMsg(Expr, msg) TestMsg(Expr, msg)

#define TestEqual(A, B) Test((A) == (B))
#define TestEqualMsg(A, B, msg) TestMsg((A) == (B), msg)

#define TestNotEqual(A, B) Test((A) != (B))
#define TestNotEqualMsg(A, B, msg) TestMsg((A) != (B), msg)

#define TestFloatEqual(A, B) Test(((A) >= (B)) && ((B) >= (A)))
#define TestFloatEqualMsg(A, B, msg) TestMsg(((A) >= (B)) && ((B) >= (A)), msg)

#define TestFloatNotEqual(A, B) Test(((A) < (B)) || ((A) > (B)))
#define TestFloatNotEqualMsg(A, B, msg) TestMsg(((A) < (B)) || ((A) > (B)), msg)

#define TestSameSize(A, B) Test(sizeof(typeof(A)) == sizeof(typeof(B)), msg)
#define TestSameSizeMsg(A, B, msg) TestMsg(sizeof(typeof(A)) == sizeof(typeof(B)), msg)

#define TestAlign(A, align) Test(sizeof(A) % (align) == 0, msg)
#define TestAlignMsg(A, align, msg) TestMsg(sizeof(A) % (align) == 0, msg)

#define TestSize(A, size) Test(sizeof(A) == (size), msg)
#define TestSizeMsg(A, size, msg) TestMsg(sizeof(A) == (size), msg)




using TestSimpleType = int;

struct TestComplexType
{
public:

	TestComplexType() = default;
	TestComplexType(int InA, short int InB) : a(InA), b(InB), res(a + b) { mem = new char[res]; }
	TestComplexType(const TestComplexType& Other) : a(Other.a), b(Other.b), res(Other.res)
	{
		mem = new char[res];
		for( unsigned long long int i = 0; i < res; ++i )
		{
			mem[i] = Other.mem[i];
		}
	}
	TestComplexType(TestComplexType&& Other) noexcept : a(Other.a), b(Other.b), res(Other.res), mem(Other.mem)
	{
		Other.a = 0;
		Other.b = 0;
		Other.res = 0;
		Other.mem = nullptr;
	}

public:

	TestComplexType& operator=(const TestComplexType& Other)
	{
		if( &Other == this ) return *this;

		this->a = Other.a;
		this->b = Other.b;
		this->res = Other.res;

		delete[] this->mem;
		this->mem = new char[res];
		for( unsigned long long int i = 0; i < res; ++i )
		{
			this->mem[i] = Other.mem[i];
		}

		return *this;
	}
	TestComplexType& operator=(TestComplexType&& Other) noexcept
	{
		this->a = Other.a;
		this->b = Other.b;
		this->res = Other.res;
		this->mem = Other.mem;

		Other.mem = nullptr;
		Other.res = 0;
		Other.a = 0;
		Other.b = 0;

		return *this;
	}

public:

	bool IsValid() const noexcept { return res == a + b && mem != nullptr; }
	bool IsEqual(const TestComplexType& Other) noexcept
	{
		if( &Other == this ) return true;

		if( a != Other.a || b != Other.b || res != Other.res ) return false;

		for( unsigned long long int i = 0; i < res; ++i )
		{
			if( mem[i] != Other.mem[i] ) return false;
		}

		return true;
	}



public:

	int a = 0;
	short int b = 0;
	char* mem = nullptr;
	unsigned long long int res = 0;
};