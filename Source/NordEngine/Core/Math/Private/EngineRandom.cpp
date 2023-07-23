// Copyright Nord Engine. All Rights Reserved.
#include "EngineRandom.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Box3D.h"
#include "Box2D.h"
#include "RotationMatrix.h"





std::mt19937 FRandom::Generator;



FVector3D FRandom::VRand3D() noexcept
{
	FVector3D Result;
	float L;

	do
	{
		// Check random vectors in the unit sphere so result is statistically uniform.
		Result.X = FRand() * 2.0f - 1.0f;
		Result.Y = FRand() * 2.0f - 1.0f;
		Result.Z = FRand() * 2.0f - 1.0f;
		L = Result.SizeSquared();
	} while( L > 1.0f || L < KINDA_SMALL_NUMBER );

	return Result * (1.0f / FMath::Sqrt(L));
}

FVector2D FRandom::VRand2D() noexcept
{
	FVector2D Result;
	float L;

	do
	{
		// Check random vectors in the unit circle so result is statistically uniform.
		Result.X = FRand() * 2.0f - 1.0f;
		Result.Y = FRand() * 2.0f - 1.0f;
		L = Result.SizeSquared();
	} while( L > 1.0f );

	return Result * (1.0f / FMath::Sqrt(L));
}

FVector3D FRandom::VRandCone(FVector3D const& Dir, float ConeHalfAngleRad) noexcept
{
	if( ConeHalfAngleRad > 0.0f )
	{
		const float RandU = FRand();
		const float RandV = FRand();

		// Get spherical coords that have an even distribution over the unit sphere
		// Method described at http://mathworld.wolfram.com/SpherePointPicking.html
		float Theta = 2.0f * PI * RandU;
		float Phi = FMath::Acos((2.0f * RandV) - 1.0f);

		// restrict phi to [0, ConeHalfAngleRad]
		// this gives an even distribution of points on the surface of the cone
		// centered at the origin, pointing upward (z), with the desired angle
		Phi = FMath::Fmod(Phi, ConeHalfAngleRad);

		// get axes we need to rotate around
		FMatrix const DirMat = FRotationMatrix(Dir.Rotation());
		// note the axis translation, since we want the variation to be around X
		FVector3D const DirZ = DirMat.GetScaledAxis(EAxis::X);
		FVector3D const DirY = DirMat.GetScaledAxis(EAxis::Y);

		FVector3D Result = Dir.RotateAngleAxis(Phi * 180.0f / PI, DirY);
		Result = Result.RotateAngleAxis(Theta * 180.0f / PI, DirZ);

		// ensure it's a unit vector (might not have been passed in that way)
		Result = Result.GetSafeNormal();

		return Result;
	}
	else
	{
		return Dir.GetSafeNormal();
	}
}

FVector3D FRandom::VRandCone(FVector3D const& Dir, float HorizontalConeHalfAngleRad, float VerticalConeHalfAngleRad) noexcept
{
	if( (VerticalConeHalfAngleRad > 0.0f) && (HorizontalConeHalfAngleRad > 0.0f) )
	{
		const float RandU = FRand();
		const float RandV = FRand();

		// Get spherical coords that have an even distribution over the unit sphere
		// Method described at http://mathworld.wolfram.com/SpherePointPicking.html
		float Theta = 2.0f * PI * RandU;
		float Phi = FMath::Acos((2.0f * RandV) - 1.0f);

		// restrict phi to [0, ConeHalfAngleRad]
		// where ConeHalfAngleRad is now a function of Theta
		// (specifically, radius of an ellipse as a function of angle)
		// function is ellipse function (x/a)^2 + (y/b)^2 = 1, converted to polar coords
		float ConeHalfAngleRad = FMath::Square(FMath::Cos(Theta) / VerticalConeHalfAngleRad) + FMath::Square(FMath::Sin(Theta) / HorizontalConeHalfAngleRad);
		ConeHalfAngleRad = FMath::Sqrt(1.f / ConeHalfAngleRad);

		// clamp to make a cone instead of a sphere
		Phi = FMath::Fmod(Phi, ConeHalfAngleRad);

		// get axes we need to rotate around
		FMatrix const DirMat = FRotationMatrix(Dir.Rotation());
		// note the axis translation, since we want the variation to be around X
		FVector3D const DirZ = DirMat.GetScaledAxis(EAxis::X);
		FVector3D const DirY = DirMat.GetScaledAxis(EAxis::Y);

		FVector3D Result = Dir.RotateAngleAxis(Phi * 180.0f / PI, DirY);
		Result = Result.RotateAngleAxis(Theta * 180.0f / PI, DirZ);

		// ensure it's a unit vector (might not have been passed in that way)
		Result = Result.GetSafeNormal();

		return Result;
	}
	else
	{
		return Dir.GetSafeNormal();
	}
}

FVector2D FRandom::RandPointInCircle(float CircleRadius) noexcept
{
	FVector2D Point;
	float L;

	do
	{
		// Check random vectors in the unit circle so result is statistically uniform.
		Point.X = FRand() * 2.0f - 1.0f;
		Point.Y = FRand() * 2.0f - 1.0f;
		L = Point.SizeSquared();
	} while( L > 1.0f );

	return Point * CircleRadius;
}

FVector3D FRandom::RandPointInSphere(float SphereRadius) noexcept
{
	FVector3D Point;
	float L;

	do
	{
		// Check random vectors in the unit sphere so result is statistically uniform.
		Point.X = FRand() * 2.0f - 1.0f;
		Point.Y = FRand() * 2.0f - 1.0f;
		Point.Z = FRand() * 2.0f - 1.0f;
		L = Point.SizeSquared();
	} while( L > 1.0f || L < KINDA_SMALL_NUMBER );

	return Point * SphereRadius;
}

FVector2D FRandom::RandPointInBox2D(const FBox2D& Box) noexcept
{
	return FVector2D(FRandRange(Box.Min.X, Box.Max.X), FRandRange(Box.Min.Y, Box.Max.Y));
}

FVector3D FRandom::RandPointInBox3D(const FBox3D& Box) noexcept
{
	return FVector3D(FRandRange(Box.Min.X, Box.Max.X), FRandRange(Box.Min.Y, Box.Max.Y), FRandRange(Box.Min.Z, Box.Max.Z));
}