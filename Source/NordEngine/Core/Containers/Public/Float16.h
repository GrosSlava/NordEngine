// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "Float32.h"
#include "EngineMath.h"




/**
	16 bit float components and conversion


	IEEE float 16
	Represented by 10-bit mantissa M, 5-bit exponent E, and 1-bit sign S

	Specials:

	E=0, M=0			== 0.0
	E=0, M!=0			== Denormalized value (M / 2^10) * 2^-14
	0<E<31, M=any		== (1 + M / 2^10) * 2^(E-15)
	E=31, M=0			== Infinity
	E=31, M!=0			== NAN
*/
struct ENGINE_API FFloat16
{
public:

	FORCEINLINE FFloat16() noexcept : Encoded(0) { }
	FORCEINLINE FFloat16(const FFloat16& FP16Value) noexcept { Encoded = FP16Value.Encoded; }
	FORCEINLINE FFloat16(float FP32Value) noexcept { Set(FP32Value); }


public:

	FORCEINLINE FFloat16& operator=(float FP32Value) noexcept
	{
		Set(FP32Value);
		return *this;
	}
	FORCEINLINE FFloat16& operator=(const FFloat16& FP16Value) noexcept
	{
		Encoded = FP16Value.Encoded;
		return *this;
	}

	FORCEINLINE operator float() const noexcept { return GetFloat(); }

public:

	/**
		Convert from Fp32 to Fp16. 
	*/
	void Set(float FP32Value) noexcept;
	/**
		Convert from Fp16 to Fp32. 
	*/
	float GetFloat() const noexcept;

	/**
		Convert from Fp32 to Fp16 without doing any checks if
		the Fp32 exponent is too large or too small. This is a
		faster alternative to Set() when you know the values
		within the single precision float don't need the checks.
	 
		@param FP32Value - Single precision float to be set as half precision.
	*/
	FORCEINLINE void SetWithoutBoundsChecks(const float FP32Value) noexcept
	{
		const FFloat32 FP32(FP32Value);

		// Make absolutely sure that you never pass in a single precision floating
		// point value that may actually need the checks. If you are not 100% sure
		// of that just use Set().

		Components.Sign = FP32.Components.Sign;
		Components.Exponent = uint16(int32(FP32.Components.Exponent) - 127 + 15);
		Components.Mantissa = uint16(FP32.Components.Mantissa >> 13);
	}



public:

	union
	{
		struct
		{
			uint16 Sign : 1;
			uint16 Exponent : 5;
			uint16 Mantissa : 10;

		} Components;

		uint16 Encoded;
	};
};