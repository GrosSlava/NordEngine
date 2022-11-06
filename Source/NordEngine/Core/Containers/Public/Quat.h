#pragma once

#include "EngineMath.h"
#include "SSE/MathSSE.h"


struct FRotator;
struct FVector3D;
struct FMatrix;



/**
 * Floating point quaternion that can represent a rotation about an axis in 3-D space.
 * The X, Y, Z, W components also double as the Axis/Angle format.
 *
 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
 * first applies B then A to any subsequent transformation (right first, then left).
 * Note that this is the opposite order of FTransform multiplication.
 *
 * Example: LocalToWorld = (LocalToWorld * DeltaRotation) will change rotation in local space by DeltaRotation.
 * Example: LocalToWorld = (DeltaRotation * LocalToWorld) will change rotation in world space by DeltaRotation.
 */
MS_ALIGN(16) struct FQuat
{

public:

	/** Default constructor (no initialization). */
	FORCEINLINE FQuat() noexcept{ }

	/**
	 * Constructor.
	 *
	 * @param InX X component of the quaternion
	 * @param InY Y component of the quaternion
	 * @param InZ Z component of the quaternion
	 * @param InW W component of the quaternion
	 */
	FORCEINLINE FQuat(float InX, float InY, float InZ, float InW) noexcept :
		X(InX), Y(InY), Z(InZ), W(InW)
	{

	}

	
	/**
	 * Creates and initializes a new quaternion from the given matrix.
	 *
	 * @param M The rotation matrix to initialize from.
	 */
	explicit FQuat(const FMatrix& M);

	/**
	 * Creates and initializes a new quaternion from the given rotator.
	 *
	 * @param R The rotator to initialize from.
	 */
	explicit FQuat(const FRotator& R) noexcept;

	/**
	 * Creates and initializes a new quaternion from the a rotation around the given axis.
	 *
	 * @param Axis assumed to be a normalized vector
	 * @param Angle angle to rotate above the given axis (in radians)
	 */
	FQuat(const FVector3D& Axis, float AngleRad) noexcept;



public:

	/**
	 * Gets the result of adding a Quaternion to this.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to add.
	 * @return The result of addition.
	 */
	FORCEINLINE FQuat operator+(const FQuat& Q) const
	{
		return FQuat(X + Q.X, Y + Q.Y, Z + Q.Z, W + Q.W);
	}

	/**
	 * Adds to this quaternion.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Other The quaternion to add to this.
	 * @return Result after addition.
	 */
	FORCEINLINE FQuat operator+=(const FQuat& Q)
	{
		this->X += Q.X;
		this->Y += Q.Y;
		this->Z += Q.Z;
		this->W += Q.W;

		return *this;
	}

	/**
	 * Gets the result of subtracting a Quaternion to this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to subtract.
	 * @return The result of subtraction.
	 */
	FORCEINLINE FQuat operator-(const FQuat& Q) const
	{
		return FQuat(X - Q.X, Y - Q.Y, Z - Q.Z, W - Q.W);
	}

	/**
	 * Subtracts another quaternion from this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The other quaternion.
	 * @return reference to this after subtraction.
	 */
	FORCEINLINE FQuat operator-=(const FQuat& Q)
	{
		this->X -= Q.X;
		this->Y -= Q.Y;
		this->Z -= Q.Z;
		this->W -= Q.W; 
		
		return *this;
	}

	/**
	 * Gets the result of multiplying this by another quaternion (this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q The Quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	FORCEINLINE FQuat operator*(const FQuat& Q) const
	{
		FQuat Result;
		VectorQuaternionMultiply(&Result, this, &Q);
		return Result;
	}

	/**
	 * Multiply this quaternion by a scaling factor.
	 *
	 * @param Scale The scaling factor.
	 * @return a reference to this after scaling.
	 */
	FORCEINLINE FQuat operator*=(const float Scale)
	{
		X *= Scale;
		Y *= Scale;
		Z *= Scale;
		W *= Scale;

		return *this;
	}

	/**
	 * Get the result of scaling this quaternion.
	 *
	 * @param Scale The scaling factor.
	 * @return The result of scaling.
	 */
	FORCEINLINE FQuat operator*(const float Scale) const
	{
		return FQuat(Scale * X, Scale * Y, Scale * Z, Scale * W);
	}

	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return a reference to this after scaling.
	 */
	FORCEINLINE FQuat operator/=(const float Scale)
	{
		const float Recip = 1.0f / Scale;
		X *= Recip;
		Y *= Recip;
		Z *= Recip;
		W *= Recip;

		return *this;
	}

	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return new Quaternion of this after division by scale.
	 */
	FORCEINLINE FQuat operator/(const float Scale) const
	{
		const float Recip = 1.0f / Scale;
		return FQuat(X * Recip, Y * Recip, Z * Recip, W * Recip);
	}

	/**
	 * Calculates dot product of two quaternions.
	 *
	 * @param Q The other quaternions.
	 * @return The dot product.
	 */
	FORCEINLINE float operator|(const FQuat& Q) const
	{
		return X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
	}




	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 * @see RotateVector
	 */
	FVector3D operator*(const FVector3D& V) const;


	/**
	 * Multiply this by a matrix.
	 * This matrix conversion came from
	 * http://www.m-hikari.com/ija/ija-password-2008/ija-password17-20-2008/aristidouIJA17-20-2008.pdf
	 * used for non-uniform scaling transform.
	 *
	 * @param M Matrix to multiply by.
	 * @return Matrix result after multiplication.
	 */
	FMatrix operator*(const FMatrix& M) const;

	/**
	 * Multiply this by a quaternion (this = this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q the quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	FQuat operator*=(const FQuat& Q);


public:

	/**
	 * Checks whether two quaternions are identical.
	 * This is an exact comparison per-component;see Equals() for a comparison
	 * that allows for a small error tolerance and flipped axes of rotation.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are identical, otherwise false.
	 * @see Equals
	 */
	FORCEINLINE bool operator==(const FQuat& Q) const
	{
		return X == Q.X && Y == Q.Y && Z == Q.Z && W == Q.W;
	}

	/**
	 * Checks whether two quaternions are not identical.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are not identical, otherwise false.
	 */
	FORCEINLINE bool operator!=(const FQuat& Q) const
	{
		return X != Q.X || Y != Q.Y || Z != Q.Z || W != Q.W;
	}

	

public:

	/**
	 * Checks whether another Quaternion is equal to this, within specified tolerance.
	 *
	 * @param Q The other Quaternion.
	 * @param Tolerance Error tolerance for comparison with other Quaternion.
	 * @return true if two Quaternions are equal, within specified tolerance, otherwise false.
	 */
	FORCEINLINE bool Equals(const FQuat& Q, float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return (FMath::Abs(X - Q.X) <= Tolerance && FMath::Abs(Y - Q.Y) <= Tolerance && FMath::Abs(Z - Q.Z) <= Tolerance && FMath::Abs(W - Q.W) <= Tolerance) ||
			   (FMath::Abs(X + Q.X) <= Tolerance && FMath::Abs(Y + Q.Y) <= Tolerance && FMath::Abs(Z + Q.Z) <= Tolerance && FMath::Abs(W + Q.W) <= Tolerance);
	}

	/**
	 * Checks whether this Quaternion is an Identity Quaternion.
	 * Assumes Quaternion tested is normalized.
	 *
	 * @param Tolerance Error tolerance for comparison with Identity Quaternion.
	 * @return true if Quaternion is a normalized Identity Quaternion.
	 */
	FORCEINLINE bool IsIdentity(float Tolerance = SMALL_NUMBER) const noexcept
	{
		return Equals(FQuat::Identity, Tolerance);
	}

	/**
	 * Identical implementation for FQuat properties.
	 * Avoids intrinsics to remain consistent with previous per-property comparison.
	 */
	FORCEINLINE bool Identical(const FQuat* Q, const uint32 PortFlags) const noexcept
	{
		return X == Q->X && Y == Q->Y && Z == Q->Z && W == Q->W;
	}


	/**
	 * Normalize this quaternion if it is large enough.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	FORCEINLINE void Normalize(float Tolerance = SMALL_NUMBER) noexcept
	{
		const float SquareSum = X * X + Y * Y + Z * Z + W * W;

		if (SquareSum >= Tolerance)
		{
			const float Scale = FMath::InvSqrt(SquareSum);

			X *= Scale;
			Y *= Scale;
			Z *= Scale;
			W *= Scale;
		}
		else
		{
			*this = FQuat::Identity;
		}
	}

	/**
	 * Get a normalized copy of this quaternion.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	FORCEINLINE FQuat GetNormalized(float Tolerance = SMALL_NUMBER) const noexcept
	{
		FQuat Result(*this);
		Result.Normalize(Tolerance);
		return Result;
	}

	// Return true if this quaternion is normalized
	FORCEINLINE bool IsNormalized() const noexcept
	{
		return (FMath::Abs(1.f - SizeSquared()) < THRESH_QUAT_NORMALIZED);
	}

	/**
	 * Get the length of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	FORCEINLINE float Size() const noexcept
	{
		return FMath::Sqrt(X * X + Y * Y + Z * Z + W * W);
	}

	/**
	 * Get the length squared of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	FORCEINLINE float SizeSquared() const noexcept
	{
		return (X * X + Y * Y + Z * Z + W * W);
	}


	/** Get the angle of this quaternion */
	FORCEINLINE float GetAngle() const noexcept
	{
		return 2.f * FMath::Acos(W);
	}

	/**
	 * @return inverse of this quaternion
	 */
	FORCEINLINE FQuat Inverse() const noexcept
	{
		return FQuat(-X, -Y, -Z, W);
	}

	/**
	 * Enforce that the delta between this Quaternion and another one represents
	 * the shortest possible rotation angle
	 */
	FORCEINLINE void EnforceShortestArcWith(const FQuat& OtherQuat) noexcept
	{
		const float DotResult = (OtherQuat | *this);
		const float Bias = FMath::FloatSelect(DotResult, 1.0f, -1.0f);

		X *= Bias;
		Y *= Bias;
		Z *= Bias;
		W *= Bias;
	}

	/** Find the angular distance between two rotation quaternions (in radians) */
	FORCEINLINE float AngularDistance(const FQuat& Q) const noexcept
	{
		float InnerProd = X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
		return FMath::Acos((2 * InnerProd * InnerProd) - 1.f);
	}

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this Quat.
	 *
	 * @return true if there are any non-finite values in this Quaternion, otherwise false.
	 */
	FORCEINLINE bool ContainsNaN() const noexcept
	{
		return (
				!FMath::IsFinite(X) ||
				!FMath::IsFinite(Y) ||
				!FMath::IsFinite(Z) ||
				!FMath::IsFinite(W)
				);
	}




	/**
	 * @return quaternion with W=0 and V=theta*v.
	 */
	FQuat Log() const noexcept;

	/**
	 * @note Exp should really only be used after Log.
	 * Assumes a quaternion with W=0 and V=theta*v (where |v| = 1).
	 * Exp(q) = (sin(theta)*v, cos(theta))
	 */
	FQuat Exp() const noexcept;

	/** Convert a Quaternion into floating-point Euler angles (in degrees). */
	FVector3D Euler() const noexcept;

	/**
	 * get the axis and angle of rotation of this quaternion
	 *
	 * @param Axis{out] vector of axis of the quaternion
	 * @param Angle{out] angle of the quaternion
	 * @warning : assumes normalized quaternions.
	 */
	void ToAxisAndAngle(FVector3D& Axis, float& Angle) const noexcept;

	/**
	 * Get the swing and twist decomposition for a specified axis
	 *
	 * @param InTwistAxis Axis to use for decomposition
	 * @param OutSwing swing component quaternion
	 * @param OutTwist Twist component quaternion
	 * @warning assumes normalized quaternion and twist axis
	 */
	void ToSwingTwist(const FVector3D& InTwistAxis, FQuat& OutSwing, FQuat& OutTwist) const noexcept;

	/**
	 * Get the twist angle (in radians) for a specified axis
	 *
	 * @param TwistAxis Axis to use for decomposition
	 * @return Twist angle (in radians)
	 * @warning assumes normalized quaternion and twist axis
	 */
	float GetTwistAngle(const FVector3D& TwistAxis) const noexcept;

	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 */
	FVector3D RotateVector(const FVector3D& V) const noexcept;

	/**
	 * Rotate a vector by the inverse of this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation by the inverse of this quaternion.
	 */
	FVector3D UnrotateVector(const FVector3D& V) const noexcept;


	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	FVector3D GetAxisX() const noexcept;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	FVector3D GetAxisY() const noexcept;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	FVector3D GetAxisZ() const noexcept;

	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	FVector3D GetForwardVector() const noexcept;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	FVector3D GetRightVector() const noexcept;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	FVector3D GetUpVector() const noexcept;

	/** Convert a rotation into a unit vector facing in its direction. Equivalent to GetForwardVector(). */
	FVector3D Vector() const noexcept;

	/** Get the FRotator representation of this Quaternion. */
	FRotator Rotator() const noexcept;

	/**
	 * Get the axis of rotation of the Quaternion.
	 * This is the axis around which rotation occurs to transform the canonical coordinate system to the target orientation.
	 * For the identity Quaternion which has no such rotation, FVector(1,0,0) is returned.
	 */
	FVector3D GetRotationAxis() const noexcept;

public:

	/**
	* Error measure (angle) between two quaternions, ranged [0..1].
	* Returns the hypersphere-angle between two quaternions; alignment shouldn't matter, though
	* @note normalized input is expected.
	*/
	static FORCEINLINE float Error(const FQuat& Q1, const FQuat& Q2) noexcept
	{
		const float cosom = FMath::Abs(Q1.X * Q2.X + Q1.Y * Q2.Y + Q1.Z * Q2.Z + Q1.W * Q2.W);
		return (FMath::Abs(cosom) < 0.9999999f) ? FMath::Acos(cosom) * (1.f / PI) : 0.0f;
	}

	/**
	* FQuat::Error with auto-normalization.
	*/
	static FORCEINLINE float ErrorAutoNormalize(const FQuat& A, const FQuat& B) noexcept
	{
		FQuat Q1 = A;
		Q1.Normalize();

		FQuat Q2 = B;
		Q2.Normalize();

		return FQuat::Error(Q1, Q2);
	}

	/**
	* Fast Linear Quaternion Interpolation.
	* Result is NOT normalized.
	*/
	static FORCEINLINE FQuat FastLerp(const FQuat& A, const FQuat& B, const float Alpha) noexcept
	{
		// To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
		const float DotResult = (A | B);
		const float Bias = FMath::FloatSelect(DotResult, 1.0f, -1.0f);
		return (B * Alpha) + (A * (Bias * (1.f - Alpha)));
	}

	/**
	* Bi-Linear Quaternion interpolation.
	* Result is NOT normalized.
	*/
	static FORCEINLINE FQuat FastBilerp(const FQuat& P00, const FQuat& P10, const FQuat& P01, const FQuat& P11, float FracX, float FracY) noexcept
	{
		return FQuat::FastLerp
		(
			FQuat::FastLerp(P00, P10, FracX),
			FQuat::FastLerp(P01, P11, FracX),
			FracY
		);
	}

	/** Spherical interpolation. Will correct alignment. Result is normalized. */
	static FORCEINLINE FQuat Slerp(const FQuat& Quat1, const FQuat& Quat2, float Slerp) noexcept
	{
		return Slerp_NotNormalized(Quat1, Quat2, Slerp).GetNormalized();
	}

	/**
	* Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	* We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	* Result is normalized.
	*/
	static FORCEINLINE FQuat SlerpFullPath(const FQuat& quat1, const FQuat& quat2, float Alpha) noexcept
	{
		return SlerpFullPath_NotNormalized(quat1, quat2, Alpha).GetNormalized();
	}

	/**
	* Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	* This will correct alignment by ensuring that the shortest path is taken.
	*/
	static FORCEINLINE FQuat Squad(const FQuat& quat1, const FQuat& tang1, const FQuat& quat2, const FQuat& tang2, float Alpha) noexcept
	{
		// Always slerp along the short path from quat1 to quat2 to prevent axis flipping.
		// This approach is taken by OGRE engine, amongst others.
		const FQuat Q1 = FQuat::Slerp_NotNormalized(quat1, quat2, Alpha);
		const FQuat Q2 = FQuat::SlerpFullPath_NotNormalized(tang1, tang2, Alpha);
		const FQuat Result = FQuat::SlerpFullPath(Q1, Q2, 2.f * Alpha * (1.f - Alpha));

		return Result;
	}

	/**
	* Simpler Squad that doesn't do any checks for 'shortest distance' etc.
	* Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	*/
	static FORCEINLINE FQuat SquadFullPath(const FQuat& quat1, const FQuat& tang1, const FQuat& quat2, const FQuat& tang2, float Alpha) noexcept
	{
		const FQuat Q1 = FQuat::SlerpFullPath_NotNormalized(quat1, quat2, Alpha);
		const FQuat Q2 = FQuat::SlerpFullPath_NotNormalized(tang1, tang2, Alpha);
		const FQuat Result = FQuat::SlerpFullPath(Q1, Q2, 2.f * Alpha * (1.f - Alpha));

		return Result;
	}

	/**
	* Calculate tangents between given points
	*
	* @param PrevP quaternion at P-1
	* @param P quaternion to return the tangent
	* @param NextP quaternion P+1
	* @param OutTan Out control point
	*/
	static FORCEINLINE void CalcTangents(const FQuat& PrevP, const FQuat& P, const FQuat& NextP, FQuat& OutTan) noexcept
	{
		const FQuat InvP = P.Inverse();
		const FQuat Part1 = (InvP * PrevP).Log();
		const FQuat Part2 = (InvP * NextP).Log();

		const FQuat PreExp = (Part1 + Part2) * -0.5f;

		OutTan = P * PreExp.Exp();
	}




	/** Spherical interpolation. Will correct alignment. Result is NOT normalized. */
	static FQuat Slerp_NotNormalized(const FQuat& Quat1, const FQuat& Quat2, float Slerp) noexcept;

	/**
	* Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	* We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	* Result is NOT normalized.
	*/
	static FQuat SlerpFullPath_NotNormalized(const FQuat& quat1, const FQuat& quat2, float Alpha) noexcept;

	/**
	 * Convert a vector of floating-point Euler angles (in degrees) into a Quaternion.
	 *
	 * @param Euler the Euler angles
	 * @return constructed FQuat
	 */
	static FQuat MakeFromEuler(const FVector3D& Euler) noexcept;

	/**
	* Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	*/
	static FORCEINLINE FQuat FindBetween(const FVector3D& Vector1, const FVector3D& Vector2) noexcept
	{
		return FindBetweenVectors(Vector1, Vector2);
	}

	/**
	* Generates the 'smallest' (geodesic) rotation between two normals (assumed to be unit length).
	*/
	static FQuat FindBetweenNormals(const FVector3D& Normal1, const FVector3D& Normal2) noexcept;

	/**
	* Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	*/
	static FQuat FindBetweenVectors(const FVector3D& Vector1, const FVector3D& Vector2) noexcept;



public:

	/** Identity quaternion. */
	static const FQuat Identity;

public:

	/** The quaternion's X-component. */
	float X = 0.0f;

	/** The quaternion's Y-component. */
	float Y = 0.0f;

	/** The quaternion's Z-component. */
	float Z = 0.0f;

	/** The quaternion's W-component. */
	float W = 0.0f;

};

