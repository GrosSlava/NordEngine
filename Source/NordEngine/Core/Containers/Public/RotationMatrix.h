// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "Matrix.h"
#include "QuatRotationTranslationMatrix.h"
#include "Vector3D.h"
#include "Rotator.h"




/**
	Combined rotation and translation matrix.
*/
struct ENGINE_API FRotationTranslationMatrix : public FMatrix
{
public:

	/**
		@param Rot - Rotation.
		@param Origin - Translation to apply.
	*/
	FRotationTranslationMatrix(const FRotator& Rot, const FVector3D& Origin) noexcept
	{
		const VectorRegister Angles = MakeVectorRegister(Rot.Pitch, Rot.Yaw, Rot.Roll, 0.0f);
		const VectorRegister HalfAngles = VectorMultiply(Angles, GlobalVectorConstants::DEG_TO_RAD);

		union
		{
			VectorRegister v;
			float f[4];
		} SinAngles, CosAngles;
		VectorSinCos(&SinAngles.v, &CosAngles.v, &HalfAngles);


		const float SP = SinAngles.f[0];
		const float SY = SinAngles.f[1];
		const float SR = SinAngles.f[2];
		const float CP = CosAngles.f[0];
		const float CY = CosAngles.f[1];
		const float CR = CosAngles.f[2];

		M[0][0] = CP * CY;
		M[0][1] = CP * SY;
		M[0][2] = SP;
		M[0][3] = 0.f;

		M[1][0] = SR * SP * CY - CR * SY;
		M[1][1] = SR * SP * SY + CR * CY;
		M[1][2] = -SR * CP;
		M[1][3] = 0.f;

		M[2][0] = -(CR * SP * CY + SR * SY);
		M[2][1] = CY * SR - CR * SP * SY;
		M[2][2] = CR * CP;
		M[2][3] = 0.f;

		M[3][0] = Origin.X;
		M[3][1] = Origin.Y;
		M[3][2] = Origin.Z;
		M[3][3] = 1.f;
	}


public:

	/** 
		Matrix factory. 
		
		@return FMatrix so we don't have type conversion issues in expressions. 
	*/
	static FORCEINLINE FMatrix Make(const FRotator& Rot, const FVector3D& Origin) { return FRotationTranslationMatrix(Rot, Origin); }
};


/** 
	Rotation matrix no translation.
*/
struct ENGINE_API FRotationMatrix : public FRotationTranslationMatrix
{
public:

	/**
		@param Rot - Rotation.
	*/
	FORCEINLINE FRotationMatrix(const FRotator& Rot) : FRotationTranslationMatrix(Rot, FVector3D::ZeroVector) { }


public:

	/**
		Matrix factory. 

		@return FMatrix so we don't have type conversion issues in expressions.
	*/
	static FORCEINLINE FMatrix Make(FRotator const& Rot) noexcept { return FRotationMatrix(Rot); }
	/** 
		Matrix factory.
	
		@return FMatrix so we don't have type conversion issues in expressions.
	*/
	static FORCEINLINE FMatrix Make(FQuat const& Rot) noexcept { return FQuatRotationTranslationMatrix(Rot, FVector3D::ZeroVector); }

	/** 
		Builds a rotation matrix given only a XAxis. 
		Y and Z are unspecified but will be orthonormal. 
		XAxis need not be normalized. 
	*/
	static FMatrix MakeFromX(FVector3D const& XAxis) noexcept
	{
		FVector3D const NewX = XAxis.GetSafeNormal();

		// try to use up if possible
		FVector3D const UpVector = (FMath::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector3D(0, 0, 1.f) : FVector3D(1.f, 0, 0);

		const FVector3D NewY = (UpVector ^ NewX).GetSafeNormal();
		const FVector3D NewZ = NewX ^ NewY;

		return FMatrix(NewX, NewY, NewZ, FVector3D::ZeroVector);
	}
	/** 
		Builds a rotation matrix given only a YAxis. X and Z are unspecified but will be orthonormal. 
		YAxis need not be normalized. 
	*/
	static FMatrix MakeFromY(FVector3D const& YAxis) noexcept
	{
		FVector3D const NewY = YAxis.GetSafeNormal();

		// try to use up if possible
		FVector3D const UpVector = (FMath::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector3D(0, 0, 1.f) : FVector3D(1.f, 0, 0);

		const FVector3D NewZ = (UpVector ^ NewY).GetSafeNormal();
		const FVector3D NewX = NewY ^ NewZ;

		return FMatrix(NewX, NewY, NewZ, FVector3D::ZeroVector);
	}
	/** 
		Builds a rotation matrix given only a ZAxis. 
		X and Y are unspecified but will be orthonormal. 
		ZAxis need not be normalized. 
	*/
	static FMatrix MakeFromZ(FVector3D const& ZAxis) noexcept
	{
		FVector3D const NewZ = ZAxis.GetSafeNormal();

		// try to use up if possible
		FVector3D const UpVector = (FMath::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector3D(0, 0, 1.f) : FVector3D(1.f, 0, 0);

		const FVector3D NewX = (UpVector ^ NewZ).GetSafeNormal();
		const FVector3D NewY = NewZ ^ NewX;

		return FMatrix(NewX, NewY, NewZ, FVector3D::ZeroVector);
	}
	/** 
		Builds a matrix with given X and Y axes.
		X will remain fixed, Y may be changed minimally to enforce orthogonality. 
		Z will be computed. 
		Inputs need not be normalized. 
	*/
	static FMatrix MakeFromXY(FVector3D const& XAxis, FVector3D const& YAxis) noexcept
	{
		FVector3D NewX = XAxis.GetSafeNormal();
		FVector3D Norm = YAxis.GetSafeNormal();

		// if they're almost same, we need to find arbitrary vector
		if( FMath::IsNearlyEqual(FMath::Abs(NewX | Norm), 1.f) )
		{
			// make sure we don't ever pick the same as NewX
			Norm = (FMath::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector3D(0, 0, 1.f) : FVector3D(1.f, 0, 0);
		}

		const FVector3D NewZ = (NewX ^ Norm).GetSafeNormal();
		const FVector3D NewY = NewZ ^ NewX;

		return FMatrix(NewX, NewY, NewZ, FVector3D::ZeroVector);
	}
	/** 
		Builds a matrix with given X and Z axes. 
		X will remain fixed, Z may be changed minimally to enforce orthogonality.
		Y will be computed. 
		Inputs need not be normalized. 
	*/
	static FMatrix MakeFromXZ(FVector3D const& XAxis, FVector3D const& ZAxis) noexcept
	{
		FVector3D const NewX = XAxis.GetSafeNormal();
		FVector3D Norm = ZAxis.GetSafeNormal();

		// if they're almost same, we need to find arbitrary vector
		if( FMath::IsNearlyEqual(FMath::Abs(NewX | Norm), 1.f) )
		{
			// make sure we don't ever pick the same as NewX
			Norm = (FMath::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector3D(0, 0, 1.f) : FVector3D(1.f, 0, 0);
		}

		const FVector3D NewY = (Norm ^ NewX).GetSafeNormal();
		const FVector3D NewZ = NewX ^ NewY;

		return FMatrix(NewX, NewY, NewZ, FVector3D::ZeroVector);
	}
	/**
		Builds a matrix with given Y and X axes.
		Y will remain fixed, X may be changed minimally to enforce orthogonality.
		Z will be computed. 
		Inputs need not be normalized. 
	*/
	static FMatrix MakeFromYX(FVector3D const& YAxis, FVector3D const& XAxis) noexcept
	{
		FVector3D const NewY = YAxis.GetSafeNormal();
		FVector3D Norm = XAxis.GetSafeNormal();

		// if they're almost same, we need to find arbitrary vector
		if( FMath::IsNearlyEqual(FMath::Abs(NewY | Norm), 1.f) )
		{
			// make sure we don't ever pick the same as NewX
			Norm = (FMath::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector3D(0, 0, 1.f) : FVector3D(1.f, 0, 0);
		}

		const FVector3D NewZ = (Norm ^ NewY).GetSafeNormal();
		const FVector3D NewX = NewY ^ NewZ;

		return FMatrix(NewX, NewY, NewZ, FVector3D::ZeroVector);
	}
	/** 
		Builds a matrix with given Y and Z axes.
		Y will remain fixed, Z may be changed minimally to enforce orthogonality.
		X will be computed. 
		Inputs need not be normalized. 
	*/
	static FMatrix MakeFromYZ(FVector3D const& YAxis, FVector3D const& ZAxis) noexcept
	{
		FVector3D const NewY = YAxis.GetSafeNormal();
		FVector3D Norm = ZAxis.GetSafeNormal();

		// if they're almost same, we need to find arbitrary vector
		if( FMath::IsNearlyEqual(FMath::Abs(NewY | Norm), 1.f) )
		{
			// make sure we don't ever pick the same as NewX
			Norm = (FMath::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector3D(0, 0, 1.f) : FVector3D(1.f, 0, 0);
		}

		const FVector3D NewX = (NewY ^ Norm).GetSafeNormal();
		const FVector3D NewZ = NewX ^ NewY;

		return FMatrix(NewX, NewY, NewZ, FVector3D::ZeroVector);
	}
	/** 
		Builds a matrix with given Z and X axes. 
		Z will remain fixed, X may be changed minimally to enforce orthogonality. 
		Y will be computed. 
		Inputs need not be normalized.
	*/
	static FMatrix MakeFromZX(FVector3D const& ZAxis, FVector3D const& XAxis) noexcept
	{
		FVector3D const NewZ = ZAxis.GetSafeNormal();
		FVector3D Norm = XAxis.GetSafeNormal();

		// if they're almost same, we need to find arbitrary vector
		if( FMath::IsNearlyEqual(FMath::Abs(NewZ | Norm), 1.f) )
		{
			// make sure we don't ever pick the same as NewX
			Norm = (FMath::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector3D(0, 0, 1.f) : FVector3D(1.f, 0, 0);
		}

		const FVector3D NewY = (NewZ ^ Norm).GetSafeNormal();
		const FVector3D NewX = NewY ^ NewZ;

		return FMatrix(NewX, NewY, NewZ, FVector3D::ZeroVector);
	}
	/** 
		Builds a matrix with given Z and Y axes.
		Z will remain fixed, Y may be changed minimally to enforce orthogonality.
		X will be computed. 
		Inputs need not be normalized.
	*/
	static FMatrix MakeFromZY(FVector3D const& ZAxis, FVector3D const& YAxis) noexcept
	{
		FVector3D const NewZ = ZAxis.GetSafeNormal();
		FVector3D Norm = YAxis.GetSafeNormal();

		// if they're almost same, we need to find arbitrary vector
		if( FMath::IsNearlyEqual(FMath::Abs(NewZ | Norm), 1.f) )
		{
			// make sure we don't ever pick the same as NewX
			Norm = (FMath::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector3D(0, 0, 1.f) : FVector3D(1.f, 0, 0);
		}

		const FVector3D NewX = (Norm ^ NewZ).GetSafeNormal();
		const FVector3D NewY = NewZ ^ NewX;

		return FMatrix(NewX, NewY, NewZ, FVector3D::ZeroVector);
	}
};
