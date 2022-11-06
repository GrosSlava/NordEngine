#pragma once

#include "Float16.h"


struct FLinearColor;



/**
 *	RGBA Color made up of FFloat16
 */
class FFloat16Color
{

public:

	/** Default constructor */
	FORCEINLINE FFloat16Color() noexcept{}

	/** Copy constructor. */
	FORCEINLINE FFloat16Color(const FFloat16Color& Src) noexcept
	{
		R = Src.R;
		G = Src.G;
		B = Src.B;
		A = Src.A;
	}

	/** Constructor from a linear color. */
	FFloat16Color(const FLinearColor& Src) noexcept;
		

public:

	/** assignment operator */
	FORCEINLINE FFloat16Color& operator=(const FFloat16Color& Src)
	{
		R = Src.R;
		G = Src.G;
		B = Src.B;
		A = Src.A;

		return *this;
	}

	/**
	 * Checks whether two colors are identical.
	 *
	 * @param Src The other color.
	 * @return true if the two colors are identical, otherwise false.
	 */
	FORCEINLINE bool operator==(const FFloat16Color& Src) const
	{
		return (
			(R == Src.R) &&
			(G == Src.G) &&
			(B == Src.B) &&
			(A == Src.A)
			);
	}



public:

	FFloat16 R;
	FFloat16 G;
	FFloat16 B;
	FFloat16 A;

};
