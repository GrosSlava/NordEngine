// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "EngineMath.h"




struct FVector2D;
struct FVector3D;
struct FLinearColor;
struct FRotator;
struct FQuat;


/**
	A 4D homogeneous vector, 4x1 FLOATs, 16-byte aligned.
*/
MS_ALIGN(16) struct ENGINE_API FVector4D
{
public:

	/**
		Creates and initializes a new vector from the specified components.
	
		@param InX - X Coordinate.
		@param InY - Y Coordinate.
		@param InZ - Z Coordinate.
		@param InW - W Coordinate.
	*/
	explicit FORCEINLINE FVector4D(float InX = 0.0f, float InY = 0.0f, float InZ = 0.0f, float InW = 1.0f) noexcept : X(InX), Y(InY), Z(InZ), W(InW) { }
	/**
		@param InVector - 3D Vector to set first three components.
		@param InW - W Coordinate.
	*/
	FVector4D(const FVector3D& InVector, float InW = 1.0f) noexcept;
	/**
		Creates and initializes a new vector from a color value.

		@param InColour - Color used to set vector.
	*/
	FVector4D(const FLinearColor& InColor) noexcept;
	/**
		Creates and initializes a new vector from the specified 2D vectors.
	 
		@param InXY - A 2D vector holding the X- and Y-components.
		@param InZW - A 2D vector holding the Z- and W-components.
	*/
	explicit FVector4D(const FVector2D& InXY, const FVector2D& InZW) noexcept;


public:

	/**
		Gets a negated copy of the vector.
	 
		@return A negated copy of the vector.
	*/
	FORCEINLINE FVector4D operator-() const noexcept { return FVector4D(-X, -Y, -Z, -W); }
	/**
	  Gets the result of subtracting a vector from this.
	 
	  @param V The vector to subtract.
	  @return The result of vector subtraction.
	*/
	FORCEINLINE FVector4D operator-(const FVector4D& V) const noexcept { return FVector4D(X - V.X, Y - V.Y, Z - V.Z, W - V.W); }

	/**
	  Subtracts another vector to this one.
	 
	  @param V The other vector to subtract.
	  @return Copy of the vector after subtraction.
	*/
	FORCEINLINE FVector4D operator-=(const FVector4D& V) noexcept
	{
		X -= V.X;
		Y -= V.Y;
		Z -= V.Z;
		W -= V.W;
		return *this;
	}
	/**
		Gets the result of adding a vector to this.
	 
		@param V - The vector to add.
		@return The result of vector addition.
	*/
	FORCEINLINE FVector4D operator+(const FVector4D& V) const noexcept { return FVector4D(X + V.X, Y + V.Y, Z + V.Z, W + V.W); }
	/**
		Adds another vector to this one.
	 
		@param V - The other vector to add.
		@return Copy of the vector after addition.
	 */
	FORCEINLINE FVector4D operator+=(const FVector4D& V) noexcept
	{
		X += V.X;
		Y += V.Y;
		Z += V.Z;
		W += V.W;
		return *this;
	}
	/**
		Gets the result of scaling this vector.
	 
		@param Scale - The scaling factor.
		@return The result of vector scaling.
	*/
	FORCEINLINE FVector4D operator*(float Scale) const noexcept { return FVector4D(X * Scale, Y * Scale, Z * Scale, W * Scale); }
	/**
		Scales a vector.
	 
		@param Scale - The scaling factor.
		@param V - The vector to scale.
		@return The result of scaling.
	*/
	friend FORCEINLINE FVector4D operator*(float Scale, const FVector4D& V) noexcept { return V.operator*(Scale); }
	/**
		Gets the result of multiplying a vector with this.
	 
		@param V - The vector to multiply.
		@return The result of vector multiplication.
	*/
	FORCEINLINE FVector4D operator*(const FVector4D& V) const noexcept { return FVector4D(X * V.X, Y * V.Y, Z * V.Z, W * V.W); }
	/**
		Gets the result of scaling this vector.
	 
		@param Scale - The scaling factor.
		@return The result of vector scaling.
	*/
	FORCEINLINE FVector4D operator*=(float S) noexcept
	{
		X *= S;
		Y *= S;
		Z *= S;
		W *= S;
		return *this;
	}
	/**
		Gets the result of multiplying a vector with another Vector (component wise).
	 
		@param V - The vector to multiply.
		@return The result of vector multiplication.
	*/
	FORCEINLINE FVector4D operator*=(const FVector4D& V) noexcept
	{
		X *= V.X;
		Y *= V.Y;
		Z *= V.Z;
		W *= V.W;
		return *this;
	}
	/**
		Gets the result of dividing this vector.
	 
		@param Scale - What to divide by.
		@return The result of division.
	*/
	FORCEINLINE FVector4D operator/(float Scale) const
	{
		const float RScale = 1.f / Scale;
		return FVector4D(X * RScale, Y * RScale, Z * RScale, W * RScale);
	}
	/**
		Gets the result of dividing this vector.
	 
		@param V - What to divide by.
		@return The result of division.
	*/
	FORCEINLINE FVector4D operator/(const FVector4D& V) const { return FVector4D(X / V.X, Y / V.Y, Z / V.Z, W / V.W); }
	/**
		Gets the result of dividing a vector with another Vector (component wise).
	 
		@param V - The vector to divide with.
		@return The result of vector multiplication.
	*/
	FORCEINLINE FVector4D operator/=(const FVector4D& V)
	{
		X /= V.X;
		Y /= V.Y;
		Z /= V.Z;
		W /= V.W;
		return *this;
	}
	/**
		Calculate Cross product between this and another vector.
	 
		@param V - The other vector.
		@return The Cross product.
	*/
	FORCEINLINE FVector4D operator^(const FVector4D& V) const noexcept { return FVector4D(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X, 0.0f); }

public:

	/**
		Checks for equality against another vector.
	 
		@param V - The other vector.
		@return true if the two vectors are the same, otherwise false.
	*/
	FORCEINLINE bool operator==(const FVector4D& V) const noexcept { return (X == V.X) && (Y == V.Y) && (Z == V.Z) && (W == V.W); }
	/**
		Checks for inequality against another vector.
	 
		@param V - The other vector.
		@return true if the two vectors are different, otherwise false.
	*/
	FORCEINLINE bool operator!=(const FVector4D& V) const noexcept { return (X != V.X) || (Y != V.Y) || (Z != V.Z) || (W != V.W); }

	/**
		Gets specific component of the vector.

		@param Index - the index of vector component.
		@return copy of component value.
	*/
	FORCEINLINE float operator[](int32 Index) const noexcept
	{
		switch( Index )
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
		}

		return 0.0f;
	}

public:

	/**
		Calculates 3D Dot product of two 4D vectors.
	 
		@param V1 - The first vector.
		@param V2 - The second vector.
		@return The 3D Dot product.
	*/
	friend FORCEINLINE float Dot3(const FVector4D& V1, const FVector4D& V2) noexcept { return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z; }
	/**
		Calculates 4D Dot product.
	 
		@param V1 - The first vector.
		@param V2 - The second vector.
		@return The 4D Dot Product.
	*/
	friend FORCEINLINE float Dot4(const FVector4D& V1, const FVector4D& V2) noexcept { return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z + V1.W * V2.W; }

public:

	/**
		Calculate the cross product of two vectors.
	 
		@param A - The first vector.
		@param B - The second vector.
		@return The cross product.
	*/
	static FORCEINLINE FVector4D CrossProduct(const FVector4D& A, const FVector4D& B) noexcept { return A ^ B; }
	/**
		Calculate the dot product of two vectors.
	 
		@param A - The first vector.
		@param B - The second vector.
		@return The dot product.
	*/
	static FORCEINLINE float DotProduct(const FVector4D& A, const FVector4D& B) noexcept { return Dot4(A, B); }

public:

	/**
		Error tolerant comparison.
	 
		@param V - Vector to compare against.
		@param Tolerance - Error Tolerance.
		@return true if the two vectors are equal within specified tolerance, otherwise false.
	*/
	FORCEINLINE bool Equals(const FVector4D& V, float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return FMath::Abs(X - V.X) <= Tolerance && FMath::Abs(Y - V.Y) <= Tolerance && FMath::Abs(Z - V.Z) <= Tolerance && FMath::Abs(W - V.W) <= Tolerance;
	}

	/**
		Check if the vector is of unit length, with specified tolerance.
	 
		@param LengthSquaredTolerance - Tolerance against squared length.
		@return true if the vector is a unit vector within the specified tolerance.
	*/
	FORCEINLINE bool IsUnit3(float LengthSquaredTolerance = KINDA_SMALL_NUMBER) const noexcept { return FMath::Abs(1.0f - SizeSquared3()) < LengthSquaredTolerance; }

	/**
		Returns a normalized copy of the vector if safe to normalize.
	 
		@param Tolerance - Minimum squared length of vector for normalization.
		@return A normalized copy of the vector or a zero vector.
	*/
	FORCEINLINE FVector4D GetSafeNormal(float Tolerance = SMALL_NUMBER) const noexcept
	{
		const float SquareSum = X * X + Y * Y + Z * Z;
		if( SquareSum > Tolerance )
		{
			const float Scale = FMath::InvSqrt(SquareSum);
			return FVector4D(X * Scale, Y * Scale, Z * Scale, 0.0f);
		}
		return FVector4D(0.f);
	}
	/**
		Calculates normalized version of vector without checking if it is non-zero.
	 
		@return Normalized - version of vector.
	*/
	FORCEINLINE FVector4D GetUnsafeNormal3() const noexcept
	{
		const float Scale = FMath::InvSqrt(X * X + Y * Y + Z * Z);
		return FVector4D(X * Scale, Y * Scale, Z * Scale, 0.0f);
	}

	/**
		Set all of the vectors coordinates.
	 
		@param InX - New X Coordinate.
		@param InY - New Y Coordinate.
		@param InZ - New Z Coordinate.
		@param InW - New W Coordinate.
	*/
	FORCEINLINE void Set(float InX, float InY, float InZ, float InW) noexcept
	{
		X = InX;
		Y = InY;
		Z = InZ;
		W = InW;
	}

	/**
		Get the length of this vector not taking W component into account.
	 
		@return The length of this vector.
	*/
	FORCEINLINE float Size3() const noexcept { return FMath::Sqrt(X * X + Y * Y + Z * Z); }
	/**
		Get the squared length of this vector not taking W component into account.
	 
		@return The squared length of this vector.
	*/
	FORCEINLINE float SizeSquared3() const noexcept { return X * X + Y * Y + Z * Z; }

	/**
	  Get the length (magnitude) of this vector, taking the W component into account
	
	  @return The length of this vector
	*/
	FORCEINLINE float Size() const noexcept { return FMath::Sqrt(SizeSquared()); }
	/**
		Get the squared length of this vector, taking the W component into account
	 
		@return The squared length of this vector
	*/
	FORCEINLINE float SizeSquared() const noexcept { return X * X + Y * Y + Z * Z + W * W; }

	/** 
		Utility to check if there are any non-finite values (NaN or Inf) in this vector.
	*/
	FORCEINLINE bool ContainsNaN() const noexcept { return (!FMath::IsFinite(X) || !FMath::IsFinite(Y) || !FMath::IsFinite(Z) || !FMath::IsFinite(W)); }

	/** 
		Utility to check if all of the components of this vector are nearly zero given the tolerance. 
	*/
	FORCEINLINE bool IsNearlyZero3(float Tolerance = KINDA_SMALL_NUMBER) const noexcept { return FMath::Abs(X) <= Tolerance && FMath::Abs(Y) <= Tolerance && FMath::Abs(Z) <= Tolerance; }

	/** 
		Reflect vector.
	*/
	FORCEINLINE FVector4D Reflect3(const FVector4D& Normal) const noexcept { return 2.0f * Dot3(*this, Normal) * Normal - *this; }

	/**
		Find good arbitrary axis vectors to represent U and V axes of a plane, given just the normal.
	*/
	FORCEINLINE void FindBestAxisVectors3(FVector4D& Axis1, FVector4D& Axis2) const noexcept
	{
		const float NX = FMath::Abs(X);
		const float NY = FMath::Abs(Y);
		const float NZ = FMath::Abs(Z);

		// Find best basis vectors.
		if( NZ > NX && NZ > NY )
		{
			Axis1 = FVector4D(1, 0, 0);
		}
		else
		{
			Axis1 = FVector4D(0, 0, 1);
		}

		Axis1 = (Axis1 - *this * Dot3(Axis1, *this)).GetSafeNormal();
		Axis2 = Axis1 ^ *this;
	}

public:

	/**
		Return the FRotator orientation corresponding to the direction in which the vector points.
		Sets Yaw and Pitch to the proper numbers, and sets roll to zero because the roll can't be determined from a vector.

		@return FRotator from the Vector's direction.
	*/
	FRotator ToOrientationRotator() const noexcept;
	/**
		Return the Quaternion orientation corresponding to the direction in which the vector points.

		@return Quaternion from the Vector's direction.
	*/
	FQuat ToOrientationQuat() const noexcept;
	/**
		Return the FRotator orientation corresponding to the direction in which the vector points.
		Sets Yaw and Pitch to the proper numbers, and sets roll to zero because the roll can't be determined from a vector.
		Identical to 'ToOrientationRotator()'.
		@see ToOrientationRotator()

		@return FRotator from the Vector's direction.
	*/
	FRotator Rotation() const noexcept;



public:

	/**
		Vector's X-component.
	*/
	float X = 0.0f;
	/** 
		Vector's Y-component. 
	*/
	float Y = 0.0f;
	/** 
		Vector's Z-component. 
	*/
	float Z = 0.0f;
	/** 
		Vector's W-component. 
	*/
	float W = 0.0f;
};
