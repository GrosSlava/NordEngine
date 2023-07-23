// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "EngineMath.h"




/**
	A vector in 2-D space composed of components (X, Y) with floating point precision.
*/
struct ENGINE_API FVector2D
{
public:

	FVector2D() = default;
	/**
		Creates and initializes a new vector from the specified components.
	
		@param InX - X Coordinate.
		@param InY - Y Coordinate.
	*/
	FORCEINLINE FVector2D(float InX, float InY) noexcept : X(InX), Y(InY) { }
	FORCEINLINE FVector2D(const FVector2D& V2) noexcept : X(V2.X), Y(V2.Y) { }

public:

	/**
		Gets the result of subtracting A from each component of the vector.

		@param A - Float to subtract from each component
		@return The result of subtracting A from each component.
	*/
	FORCEINLINE FVector2D operator-(float A) const noexcept { return FVector2D(X - A, Y - A); }
	/**
		Gets the result of subtracting a vector from this one.

		@param V - The other vector to subtract from this.
		@return The result of the subtraction.
	*/
	FORCEINLINE FVector2D operator-(const FVector2D& V) const noexcept { return FVector2D(X - V.X, Y - V.Y); }
	/**
		Gets a negated copy of the vector.

		@return A negated copy of the vector.
	*/
	FORCEINLINE FVector2D operator-() const noexcept { return FVector2D(-X, -Y); }
	/**
		Subtracts another vector from this.

		@param V - The other vector to subtract.
		@return Copy of the vector after subtraction.
	*/
	FORCEINLINE FVector2D operator-=(const FVector2D& V) noexcept
	{
		X -= V.X;
		Y -= V.Y;
		return *this;
	}
	/**
		Gets the result of adding A to each component of the vector.

		@param A - Float to add to each component.
		@return The result of adding A to each component.
	*/
	FORCEINLINE FVector2D operator+(float A) const noexcept { return FVector2D(X + A, Y + A); }
	/**
		Gets the result of adding two vectors together.

		@param V - The other vector to add to this.
		@return The result of adding the vectors together.
	*/
	FORCEINLINE FVector2D operator+(const FVector2D& V) const noexcept { return FVector2D(X + V.X, Y + V.Y); }
	/**
		Adds another vector to this.

		@param V - The other vector to add.
		@return Copy of the vector after addition.
	*/
	FORCEINLINE FVector2D operator+=(const FVector2D& V) noexcept
	{
		X += V.X;
		Y += V.Y;
		return *this;
	}
	/**
		Gets the result of scaling the vector (multiplying each component by a value).

		@param Scale - How much to scale the vector by.
		@return The result of scaling this vector.
	*/
	FORCEINLINE FVector2D operator*(float Scale) const noexcept { return FVector2D(X * Scale, Y * Scale); }
	/**
		Gets the result of component-wise multiplication of this vector by another.

		@param V - The other vector to multiply this by.
		@return The result of the multiplication.
	*/
	FORCEINLINE FVector2D operator*(const FVector2D& V) const noexcept { return FVector2D(X * V.X, Y * V.Y); }
	/**
		Scales this vector.

		@param Scale - The scale to multiply vector by.
		@return Copy of the vector after scaling.
	*/
	FORCEINLINE FVector2D operator*=(float Scale) noexcept
	{
		X *= Scale;
		Y *= Scale;
		return *this;
	}
	/**
		Multiplies this vector with another vector, using component-wise multiplication.

		@param V - The vector to multiply with.
		@return Copy of the vector after multiplication.
	*/
	FORCEINLINE FVector2D operator*=(const FVector2D& V) noexcept
	{
		X *= V.X;
		Y *= V.Y;
		return *this;
	}
	/**
		Gets the result of dividing each component of the vector by a value.

		@param Scale - How much to divide the vector by.
		@return The result of division on this vector.
	*/
	FORCEINLINE FVector2D operator/(float Scale) const noexcept
	{
		const float RScale = 1.f / Scale;
		return FVector2D(X * RScale, Y * RScale);
	}
	/**
		Gets the result of component-wise division of this vector by another.

		@param V - The other vector to divide this by.
		@return The result of the division.
	*/
	FORCEINLINE FVector2D operator/(const FVector2D& V) const noexcept { return FVector2D(X / V.X, Y / V.Y); }
	/**
		Divides this vector.

		@param V - What to divide vector by.
		@return Copy of the vector after division.
	*/
	FORCEINLINE FVector2D operator/=(float V)
	{
		const float RV = 1.f / V;
		X *= RV;
		Y *= RV;
		return *this;
	}
	/**
		Divides this vector by another vector, using component-wise division.

		@param V - The vector to divide by.
		@return Copy of the vector after division.
	*/
	FORCEINLINE FVector2D operator/=(const FVector2D& V)
	{
		X /= V.X;
		Y /= V.Y;
		return *this;
	}
	/**
		Calculates dot product of this vector and another.

		@param V - The other vector.
		@return The dot product.
	*/
	FORCEINLINE float operator|(const FVector2D& V) const noexcept { return X * V.X + Y * V.Y; }
	/**
		Calculates cross product of this vector and another.

		@param V - The other vector.
		@return The cross product.
	*/
	FORCEINLINE float operator^(const FVector2D& V) const noexcept { return X * V.Y - Y * V.X; }

public:

	/**
		Compares this vector against another for equality.

		@param V - The vector to compare against.
		@return true if the two vectors are equal, otherwise false.
	*/
	FORCEINLINE bool operator==(const FVector2D& V) const noexcept { return X == V.X && Y == V.Y; }
	/**
		Compares this vector against another for inequality.

		@param V - The vector to compare against.
		@return true if the two vectors are not equal, otherwise false.
	*/
	FORCEINLINE bool operator!=(const FVector2D& V) const noexcept { return X != V.X || Y != V.Y; }
	/**
		Checks whether both components of this vector are less than another.

		@param Other - The vector to compare against.
		@return true if this is the smaller vector, otherwise false.
	*/
	FORCEINLINE bool operator<(const FVector2D& Other) const noexcept { return X < Other.X && Y < Other.Y; }
	/**
		Checks whether both components of this vector are greater than another.

		@param Other - The vector to compare against.
		@return true if this is the larger vector, otherwise false.
	*/
	FORCEINLINE bool operator>(const FVector2D& Other) const noexcept { return X > Other.X && Y > Other.Y; }
	/**
		Checks whether both components of this vector are less than or equal to another.

		@param Other - The vector to compare against.
		@return true if this vector is less than or equal to the other vector, otherwise false.
	*/
	FORCEINLINE bool operator<=(const FVector2D& Other) const noexcept { return X <= Other.X && Y <= Other.Y; }
	/**
		Checks whether both components of this vector are greater than or equal to another.

		@param Other - The vector to compare against.
		@return true if this vector is greater than or equal to the other vector, otherwise false.
	*/
	FORCEINLINE bool operator>=(const FVector2D& Other) const noexcept { return X >= Other.X && Y >= Other.Y; }

	/**
		Gets specific component of the vector.

		@param Index - the index of vector component.
		@return copy of component value.
	*/
	FORCEINLINE float operator[](int32 Index) const noexcept
	{
		if( Index < 0 || Index >= 2 ) return 0.0f;

		return ((Index == 0) ? X : Y);
	}

public:

	/**
		Calculate the cross product of two vectors.
	 
		@param A - The first vector.
		@param B - The second vector.
		@return The cross product.
	*/
	static FORCEINLINE float CrossProduct(const FVector2D& A, const FVector2D& B) noexcept { return A ^ B; }
	/**
		Calculates the dot product of two vectors.

		@param A - The first vector.
		@param B - The second vector.
		@return The dot product.
	*/
	static FORCEINLINE float DotProduct(const FVector2D& A, const FVector2D& B) noexcept { return A | B; }

	/**
		Squared distance between two 2D points.

		@param V1 - The first point.
		@param V2 - The second point.
		@return The squared distance between two 2D points.
	*/
	static FORCEINLINE float DistSquared(const FVector2D& V1, const FVector2D& V2) noexcept { return FMath::Square(V2.X - V1.X) + FMath::Square(V2.Y - V1.Y); }
	/**
		Distance between two 2D points.

		@param V1 - The first point.
		@param V2 - The second point.
		@return The distance between two 2D points.
	*/
	static FORCEINLINE float Distance(const FVector2D& V1, const FVector2D& V2) noexcept { return FMath::Sqrt(FVector2D::DistSquared(V1, V2)); }

	/**
		Returns a vector with the maximum component for each dimension from the pair of vectors.

		@param A - The first vector.
		@param B - The second vector.
		@return The max vector.
	*/
	static FORCEINLINE FVector2D Max(const FVector2D& A, const FVector2D& B) noexcept { return FVector2D(FMath::Max(A.X, B.X), FMath::Max(A.Y, B.Y)); }
	/**
		Returns a vector with the minimum component for each dimension from the pair of vectors.

		@param A - The first vector.
		@param B - The second vector.
		@return The min vector.
	*/
	static FORCEINLINE FVector2D Min(const FVector2D& A, const FVector2D& B) noexcept { return FVector2D(FMath::Min(A.X, B.X), FMath::Min(A.Y, B.Y)); }

	/** 
		Given a direction vector and a surface normal, returns the vector reflected across the surface normal.
		Produces a result like shining a laser at a mirror!
	 
		@param Direction - Direction vector the ray is coming from.
		@param SurfaceNormal - A normal of the surface the ray should be reflected on.
	 
		@returns Reflected vector.
	*/
	static FORCEINLINE FVector2D GetReflectionVector(const FVector2D& Direction, const FVector2D& SurfaceNormal)
	{
		const FVector2D SafeNormal(SurfaceNormal.GetSafeNormal());

		return Direction - SafeNormal * (Direction | SafeNormal) * 2;
	}

public:

	/**
		Checks for equality with error-tolerant comparison.

		@param V - The vector to compare.
		@param Tolerance - Error tolerance.
		@return true if the vectors are equal within specified tolerance, otherwise false.
	*/
	FORCEINLINE bool Equals(const FVector2D& V, float Tolerance = KINDA_SMALL_NUMBER) const noexcept { return FMath::Abs(X - V.X) <= Tolerance && FMath::Abs(Y - V.Y) <= Tolerance; }

	/**
		Set the values of the vector directly.

		@param InX - New X coordinate.
		@param InY - New Y coordinate.
	*/
	FORCEINLINE void Set(float InX, float InY) noexcept
	{
		X = InX;
		Y = InY;
	}

	/**
		Get the maximum value of the vector's components.

		@return The maximum value of the vector's components.
	*/
	FORCEINLINE float GetMax() const noexcept { return FMath::Max(X, Y); }
	/**
		Get the maximum absolute value of the vector's components.

		@return The maximum absolute value of the vector's components.
	*/
	FORCEINLINE float GetAbsMax() const noexcept { return FMath::Max(FMath::Abs(X), FMath::Abs(Y)); }
	/**
		Get the minimum value of the vector's components.

		@return The minimum value of the vector's components.
	*/
	FORCEINLINE float GetMin() const noexcept { return FMath::Min(X, Y); }
	/**
		Get the minimum absolute value of the vector's components.

		@return The minimum absolute value of the vector's components.
	*/
	FORCEINLINE float GetAbsMin() const noexcept { return FMath::Min(FMath::Abs(X), FMath::Abs(Y)); }

	/**
		Get the length (magnitude) of this vector.

		@return The length of this vector.
	*/
	FORCEINLINE float Size() const noexcept { return FMath::Sqrt(X * X + Y * Y); }
	/**
		Get the squared length of this vector.

		@return The squared length of this vector.
	*/
	FORCEINLINE float SizeSquared() const noexcept { return X * X + Y * Y; }

	/**
		Checks whether all components of the vector are exactly zero.

		@return true if vector is exactly zero, otherwise false.
	*/
	FORCEINLINE bool IsZero() const noexcept { return X == 0.0f && Y == 0.0f; }
	/**
		Checks whether vector is near to zero within a specified tolerance.

		@param Tolerance - Error tolerance.
		@return true if vector is in tolerance to zero, otherwise false.
	*/
	FORCEINLINE bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const noexcept { return FMath::Abs(X) <= Tolerance && FMath::Abs(Y) <= Tolerance; }

	/**
		Get this vector as a vector where each component has been rounded to the nearest int.

		@return New FVector2D from this vector that is rounded.
	*/
	FORCEINLINE FVector2D RoundToVector() const noexcept { return FVector2D(FMath::RoundToFloat(X), FMath::RoundToFloat(Y)); }

	/**
		Creates a copy of this vector with both axes clamped to the given range.

		@return New vector with clamped axes.
	*/
	FORCEINLINE FVector2D ClampAxes(float MinAxisVal, float MaxAxisVal) const noexcept { return FVector2D(FMath::Clamp(X, MinAxisVal, MaxAxisVal), FMath::Clamp(Y, MinAxisVal, MaxAxisVal)); }

	/**
		Get a copy of this vector with absolute value of each component.

		@return A copy of this vector with absolute value of each component.
	*/
	FORCEINLINE FVector2D GetAbs() const noexcept { return FVector2D(FMath::Abs(X), FMath::Abs(Y)); }

public:

	/**
		Rotates around axis (0,0,1)

		@param AngleDeg - Angle to rotate (in degrees).
		@return Rotated Vector.
	*/
	FVector2D GetRotated(float AngleDeg) const;

	/**
		Gets a normalized copy of the vector, checking it is safe to do so based on the length.
		Returns zero vector if vector length is too small to safely normalize.

		@param Tolerance - Minimum squared length of vector for normalization.
		@return A normalized copy of the vector if safe, (0,0) otherwise.
	*/
	FVector2D GetSafeNormal(float Tolerance = SMALL_NUMBER) const;

	/**
		Normalize this vector in-place if it is large enough, set it to (0,0) otherwise.
		@see GetSafeNormal()

		@param Tolerance - Minimum squared length of vector for normalization.
	*/
	void Normalize(float Tolerance = SMALL_NUMBER);

	/**
		Util to convert this vector into a unit direction vector and its original length.

		@param OutDir - Reference passed in to store unit direction vector.
		@param OutLength - Reference passed in to store length of the vector.
	*/
	void ToDirectionAndLength(FVector2D& OutDir, float& OutLength) const;



public:

	/*
		Globa*l 2D zero vector constant (0,0).
	*/
	static const FVector2D ZeroVector;
	/**
		Global 2D one vector (poorly named) constant (1,1).

		@note Incorrectly named "unit" vector though its magnitude/length/size is not one. Would fix, though likely used all over the world. Use `Unit45Deg` below for an actual unit vector.
	*/
	static const FVector2D UnitVector;
	/**
		Global 2D unit vector constant along the 45 degree angle or symmetrical positive axes (sqrt(.5),sqrt(.5)) or (.707,.707). https://en.wikipedia.org/wiki/Unit_vector

		@note The `UnitVector` above is actually a value with axes of 1 rather than a magnitude of one.
	*/
	static const FVector2D Unit45Deg;

public:

	/** 
		Vector's X component.
	*/
	float X = 0.0f;
	/** 
		Vector's Y component.
	*/
	float Y = 0.0f;
};



/**
	Multiplies a vector by a scaling factor.
 
	@param Scale - Scaling factor.
	@param V - Vector to scale.
	@return Result of multiplication.
*/
FORCEINLINE FVector2D operator*(float Scale, const FVector2D& V) noexcept
{
	return V.operator*(Scale);
}