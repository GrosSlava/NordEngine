// Copyright Nord Engine. All Rights Reserved.
#include "Float16.h"





void FFloat16::Set(float FP32Value) noexcept
{
	FFloat32 FP32(FP32Value);

	// Copy sign-bit
	Components.Sign = FP32.Components.Sign;

	// Check for zero, denormal or too small value.
	if( FP32.Components.Exponent <= 112 ) // Too small exponent? (0+127-15)
	{
		// Set to 0.
		Components.Exponent = 0;
		Components.Mantissa = 0;

		// Exponent unbias the single, then bias the halfp
		const int32 NewExp = FP32.Components.Exponent - 127 + 15;

		if( (14 - NewExp) <= 24 ) // Mantissa might be non-zero
		{
			uint32 Mantissa = FP32.Components.Mantissa | 0x800000; // Hidden 1 bit
			Components.Mantissa = (uint16)(Mantissa >> (14 - NewExp));
			// Check for rounding
			if( (Mantissa >> (13 - NewExp)) & 1 )
			{
				Encoded++; // Round, might overflow into exp bit, but this is OK
			}
		}
	}
	// Check for INF or NaN, or too high value
	else if( FP32.Components.Exponent >= 143 ) // Too large exponent? (31+127-15)
	{
		// Set to 65504.0 (max value)
		Components.Exponent = 30;
		Components.Mantissa = 1023;
	}
	// Handle normal number.
	else
	{
		Components.Exponent = uint16(int32(FP32.Components.Exponent) - 127 + 15);
		Components.Mantissa = uint16(FP32.Components.Mantissa >> 13);
	}
}

float FFloat16::GetFloat() const noexcept
{
	FFloat32 Result;

	Result.Components.Sign = Components.Sign;
	if( Components.Exponent == 0 )
	{
		uint32 Mantissa = Components.Mantissa;
		if( Mantissa == 0 )
		{
			// Zero.
			Result.Components.Exponent = 0;
			Result.Components.Mantissa = 0;
		}
		else
		{
			// Denormal.
			uint32 MantissaShift = 10 - (uint32)FMath::TruncToInt(FMath::Log2((float)Mantissa));
			Result.Components.Exponent = 127 - (15 - 1) - MantissaShift;
			Result.Components.Mantissa = Mantissa << (MantissaShift + 23 - 10);
		}
	}
	else if( Components.Exponent == 31 ) // 2^5 - 1
	{
		// Infinity or NaN. Set to 65504.0
		Result.Components.Exponent = 142;
		Result.Components.Mantissa = 8380416;
	}
	else
	{
		// Normal number.
		Result.Components.Exponent = int32(Components.Exponent) - 15 + 127; // Stored exponents are biased by half their range.
		Result.Components.Mantissa = uint32(Components.Mantissa) << 13;
	}

	return Result.FloatValue;
}
