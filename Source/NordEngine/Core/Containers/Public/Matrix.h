#pragma once

#include "GenericPlatform.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Plane.h"

#include "Axis.h"
#include "SSE/MathSSE.h"




/**
* 4x4 matrix of floating point values.
* Matrix-matrix multiplication happens with a pre-multiple of the transpose --
* in other words, Res = Mat1.operator*(Mat2) means Res = Mat2^T * Mat1, as
* opposed to Res = Mat1 * Mat2.
* Matrix elements are accessed with M[RowIndex][ColumnIndex].
*/
struct ENGINE_API FMatrix
{

public:

	// Constructors.
	FORCEINLINE FMatrix() noexcept { }

	/**
	 * Constructor.
	 *
	 * @param InX X plane
	 * @param InY Y plane
	 * @param InZ Z plane
	 * @param InW W plane
	 */
	FORCEINLINE FMatrix(const FPlane& InX, const FPlane& InY, const FPlane& InZ, const FPlane& InW) noexcept
	{
		M[0][0] = InX.X; M[0][1] = InX.Y;  M[0][2] = InX.Z;  M[0][3] = InX.W;
		M[1][0] = InY.X; M[1][1] = InY.Y;  M[1][2] = InY.Z;  M[1][3] = InY.W;
		M[2][0] = InZ.X; M[2][1] = InZ.Y;  M[2][2] = InZ.Z;  M[2][3] = InZ.W;
		M[3][0] = InW.X; M[3][1] = InW.Y;  M[3][2] = InW.Z;  M[3][3] = InW.W;
	}

	/**
	 * Constructor.
	 *
	 * @param InX X vector
	 * @param InY Y vector
	 * @param InZ Z vector
	 * @param InW W vector
	 */
	FORCEINLINE FMatrix(const FVector3D& InX, const FVector3D& InY, const FVector3D& InZ, const FVector3D& InW) noexcept
	{
		M[0][0] = InX.X; M[0][1] = InX.Y;  M[0][2] = InX.Z;  M[0][3] = 0.0f;
		M[1][0] = InY.X; M[1][1] = InY.Y;  M[1][2] = InY.Z;  M[1][3] = 0.0f;
		M[2][0] = InZ.X; M[2][1] = InZ.Y;  M[2][2] = InZ.Z;  M[2][3] = 0.0f;
		M[3][0] = InW.X; M[3][1] = InW.Y;  M[3][2] = InW.Z;  M[3][3] = 1.0f;
	}

	


public:

	/**
	 * Gets the result of multiplying a Matrix to this.
	 *
	 * @param Other The matrix to multiply this by.
	 * @return The result of multiplication.
	 */
	FORCEINLINE FMatrix operator* (const FMatrix& Other) const noexcept
	{
		FMatrix Result;
		VectorMatrixMultiply(&Result, this, &Other);
		return Result;
	}

	/**
	 * Multiply this by a matrix.
	 *
	 * @param Other the matrix to multiply by this.
	 * @return reference to this after multiply.
	 */
	FORCEINLINE void operator*=(const FMatrix& Other) noexcept
	{
		VectorMatrixMultiply(this, this, &Other);
	}

	/**
	 * Gets the result of adding a matrix to this.
	 *
	 * @param Other The Matrix to add.
	 * @return The result of addition.
	 */
	FORCEINLINE FMatrix operator+ (const FMatrix& Other) const noexcept
	{
		FMatrix ResultMat;

		for (int32 X = 0; X < 4; ++X)
		{
			for (int32 Y = 0; Y < 4; ++Y)
			{
				ResultMat.M[X][Y] = M[X][Y] + Other.M[X][Y];
			}
		}

		return ResultMat;
	}

	/**
	 * Adds to this matrix.
	 *
	 * @param Other The matrix to add to this.
	 * @return Reference to this after addition.
	 */
	FORCEINLINE void operator+=(const FMatrix& Other) noexcept
	{
		*this = *this + Other;
	}

	/**
	* This isn't applying SCALE, just multiplying the value to all members - i.e. weighting
	*/
	FORCEINLINE FMatrix operator* (float Other) const noexcept
	{
		FMatrix ResultMat;

		for (int32 X = 0; X < 4; ++X)
		{
			for (int32 Y = 0; Y < 4; ++Y)
			{
				ResultMat.M[X][Y] = M[X][Y] * Other;
			}
		}

		return ResultMat;
	}

	/**
	 * Multiply this matrix by a weighting factor.
	 *
	 * @param other The weight.
	 * @return a reference to this after weighting.
	 */
	FORCEINLINE void operator*=(float Other) noexcept
	{
		*this = *this * Other;
	}

	

public:

	/**
	 * Checks whether two matrix are identical.
	 *
	 * @param Other The other matrix.
	 * @return true if two matrix are identical, otherwise false.
	 */
	FORCEINLINE bool operator==(const FMatrix& Other) const
	{
		for (int32 X = 0; X < 4; ++X)
		{
			for (int32 Y = 0; Y < 4; ++Y)
			{
				if (M[X][Y] != Other.M[X][Y])
				{
					return false;
				}
			}
		}

		return true;
	}

	/**
	 * Checks whether another Matrix is not equal to this, within specified tolerance.
	 *
	 * @param Other The other Matrix.
	 * @return true if two Matrix are not equal, within specified tolerance, otherwise false.
	 */
	FORCEINLINE bool operator!=(const FMatrix& Other) const
	{
		return !(*this == Other);
	}


public:

	/**
	 * Checks whether another Matrix is equal to this, within specified tolerance.
	 *
	 * @param Other The other Matrix.
	 * @param Tolerance Error Tolerance.
	 * @return true if two Matrix are equal, within specified tolerance, otherwise false.
	 */
	FORCEINLINE bool Equals(const FMatrix& Other, float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		for (int32 X = 0; X < 4; ++X)
		{
			for (int32 Y = 0; Y < 4; ++Y)
			{
				if (FMath::Abs(M[X][Y] - Other.M[X][Y]) > Tolerance)
				{
					return false;
				}
			}
		}

		return true;
	}


	// Set this to the identity matrix
	FORCEINLINE void SetIdentity() noexcept
	{
		M[0][0] = 1; M[0][1] = 0;  M[0][2] = 0;  M[0][3] = 0;
		M[1][0] = 0; M[1][1] = 1;  M[1][2] = 0;  M[1][3] = 0;
		M[2][0] = 0; M[2][1] = 0;  M[2][2] = 1;  M[2][3] = 0;
		M[3][0] = 0; M[3][1] = 0;  M[3][2] = 0;  M[3][3] = 1;
	}





	// Homogeneous transform.
	FORCEINLINE FVector4D TransformFVector4(const FVector4D& V) const noexcept
	{
		FVector4D Result;
		VectorRegister VecP = VectorLoadAligned(&V);
		VectorRegister VecR = VectorTransformVector(VecP, this);
		VectorStoreAligned(VecR, &Result);
		return Result;

	}

	/** Transform a location - will take into account translation part of the FMatrix. */
	FORCEINLINE FVector4D TransformPosition(const FVector3D& V) const noexcept
	{
		return TransformFVector4(FVector4D(V.X, V.Y, V.Z, 1.0f));
	}

	/** Inverts the matrix and then transforms V - correctly handles scaling in this matrix. */
	FORCEINLINE FVector3D InverseTransformPosition(const FVector3D& V) const noexcept
	{
		FMatrix InvSelf = this->InverseFast();
		return InvSelf.TransformPosition(V);
	}

	/**
	 *	Transform a direction vector - will not take into account translation part of the FMatrix.
	 *	If you want to transform a surface normal (or plane) and correctly account for non-uniform scaling you should use TransformByUsingAdjointT.
	 */
	FORCEINLINE FVector4D TransformVector(const FVector3D& V) const noexcept
	{
		return TransformFVector4(FVector4D(V.X, V.Y, V.Z, 0.0f));
	}

	/**
	 *	Transform a direction vector by the inverse of this matrix - will not take into account translation part.
	 *	If you want to transform a surface normal (or plane) and correctly account for non-uniform scaling you should use TransformByUsingAdjointT with adjoint of matrix inverse.
	 */
	FORCEINLINE FVector3D InverseTransformVector(const FVector3D& V) const noexcept
	{
		FMatrix InvSelf = this->InverseFast();
		return InvSelf.TransformVector(V);
	}




	/** @return the determinant of rotation 3x3 matrix */
	FORCEINLINE float RotDeterminant() const noexcept
	{
		return
			M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
			M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
			M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1]);
	}

	/** Fast path, doesn't check for nil matrices in final release builds */
	FORCEINLINE FMatrix InverseFast() const noexcept
	{
		FMatrix Result;
		VectorMatrixInverse(&Result, this);
		return Result;
	}



	// Returns matrix after RemoveScaling with error Tolerance
	FORCEINLINE FMatrix GetMatrixWithoutScale(float Tolerance = SMALL_NUMBER) const noexcept
	{
		FMatrix Result = *this;
		Result.RemoveScaling(Tolerance);
		return Result;
	}

	/** return a 3D scale vector calculated from this matrix (where each component is the magnitude of a row vector) with error Tolerance. */
	FORCEINLINE FVector3D GetScaleVector(float Tolerance = SMALL_NUMBER) const noexcept
	{
		FVector3D Scale3D(1, 1, 1);

		const float SquareSumX = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
		Scale3D.X = (SquareSumX > Tolerance) ? FMath::Sqrt(SquareSumX) : 0.0f;

		const float SquareSumY = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
		Scale3D.Y = (SquareSumY > Tolerance) ? FMath::Sqrt(SquareSumY) : 0.0f;

		const float SquareSumZ = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);
		Scale3D.Z = (SquareSumZ > Tolerance) ? FMath::Sqrt(SquareSumZ) : 0.0f;

		return Scale3D;
	}

	// Remove any translation from this matrix
	FORCEINLINE FMatrix RemoveTranslation() const noexcept
	{
		FMatrix Result = *this;
		Result.M[3][0] = 0.0f;
		Result.M[3][1] = 0.0f;
		Result.M[3][2] = 0.0f;
		return Result;
	}

	/** Returns true if any element of this matrix is NaN */
	FORCEINLINE bool ContainsNaN() const noexcept
	{
		for (int32 i = 0; i < 4; ++i)
		{
			for (int32 j = 0; j < 4; ++j)
			{
				if (!FMath::IsFinite(M[i][j]))
				{
					return true;
				}
			}
		}

		return false;
	}

	/** Scale the translation part of the matrix by the supplied vector. */
	FORCEINLINE void ScaleTranslation(const FVector3D& Scale3D) noexcept
	{
		M[3][0] *= Scale3D.X;
		M[3][1] *= Scale3D.Y;
		M[3][2] *= Scale3D.Z;
	}

	/** @return the maximum magnitude of any row of the matrix. */
	FORCEINLINE float GetMaximumAxisScale() const noexcept
	{
		const float MaxRowScaleSquared = FMath::Max
		(
			GetScaledAxis(EAxis::X).SizeSquared(),
			FMath::Max(	GetScaledAxis(EAxis::Y).SizeSquared(),	GetScaledAxis(EAxis::Z).SizeSquared() )
		);

		return FMath::Sqrt(MaxRowScaleSquared);
	}

	/** Apply Scale to this matrix **/
	FORCEINLINE FMatrix ApplyScale(float Scale) const noexcept
	{
		FMatrix ScaleMatrix
		(
			FPlane(Scale, 0.0f, 0.0f, 0.0f),
			FPlane(0.0f, Scale, 0.0f, 0.0f),
			FPlane(0.0f, 0.0f, Scale, 0.0f),
			FPlane(0.0f, 0.0f, 0.0f, 1.0f)
		);

		return ScaleMatrix * (*this);
	}




	// @return the origin of the co-ordinate system
	FORCEINLINE FVector3D GetOrigin() const noexcept
	{
		return FVector3D(M[3][0], M[3][1], M[3][2]);
	}

	// Set the origin of the coordinate system to the given vector
	FORCEINLINE void SetOrigin(const FVector3D& NewOrigin) noexcept
	{
		M[3][0] = NewOrigin.X;
		M[3][1] = NewOrigin.Y;
		M[3][2] = NewOrigin.Z;
	}




	/**
	 * get axis of this matrix scaled by the scale of the matrix
	 *
	 * @param i index into the axis of the matrix
	 * @ return vector of the axis
	 */
	FORCEINLINE FVector3D GetScaledAxis(EAxis Axis) const noexcept
	{
		switch (Axis)
		{

		case EAxis::X:	return FVector3D(M[0][0], M[0][1], M[0][2]);

		case EAxis::Y:	return FVector3D(M[1][0], M[1][1], M[1][2]);

		case EAxis::Z:	return FVector3D(M[2][0], M[2][1], M[2][2]);

		default:		return FVector3D::ZeroVector;

		}
	}

	/**
	 * get axes of this matrix scaled by the scale of the matrix
	 *
	 * @param X axes returned to this param
	 * @param Y axes returned to this param
	 * @param Z axes returned to this param
	 */
	FORCEINLINE void GetScaledAxes(FVector3D& X, FVector3D& Y, FVector3D& Z) const noexcept
	{
		X.X = M[0][0]; X.Y = M[0][1]; X.Z = M[0][2];
		Y.X = M[1][0]; Y.Y = M[1][1]; Y.Z = M[1][2];
		Z.X = M[2][0]; Z.Y = M[2][1]; Z.Z = M[2][2];
	}

	/**
	 * get unit length axis of this matrix
	 *
	 * @param i index into the axis of the matrix
	 * @return vector of the axis
	 */
	FORCEINLINE FVector3D GetUnitAxis(EAxis Axis) const noexcept
	{
		return GetScaledAxis(Axis).GetSafeNormal();
	}

	/**
	 * get unit length axes of this matrix
	 *
	 * @param X axes returned to this param
	 * @param Y axes returned to this param
	 * @param Z axes returned to this param
	 */
	FORCEINLINE void GetUnitAxes(FVector3D& X, FVector3D& Y, FVector3D& Z) const noexcept
	{
		GetScaledAxes(X, Y, Z);
		X.Normalize();
		Y.Normalize();
		Z.Normalize();
	}

	/**
	 * set an axis of this matrix
	 *
	 * @param i index into the axis of the matrix
	 * @param Axis vector of the axis
	 */
	FORCEINLINE void SetAxis(int32 i, const FVector3D& Axis) noexcept
	{
		M[i][0] = Axis.X;
		M[i][1] = Axis.Y;
		M[i][2] = Axis.Z;
	}

	/**
	 * Update the axes of the matrix if any value is NULL do not update that axis
	 *
	 * @param Axis0 set matrix row 0
	 * @param Axis1 set matrix row 1
	 * @param Axis2 set matrix row 2
	 * @param Origin set matrix row 3
	 */
	void SetAxes(FVector3D* Axis0 = NULL, FVector3D* Axis1 = NULL, FVector3D* Axis2 = NULL, FVector3D* Origin = NULL) noexcept;
	


	/**
	 * get a column of this matrix
	 *
	 * @param i index into the column of the matrix
	 * @return vector of the column
	 */
	FORCEINLINE FVector3D GetColumn(int32 i) const noexcept
	{
		return FVector3D(M[0][i], M[1][i], M[2][i]);
	}

	/**
	 * Set a column of this matrix
	 *
	 * @param i index of the matrix column
	 * @param Value new value of the column
	 */
	FORCEINLINE void SetColumn(int32 i, FVector3D Value) noexcept
	{
		M[0][i] = Value.X;
		M[1][i] = Value.Y;
		M[2][i] = Value.Z;
	}

	


	static FORCEINLINE bool MakeFrustumPlane(float A, float B, float C, float D, FPlane& OutPlane)
	{
		const float	LengthSquared = A * A + B * B + C * C;
		if (LengthSquared > DELTA * DELTA)
		{
			const float	InvLength = FMath::InvSqrt(LengthSquared);
			OutPlane = FPlane(-A * InvLength, -B * InvLength, -C * InvLength, D * InvLength);
			return 1;
		}
		else
		{
			return 0;
		}
			
	}

	// Frustum plane extraction.
	/** @param OutPlane the near plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumNearPlane(FPlane& OutPlane) const noexcept
	{
		return MakeFrustumPlane(
			M[0][2],
			M[1][2],
			M[2][2],
			M[3][2],
			OutPlane
		);
			
	}

	/** @param OutPlane the far plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumFarPlane(FPlane& OutPlane) const noexcept
	{
		return MakeFrustumPlane(
			M[0][3] - M[0][2],
			M[1][3] - M[1][2],
			M[2][3] - M[2][2],
			M[3][3] - M[3][2],
			OutPlane
		);
	}

	/** @param OutPlane the left plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumLeftPlane(FPlane& OutPlane) const noexcept
	{
		return MakeFrustumPlane(
			M[0][3] + M[0][0],
			M[1][3] + M[1][0],
			M[2][3] + M[2][0],
			M[3][3] + M[3][0],
			OutPlane
		);
	}

	/** @param OutPlane the right plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumRightPlane(FPlane& OutPlane) const noexcept
	{
		return MakeFrustumPlane(
			M[0][3] - M[0][0],
			M[1][3] - M[1][0],
			M[2][3] - M[2][0],
			M[3][3] - M[3][0],
			OutPlane
		);
	}

	/** @param OutPlane the top plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumTopPlane(FPlane& OutPlane) const noexcept
	{
		return MakeFrustumPlane(
			M[0][3] - M[0][1],
			M[1][3] - M[1][1],
			M[2][3] - M[2][1],
			M[3][3] - M[3][1],
			OutPlane
		);

	}

	/** @param OutPlane the bottom plane of the Frustum of this matrix */
	FORCEINLINE bool GetFrustumBottomPlane(FPlane& OutPlane) const noexcept
	{
		return MakeFrustumPlane(
			M[0][3] + M[0][1],
			M[1][3] + M[1][1],
			M[2][3] + M[2][1],
			M[3][3] + M[3][1],
			OutPlane
		);
	}




	FMatrix GetTransposed() const noexcept;

	// @return determinant of this matrix.
	float Determinant() const noexcept;

	/** Fast path, and handles nil matrices. */
	FMatrix Inverse() const noexcept;

	FMatrix TransposeAdjoint() const noexcept;

	// NOTE: There is some compiler optimization issues with WIN64 that cause FORCEINLINE to cause a crash
	// Remove any scaling from this matrix (ie magnitude of each row is 1) with error Tolerance
	void RemoveScaling(float Tolerance = SMALL_NUMBER) noexcept;

	/** Remove any scaling from this matrix (ie magnitude of each row is 1) and return the 3D scale vector that was initially present with error Tolerance */
	FVector3D ExtractScaling(float Tolerance = SMALL_NUMBER) noexcept;

	/**
	 * Utility for mirroring this transform across a certain plane, and flipping one of the axis as well.
	 */
	void Mirror(EAxis MirrorAxis, EAxis FlipAxis) noexcept;
	
	/** Returns a matrix with an additional translation concatenated. */
	FMatrix ConcatTranslation(const FVector3D& Translation) const noexcept;



	/** @return rotator representation of this matrix */
	FRotator Rotator() const;

	/**
	 * Transform a rotation matrix into a quaternion.
	 *
	 * @warning rotation part will need to be unit length for this to be right!
	 */
	FQuat ToQuat() const;
	



	/**
	 * Convert this Atom to the 3x4 transpose of the transformation matrix.
	 */
	void To3x4MatrixTranspose(float* Out) const noexcept;
	




public:

	//Identity matrix
	MS_ALIGN(16) static const FMatrix Identity;


public:

	union
	{
		MS_ALIGN(16) float M[4][4];
	};

};








/**
 * A storage class for compile-time fixed size matrices.
 */
template<uint32 NumRows, uint32 NumColumns>
class TMatrix
{

public:

	// Variables.
	MS_ALIGN(16) float M[NumRows][NumColumns];

	// Constructor
	FORCEINLINE TMatrix() { }

	/**
	 * Constructor
	 *
	 * @param InMatrix FMatrix reference
	 */
	FORCEINLINE TMatrix(const FMatrix& InMatrix)
	{
		for (uint32 RowIndex = 0; (RowIndex < NumRows) && (RowIndex < 4); ++RowIndex)
		{
			for (uint32 ColumnIndex = 0; (ColumnIndex < NumColumns) && (ColumnIndex < 4); ++ColumnIndex)
			{
				M[RowIndex][ColumnIndex] = InMatrix.M[RowIndex][ColumnIndex];
			}
		}
	}

};






struct FBasisVectorMatrix : public FMatrix
{

public:

	// Create Basis matrix from 3 axis vectors and the origin
	FORCEINLINE FBasisVectorMatrix(const FVector3D& XAxis, const FVector3D& YAxis, const FVector3D& ZAxis, const FVector3D& Origin) noexcept
	{
		for (uint32 RowIndex = 0; RowIndex < 3; ++RowIndex)
		{
			M[RowIndex][0] = (&XAxis.X)[RowIndex];
			M[RowIndex][1] = (&YAxis.X)[RowIndex];
			M[RowIndex][2] = (&ZAxis.X)[RowIndex];
			M[RowIndex][3] = 0.0f;
		}

		M[3][0] = Origin | XAxis;
		M[3][1] = Origin | YAxis;
		M[3][2] = Origin | ZAxis;
		M[3][3] = 1.0f;
	}
};






struct FLookFromMatrix : public FMatrix
{

public:

	/**
	 * Creates a view matrix given an eye position, a direction to look in, and an up vector.
	 * Direction or up vectors need not be normalized.
	 * This does the same thing as FLookAtMatrix, except without completely destroying precision when position is large,
	 * Always use this instead of e.g., FLookAtMatrix(Pos, Pos + Dir,...);
	 */
	FORCEINLINE FLookFromMatrix(const FVector3D& EyePosition, const FVector3D& LookDirection, const FVector3D& UpVector) noexcept
	{
		const FVector3D ZAxis = LookDirection.GetSafeNormal();
		const FVector3D XAxis = (UpVector ^ ZAxis).GetSafeNormal();
		const FVector3D YAxis = ZAxis ^ XAxis;

		for (uint32 RowIndex = 0; RowIndex < 3; RowIndex++)
		{
			M[RowIndex][0] = (&XAxis.X)[RowIndex];
			M[RowIndex][1] = (&YAxis.X)[RowIndex];
			M[RowIndex][2] = (&ZAxis.X)[RowIndex];
			M[RowIndex][3] = 0.0f;
		}

		M[3][0] = -EyePosition | XAxis;
		M[3][1] = -EyePosition | YAxis;
		M[3][2] = -EyePosition | ZAxis;
		M[3][3] = 1.0f;
	}
};






struct FLookAtMatrix : public FLookFromMatrix
{

public:

	/**
	 * Creates a view matrix given an eye position, a position to look at, and an up vector.
	 * Equivalent of FLookFromMatrix(EyePosition, LookAtPosition - EyePosition, UpVector)
	 * The up vector need not be normalized.
	 * This does the same thing as D3DXMatrixLookAtLH.
	 */
	FORCEINLINE FLookAtMatrix(const FVector3D& EyePosition, const FVector3D& LookAtPosition, const FVector3D& UpVector) noexcept :
		FLookFromMatrix(EyePosition, LookAtPosition - EyePosition, UpVector)
	{

	}
};
