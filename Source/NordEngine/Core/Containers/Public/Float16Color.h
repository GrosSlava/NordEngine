// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "Float16.h"




struct FLinearColor;


/**
	RGBA Color made up of FFloat16.
*/
struct ENGINE_API FFloat16Color
{
public:

	FFloat16Color() = default;
	FORCEINLINE FFloat16Color(const FFloat16Color& Src) noexcept
	{
		R = Src.R;
		G = Src.G;
		B = Src.B;
		A = Src.A;
	}
	FFloat16Color(const FLinearColor& Src) noexcept;


public:

	FORCEINLINE FFloat16Color& operator=(const FFloat16Color& Src)
	{
		R = Src.R;
		G = Src.G;
		B = Src.B;
		A = Src.A;

		return *this;
	}

public:

	/**
		Checks whether two colors are identical.
	 
		@param Src - The other color.
		@return true if the two colors are identical, otherwise false.
	*/
	FORCEINLINE bool operator==(const FFloat16Color& Src) const { return ((R == Src.R) && (G == Src.G) && (B == Src.B) && (A == Src.A)); }



public:

	FFloat16 R;
	FFloat16 G;
	FFloat16 B;
	FFloat16 A;
};
