// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#include "AreTypesEqual.h"
#include "TypeTraits/TypeSpecifier.h"
#include "RemoveCV.h"
#include "TypeTraits/IsTriviallyCopyConstructible.h"




// clang-format off
/**
	Tests if a type T is bitwise-constructible from a given argument type U.  
	That is, whether or not	the U can be memcpy'd in order to produce an instance of T, rather than having to go via a constructor.
 
	Examples:
	TIsBitwiseConstructible<PODType,    PODType   >::Value == true  // PODs can be trivially copied
	TIsBitwiseConstructible<const int*, int*      >::Value == true  // a non-const Derived pointer is trivially copyable as a const Base pointer
	TIsBitwiseConstructible<int*,       const int*>::Value == false // not legal the other way because it would be a const-correctness violation
	TIsBitwiseConstructible<int32,      uint32    >::Value == true  // signed integers can be memcpy'd as unsigned integers
	TIsBitwiseConstructible<uint32,     int32     >::Value == true  // and vice versa
*/
template<typename T, typename Arg>
struct TIsBitwiseConstructible
{
	static_assert(
		!TIsReferenceType<T  >::Value &&
		!TIsReferenceType<Arg>::Value,
		"TIsBitwiseConstructible is not designed to accept reference types");

	static_assert(
		TAreTypesEqual<T,   typename TRemoveCV<T  >::Type>::Value &&
		TAreTypesEqual<Arg, typename TRemoveCV<Arg>::Type>::Value,
		"TIsBitwiseConstructible is not designed to accept qualified types");

	// Assume no bitwise construction in general
	enum { Value = false };
};

template<typename T>
struct TIsBitwiseConstructible<T, T>
{
	// Ts can always be bitwise constructed from itself if it is trivially copyable.
	enum { Value = TIsTriviallyCopyConstructible<T>::Value };
};

template<typename T, typename U>
struct TIsBitwiseConstructible<const T, U> : TIsBitwiseConstructible<T, U>
{
	// Constructing a const T is the same as constructing a T
};
// clang-format on


/**
	Const pointers can be bitwise constructed from non-const pointers.
	This is not true for pointer conversions in general, e.g. where an offset may need to be applied in the case
	of multiple inheritance, but there is no way of detecting that at compile-time.
*/
template<typename T>
struct TIsBitwiseConstructible<const T*, T*>
{
	// Constructing a const T is the same as constructing a T
	enum
	{
		Value = true
	};
};

// clang-format off
/** 
	Unsigned types can be bitwise converted to their signed equivalents, and vice versa.
	(assuming two's-complement, which we are)
*/
template<> struct TIsBitwiseConstructible<uint8,   int8>  { enum { Value = true }; };
template<> struct TIsBitwiseConstructible< int8,  uint8>  { enum { Value = true }; };
template<> struct TIsBitwiseConstructible<uint16,  int16> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible< int16, uint16> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible<uint32,  int32> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible< int32, uint32> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible<uint64,  int64> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible< int64, uint64> { enum { Value = true }; };
// clang-format on
