// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "Matrix.h"
#include "Quat.h"
#include "Vector3D.h"




/** 
	Rotation and translation matrix using quaternion rotation.
*/
struct ENGINE_API FQuatRotationTranslationMatrix : public FMatrix
{
public:

	/**
		@param Q - Rotation.
		@param Origin - Translation to apply.
	*/
	FORCEINLINE FQuatRotationTranslationMatrix(const FQuat& Q, const FVector3D& Origin) noexcept
	{
		// clang-format off
		const float x2 = Q.X + Q.X;  const float y2 = Q.Y + Q.Y;  const float z2 = Q.Z + Q.Z;
		const float xx = Q.X * x2;   const float xy = Q.X * y2;   const float xz = Q.X * z2;
		const float yy = Q.Y * y2;   const float yz = Q.Y * z2;   const float zz = Q.Z * z2;
		const float wx = Q.W * x2;   const float wy = Q.W * y2;   const float wz = Q.W * z2;

		M[0][0] = 1.0f - (yy + zz);	M[1][0] = xy - wz;				M[2][0] = xz + wy;			M[3][0] = Origin.X;
		M[0][1] = xy + wz;			M[1][1] = 1.0f - (xx + zz);		M[2][1] = yz - wx;			M[3][1] = Origin.Y;
		M[0][2] = xz - wy;			M[1][2] = yz + wx;				M[2][2] = 1.0f - (xx + yy);	M[3][2] = Origin.Z;
		M[0][3] = 0.0f;				M[1][3] = 0.0f;					M[2][3] = 0.0f;				M[3][3] = 1.0f;
		// clang-format on
	}


public:

	/** 
		Matrix factory.

		@return FMatrix so we don't have type conversion issues in expressions.
	*/
	static FORCEINLINE FMatrix Make(const FQuat& Q, const FVector3D& Origin) noexcept { return FQuatRotationTranslationMatrix(Q, Origin); }
};


/** 
	Rotation matrix using quaternion rotation.
*/
struct ENGINE_API FQuatRotationMatrix : public FQuatRotationTranslationMatrix
{
public:

	/**
		@param Q - Rotation,
	*/
	FORCEINLINE FQuatRotationMatrix(const FQuat& Q) noexcept : FQuatRotationTranslationMatrix(Q, FVector3D::ZeroVector) { }


public:

	/** 
		Matrix factory.

		@return FMatrix so we don't have type conversion issues in expressions.
	*/
	static FORCEINLINE FMatrix Make(const FQuat& Q) noexcept { return FQuatRotationMatrix(Q); }
};
