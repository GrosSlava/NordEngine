
#include "Vector3D.h"
#include "Vector4D.h"
#include "Vector2D.h"
#include "Plane.h"
#include "Rotator.h"
#include "Quat.h"




const FVector3D FVector3D::ZeroVector(0.0f, 0.0f, 0.0f);
const FVector3D FVector3D::OneVector(1.0f, 1.0f, 1.0f);

const FVector3D FVector3D::UpVector(0.0f, 1.0f, 0.0f);
const FVector3D FVector3D::DownVector(0.0f, -1.0f, 0.0f);
const FVector3D FVector3D::ForwardVector(0.0f, 0.0f, 1.0f);
const FVector3D FVector3D::BackwardVector(0.0f, 0.0f, -1.0f);
const FVector3D FVector3D::RightVector(1.0f, 0.0f, 0.0f);
const FVector3D FVector3D::LeftVector(-1.0f, 0.0f, 0.0f);

const FVector3D FVector3D::XAxisVector(1.0f, 0.0f, 0.0f);
const FVector3D FVector3D::YAxisVector(0.0f, 1.0f, 0.0f);
const FVector3D FVector3D::ZAxisVector(0.0f, 0.0f, 1.0f);





FVector3D::FVector3D(const FVector4D& V) noexcept :
	 X(V.X), Y(V.Y), Z(V.Z)
{
}




FVector3D FVector3D::PointPlaneProject(const FVector3D& Point, const FPlane& Plane) noexcept
{
	//Find the distance of X from the plane
	//Add the distance back along the normal from the point
	return Point - Plane.PlaneDot(Point) * Plane;
}

FVector3D FVector3D::PointPlaneProject(const FVector3D& Point, const FVector3D& A, const FVector3D& B, const FVector3D& C) noexcept
{
	//Compute the plane normal from ABC
	FPlane Plane(A, B, C);

	//Find the distance of X from the plane
	//Add the distance back along the normal from the point
	return Point - Plane.PlaneDot(Point) * Plane;
}

FVector3D FVector3D::PointPlaneProject(const FVector3D& Point, const FVector3D& PlaneBase, const FVector3D& PlaneNormal) noexcept
{
	//Find the distance of X from the plane
	//Add the distance back along the normal from the point
	return Point - FVector3D::PointPlaneDist(Point, PlaneBase, PlaneNormal) * PlaneNormal;
}










float FVector3D::EvaluateBezier(const FVector3D* ControlPoints, int32 NumPoints, TArray<FVector3D>& OutPoints) noexcept
{
	if (ControlPoints == nullptr || NumPoints < 2) return 0.0f;


	// var q is the change in t between successive evaluations.
	const float q = 1.f / (float)(NumPoints - 1); // q is dependent on the number of GAPS = POINTS-1

	// recreate the names used in the derivation
	const FVector3D& P0 = ControlPoints[0];
	const FVector3D& P1 = ControlPoints[1];
	const FVector3D& P2 = ControlPoints[2];
	const FVector3D& P3 = ControlPoints[3];

	// coefficients of the cubic polynomial that we're FDing -
	const FVector3D a = P0;
	const FVector3D b = 3 * (P1 - P0);
	const FVector3D c = 3 * (P2 - 2 * P1 + P0);
	const FVector3D d = P3 - 3 * P2 + 3 * P1 - P0;

	// initial values of the poly and the 3 diffs -
	FVector3D S = a;						// the poly value
	FVector3D U = b * q + c * q * q + d * q * q * q;	// 1st order diff (quadratic)
	FVector3D V = 2 * c * q * q + 6 * d * q * q * q;	// 2nd order diff (linear)
	FVector3D W = 6 * d * q * q * q;				// 3rd order diff (constant)

	// Path length.
	float Length = 0.f;

	FVector3D OldPos = P0;
	OutPoints.PushBack(P0);	// first point on the curve is always P0.

	for (int32 i = 1; i < NumPoints; ++i)
	{
		// calculate the next value and update the deltas
		S += U;			// update poly value
		U += V;			// update 1st order diff value
		V += W;			// update 2st order diff value
		// 3rd order diff is constant => no update needed.

		// Update Length.
		Length += FVector3D::Dist(S, OldPos);
		OldPos = S;

		OutPoints.PushBack(S);
	}

	// Return path length as experienced in sequence (linear interpolation between points).
	return Length;
}




struct FClusterMovedHereToMakeCompile
{
	FVector3D ClusterPosAccum;
	int32 ClusterSize;
};

void FVector3D::GenerateClusterCenters(TArray<FVector3D>& Clusters, const TArray<FVector3D>& Points, int32 NumIterations, int32 NumConnectionsToBeValid) noexcept
{
	// Check we have >0 points and clusters
	if (Points.Num() == 0 || Clusters.Num() == 0) return;
	

	// Temp storage for each cluster that mirrors the order of the passed in Clusters array
	TArray<FClusterMovedHereToMakeCompile> ClusterData(Clusters.Num());


	// Then iterate
	for (int32 ItCount = 0; ItCount < NumIterations; ++ItCount)
	{
		// Classify each point - find closest cluster center
		for (const FVector3D& LPoint : Points)
		{
			// Iterate over all clusters to find closes one
			int32 NearestClusterIndex = -1;
			float NearestClusterDistSqr = BIG_NUMBER;
			for (uint32 j = 0; j < Clusters.Num(); ++j)
			{
				const float DistSqr = (LPoint - Clusters[j]).SizeSquared();
				if (DistSqr < NearestClusterDistSqr)
				{
					NearestClusterDistSqr = DistSqr;
					NearestClusterIndex = j;
				}
			}
			// Update its info with this point
			if (NearestClusterIndex != -1)
			{
				ClusterData[NearestClusterIndex].ClusterPosAccum += LPoint;
				++ClusterData[NearestClusterIndex].ClusterSize;
			}
		}

		// All points classified - update cluster center as average of membership
		for (uint32 i = 0; i < Clusters.Num(); ++i)
		{
			if (ClusterData[i].ClusterSize > 0)
			{
				Clusters[i] = ClusterData[i].ClusterPosAccum / (float)ClusterData[i].ClusterSize;
			}
		}
	}

	// so now after we have possible cluster centers we want to remove the ones that are outliers and not part of the main cluster
	for (uint32 i = 0; i < ClusterData.Num(); ++i)
	{
		if (ClusterData[i].ClusterSize < NumConnectionsToBeValid)
		{
			Clusters.RemoveAt(i);
		}
	}
}


void FVector3D::CreateOrthonormalBasis(FVector3D& XAxis, FVector3D& YAxis, FVector3D& ZAxis) noexcept
{
	// Project the X and Y axes onto the plane perpendicular to the Z axis.
	XAxis -= (XAxis | ZAxis) / (ZAxis | ZAxis) * ZAxis;
	YAxis -= (YAxis | ZAxis) / (ZAxis | ZAxis) * ZAxis;

	// If the X axis was parallel to the Z axis, choose a vector which is orthogonal to the Y and Z axes.
	if (XAxis.SizeSquared() < DELTA * DELTA)
	{
		XAxis = YAxis ^ ZAxis;
	}

	// If the Y axis was parallel to the Z axis, choose a vector which is orthogonal to the X and Z axes.
	if (YAxis.SizeSquared() < DELTA * DELTA)
	{
		YAxis = XAxis ^ ZAxis;
	}

	// Normalize the basis vectors.
	XAxis.Normalize();
	YAxis.Normalize();
	ZAxis.Normalize();
}







FVector3D FVector3D::MirrorByPlane(const FPlane& Plane) const noexcept
{
	return *this - Plane * (2.f * Plane.PlaneDot(*this));
}






bool FVector3D::Normalize(float Tolerance) noexcept
{
	const float SquareSum = X * X + Y * Y + Z * Z;
	if (SquareSum > Tolerance)
	{
		const float Scale = FMath::InvSqrt(SquareSum);
		X *= Scale; Y *= Scale; Z *= Scale;
		return true;
	}

	return false;
}

FVector3D FVector3D::GetSafeNormal(float Tolerance) const noexcept
{
	const float SquareSum = X * X + Y * Y + Z * Z;

	// Not sure if it's safe to add tolerance in there. Might introduce too many errors
	if (SquareSum == 1.f)
	{
		return *this;
	}
	else if (SquareSum < Tolerance)
	{
		return FVector3D::ZeroVector;
	}
	const float Scale = FMath::InvSqrt(SquareSum);
	return FVector3D(X * Scale, Y * Scale, Z * Scale);
}

FVector3D FVector3D::GetSafeNormal2D(float Tolerance) const noexcept
{
	const float SquareSum = X * X + Y * Y;

	// Not sure if it's safe to add tolerance in there. Might introduce too many errors
	if (SquareSum == 1.f)
	{
		if (Z == 0.f)
		{
			return *this;
		}
		else
		{
			return FVector3D(X, Y, 0.f);
		}
	}
	else if (SquareSum < Tolerance)
	{
		return FVector3D::ZeroVector;
	}

	const float Scale = FMath::InvSqrt(SquareSum);
	return FVector3D(X * Scale, Y * Scale, 0.f);
}



void FVector3D::ToDirectionAndLength(FVector3D& OutDir, float& OutLength) const noexcept
{
	OutLength = Size();
	if (OutLength > SMALL_NUMBER)
	{
		float OneOverLength = 1.0f / OutLength;
		OutDir = FVector3D(X * OneOverLength, Y * OneOverLength,
			Z * OneOverLength);
	}
	else
	{
		OutDir = FVector3D::ZeroVector;
	}
}

FVector3D FVector3D::GetClampedToSize(float Min, float Max) const noexcept
{
	float VecSize = Size();
	const FVector3D VecDir = (VecSize > SMALL_NUMBER) ? (*this / VecSize) : FVector3D::ZeroVector;

	VecSize = FMath::Clamp(VecSize, Min, Max);

	return VecSize * VecDir;
}

FVector3D FVector3D::GetClampedToSize2D(float Min, float Max) const noexcept
{
	float VecSize2D = Size2D();
	const FVector3D VecDir = (VecSize2D > SMALL_NUMBER) ? (*this / VecSize2D) : FVector3D::ZeroVector;

	VecSize2D = FMath::Clamp(VecSize2D, Min, Max);

	return FVector3D(VecSize2D * VecDir.X, VecSize2D * VecDir.Y, Z);
}

FVector3D FVector3D::GetClampedToMaxSize(float MaxSize) const noexcept
{
	if (MaxSize < KINDA_SMALL_NUMBER)
	{
		return FVector3D::ZeroVector;
	}

	const float VSq = SizeSquared();
	if (VSq > FMath::Square(MaxSize))
	{
		const float Scale = MaxSize * FMath::InvSqrt(VSq);
		return FVector3D(X * Scale, Y * Scale, Z * Scale);
	}
	else
	{
		return *this;
	}
}

FVector3D FVector3D::GetClampedToMaxSize2D(float MaxSize) const noexcept
{
	if (MaxSize < KINDA_SMALL_NUMBER)
	{
		return FVector3D(0.f, 0.f, Z);
	}

	const float VSq2D = SizeSquared2D();
	if (VSq2D > FMath::Square(MaxSize))
	{
		const float Scale = MaxSize * FMath::InvSqrt(VSq2D);
		return FVector3D(X * Scale, Y * Scale, Z);
	}
	else
	{
		return *this;
	}
}



FVector3D FVector3D::Reciprocal() const noexcept
{
	FVector3D RecVector;
	if (X != 0.f)
	{
		RecVector.X = 1.f / X;
	}
	else
	{
		RecVector.X = BIG_NUMBER;
	}
	if (Y != 0.f)
	{
		RecVector.Y = 1.f / Y;
	}
	else
	{
		RecVector.Y = BIG_NUMBER;
	}
	if (Z != 0.f)
	{
		RecVector.Z = 1.f / Z;
	}
	else
	{
		RecVector.Z = BIG_NUMBER;
	}

	return RecVector;
}


FVector3D FVector3D::RotateAngleAxis(const float AngleDeg, const FVector3D& Axis) const noexcept
{
	float S, C;
	FMath::SinCos(&S, &C, FMath::DegreesToRadians(AngleDeg));

	const float XX = Axis.X * Axis.X;
	const float YY = Axis.Y * Axis.Y;
	const float ZZ = Axis.Z * Axis.Z;

	const float XY = Axis.X * Axis.Y;
	const float YZ = Axis.Y * Axis.Z;
	const float ZX = Axis.Z * Axis.X;

	const float XS = Axis.X * S;
	const float YS = Axis.Y * S;
	const float ZS = Axis.Z * S;

	const float OMC = 1.f - C;

	return FVector3D(
		(OMC * XX + C) * X + (OMC * XY - ZS) * Y + (OMC * ZX + YS) * Z,
		(OMC * XY + ZS) * X + (OMC * YY + C) * Y + (OMC * YZ - XS) * Z,
		(OMC * ZX - YS) * X + (OMC * YZ + XS) * Y + (OMC * ZZ + C) * Z
	);
}


FRotator FVector3D::ToOrientationRotator() const noexcept
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


FQuat FVector3D::ToOrientationQuat() const noexcept
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

FRotator FVector3D::Rotation() const noexcept
{
	return ToOrientationRotator();
}

FVector2D FVector3D::UnitCartesianToSpherical() const noexcept
{
	if (!IsUnit()) return FVector2D(0, 0);

	const float Theta = FMath::Acos(Z / Size());
	const float Phi = FMath::Atan2(Y, X);
	return FVector2D(Theta, Phi);
}




void FVector3D::FindBestAxisVectors(FVector3D& Axis1, FVector3D& Axis2) const noexcept
{
	const float NX = FMath::Abs(X);
	const float NY = FMath::Abs(Y);
	const float NZ = FMath::Abs(Z);

	// Find best basis vectors.
	if (NZ > NX && NZ > NY)	Axis1 = FVector3D(1, 0, 0);
	else					Axis1 = FVector3D(0, 0, 1);

	Axis1 = (Axis1 - *this * (Axis1 | *this)).GetSafeNormal();
	Axis2 = Axis1 ^ *this;
}

float FVector3D::HeadingAngle() const noexcept
{
	// Project Dir into Z plane.
	FVector3D PlaneDir = *this;
	PlaneDir.Z = 0.f;
	PlaneDir = PlaneDir.GetSafeNormal();

	float Angle = FMath::Acos(PlaneDir.X);

	if (PlaneDir.Y < 0.0f)
	{
		Angle *= -1.0f;
	}

	return Angle;
}
