
#include "Matrix.h"

#include "Quat.h"
#include "Rotator.h"

#include "RotationMatrix.h"




const FMatrix FMatrix::Identity(FPlane(1, 0, 0, 0), FPlane(0, 1, 0, 0), FPlane(0, 0, 1, 0), FPlane(0, 0, 0, 1));




FMatrix FMatrix::GetTransposed() const noexcept
{
	FMatrix	Result;

	Result.M[0][0] = M[0][0];
	Result.M[0][1] = M[1][0];
	Result.M[0][2] = M[2][0];
	Result.M[0][3] = M[3][0];

	Result.M[1][0] = M[0][1];
	Result.M[1][1] = M[1][1];
	Result.M[1][2] = M[2][1];
	Result.M[1][3] = M[3][1];

	Result.M[2][0] = M[0][2];
	Result.M[2][1] = M[1][2];
	Result.M[2][2] = M[2][2];
	Result.M[2][3] = M[3][2];

	Result.M[3][0] = M[0][3];
	Result.M[3][1] = M[1][3];
	Result.M[3][2] = M[2][3];
	Result.M[3][3] = M[3][3];

	return Result;
}


float FMatrix::Determinant() const noexcept
{
	return
		M[0][0] *
			(
				M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
				M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
			)
		-
		M[1][0] * 
			(
				M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
			)
		+
		M[2][0] *
			(
				M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
				M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			)
		-
		M[3][0] *
			(
				M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
				M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
				M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			);
}



FMatrix FMatrix::Inverse() const noexcept
{
	FMatrix Result;

	// Check for zero scale matrix to invert
	if (GetScaledAxis(EAxis::X).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(EAxis::Y).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(EAxis::Z).IsNearlyZero(SMALL_NUMBER))
	{
		// just set to zero - avoids unsafe inverse of zero and duplicates what QNANs were resulting in before (scaling away all children)
		Result = FMatrix::Identity;
	}
	else
	{
		const float	Det = Determinant();

		if (Det == 0.0f)
		{
			Result = FMatrix::Identity;
		}
		else
		{
			VectorMatrixInverse(&Result, this);
		}
	}

	return Result;
}

FMatrix FMatrix::TransposeAdjoint() const noexcept
{
	FMatrix TA;

	TA.M[0][0] = this->M[1][1] * this->M[2][2] - this->M[1][2] * this->M[2][1];
	TA.M[0][1] = this->M[1][2] * this->M[2][0] - this->M[1][0] * this->M[2][2];
	TA.M[0][2] = this->M[1][0] * this->M[2][1] - this->M[1][1] * this->M[2][0];
	TA.M[0][3] = 0.f;

	TA.M[1][0] = this->M[2][1] * this->M[0][2] - this->M[2][2] * this->M[0][1];
	TA.M[1][1] = this->M[2][2] * this->M[0][0] - this->M[2][0] * this->M[0][2];
	TA.M[1][2] = this->M[2][0] * this->M[0][1] - this->M[2][1] * this->M[0][0];
	TA.M[1][3] = 0.f;

	TA.M[2][0] = this->M[0][1] * this->M[1][2] - this->M[0][2] * this->M[1][1];
	TA.M[2][1] = this->M[0][2] * this->M[1][0] - this->M[0][0] * this->M[1][2];
	TA.M[2][2] = this->M[0][0] * this->M[1][1] - this->M[0][1] * this->M[1][0];
	TA.M[2][3] = 0.f;

	TA.M[3][0] = 0.f;
	TA.M[3][1] = 0.f;
	TA.M[3][2] = 0.f;
	TA.M[3][3] = 1.f;

	return TA;
}

void FMatrix::RemoveScaling(float Tolerance) noexcept
{
	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	const float SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const float SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const float SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);
	const float Scale0 = FMath::FloatSelect(SquareSum0 - Tolerance, FMath::InvSqrt(SquareSum0), 1.0f);
	const float Scale1 = FMath::FloatSelect(SquareSum1 - Tolerance, FMath::InvSqrt(SquareSum1), 1.0f);
	const float Scale2 = FMath::FloatSelect(SquareSum2 - Tolerance, FMath::InvSqrt(SquareSum2), 1.0f);
	M[0][0] *= Scale0;
	M[0][1] *= Scale0;
	M[0][2] *= Scale0;
	M[1][0] *= Scale1;
	M[1][1] *= Scale1;
	M[1][2] *= Scale1;
	M[2][0] *= Scale2;
	M[2][1] *= Scale2;
	M[2][2] *= Scale2;
}

FVector3D FMatrix::ExtractScaling(float Tolerance) noexcept
{
	FVector3D Scale3D(0, 0, 0);

	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	const float SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const float SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const float SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);


	if (SquareSum0 > Tolerance)
	{
		float Scale0 = FMath::Sqrt(SquareSum0);
		Scale3D.X = Scale0;
		float InvScale0 = 1.f / Scale0;
		M[0][0] *= InvScale0;
		M[0][1] *= InvScale0;
		M[0][2] *= InvScale0;
	}
	else
	{
		Scale3D.X = 0;
	}


	if (SquareSum1 > Tolerance)
	{
		float Scale1 = FMath::Sqrt(SquareSum1);
		Scale3D.Y = Scale1;
		float InvScale1 = 1.f / Scale1;
		M[1][0] *= InvScale1;
		M[1][1] *= InvScale1;
		M[1][2] *= InvScale1;
	}
	else
	{
		Scale3D.Y = 0;
	}


	if (SquareSum2 > Tolerance)
	{
		float Scale2 = FMath::Sqrt(SquareSum2);
		Scale3D.Z = Scale2;
		float InvScale2 = 1.f / Scale2;
		M[2][0] *= InvScale2;
		M[2][1] *= InvScale2;
		M[2][2] *= InvScale2;
	}
	else
	{
		Scale3D.Z = 0;
	}


	return Scale3D;
}




FMatrix FMatrix::ConcatTranslation(const FVector3D& Translation) const noexcept
{
	FMatrix Result;

	float* RESTRICT Dest = &Result.M[0][0];
	const float* RESTRICT Src = &M[0][0];
	const float* RESTRICT Trans = &Translation.X;

	Dest[0] = Src[0];
	Dest[1] = Src[1];
	Dest[2] = Src[2];
	Dest[3] = Src[3];
	Dest[4] = Src[4];
	Dest[5] = Src[5];
	Dest[6] = Src[6];
	Dest[7] = Src[7];
	Dest[8] = Src[8];
	Dest[9] = Src[9];
	Dest[10] = Src[10];
	Dest[11] = Src[11];
	Dest[12] = Src[12] + Trans[0];
	Dest[13] = Src[13] + Trans[1];
	Dest[14] = Src[14] + Trans[2];
	Dest[15] = Src[15];

	return Result;
}


void FMatrix::SetAxes(FVector3D* Axis0, FVector3D* Axis1, FVector3D* Axis2, FVector3D* Origin) noexcept
{
	if (Axis0 != NULL)
	{
		M[0][0] = Axis0->X;
		M[0][1] = Axis0->Y;
		M[0][2] = Axis0->Z;
	}
	if (Axis1 != NULL)
	{
		M[1][0] = Axis1->X;
		M[1][1] = Axis1->Y;
		M[1][2] = Axis1->Z;
	}
	if (Axis2 != NULL)
	{
		M[2][0] = Axis2->X;
		M[2][1] = Axis2->Y;
		M[2][2] = Axis2->Z;
	}
	if (Origin != NULL)
	{
		M[3][0] = Origin->X;
		M[3][1] = Origin->Y;
		M[3][2] = Origin->Z;
	}
}

void FMatrix::Mirror(EAxis MirrorAxis, EAxis FlipAxis) noexcept
{
	if (MirrorAxis == EAxis::X)
	{
		M[0][0] *= -1.f;
		M[1][0] *= -1.f;
		M[2][0] *= -1.f;

		M[3][0] *= -1.f;
	}
	else if (MirrorAxis == EAxis::Y)
	{
		M[0][1] *= -1.f;
		M[1][1] *= -1.f;
		M[2][1] *= -1.f;

		M[3][1] *= -1.f;
	}
	else if (MirrorAxis == EAxis::Z)
	{
		M[0][2] *= -1.f;
		M[1][2] *= -1.f;
		M[2][2] *= -1.f;

		M[3][2] *= -1.f;
	}

	if (FlipAxis == EAxis::X)
	{
		M[0][0] *= -1.f;
		M[0][1] *= -1.f;
		M[0][2] *= -1.f;
	}
	else if (FlipAxis == EAxis::Y)
	{
		M[1][0] *= -1.f;
		M[1][1] *= -1.f;
		M[1][2] *= -1.f;
	}
	else if (FlipAxis == EAxis::Z)
	{
		M[2][0] *= -1.f;
		M[2][1] *= -1.f;
		M[2][2] *= -1.f;
	}
}






FRotator FMatrix::Rotator() const
{
	const FVector3D		XAxis = GetScaledAxis(EAxis::X);
	const FVector3D		YAxis = GetScaledAxis(EAxis::Y);
	const FVector3D		ZAxis = GetScaledAxis(EAxis::Z);

	FRotator Rotator = FRotator
	(
		FMath::Atan2(XAxis.Z, FMath::Sqrt(FMath::Square(XAxis.X) + FMath::Square(XAxis.Y))) * 180.f / PI,
		FMath::Atan2(XAxis.Y, XAxis.X) * 180.f / PI,
		0
	);

	const FVector3D	SYAxis = FRotationMatrix(Rotator).GetScaledAxis(EAxis::Y);
	Rotator.Roll = FMath::Atan2(ZAxis | SYAxis, YAxis | SYAxis) * 180.f / PI;

	return Rotator;
}

FQuat FMatrix::ToQuat() const
{
	FQuat Result(*this);
	return Result;
}




void FMatrix::To3x4MatrixTranspose(float* Out) const noexcept
{
	const float* RESTRICT Src = &(M[0][0]);
	float* RESTRICT Dest = Out;

	Dest[0] = Src[0];   // [0][0]
	Dest[1] = Src[4];   // [1][0]
	Dest[2] = Src[8];   // [2][0]
	Dest[3] = Src[12];  // [3][0]

	Dest[4] = Src[1];   // [0][1]
	Dest[5] = Src[5];   // [1][1]
	Dest[6] = Src[9];   // [2][1]
	Dest[7] = Src[13];  // [3][1]

	Dest[8] = Src[2];   // [0][2]
	Dest[9] = Src[6];   // [1][2]
	Dest[10] = Src[10]; // [2][2]
	Dest[11] = Src[14]; // [3][2]
}
