
#pragma once

#include "GenericPlatform.h"

#include "TypeTraits/EnableIf.h"
#include "TypeTraits/TypeTraits.h"




/*
	This exists to avoid a Visual Studio bug where using a cast to forward an rvalue reference array argument
	to a pointer parameter will cause bad code generation.  
	Wrapping the cast in a function causes the correct code to be generated.
*/
template<typename T, typename ArgType>
inline T StaticCast(ArgType&& Arg)
{
	return static_cast<T>(Arg);
}



/*
	Reverses the order of the bits of a value.
	This is an TEnableIf'd template to ensure that no undesirable conversions occur.  Overloads for other types can be added in the same way.
 
	@param Bits - The value to bit-swap.
	@return The bit-swapped value.
*/
template<typename T>
inline typename TEnableIf<TAreTypesEqual<T, uint32>::Value, T>::Type ReverseBits(T Bits)
{
	Bits = (Bits << 16) | (Bits >> 16);
	Bits = ((Bits & 0x00ff00ff) << 8) | ((Bits & 0xff00ff00) >> 8);
	Bits = ((Bits & 0x0f0f0f0f) << 4) | ((Bits & 0xf0f0f0f0) >> 4);
	Bits = ((Bits & 0x33333333) << 2) | ((Bits & 0xcccccccc) >> 2);
	Bits = ((Bits & 0x55555555) << 1) | ((Bits & 0xaaaaaaaa) >> 1);
	return Bits;
}