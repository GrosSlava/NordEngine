// Copyright Nord Engine. All Rights Reserved.
#pragma once


#include "GenericPlatform.h"
#include "TypeTraits/IsEnum.h"
#include "InitializerList.h"
#include <initializer_list>




/**
	Traits class which tests if a type is an initializer list.
*/
template<typename T>
struct TIsInitializerList
{
	static constexpr bool Value = false;
};
template<typename T>
struct TIsInitializerList<std::initializer_list<T>>
{
	static constexpr bool Value = true;
};
template<typename T>
struct TIsInitializerList<TInitializerList<T>>
{
	static constexpr bool Value = true;
};
// clang-format off
template<typename T> struct TIsInitializerList<const          T> { enum { Value = TIsInitializerList<T>::Value }; };
template<typename T> struct TIsInitializerList<      volatile T> { enum { Value = TIsInitializerList<T>::Value }; };
template<typename T> struct TIsInitializerList<const volatile T> { enum { Value = TIsInitializerList<T>::Value }; };
// clang-format on



/**
	Tests if a type is a valid argument to a variadic function, e.g. printf.
*/
template<typename T, bool = TIsEnum<T>::Value>
struct TIsValidVariadicFunctionArg
{
private:

	static uint32 Tester(uint32);
	static uint32 Tester(uint8);
	static uint32 Tester(int32);
	static uint32 Tester(uint64);
	static uint32 Tester(int64);
	static uint32 Tester(double);
	static uint32 Tester(long);
	static uint32 Tester(unsigned long);
	static uint32 Tester(TCHAR);
	static uint32 Tester(bool);
	static uint32 Tester(const void*);
	static uint8 Tester(...);

	static T DeclValT();

public:

	enum
	{
		Value = sizeof(Tester(DeclValT())) == sizeof(uint32)
	};
};

template<typename T>
struct TIsValidVariadicFunctionArg<T, true>
{
	enum
	{
		Value = true
	};
};
