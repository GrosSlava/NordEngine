// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"




/**
	32 bit float components.
*/
struct ENGINE_API FFloat32
{
public:

	/**
		@param InValue - Value of the float.
	*/
	FORCEINLINE FFloat32(float InValue = 0.0f) noexcept : FloatValue(InValue) { }



public:

	union
	{
		struct
		{
			uint32 Sign : 1;
			uint32 Exponent : 8;
			uint32 Mantissa : 23;
		} Components;

		float FloatValue = 0.0f;
	};
};


FORCEINLINE uint64 GetTypeHash(FFloat32 Value) noexcept
{
	return *(uint32*)&(Value.FloatValue);
}