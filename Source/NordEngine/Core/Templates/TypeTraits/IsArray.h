// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"





/**
	Traits class which tests if a type is a C++ array.
*/
template<typename T>
struct TIsArray
{
	enum
	{
		Value = false
	};
};
// clang-format off
template<typename T>           struct TIsArray<T[]>		{ enum { Value = true }; };
template<typename T, uint32 N> struct TIsArray<T[N]>	{ enum { Value = true }; };
// clang-format on

// clang-format off
/**
	Traits class which tests if a type is a bounded C++ array.
*/
template<typename T>           struct TIsBoundedArray			{ enum { Value = false }; };
template<typename T, uint32 N> struct TIsBoundedArray<T[N]>		{ enum { Value = true }; };
// clang-format on

// clang-format off
/**
	Traits class which tests if a type is an unbounded C++ array.
*/
template<typename T> struct TIsUnboundedArray		{ enum { Value = false }; };
template<typename T> struct TIsUnboundedArray<T[]>	{ enum { Value = true }; };
// clang-format on

/**
	Type trait which returns true if the type T is an array or a reference to an array of ArrType.
*/
template<typename T, typename ArrType>
struct TIsArrayOrRefOfType
{
	enum
	{
		Value = false
	};
};
// clang-format off
template<typename ArrType> struct TIsArrayOrRefOfType<               ArrType[], ArrType> { enum { Value = true }; };
template<typename ArrType> struct TIsArrayOrRefOfType<const          ArrType[], ArrType> { enum { Value = true }; };
template<typename ArrType> struct TIsArrayOrRefOfType<      volatile ArrType[], ArrType> { enum { Value = true }; };
template<typename ArrType> struct TIsArrayOrRefOfType<const volatile ArrType[], ArrType> { enum { Value = true }; };

template<typename ArrType, unsigned int N> struct TIsArrayOrRefOfType<               ArrType[N], ArrType> { enum { Value = true }; };
template<typename ArrType, unsigned int N> struct TIsArrayOrRefOfType<const          ArrType[N], ArrType> { enum { Value = true }; };
template<typename ArrType, unsigned int N> struct TIsArrayOrRefOfType<      volatile ArrType[N], ArrType> { enum { Value = true }; };
template<typename ArrType, unsigned int N> struct TIsArrayOrRefOfType<const volatile ArrType[N], ArrType> { enum { Value = true }; };

template<typename ArrType, unsigned int N> struct TIsArrayOrRefOfType<               ArrType(&)[N], ArrType> { enum { Value = true }; };
template<typename ArrType, unsigned int N> struct TIsArrayOrRefOfType<const          ArrType(&)[N], ArrType> { enum { Value = true }; };
template<typename ArrType, unsigned int N> struct TIsArrayOrRefOfType<      volatile ArrType(&)[N], ArrType> { enum { Value = true }; };
template<typename ArrType, unsigned int N> struct TIsArrayOrRefOfType<const volatile ArrType(&)[N], ArrType> { enum { Value = true }; };
// clang-format on
