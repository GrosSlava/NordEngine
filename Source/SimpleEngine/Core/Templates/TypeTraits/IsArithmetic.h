
#pragma once

#include "GenericPlatform.h"




/*
	Traits class which tests if a type is arithmetic.
*/
template <typename T>
struct TIsArithmetic
{
	enum { Value = false };
};

template<> struct TIsArithmetic<float>			{ enum { Value = true }; };
template<> struct TIsArithmetic<double>			{ enum { Value = true }; };
template<> struct TIsArithmetic<long double>	{ enum { Value = true }; };
template<> struct TIsArithmetic<uint8>			{ enum { Value = true }; };
template<> struct TIsArithmetic<uint16>			{ enum { Value = true }; };
template<> struct TIsArithmetic<uint32>			{ enum { Value = true }; };
template<> struct TIsArithmetic<uint64>			{ enum { Value = true }; };
template<> struct TIsArithmetic<int8>			{ enum { Value = true }; };
template<> struct TIsArithmetic<int16>			{ enum { Value = true }; };
template<> struct TIsArithmetic<int32>			{ enum { Value = true }; };
template<> struct TIsArithmetic<int64>			{ enum { Value = true }; };
template<> struct TIsArithmetic<long>			{ enum { Value = true }; };
template<> struct TIsArithmetic<unsigned long>	{ enum { Value = true }; };
template<> struct TIsArithmetic<bool>			{ enum { Value = true }; };
template<> struct TIsArithmetic<WIDECHAR>		{ enum { Value = true }; };
template<> struct TIsArithmetic<ANSICHAR>		{ enum { Value = true }; };

template<typename T> struct TIsArithmetic<const T>			{ enum { Value = TIsArithmetic<T>::Value }; };
template<typename T> struct TIsArithmetic<volatile T>		{ enum { Value = TIsArithmetic<T>::Value }; };
template<typename T> struct TIsArithmetic<const volatile T> { enum { Value = TIsArithmetic<T>::Value }; };



/*
	Traits class which tests if a type is floating point.
*/
template <typename T>
struct TIsFloatingPoint
{
	enum { Value = false };
};

template<> struct TIsFloatingPoint<float>			{ enum { Value = true }; };
template<> struct TIsFloatingPoint<double>			{ enum { Value = true }; };
template<> struct TIsFloatingPoint<long double>		{ enum { Value = true }; };

template<typename T> struct TIsFloatingPoint<const T>			{ enum { Value = TIsFloatingPoint<T>::Value }; };
template<typename T> struct TIsFloatingPoint<volatile T>		{ enum { Value = TIsFloatingPoint<T>::Value }; };
template<typename T> struct TIsFloatingPoint<const volatile T>	{ enum { Value = TIsFloatingPoint<T>::Value }; };





/*
	Traits class which tests if a type is integral.
*/
template <typename T>
struct TIsIntegral
{
	enum { Value = false };
};

template<> struct TIsIntegral<bool>					{ enum { Value = true }; };
template<> struct TIsIntegral<char>					{ enum { Value = true }; };
template<> struct TIsIntegral<signed char>			{ enum { Value = true }; };
template<> struct TIsIntegral<unsigned char>		{ enum { Value = true }; };
template<> struct TIsIntegral<char16_t>				{ enum { Value = true }; };
template<> struct TIsIntegral<char32_t>				{ enum { Value = true }; };
template<> struct TIsIntegral<wchar_t>				{ enum { Value = true }; };
template<> struct TIsIntegral<short>				{ enum { Value = true }; };
template<> struct TIsIntegral<unsigned short>		{ enum { Value = true }; };
template<> struct TIsIntegral<int>					{ enum { Value = true }; };
template<> struct TIsIntegral<unsigned int>			{ enum { Value = true }; };
template<> struct TIsIntegral<long>					{ enum { Value = true }; };
template<> struct TIsIntegral<unsigned long>		{ enum { Value = true }; };
template<> struct TIsIntegral<long long>			{ enum { Value = true }; };
template<> struct TIsIntegral<unsigned long long>	{ enum { Value = true }; };

template<typename T> struct TIsIntegral<const T>			{ enum { Value = TIsIntegral<T>::Value }; };
template<typename T> struct TIsIntegral< volatile T>		{ enum { Value = TIsIntegral<T>::Value }; };
template<typename T> struct TIsIntegral<const volatile T>	{ enum { Value = TIsIntegral<T>::Value }; };
