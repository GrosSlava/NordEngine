// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"




/**
	Traits class which tests if a type is a signed integral type.
*/
template<typename T>
struct TIsSigned
{
	enum
	{
		Value = false
	};
};
// clang-format off
template<> struct TIsSigned<int8>	{ enum { Value = true }; };
template<> struct TIsSigned<int16>	{ enum { Value = true }; };
template<> struct TIsSigned<int32>	{ enum { Value = true }; };
template<> struct TIsSigned<int64>	{ enum { Value = true }; };

template<typename T> struct TIsSigned<const T>				{ enum { Value = TIsSigned<T>::Value }; };
template<typename T> struct TIsSigned<volatile T>			{ enum { Value = TIsSigned<T>::Value }; };
template<typename T> struct TIsSigned<const volatile T>		{ enum { Value = TIsSigned<T>::Value }; };
// clang-format on



/**
	Type trait which yields a signed integer type of a given number of bytes.
	If there is no such type, the Type member type will be absent, allowing it to be used in SFINAE contexts.
*/
template<int NumBytes>
struct TSignedIntType
{
};
// clang-format off
template<> struct TSignedIntType<1> { using Type = int8; };
template<> struct TSignedIntType<2> { using Type = int16; };
template<> struct TSignedIntType<4> { using Type = int32; };
template<> struct TSignedIntType<8> { using Type = int64; };
// clang-format on

/**
	Helper for TSignedIntType which expands out to the nested Type.
*/
template<int NumBytes>
using TSignedIntType_T = typename TSignedIntType<NumBytes>::Type;


/**
	Type trait which yields an unsigned integer type of a given number of bytes.
	If there is no such type, the Type member type will be absent, allowing it to be used in SFINAE contexts.
*/
template<int NumBytes>
struct TUnsignedIntType
{
};
// clang-format off
template<> struct TUnsignedIntType<1> { using Type = uint8; };
template<> struct TUnsignedIntType<2> { using Type = uint16; };
template<> struct TUnsignedIntType<4> { using Type = uint32; };
template<> struct TUnsignedIntType<8> { using Type = uint64; };
// clang-format on

/**
	Helper for TUnsignedIntType which expands out to the nested Type.
*/
template<int NumBytes>
using TUnsignedIntType_T = typename TUnsignedIntType<NumBytes>::Type;
