
#include "Rotator.h"
#include "Quat.h"
#include "Vector3D.h"

#include "RotationMatrix.h"



const FRotator FRotator::ZeroRotator(0.0f, 0.0f, 1.0f);



FRotator::FRotator(const FQuat& Quat)
{
	*this = Quat.Rotator();
}






FRotator FRotator::GetInverse() const noexcept
{
	return Quaternion().Inverse().Rotator();
}

FVector3D FRotator::Vector() const noexcept
{
	// Remove winding and clamp to [-360, 360]
	const float PitchNoWinding = FMath::Fmod(Pitch, 360.0f);
	const float YawNoWinding = FMath::Fmod(Yaw, 360.0f);

	float CP, SP, CY, SY;
	FMath::SinCos(&SP, &CP, FMath::DegreesToRadians(PitchNoWinding));
	FMath::SinCos(&SY, &CY, FMath::DegreesToRadians(YawNoWinding));
	FVector3D V = FVector3D(CP * CY, CP * SY, SP);

	return V;
}

FQuat FRotator::Quaternion() const noexcept
{
	const float DEG_TO_RAD = PI / (180.f);
	const float RADS_DIVIDED_BY_2 = DEG_TO_RAD / 2.f;
	float SP, SY, SR;
	float CP, CY, CR;

	const float PitchNoWinding = FMath::Fmod(Pitch, 360.0f);
	const float YawNoWinding = FMath::Fmod(Yaw, 360.0f);
	const float RollNoWinding = FMath::Fmod(Roll, 360.0f);

	FMath::SinCos(&SP, &CP, PitchNoWinding * RADS_DIVIDED_BY_2);
	FMath::SinCos(&SY, &CY, YawNoWinding * RADS_DIVIDED_BY_2);
	FMath::SinCos(&SR, &CR, RollNoWinding * RADS_DIVIDED_BY_2);

	FQuat RotationQuat;
	RotationQuat.X = CR * SP * SY - SR * CP * CY;
	RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
	RotationQuat.Z = CR * CP * SY - SR * SP * CY;
	RotationQuat.W = CR * CP * CY + SR * SP * SY;

	return RotationQuat;
}

FVector3D FRotator::Euler() const noexcept
{
	return FVector3D(Roll, Pitch, Yaw);
}

FVector3D FRotator::RotateVector(const FVector3D& V) const noexcept
{
	return FRotationMatrix(*this).TransformVector(V);
}

FVector3D FRotator::UnrotateVector(const FVector3D& V) const noexcept
{
	return FRotationMatrix(*this).GetTransposed().TransformVector( V );
}






FRotator FRotator::MakeFromEuler(const FVector3D& Euler)
{
	return FRotator(Euler.Y, Euler.Z, Euler.X);
}
