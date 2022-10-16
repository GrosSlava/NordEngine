
#include "Vector2D.h"





const FVector2D FVector2D::ZeroVector(0.0f, 0.0f);
const FVector2D FVector2D::UnitVector(1.0f, 1.0f);
const FVector2D FVector2D::Unit45Deg(INV_SQRT_2, INV_SQRT_2);




FVector2D FVector2D::GetRotated(float AngleDeg) const
{
	float S, C;
	FMath::SinCos(&S, &C, FMath::DegreesToRadians(AngleDeg));

	const float OMC = 1.0f - C;

	return FVector2D(C * X - S * Y, S * X + C * Y);
}

FVector2D FVector2D::GetSafeNormal(float Tolerance) const
{
	const float SquareSum = X * X + Y * Y;
	if( SquareSum > Tolerance )
	{
		const float Scale = FMath::InvSqrt(SquareSum);
		return FVector2D(X * Scale, Y * Scale);
	}

	return FVector2D(0.f, 0.f);
}

void FVector2D::Normalize(float Tolerance)
{
	const float SquareSum = X * X + Y * Y;
	if( SquareSum > Tolerance )
	{
		const float Scale = FMath::InvSqrt(SquareSum);
		X *= Scale;
		Y *= Scale;
		return;
	}

	X = 0.0f;
	Y = 0.0f;
}

void FVector2D::ToDirectionAndLength(FVector2D& OutDir, float& OutLength) const
{
	OutLength = Size();

	if( OutLength > SMALL_NUMBER )
	{
		float OneOverLength = 1.0f / OutLength;
		OutDir = FVector2D(X * OneOverLength, Y * OneOverLength);
	}
	else
	{
		OutDir = FVector2D::ZeroVector;
	}
}