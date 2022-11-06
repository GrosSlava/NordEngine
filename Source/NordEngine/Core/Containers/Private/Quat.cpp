
#include "Quat.h"
#include "Rotator.h"
#include "Vector3D.h"
#include "Matrix.h"


const FQuat FQuat::Identity(0.0f, 0.0f, 0.0f, 1.0f);




FQuat::FQuat(const FMatrix& M)
{
	// If Matrix is NULL, return Identity quaternion. If any of them is 0, you won't be able to construct rotation
	// if you have two plane at least, we can reconstruct the frame using cross product, but that's a bit expensive op to do here
	// for now, if you convert to matrix from 0 scale and convert back, you'll lose rotation. Don't do that. 
	if (M.GetScaledAxis(EAxis::X).IsNearlyZero() || M.GetScaledAxis(EAxis::Y).IsNearlyZero() || M.GetScaledAxis(EAxis::Z).IsNearlyZero())
	{
		*this = FQuat::Identity;
		return;
	}

	//const MeReal *const t = (MeReal *) tm;
	float	s;

	// Check diagonal (trace)
	const float tr = M.M[0][0] + M.M[1][1] + M.M[2][2];

	if (tr > 0.0f)
	{
		float InvS = FMath::InvSqrt(tr + 1.f);
		this->W = 0.5f * (1.f / InvS);
		s = 0.5f * InvS;

		this->X = (M.M[1][2] - M.M[2][1]) * s;
		this->Y = (M.M[2][0] - M.M[0][2]) * s;
		this->Z = (M.M[0][1] - M.M[1][0]) * s;
	}
	else
	{
		// diagonal is negative
		int32 i = 0;

		if (M.M[1][1] > M.M[0][0])
			i = 1;

		if (M.M[2][2] > M.M[i][i])
			i = 2;

		static const int32 nxt[3] = { 1, 2, 0 };
		const int32 j = nxt[i];
		const int32 k = nxt[j];

		s = M.M[i][i] - M.M[j][j] - M.M[k][k] + 1.0f;

		float InvS = FMath::InvSqrt(s);

		float qt[4];
		qt[i] = 0.5f * (1.f / InvS);

		s = 0.5f * InvS;

		qt[3] = (M.M[j][k] - M.M[k][j]) * s;
		qt[j] = (M.M[i][j] + M.M[j][i]) * s;
		qt[k] = (M.M[i][k] + M.M[k][i]) * s;

		this->X = qt[0];
		this->Y = qt[1];
		this->Z = qt[2];
		this->W = qt[3];

	}
}

FQuat::FQuat(const FRotator& R) noexcept
{
	*this = R.Quaternion();
}

FQuat::FQuat(const FVector3D& Axis, float AngleRad) noexcept
{
	const float half_a = 0.5f * AngleRad;
	float s, c;
	FMath::SinCos(&s, &c, half_a);

	X = s * Axis.X;
	Y = s * Axis.Y;
	Z = s * Axis.Z;
	W = c;
}





FQuat FQuat::operator*=(const FQuat& Q)
{
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorLoadAligned(&Q);
	VectorRegister Result;
	VectorQuaternionMultiply(&Result, &A, &B);
	VectorStoreAligned(Result, this);

	return *this;
}

FVector3D FQuat::operator*(const FVector3D& V) const
{
	return RotateVector(V);
}

FMatrix FQuat::operator*(const FMatrix& M) const
{
	FMatrix Result;
	FQuat VT, VR;
	FQuat Inv = Inverse();
	for (int32 I = 0; I < 4; ++I)
	{
		FQuat VQ(M.M[I][0], M.M[I][1], M.M[I][2], M.M[I][3]);
		VectorQuaternionMultiply(&VT, this, &VQ);
		VectorQuaternionMultiply(&VR, &VT, &Inv);
		Result.M[I][0] = VR.X;
		Result.M[I][1] = VR.Y;
		Result.M[I][2] = VR.Z;
		Result.M[I][3] = VR.W;
	}

	return Result;
}






FQuat FQuat::Log() const noexcept
{
	FQuat Result;
	Result.W = 0.f;

	if (FMath::Abs(W) < 1.f)
	{
		const float Angle = FMath::Acos(W);
		const float SinAngle = FMath::Sin(Angle);

		if (FMath::Abs(SinAngle) >= SMALL_NUMBER)
		{
			const float Scale = Angle / SinAngle;
			Result.X = Scale * X;
			Result.Y = Scale * Y;
			Result.Z = Scale * Z;

			return Result;
		}
	}

	Result.X = X;
	Result.Y = Y;
	Result.Z = Z;

	return Result;
}

FQuat FQuat::Exp() const noexcept
{
	const float Angle = FMath::Sqrt(X * X + Y * Y + Z * Z);
	const float SinAngle = FMath::Sin(Angle);

	FQuat Result;
	Result.W = FMath::Cos(Angle);

	if (FMath::Abs(SinAngle) >= SMALL_NUMBER)
	{
		const float Scale = SinAngle / Angle;
		Result.X = Scale * X;
		Result.Y = Scale * Y;
		Result.Z = Scale * Z;
	}
	else
	{
		Result.X = X;
		Result.Y = Y;
		Result.Z = Z;
	}

	return Result;
}

FVector3D FQuat::Euler() const noexcept
{
	return Rotator().Euler();
}

void FQuat::ToAxisAndAngle(FVector3D& Axis, float& Angle) const noexcept
{
	Angle = GetAngle();
	Axis = GetRotationAxis();
}

void FQuat::ToSwingTwist(const FVector3D& InTwistAxis, FQuat& OutSwing, FQuat& OutTwist) const noexcept
{
	// Vector part projected onto twist axis
	FVector3D Projection = FVector3D::DotProduct(InTwistAxis, FVector3D(X, Y, Z)) * InTwistAxis;

	// Twist quaternion
	OutTwist = FQuat(Projection.X, Projection.Y, Projection.Z, W);

	// Singularity close to 180deg
	if (OutTwist.SizeSquared() == 0.0f)
	{
		OutTwist = FQuat::Identity;
	}
	else
	{
		OutTwist.Normalize();
	}

	// Set swing
	OutSwing = *this * OutTwist.Inverse();
}

float FQuat::GetTwistAngle(const FVector3D& TwistAxis) const noexcept
{
	float XYZ = FVector3D::DotProduct(TwistAxis, FVector3D(X, Y, Z));
	return FMath::UnwindRadians(2.0f * FMath::Atan2(XYZ, W));
}

FVector3D FQuat::RotateVector(const FVector3D& V) const noexcept
{
	// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
	// V' = V + 2w(Q x V) + (2Q x (Q x V))
	// refactor:
	// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
	// T = 2(Q x V);
	// V' = V + w*(T) + (Q x T)

	const FVector3D Q(X, Y, Z);
	const FVector3D T = 2.f * FVector3D::CrossProduct(Q, V);
	const FVector3D Result = V + (W * T) + FVector3D::CrossProduct(Q, T);
	return Result;
}

FVector3D FQuat::UnrotateVector(const FVector3D& V) const noexcept
{
	const FVector3D Q(-X, -Y, -Z); // Inverse
	const FVector3D T = 2.f * FVector3D::CrossProduct(Q, V);
	const FVector3D Result = V + (W * T) + FVector3D::CrossProduct(Q, T);
	return Result;
}




FVector3D FQuat::GetAxisX() const noexcept
{
	return RotateVector(FVector3D(1.f, 0.f, 0.f));
}

FVector3D FQuat::GetAxisY() const noexcept
{
	return RotateVector(FVector3D(0.f, 1.f, 0.f));
}

FVector3D FQuat::GetAxisZ() const noexcept
{
	return RotateVector(FVector3D(0.f, 0.f, 1.f));
}

FVector3D FQuat::GetForwardVector() const noexcept
{
	return GetAxisX();
}

FVector3D FQuat::GetRightVector() const noexcept
{
	return GetAxisY();
}

FVector3D FQuat::GetUpVector() const noexcept
{
	return GetAxisZ();
}

FVector3D FQuat::Vector() const noexcept
{
	return GetAxisX();
}


FRotator FQuat::Rotator() const noexcept
{
	const float SingularityTest = Z * X - W * Y;
	const float YawY = 2.f * (W * Z + X * Y);
	const float YawX = (1.f - 2.f * (FMath::Square(Y) + FMath::Square(Z)));

	// reference 
	// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

	// this value was found from experience, the above websites recommend different values
	// but that isn't the case for us, so I went through different testing, and finally found the case 
	// where both of world lives happily. 
	const float SINGULARITY_THRESHOLD = 0.4999995f;
	const float RAD_TO_DEG = (180.f) / PI;
	FRotator RotatorFromQuat;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = -90.f;
		RotatorFromQuat.Yaw = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = FRotator::NormalizeAxis(-RotatorFromQuat.Yaw - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = 90.f;
		RotatorFromQuat.Yaw = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = FRotator::NormalizeAxis(RotatorFromQuat.Yaw - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
	}
	else
	{
		RotatorFromQuat.Pitch = FMath::FastAsin(2.f * (SingularityTest)) * RAD_TO_DEG;
		RotatorFromQuat.Yaw = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = FMath::Atan2(-2.f * (W * X + Y * Z), (1.f - 2.f * (FMath::Square(X) + FMath::Square(Y)))) * RAD_TO_DEG;
	}

	return RotatorFromQuat;
}

FVector3D FQuat::GetRotationAxis() const noexcept
{
	const float SquareSum = X * X + Y * Y + Z * Z;
	if (SquareSum < SMALL_NUMBER)
	{
		return FVector3D::XAxisVector;
	}

	const float Scale = FMath::InvSqrt(SquareSum);
	return FVector3D(X * Scale, Y * Scale, Z * Scale);
}








FQuat FQuat::Slerp_NotNormalized(const FQuat& Quat1, const FQuat& Quat2, float Slerp) noexcept
{
	// Get cosine of angle between quats.
	const float RawCosom =
		Quat1.X * Quat2.X +
		Quat1.Y * Quat2.Y +
		Quat1.Z * Quat2.Z +
		Quat1.W * Quat2.W;
	// Unaligned quats - compensate, results in taking shorter route.
	const float Cosom = FMath::FloatSelect(RawCosom, RawCosom, -RawCosom);

	float Scale0, Scale1;

	if (Cosom < 0.9999f)
	{
		const float Omega = FMath::Acos(Cosom);
		const float InvSin = 1.f / FMath::Sin(Omega);
		Scale0 = FMath::Sin((1.f - Slerp) * Omega) * InvSin;
		Scale1 = FMath::Sin(Slerp * Omega) * InvSin;
	}
	else
	{
		// Use linear interpolation.
		Scale0 = 1.0f - Slerp;
		Scale1 = Slerp;
	}

	// In keeping with our flipped Cosom:
	Scale1 = FMath::FloatSelect(RawCosom, Scale1, -Scale1);

	FQuat Result;

	Result.X = Scale0 * Quat1.X + Scale1 * Quat2.X;
	Result.Y = Scale0 * Quat1.Y + Scale1 * Quat2.Y;
	Result.Z = Scale0 * Quat1.Z + Scale1 * Quat2.Z;
	Result.W = Scale0 * Quat1.W + Scale1 * Quat2.W;

	return Result;
}

FQuat FQuat::SlerpFullPath_NotNormalized(const FQuat& quat1, const FQuat& quat2, float Alpha) noexcept
{
	const float CosAngle = FMath::Clamp(quat1 | quat2, -1.f, 1.f);
	const float Angle = FMath::Acos(CosAngle);

	//UE_LOG(LogUnrealMath, Log,  TEXT("CosAngle: %f Angle: %f"), CosAngle, Angle );

	if (FMath::Abs(Angle) < KINDA_SMALL_NUMBER)
	{
		return quat1;
	}

	const float SinAngle = FMath::Sin(Angle);
	const float InvSinAngle = 1.f / SinAngle;

	const float Scale0 = FMath::Sin((1.0f - Alpha) * Angle) * InvSinAngle;
	const float Scale1 = FMath::Sin(Alpha * Angle) * InvSinAngle;

	return quat1 * Scale0 + quat2 * Scale1;
}



FQuat FQuat::MakeFromEuler(const FVector3D& Euler) noexcept
{
	return FRotator::MakeFromEuler(Euler).Quaternion();
}


//
// Based on:
// http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
// http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
//
FQuat FindBetween_Helper(const FVector3D& A, const FVector3D& B, float NormAB)
{
	float W = NormAB + FVector3D::DotProduct(A, B);
	FQuat Result;

	if (W >= 1e-6f * NormAB)
	{
		//Axis = FVector::CrossProduct(A, B);
		Result = FQuat(A.Y * B.Z - A.Z * B.Y,
			A.Z * B.X - A.X * B.Z,
			A.X * B.Y - A.Y * B.X,
			W);
	}
	else
	{
		// A and B point in opposite directions
		W = 0.f;
		Result = FMath::Abs(A.X) > FMath::Abs(A.Y)
			? FQuat(-A.Z, 0.f, A.X, W)
			: FQuat(0.f, -A.Z, A.Y, W);
	}

	Result.Normalize();
	return Result;
}

FQuat FQuat::FindBetweenNormals(const FVector3D& Normal1, const FVector3D& Normal2) noexcept
{
	const float NormAB = 1.f;
	return FindBetween_Helper(Normal1, Normal2, NormAB);
}

FQuat FQuat::FindBetweenVectors(const FVector3D& Vector1, const FVector3D& Vector2) noexcept
{
	const float NormAB = FMath::Sqrt(Vector1.SizeSquared() * Vector2.SizeSquared());
	return FindBetween_Helper(Vector1, Vector2, NormAB);
}
