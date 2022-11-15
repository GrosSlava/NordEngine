
#pragma once

#include "GenericPlatform.h"

#include "TypeTraits/IsVoidType.h"
#include "TypeTraits/IsEnum.h"
#include "TypeTraits/IsArithmetic.h"
#include "TypeTraits/AndOrNot.h"
#include "TypeTraits/TypeSpecifier.h"
#include "TypeTraits/CallTraits.h"
#include "TypeTraits/IsTriviallyCopyConstructible.h"
#include "RemoveCV.h"




/* 
	Is type DerivedType inherited from BaseType.
*/
template<typename DerivedType, typename BaseType>
struct TIsDerivedFrom
{
	// Different size types so we can compare their sizes later.
	typedef char No[1];
	typedef char Yes[2];

	// Overloading Test() s.t. only calling it with something that is
	// a BaseType (or inherited from the BaseType) will return a Yes.
	static Yes& Test(BaseType*);
	static Yes& Test(const BaseType*);
	static No& Test(...);

	// Makes a DerivedType ptr.
	static DerivedType* DerivedTypePtr() { return nullptr; }

public:

	// Test the derived type pointer. If it inherits from BaseType, the Test( BaseType* ) 
	// will be chosen. If it does not, Test( ... ) will be chosen.
	static const bool Value = sizeof(Test(DerivedTypePtr())) == sizeof(Yes);

	static const bool IsDerived = Value;
};



/*
	Tests whether two typenames refer to the same type.
*/
template<typename A, typename B>
struct TAreTypesEqual;

template<typename, typename>
struct TAreTypesEqual
{
	enum { Value = false };
};

template<typename A>
struct TAreTypesEqual<A, A>
{
	enum { Value = true };
};



/*
	TIsSame
 
	Implementation of std::is_same trait.
*/
template<typename A, typename B>	struct TIsSame			{ enum { Value = false }; };
template<typename T>				struct TIsSame<T, T>	{ enum { Value = true }; };

/*
	Gets the Nth type in a template parameter pack. N must be less than sizeof...(Types). 
*/
template<int32 N, typename... Types>
struct TNthTypeFromParameterPack;

template<int32 N, typename T, typename... OtherTypes>
struct TNthTypeFromParameterPack<N, T, OtherTypes...>
{
	using Type = typename TNthTypeFromParameterPack<N - 1, OtherTypes...>::Type;
};

template<typename T, typename... OtherTypes>
struct TNthTypeFromParameterPack<0, T, OtherTypes...>
{
	using Type = T;
};



/*
	TIsFundamentalType
*/
template<typename T>
struct TIsFundamentalType
{
	enum { Value = TOr<TIsArithmetic<T>, TIsVoidType<T>>::Value };
};


/*
	TIsZeroConstructType
*/
template<typename T>
struct TIsZeroConstructType
{
	enum { Value = TOr<TIsEnum<T>, TIsArithmetic<T>, TIsPointer<T>>::Value };
};






/*
	TFormatSpecifier, only applies to numeric types.
*/
template<typename T>
struct TFormatSpecifier
{
	FORCEINLINE static TCHAR const* GetFormatSpecifier()
	{
		// Force the template instantiation to be dependent upon T so the compiler cannot automatically decide that this template can never be instantiated.
		// If the static_assert below were a constant 0 or something not dependent on T, compilers are free to detect this and fail to compile the template.
		// As specified in the C++ standard s14.6p8. A compiler is free to give a diagnostic here or not. MSVC ignores it, and clang/gcc instantiates the 
		// template and triggers the static_assert.
		static_assert(sizeof(T) < 0, "Format specifier not supported for this type."); // request for a format specifier for a type we do not know about
		return TEXT("Unknown");
	}
};
#define Expose_TFormatSpecifier(type, format) \
template<> \
struct TFormatSpecifier<type> \
{  \
	FORCEINLINE static const TCHAR (&GetFormatSpecifier())[5] \
	{ \
		static const TCHAR Spec[5] = TEXT(format); \
		return Spec; \
	} \
};

Expose_TFormatSpecifier(bool, "%i")
Expose_TFormatSpecifier(uint8, "%u")
Expose_TFormatSpecifier(uint16, "%u")
Expose_TFormatSpecifier(uint32, "%u")
Expose_TFormatSpecifier(uint64, "%llu")
Expose_TFormatSpecifier(int8, "%d")
Expose_TFormatSpecifier(int16, "%d")
Expose_TFormatSpecifier(int32, "%d")
Expose_TFormatSpecifier(int64, "%lld")
Expose_TFormatSpecifier(float, "%f")
Expose_TFormatSpecifier(double, "%f")
Expose_TFormatSpecifier(long double, "%f")
Expose_TFormatSpecifier(long, "%ld")
Expose_TFormatSpecifier(unsigned long, "%lu")



/*
	TNameOf
*/
template<typename T>
struct TNameOf
{
	FORCEINLINE static TCHAR const* GetName()
	{
		return TEXT("Unknown");
	}
};

#define Expose_TNameOf(type) \
template<> \
struct TNameOf<type> \
{  \
	FORCEINLINE static TCHAR const* GetName() \
	{ \
		return TEXT(#type); \
	} \
};

Expose_TNameOf(uint8)
Expose_TNameOf(uint16)
Expose_TNameOf(uint32)
Expose_TNameOf(uint64)
Expose_TNameOf(int8)
Expose_TNameOf(int16)
Expose_TNameOf(int32)
Expose_TNameOf(int64)
Expose_TNameOf(float)
Expose_TNameOf(double)




/*
	Helper for array traits. Provides a common base to more easily refine a portion of the traits
	when specializing. Mainly used by MemoryOps.h.
*/
template<typename T>
struct TTypeTraitsBase
{
	typedef typename TCallTraits<T>::ParamType ConstInitType;
	typedef typename TCallTraits<T>::ConstPointerType ConstPointerType;

	// There's no good way of detecting this so we'll just assume it to be true for certain known types and expect
	// users to customize it for their custom types.
	enum
	{
		IsBytewiseComparable = TOr<TIsEnum<T>, TIsArithmetic<T>, TIsPointer < T >> ::Value
	};
};

/*
	Traits for types.
*/
template<typename T>
struct TTypeTraits : public TTypeTraitsBase<T>
{
};




/*
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

/*
	Const pointers can be bitwise constructed from non-const pointers.
	This is not true for pointer conversions in general, e.g. where an offset may need to be applied in the case
	of multiple inheritance, but there is no way of detecting that at compile-time.
*/
template<typename T>
struct TIsBitwiseConstructible<const T*, T*>
{
	// Constructing a const T is the same as constructing a T
	enum { Value = true };
};

// Unsigned types can be bitwise converted to their signed equivalents, and vice versa.
// (assuming two's-complement, which we are)
template<> struct TIsBitwiseConstructible<uint8,   int8>  { enum { Value = true }; };
template<> struct TIsBitwiseConstructible< int8,  uint8>  { enum { Value = true }; };
template<> struct TIsBitwiseConstructible<uint16,  int16> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible< int16, uint16> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible<uint32,  int32> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible< int32, uint32> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible<uint64,  int64> { enum { Value = true }; };
template<> struct TIsBitwiseConstructible< int64, uint64> { enum { Value = true }; };
