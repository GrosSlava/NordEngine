// Copyright Nord Engine. All Rights Reserved.
#include "Vector4D.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Rotator.h"
#include "Quat.h"
#include "Color.h"





FVector4D::FVector4D(const FVector3D& InVector, float InW) noexcept : X(InVector.X), Y(InVector.Y), Z(InVector.Z), W(InW)
{
}

FVector4D::FVector4D(const FLinearColor& InColor) noexcept : X(InColor.R), Y(InColor.G), Z(InColor.B), W(InColor.A)
{
}

FVector4D::FVector4D(const FVector2D& InXY, const FVector2D& InZW) noexcept : X(InXY.X), Y(InXY.Y), Z(InZW.X), W(InZW.Y)
{
}




FRotator FVector4D::ToOrientationRotator() const noexcept
{
	FRotator R;

	// Find yaw.
	R.Yaw = FMath::Atan2(Y, X) * (180.f / PI);

	// Find pitch.
	R.Pitch = FMath::Atan2(Z, FMath::Sqrt(X * X + Y * Y)) * (180.f / PI);

	// Find roll.
	R.Roll = 0;

	return R;
}

FQuat FVector4D::ToOrientationQuat() const noexcept
{
	// Essentially an optimized Vector->Rotator->Quat made possible by knowing Roll == 0, and avoiding radians->degrees->radians.
	// This is done to avoid adding any roll (which our API states as a constraint).
	const float YawRad = FMath::Atan2(Y, X);
	const float PitchRad = FMath::Atan2(Z, FMath::Sqrt(X * X + Y * Y));

	const float DIVIDE_BY_2 = 0.5f;
	float SP, SY;
	float CP, CY;

	FMath::SinCos(&SP, &CP, PitchRad * DIVIDE_BY_2);
	FMath::SinCos(&SY, &CY, YawRad * DIVIDE_BY_2);

	FQuat RotationQuat;
	RotationQuat.X = SP * SY;
	RotationQuat.Y = -SP * CY;
	RotationQuat.Z = CP * SY;
	RotationQuat.W = CP * CY;
	return RotationQuat;
}

FRotator FVector4D::Rotation() const noexcept
{
	return ToOrientationRotator();
}
