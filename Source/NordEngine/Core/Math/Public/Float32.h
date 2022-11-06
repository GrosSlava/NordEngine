#pragma once

#include "GenericPlatform.h"




/*
	32 bit float components.
*/
class FFloat32
{
public:

	union
	{
		struct
		{
			uint32 Sign : 1;
			uint32 Exponent : 8;
			uint32 Mantissa : 23;
		} Components;

		float FloatValue;
	};


public:

	/*
		Constructor
	 
		@param InValue - value of the float.
	*/
	FORCEINLINE FFloat32(float InValue = 0.0f) noexcept : FloatValue(InValue) { }
};
