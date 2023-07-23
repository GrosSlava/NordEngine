// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "Vector3D.h"




struct FMatrix;
struct FVector4D;


/**
	Structure for three dimensional planes.
 
	Stores the coeffecients as Xx+Yy+Zz=W.
	Note that this is different from many other Plane classes that use Xx+Yy+Zz+W=0.
*/
MS_ALIGN(16) struct ENGINE_API FPlane : public FVector3D
{
public:

	FPlane() = default;
	/**
		@param InX - X-coefficient.
		@param InY - Y-coefficient.
		@param InZ - Z-coefficient.
		@param InW - W-coefficient.
	*/
	FORCEINLINE FPlane(float InX, float InY, float InZ, float InW) noexcept : FVector3D(InX, InY, InZ), W(InW) { }
	/**
		@param InNormal - Plane Normal Vector.
		@param InW - Plane W-coefficient.
	*/
	FORCEINLINE FPlane(FVector3D InNormal, float InW) noexcept : FVector3D(InNormal), W(InW) { }
	/**
		@param InBase - Base point in plane.
		@param InNormal - Plane Normal Vector.
	*/
	FORCEINLINE FPlane(FVector3D InBase, const FVector3D& InNormal) noexcept : FVector3D(InNormal), W(InBase | InNormal) { }
	/**
		@param A - First point in the plane.
		@param B - Second point in the plane.
		@param C - Third point in the plane.
	*/
	FORCEINLINE FPlane(FVector3D A, FVector3D B, FVector3D C) noexcept : FVector3D(((B - A) ^ (C - A)).GetSafeNormal()) { W = A | (FVector3D)(*this); }
	/**
		@param V - 4D vector to set up plane.
	*/
	FPlane(const FVector4D& V) noexcept;


public:

	/**
		Gets result of subtracting a plane from this.
	 
		@param V - The other plane.
		@return The result of subtracting a plane from this.
	*/
	FORCEINLINE FPlane operator-(const FPlane& V) const noexcept { return FPlane(X - V.X, Y - V.Y, Z - V.Z, W - V.W); }
	/**
		Subtract another plane from this.
	 
		@param V - The other plane.
		@return Copy of plane after subtraction.
	*/
	FORCEINLINE FPlane operator-=(const FPlane& V) noexcept
	{
		X -= V.X;
		Y -= V.Y;
		Z -= V.Z;
		W -= V.W;
		return *this;
	}
	/**
		Gets result of adding a plane to this.
	 
		@param V - The other plane.
		@return The result of adding a plane to this.
	*/
	FORCEINLINE FPlane operator+(const FPlane& V) const noexcept { return FPlane(X + V.X, Y + V.Y, Z + V.Z, W + V.W); }
	/**
		Add another plane to this.
	 
		@param V - The other plane.
		@return Copy of plane after addition.
	*/
	FORCEINLINE FPlane operator+=(const FPlane& V) noexcept
	{
		X += V.X;
		Y += V.Y;
		Z += V.Z;
		W += V.W;
		return *this;
	}
	/**
		Gets result of dividing a plane.
	 
		@param Scale - What to divide by.
		@return The result of division.
	*/
	FORCEINLINE FPlane operator/(float Scale) const
	{
		const float RScale = 1.f / Scale;
		return FPlane(X * RScale, Y * RScale, Z * RScale, W * RScale);
	}
	/**
		Divide this plane.
	 
		@param V - What to divide by.
		@return Copy of plane after division.
	*/
	FORCEINLINE FPlane operator/=(float V)
	{
		const float RV = 1.f / V;
		X *= RV;
		Y *= RV;
		Z *= RV;
		W *= RV;
		return *this;
	}
	/**
		Gets result of scaling a plane.
	 
		@param Scale - The scaling factor.
		@return The result of scaling.
	*/
	FORCEINLINE FPlane operator*(float Scale) const noexcept { return FPlane(X * Scale, Y * Scale, Z * Scale, W * Scale); }
	/**
		Gets result of multiplying a plane with this.
	 
		@param V - The other plane.
		@return The result of multiplying a plane with this.
	*/
	FORCEINLINE FPlane operator*(const FPlane& V) noexcept { return FPlane(X * V.X, Y * V.Y, Z * V.Z, W * V.W); }
	/**
		Scale this plane.
	 
		@param Scale - The scaling factor.
		@return Copy of plane after scaling.
	*/
	FORCEINLINE FPlane operator*=(float Scale) noexcept
	{
		X *= Scale;
		Y *= Scale;
		Z *= Scale;
		W *= Scale;
		return *this;
	}
	/**
		Multiply another plane with this.
	 
		@param V - The other plane.
		@return Copy of plane after multiplication.
	*/
	FORCEINLINE FPlane operator*=(const FPlane& V) noexcept
	{
		X *= V.X;
		Y *= V.Y;
		Z *= V.Z;
		W *= V.W;
		return *this;
	}
	/**
		Calculates dot product of two planes.
	 
		@param V - The other plane.
		@return The dot product.
	*/
	FORCEINLINE float operator|(const FPlane& V) const noexcept { return X * V.X + Y * V.Y + Z * V.Z + W * V.W; }

public:

	/**
		Check if two planes are identical.
	 
		@param V - The other plane.
		@return true if planes are identical, otherwise false.
	*/
	FORCEINLINE bool operator==(const FPlane& V) const { return (X == V.X) && (Y == V.Y) && (Z == V.Z) && (W == V.W); }
	/**
		Check if two planes are different.
	 
		@param V - The other plane.
		@return true if planes are different, otherwise false.
	*/
	FORCEINLINE bool operator!=(const FPlane& V) const { return (X != V.X) || (Y != V.Y) || (Z != V.Z) || (W != V.W); }

public:

	/**
		Checks whether two planes are equal within specified tolerance.
	 
		@param V - The other plane.
		@param Tolerance - Error Tolerance.
		@return true if the two planes are equal within specified tolerance, otherwise false.
	*/
	FORCEINLINE bool Equals(const FPlane& V, float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return (FMath::Abs(X - V.X) < Tolerance) && (FMath::Abs(Y - V.Y) < Tolerance) && (FMath::Abs(Z - V.Z) < Tolerance) && (FMath::Abs(W - V.W) < Tolerance);
	}

	/**
		Checks if this plane is valid (ie: if it has a non-zero normal).
	 
		@return true if the plane is well-defined (has a non-zero normal), otherwise false.
	*/
	FORCEINLINE bool IsValid() const noexcept { return !IsNearlyZero(); }

	/**
		Get the origin of this plane.
	 
		@return The origin (base point) of this plane.
	*/
	FORCEINLINE FVector3D GetOrigin() const noexcept { return GetNormal() * W; }

	/**
		Get the normal of this plane.
	 
		@return The normal of this plane.
	*/
	FORCEINLINE const FVector3D& GetNormal() const noexcept { return *this; }

	/**
		Calculates distance between plane and a point.
	 
		@param P - The other point.
		@return The distance from the plane to the point. 0: Point is on the plane. >0: Point is in front of the plane. <0: Point is behind the plane.
	*/
	FORCEINLINE float PlaneDot(const FVector3D& P) const noexcept { return X * P.X + Y * P.Y + Z * P.Z - W; }

	/**
		Normalize this plane in-place if it is larger than a given tolerance. Leaves it unchanged if not.
	 
		@param Tolerance - Minimum squared length of vector for normalization.
		@return true if the plane was normalized correctly, false otherwise.
	*/
	FORCEINLINE bool Normalize(float Tolerance = SMALL_NUMBER) noexcept
	{
		const float SquareSum = X * X + Y * Y + Z * Z;
		if( SquareSum > Tolerance )
		{
			const float Scale = FMath::InvSqrt(SquareSum);
			X *= Scale;
			Y *= Scale;
			Z *= Scale;
			W *= Scale;
			return true;
		}
		return false;
	}

	/**
		Get a flipped version of the plane.
	 
		@return A flipped version of the plane.
	*/
	FORCEINLINE FPlane Flip() const noexcept { return FPlane(-X, -Y, -Z, -W); }

public:

	/**
		Get the result of transforming the plane by a Matrix.
	 
		@param M - The matrix to transform plane with.
		@return The result of transform.
	*/
	FPlane TransformBy(const FMatrix& M) const noexcept;
	/**
		You can optionally pass in the matrices transpose-adjoint, which save it recalculating it.
		MSM: If we are going to save the transpose-adjoint we should also save the more expensive  determinant.
	 
		@param M - The Matrix to transform plane with.
		@param DetM - Determinant of Matrix.
		@param TA - Transpose-adjoint of Matrix.
		@return The result of transform.
	*/
	FPlane TransformByUsingAdjointT(const FMatrix& M, float DetM, const FMatrix& TA) const noexcept;



public:

	/** 
		The w-component. 
	*/
	float W = 0.0f;
};
