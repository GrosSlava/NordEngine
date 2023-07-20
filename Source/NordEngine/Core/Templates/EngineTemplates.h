// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#include "TypeTraits/EnableIf.h"
#include "TypeTraits/AreTypesEqual.h"





/**
	This exists to avoid a Visual Studio bug where using a cast to forward an rvalue reference array argument
	to a pointer parameter will cause bad code generation.  
	Wrapping the cast in a function causes the correct code to be generated.
*/
template<typename T, typename ArgType>
FORCEINLINE T StaticCast(ArgType&& Arg) noexcept
{
	return static_cast<T>(Arg);
}



/**
	Reverses the order of the bits of a value.
	This is an TEnableIf'd template to ensure that no undesirable conversions occur.  Overloads for other types can be added in the same way.
 
	@param Bits - The value to bit-swap.
	@return The bit-swapped value.
*/
template<typename T>
FORCEINLINE typename TEnableIf<TAreTypesEqual<T, uint32>::Value, T>::Type ReverseBits(T Bits) noexcept
{
	Bits = (Bits << 16) | (Bits >> 16);
	Bits = ((Bits & 0x00ff00ff) << 8) | ((Bits & 0xff00ff00) >> 8);
	Bits = ((Bits & 0x0f0f0f0f) << 4) | ((Bits & 0xf0f0f0f0) >> 4);
	Bits = ((Bits & 0x33333333) << 2) | ((Bits & 0xcccccccc) >> 2);
	Bits = ((Bits & 0x55555555) << 1) | ((Bits & 0xaaaaaaaa) >> 1);
	return Bits;
}



/**
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
#define Expose_TFormatSpecifier(type, format)                     \
	template<>                                                    \
	struct TFormatSpecifier<type>                                 \
	{                                                             \
		FORCEINLINE static const TCHAR (&GetFormatSpecifier())[5] \
		{                                                         \
			static const TCHAR Spec[5] = TEXT(format);            \
			return Spec;                                          \
		}                                                         \
	}

Expose_TFormatSpecifier(bool, "%i");
Expose_TFormatSpecifier(uint8, "%u");
Expose_TFormatSpecifier(uint16, "%u");
Expose_TFormatSpecifier(uint32, "%u");
Expose_TFormatSpecifier(uint64, "%llu");
Expose_TFormatSpecifier(int8, "%d");
Expose_TFormatSpecifier(int16, "%d");
Expose_TFormatSpecifier(int32, "%d");
Expose_TFormatSpecifier(int64, "%lld");
Expose_TFormatSpecifier(float, "%f");
Expose_TFormatSpecifier(double, "%f");
Expose_TFormatSpecifier(long double, "%f");
Expose_TFormatSpecifier(long, "%ld");
Expose_TFormatSpecifier(unsigned long, "%lu");

#undef Expose_TFormatSpecifier
