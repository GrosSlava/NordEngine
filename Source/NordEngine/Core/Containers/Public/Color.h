#pragma once

#include "EngineMath.h"
#include "TArray.h"


class FFloat16Color;

struct FColor;
struct FVector3D;
struct FVector4D;



/**
 * Enum for the different kinds of gamma spaces we expect to need to convert from/to.
 */
enum class EGammaSpace : uint8
{
	/** No gamma correction is applied to this space, the incoming colors are assumed to already be in linear space. */
	Linear,
	/** A simplified sRGB gamma correction is applied, pow(1/2.2). */
	Pow22,
	/** Use the standard sRGB conversion. */
	sRGB,
};





/**
* A linear, 32-bit/component floating point RGBA color.
*/
struct FLinearColor
{

public:

	FORCEINLINE FLinearColor() { }

	FORCEINLINE FLinearColor(float InR, float InG, float InB, float InA = 1.0f) noexcept:
		R(InR), G(InG), B(InB), A(InA)
	{

	}

	/**
	 * Converts an FColor which is assumed to be in sRGB space, into linear color space.
	 * @param Color The sRGB color that needs to be converted into linear space.
	 */
	FLinearColor(const FColor& Color) noexcept;

	FLinearColor(const FVector3D& Vector) noexcept;

	explicit FLinearColor(const FVector4D& Vector) noexcept;

	explicit FLinearColor(const FFloat16Color& C) noexcept;


public:

	FORCEINLINE FLinearColor operator+(const FLinearColor& ColorB) const noexcept
	{
		return FLinearColor(
			this->R + ColorB.R,
			this->G + ColorB.G,
			this->B + ColorB.B,
			this->A + ColorB.A
		);
	}
	FORCEINLINE FLinearColor& operator+=(const FLinearColor& ColorB) noexcept
	{
		R += ColorB.R;
		G += ColorB.G;
		B += ColorB.B;
		A += ColorB.A;
		return *this;
	}

	FORCEINLINE FLinearColor operator-(const FLinearColor& ColorB) const noexcept
	{
		return FLinearColor(
			this->R - ColorB.R,
			this->G - ColorB.G,
			this->B - ColorB.B,
			this->A - ColorB.A
		);
	}
	FORCEINLINE FLinearColor& operator-=(const FLinearColor& ColorB) noexcept
	{
		R -= ColorB.R;
		G -= ColorB.G;
		B -= ColorB.B;
		A -= ColorB.A;
		return *this;
	}

	FORCEINLINE FLinearColor operator*(const FLinearColor& ColorB) const noexcept
	{
		return FLinearColor(
			this->R * ColorB.R,
			this->G * ColorB.G,
			this->B * ColorB.B,
			this->A * ColorB.A
		);
	}
	FORCEINLINE FLinearColor& operator*=(const FLinearColor& ColorB) noexcept
	{
		R *= ColorB.R;
		G *= ColorB.G;
		B *= ColorB.B;
		A *= ColorB.A;
		return *this;
	}

	FORCEINLINE FLinearColor operator*(float Scalar) const noexcept
	{
		return FLinearColor(
			this->R * Scalar,
			this->G * Scalar,
			this->B * Scalar,
			this->A * Scalar
		);
	}

	FORCEINLINE FLinearColor& operator*=(float Scalar) noexcept
	{
		R *= Scalar;
		G *= Scalar;
		B *= Scalar;
		A *= Scalar;
		return *this;
	}

	FORCEINLINE FLinearColor operator/(const FLinearColor& ColorB) const noexcept
	{
		return FLinearColor(
			this->R / ColorB.R,
			this->G / ColorB.G,
			this->B / ColorB.B,
			this->A / ColorB.A
		);
	}
	FORCEINLINE FLinearColor& operator/=(const FLinearColor& ColorB) noexcept
	{
		R /= ColorB.R;
		G /= ColorB.G;
		B /= ColorB.B;
		A /= ColorB.A;
		return *this;
	}

	FORCEINLINE FLinearColor operator/(float Scalar) const noexcept
	{
		const float	InvScalar = 1.0f / Scalar;
		return FLinearColor(
			this->R * InvScalar,
			this->G * InvScalar,
			this->B * InvScalar,
			this->A * InvScalar
		);
	}
	FORCEINLINE FLinearColor& operator/=(float Scalar) noexcept
	{
		const float	InvScalar = 1.0f / Scalar;
		R *= InvScalar;
		G *= InvScalar;
		B *= InvScalar;
		A *= InvScalar;
		return *this;
	}


public:

	/** Comparison operators */
	FORCEINLINE bool operator==(const FLinearColor& ColorB) const
	{
		return this->R == ColorB.R && this->G == ColorB.G && this->B == ColorB.B && this->A == ColorB.A;
	}
	FORCEINLINE bool operator!=(const FLinearColor& Other) const
	{
		return this->R != Other.R || this->G != Other.G || this->B != Other.B || this->A != Other.A;
	}



public:

	// clamped in 0..1 range
	FORCEINLINE FLinearColor GetClamped(float InMin = 0.0f, float InMax = 1.0f) const noexcept
	{
		FLinearColor Ret;

		Ret.R = FMath::Clamp(R, InMin, InMax);
		Ret.G = FMath::Clamp(G, InMin, InMax);
		Ret.B = FMath::Clamp(B, InMin, InMax);
		Ret.A = FMath::Clamp(A, InMin, InMax);

		return Ret;
	}

	// Error-tolerant comparison.
	FORCEINLINE bool Equals(const FLinearColor& ColorB, float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return FMath::Abs(this->R - ColorB.R) < Tolerance && FMath::Abs(this->G - ColorB.G) < Tolerance && FMath::Abs(this->B - ColorB.B) < Tolerance && FMath::Abs(this->A - ColorB.A) < Tolerance;
	}

	FORCEINLINE FLinearColor CopyWithNewOpacity(float NewOpacicty) const noexcept
	{
		FLinearColor NewCopy = *this;
		NewCopy.A = NewOpacicty;
		return NewCopy;
	}

	/**
	 * Returns a desaturated color, with 0 meaning no desaturation and 1 == full desaturation
	 *
	 * @param	Desaturation	Desaturation factor in range [0..1]
	 * @return	Desaturated color
	 */
	FORCEINLINE FLinearColor Desaturate(float Desaturation) const noexcept
	{
		float Lum = ComputeLuminance();
		return FMath::Lerp(*this, FLinearColor(Lum, Lum, Lum, 0), Desaturation);
	}

	/** Computes the perceptually weighted luminance value of a color. */
	FORCEINLINE float ComputeLuminance() const noexcept
	{
		return R * 0.3f + G * 0.59f + B * 0.11f;
	}

	/**
	 * Returns the maximum value in this color structure
	 *
	 * @return The maximum color channel value
	 */
	FORCEINLINE float GetMax() const noexcept
	{
		return FMath::Max(FMath::Max(FMath::Max(R, G), B), A);
	}

	/** useful to detect if a light contribution needs to be rendered */
	FORCEINLINE bool IsAlmostBlack() const noexcept
	{
		return FMath::Square(R) < DELTA && FMath::Square(G) < DELTA && FMath::Square(B) < DELTA;
	}

	/**
	 * Returns the minimum value in this color structure
	 *
	 * @return The minimum color channel value
	 */
	FORCEINLINE float GetMin() const noexcept
	{
		return FMath::Min(FMath::Min(FMath::Min(R, G), B), A);
	}

	FORCEINLINE float GetLuminance() const noexcept
	{
		return R * 0.3f + G * 0.59f + B * 0.11f;
	}

	

	



	FColor ToRGBE() const noexcept;

	/** Converts a linear space RGB color to an HSV color */
	FLinearColor LinearRGBToHSV() const noexcept;

	/** Converts an HSV color to a linear space RGB color */
	FLinearColor HSVToLinearRGB() const noexcept;


	/** Quantizes the linear color and returns the result as a FColor.  This bypasses the SRGB conversion. */
	FColor Quantize() const noexcept;

	/** Quantizes the linear color with rounding and returns the result as a FColor.  This bypasses the SRGB conversion. */
	FColor QuantizeRound() const noexcept;

	/** Quantizes the linear color and returns the result as a FColor with optional sRGB conversion and quality as goal. */
	FColor ToFColor(const bool bSRGB) const noexcept;


public:

	/**
	 * Converts byte hue-saturation-brightness to floating point red-green-blue.
	 */
	static FORCEINLINE FLinearColor MakeFromHSV8(uint8 H, uint8 S, uint8 V)
	{
		// want a given H value of 255 to map to just below 360 degrees
		const FLinearColor HSVColor((float)H * (360.0f / 256.0f), (float)S / 255.0f, (float)V / 255.0f);
		return HSVColor.HSVToLinearRGB();
	}


	/**
	 * Euclidean distance between two points.
	 */
	static FORCEINLINE float Dist(const FLinearColor& V1, const FLinearColor& V2)
	{
		return FMath::Sqrt(FMath::Square(V2.R - V1.R) + FMath::Square(V2.G - V1.G) + FMath::Square(V2.B - V1.B) + FMath::Square(V2.A - V1.A));
	}




	/**
	* Converts temperature in Kelvins of a black body radiator to RGB chromaticity.
	*/
	static FLinearColor MakeFromColorTemperature(float Temp);

	/**
	 * Generates a list of sample points on a Bezier curve defined by 2 points.
	 *
	 * @param	ControlPoints	Array of 4 Linear Colors (vert1, controlpoint1, controlpoint2, vert2).
	 * @param	NumPoints		Number of samples.
	 * @param	OutPoints		Receives the output samples.
	 * @return					Path length.
	 */
	static float EvaluateBezier(const FLinearColor* ControlPoints, int32 NumPoints, TArray<FLinearColor>& OutPoints);

	/**
	 * Linearly interpolates between two colors by the specified progress amount.  The interpolation is performed in HSV color space
	 * taking the shortest path to the new color's hue.  This can give better results than FMath::Lerp(), but is much more expensive.
	 * The incoming colors are in RGB space, and the output color will be RGB.  The alpha value will also be interpolated.
	 *
	 * @param	From		The color and alpha to interpolate from as linear RGBA
	 * @param	To			The color and alpha to interpolate to as linear RGBA
	 * @param	Progress	Scalar interpolation amount (usually between 0.0 and 1.0 inclusive)
	 * @return	The interpolated color in linear RGB space along with the interpolated alpha value
	 */
	static FLinearColor LerpUsingHSV(const FLinearColor& From, const FLinearColor& To, const float Progress);

	/**
	 * Converts an FColor coming from an observed sRGB output, into a linear color.
	 * @param Color The sRGB color that needs to be converted into linear space.
	 */
	static FLinearColor FromSRGBColor(const FColor& Color);

	/**
	 * Converts an FColor coming from an observed Pow(1/2.2) output, into a linear color.
	 * @param Color The Pow(1/2.2) color that needs to be converted into linear space.
	 */
	static FLinearColor FromPow22Color(const FColor& Color);

	/**
	* Makes a random but quite nice color.
	*/
	static FLinearColor MakeRandomColor();

public:

	/** Static lookup table used for FColor -> FLinearColor conversion. Pow(2.2) */
	static float Pow22OneOver255Table[256];

	/** Static lookup table used for FColor -> FLinearColor conversion. sRGB */
	static float sRGBToLinearTable[256];

	// Common colors.	
	static const FLinearColor White;
	static const FLinearColor Gray;
	static const FLinearColor Black;
	static const FLinearColor Transparent;
	static const FLinearColor Red;
	static const FLinearColor Green;
	static const FLinearColor Blue;
	static const FLinearColor Yellow;


public:

	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	float A = 0.0f;

};


FORCEINLINE FLinearColor operator*(float Scalar, const FLinearColor& Color)
{
	return Color.operator*(Scalar);
}






struct FColor
{

public:

	uint32& DWColor(void) { return *((uint32*)this); }
	const uint32& DWColor(void) const { return *((uint32*)this); }

	FORCEINLINE FColor() {}
	
	FORCEINLINE FColor(uint8 InR, uint8 InG, uint8 InB, uint8 InA = 255) noexcept : 
		A(InA), R(InR), G(InG), B(InB)
	{

	}

	FORCEINLINE explicit FColor(uint32 InColor) noexcept
	{
		DWColor() = InColor;
	}

	explicit FColor(const FLinearColor& LinearColor) = delete;


public:

	FORCEINLINE void operator+=(const FColor& C) noexcept
	{
		R = (uint8)FMath::Min((int32)R + (int32)C.R, 255);
		G = (uint8)FMath::Min((int32)G + (int32)C.G, 255);
		B = (uint8)FMath::Min((int32)B + (int32)C.B, 255);
		A = (uint8)FMath::Min((int32)A + (int32)C.A, 255);
	}

public:

	FORCEINLINE bool operator==(const FColor& C) const
	{
		return DWColor() == C.DWColor();
	}

	FORCEINLINE bool operator!=(const FColor& C) const
	{
		return DWColor() != C.DWColor();
	}




public:

	FORCEINLINE FLinearColor FromRGBE() const noexcept
	{
		if (A == 0)
		{
			return FLinearColor::Black;
		}
		else
		{
			const float Scale = (float)ldexp(1 / 255.0f, A - 128);
			return FLinearColor(R * Scale, G * Scale, B * Scale, 1.0f);
		}
	}

	/**
	 *	@return a new FColor based of this color with the new alpha value.
	 *	Usage: const FColor& MyColor = FColorList::Green.WithAlpha(128);
	 */
	FORCEINLINE FColor WithAlpha(uint8 Alpha) const noexcept
	{
		return FColor(R, G, B, Alpha);
	}

	/**
	 * Reinterprets the color as a linear color.
	 *
	 * @return The linear color representation.
	 */
	FORCEINLINE FLinearColor ReinterpretAsLinear() const noexcept
	{
		return FLinearColor(R / 255.f, G / 255.f, B / 255.f, A / 255.f);
	}

	/**
	 * Gets the color in a packed uint32 format packed in the order ARGB.
	 */
	FORCEINLINE uint32 ToPackedARGB() const noexcept
	{
		return (A << 24) | (R << 16) | (G << 8) | (B << 0);
	}

	/**
	 * Gets the color in a packed uint32 format packed in the order ABGR.
	 */
	FORCEINLINE uint32 ToPackedABGR() const noexcept
	{
		return (A << 24) | (B << 16) | (G << 8) | (R << 0);
	}

	/**
	 * Gets the color in a packed uint32 format packed in the order RGBA.
	 */
	FORCEINLINE uint32 ToPackedRGBA() const noexcept
	{
		return (R << 24) | (G << 16) | (B << 8) | (A << 0);
	}

	/**
	 * Gets the color in a packed uint32 format packed in the order BGRA.
	 */
	FORCEINLINE uint32 ToPackedBGRA() const noexcept
	{
		return (B << 24) | (G << 16) | (R << 8) | (A << 0);
	}


public:

	/**
	 * Makes a random but quite nice color.
	 */
	static FColor MakeRandomColor();

	/**
	 * Makes a color red->green with the passed in scalar (e.g. 0 is red, 1 is green)
	 */
	static FColor MakeRedToGreenColorFromScalar(float Scalar);

	/**
	* Converts temperature in Kelvins of a black body radiator to RGB chromaticity.
	*/
	static FColor MakeFromColorTemperature(float Temp);

	



public:

	/** Some pre-inited colors, useful for debug code */
	static const FColor White;
	static const FColor Red;
	static const FColor Green;
	static const FColor Blue;
	static const FColor Magenta;
	static const FColor Cyan;
	static const FColor Yellow;
	static const FColor Black;
	static const FColor Aquamarine;
	static const FColor BakerChocolate;
	static const FColor BlueViolet;
	static const FColor Brass;
	static const FColor BrightGold;
	static const FColor Brown;
	static const FColor Bronze;
	static const FColor BronzeII;
	static const FColor CadetBlue;
	static const FColor CoolCopper;
	static const FColor Copper;
	static const FColor Coral;
	static const FColor CornFlowerBlue;
	static const FColor DarkBrown;
	static const FColor DarkGreen;
	static const FColor DarkGreenCopper;
	static const FColor DarkOliveGreen;
	static const FColor DarkOrchid;
	static const FColor DarkPurple;
	static const FColor DarkSlateBlue;
	static const FColor DarkSlateGrey;
	static const FColor DarkTan;
	static const FColor DarkTurquoise;
	static const FColor DarkWood;
	static const FColor DimGrey;
	static const FColor DustyRose;
	static const FColor Feldspar;
	static const FColor Firebrick;
	static const FColor ForestGreen;
	static const FColor Gold;
	static const FColor Goldenrod;
	static const FColor Grey;
	static const FColor GreenCopper;
	static const FColor GreenYellow;
	static const FColor HunterGreen;
	static const FColor IndianRed;
	static const FColor Khaki;
	static const FColor LightBlue;
	static const FColor LightGrey;
	static const FColor LightSteelBlue;
	static const FColor LightWood;
	static const FColor LimeGreen;
	static const FColor MandarianOrange;
	static const FColor Maroon;
	static const FColor MediumAquamarine;
	static const FColor MediumBlue;
	static const FColor MediumForestGreen;
	static const FColor MediumGoldenrod;
	static const FColor MediumOrchid;
	static const FColor MediumSeaGreen;
	static const FColor MediumSlateBlue;
	static const FColor MediumSpringGreen;
	static const FColor MediumTurquoise;
	static const FColor MediumVioletRed;
	static const FColor MediumWood;
	static const FColor MidnightBlue;
	static const FColor NavyBlue;
	static const FColor NeonBlue;
	static const FColor NeonPink;
	static const FColor NewMidnightBlue;
	static const FColor NewTan;
	static const FColor OldGold;
	static const FColor Orange;
	static const FColor OrangeRed;
	static const FColor Orchid;
	static const FColor PaleGreen;
	static const FColor Pink;
	static const FColor Plum;
	static const FColor Quartz;
	static const FColor RichBlue;
	static const FColor Salmon;
	static const FColor Scarlet;
	static const FColor SeaGreen;
	static const FColor SemiSweetChocolate;
	static const FColor Sienna;
	static const FColor Silver;
	static const FColor SkyBlue;
	static const FColor SlateBlue;
	static const FColor SpicyPink;
	static const FColor SpringGreen;
	static const FColor SteelBlue;
	static const FColor SummerSky;
	static const FColor Tan;
	static const FColor Thistle;
	static const FColor Turquoise;
	static const FColor VeryDarkBrown;
	static const FColor VeryLightGrey;
	static const FColor Violet;
	static const FColor VioletRed;
	static const FColor Wheat;
	static const FColor YellowGreen;


public:

	union { struct { uint8 A, R, G, B; }; uint32 AlignmentDummy; };

};



FORCEINLINE uint32 GetTypeHash(const FColor& Color)
{
	return Color.DWColor();
}

