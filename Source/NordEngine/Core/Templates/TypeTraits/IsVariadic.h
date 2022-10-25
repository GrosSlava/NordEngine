
#pragma once

#include <initializer_list>
#include "TypeTraits/IsEnum.h"




/*
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

template<typename T> struct TIsInitializerList<const          T> { enum { Value = TIsInitializerList<T>::Value }; };
template<typename T> struct TIsInitializerList<      volatile T> { enum { Value = TIsInitializerList<T>::Value }; };
template<typename T> struct TIsInitializerList<const volatile T> { enum { Value = TIsInitializerList<T>::Value }; };





/*
	Determines if T is constructible from a set of arguments.
*/
template<typename T, typename... Args>
struct TIsConstructible
{
	enum { Value = __is_constructible(T, Args...) };
};




/*
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
	static uint8  Tester(...);

	static T DeclValT();

public:

	enum { Value = sizeof(Tester(DeclValT())) == sizeof(uint32) };
};

template<typename T>
struct TIsValidVariadicFunctionArg<T, true>
{
	enum { Value = true };
};





