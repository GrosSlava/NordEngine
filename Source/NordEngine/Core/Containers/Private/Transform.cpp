
#include "Transform.h"
#include "Matrix.h"



const FTransform FTransform::Identity(FQuat(0.f, 0.f, 0.f, 1.f), FVector3D(0.f), FVector3D(1.f));






FTransform::FTransform(const FVector3D& InX, const FVector3D& InY, const FVector3D& InZ, const FVector3D& InTranslation) noexcept
{
	SetFromMatrix(FMatrix(InX, InY, InZ, InTranslation));
}






FMatrix FTransform::ToInverseMatrixWithScale() const noexcept
{
	return ToMatrixWithScale().Inverse();
}


FTransform FTransform::GetRelativeTransform(const FTransform& Other) const noexcept
{
	// A * B(-1) = VQS(B)(-1) (VQS (A))
	// 
	// Scale = S(A)/S(B)
	// Rotation = Q(B)(-1) * Q(A)
	// Translation = 1/S(B) *[Q(B)(-1)*(T(A)-T(B))*Q(B)]
	// where A = this, B = Other
	FTransform Result;

	if (Other.IsRotationNormalized() == false)
	{
		return FTransform::Identity;
	}


	if (Private_AnyHasNegativeScale(this->Scale3D, Other.Scale3D))
	{
		// @note, if you have 0 scale with negative, you're going to lose rotation as it can't convert back to quat
		GetRelativeTransformUsingMatrixWithScale(&Result, this, &Other);
	}
	else
	{
		// Scale = S(A)/S(B)
		static ScalarRegister STolerance(SMALL_NUMBER);
		VectorRegister VSafeScale3D = VectorSet_W0(GetSafeScaleReciprocal(Other.Scale3D, STolerance));

		VectorRegister VScale3D = VectorMultiply(Scale3D, VSafeScale3D);

		//VQTranslation = (  ( T(A).X - T(B).X ),  ( T(A).Y - T(B).Y ), ( T(A).Z - T(B).Z), 0.f );
		VectorRegister VQTranslation = VectorSet_W0(VectorSubtract(Translation, Other.Translation));

		// Inverse RotatedTranslation
		VectorRegister VInverseRot = VectorQuaternionInverse(Other.Rotation);
		VectorRegister VR = VectorQuaternionRotateVector(VInverseRot, VQTranslation);

		//Translation = 1/S(B)
		VectorRegister VTranslation = VectorMultiply(VR, VSafeScale3D);

		// Rotation = Q(B)(-1) * Q(A)	
		VectorRegister VRotation = VectorQuaternionMultiply2(VInverseRot, Rotation);

		Result.Scale3D = VScale3D;
		Result.Translation = VTranslation;
		Result.Rotation = VRotation;
	}

	return Result;
}

FTransform FTransform::GetRelativeTransformReverse(const FTransform& Other) const noexcept
{
	// A (-1) * B = VQS(B)(VQS (A)(-1))
	// 
	// Scale = S(B)/S(A)
	// Rotation = Q(B) * Q(A)(-1)
	// Translation = T(B)-S(B)/S(A) *[Q(B)*Q(A)(-1)*T(A)*Q(A)*Q(B)(-1)]
	// where A = this, and B = Other
	FTransform Result;

	// Scale = S(B)/S(A)	
	VectorRegister VSafeScale3D = VectorSet_W0(GetSafeScaleReciprocal(Scale3D));
	VectorRegister VScale3D = VectorMultiply(Other.Scale3D, VSafeScale3D);

	// Rotation = Q(B) * Q(A)(-1)	
	VectorRegister VInverseRot = VectorQuaternionInverse(Rotation);
	VectorRegister VRotation = VectorQuaternionMultiply2(Other.Rotation, VInverseRot);

	// RotatedTranslation
	VectorRegister VR = VectorQuaternionRotateVector(VRotation, Translation);

	// Translation = T(B)-S(B)/S(A) *[Q(B)*Q(A)(-1)*T(A)*Q(A)*Q(B)(-1)]	
	VectorRegister VTranslation = VectorSet_W0(VectorSubtract(Other.Translation, VectorMultiply(VScale3D, VR)));

	Result.Scale3D = VScale3D;
	Result.Translation = VTranslation;
	Result.Rotation = VRotation;

	return Result;
}

void FTransform::SetToRelativeTransform(const FTransform& ParentTransform) noexcept
{
	// A * B(-1) = VQS(B)(-1) (VQS (A))
	// 
	// Scale = S(A)/S(B)
	// Rotation = Q(B)(-1) * Q(A)
	// Translation = 1/S(B) *[Q(B)(-1)*(T(A)-T(B))*Q(B)]
	// where A = this, B = Other


	// Scale = S(A)/S(B)	
	VectorRegister VSafeScale3D = VectorSet_W0(GetSafeScaleReciprocal(ParentTransform.Scale3D, ScalarRegister(SMALL_NUMBER)));
	Scale3D = VectorMultiply(Scale3D, VSafeScale3D);

	//VQTranslation = (  ( T(A).X - T(B).X ),  ( T(A).Y - T(B).Y ), ( T(A).Z - T(B).Z), 0.f );
	VectorRegister VQTranslation = VectorSet_W0(VectorSubtract(Translation, ParentTransform.Translation));

	// Inverse RotatedTranslation
	VectorRegister VInverseParentRot = VectorQuaternionInverse(ParentTransform.Rotation);
	VectorRegister VR = VectorQuaternionRotateVector(VInverseParentRot, VQTranslation);

	// Translation = 1/S(B)
	Translation = VectorMultiply(VR, VSafeScale3D);

	// Rotation = Q(B)(-1) * Q(A)	
	Rotation = VectorQuaternionMultiply2(VInverseParentRot, Rotation);

}




FMatrix FTransform::ToMatrixNoScale() const noexcept
{
	FMatrix OutMatrix;
	VectorRegister DiagonalsXYZ;
	VectorRegister Adds;
	VectorRegister Subtracts;

	ToMatrixInternalNoScale(DiagonalsXYZ, Adds, Subtracts);
	const VectorRegister DiagonalsXYZ_W0 = VectorSet_W0(DiagonalsXYZ);

	// OutMatrix.M[0][0] = (1.0f - (yy2 + zz2));			// Diagonal.X
	// OutMatrix.M[0][1] = (xy2 + wz2);						// Adds.X
	// OutMatrix.M[0][2] = (xz2 - wy2);						// Subtracts.Z
	// OutMatrix.M[0][3] = 0.0f;                            // DiagonalsXYZ_W0.W
	const VectorRegister AddX_DC_DiagX_DC = VectorShuffle(Adds, DiagonalsXYZ_W0, 0, 0, 0, 0);
	const VectorRegister SubZ_DC_DiagW_DC = VectorShuffle(Subtracts, DiagonalsXYZ_W0, 2, 0, 3, 0);
	const VectorRegister Row0 = VectorShuffle(AddX_DC_DiagX_DC, SubZ_DC_DiagW_DC, 2, 0, 0, 2);

	// OutMatrix.M[1][0] = (xy2 - wz2);			            // Subtracts.X
	// OutMatrix.M[1][1] = (1.0f - (xx2 + zz2));		    // Diagonal.Y
	// OutMatrix.M[1][2] = (yz2 + wx2);						// Adds.Y
	// OutMatrix.M[1][3] = 0.0f;                            // DiagonalsXYZ_W0.W
	const VectorRegister SubX_DC_DiagY_DC = VectorShuffle(Subtracts, DiagonalsXYZ_W0, 0, 0, 1, 0);
	const VectorRegister AddY_DC_DiagW_DC = VectorShuffle(Adds, DiagonalsXYZ_W0, 1, 0, 3, 0);
	const VectorRegister Row1 = VectorShuffle(SubX_DC_DiagY_DC, AddY_DC_DiagW_DC, 0, 2, 0, 2);

	// OutMatrix.M[2][0] = (xz2 + wy2);						// Adds.Z
	// OutMatrix.M[2][1] = (yz2 - wx2);						// Subtracts.Y
	// OutMatrix.M[2][2] = (1.0f - (xx2 + yy2));		    // Diagonals.Z
	// OutMatrix.M[2][3] = 0.0f;                            // DiagonalsXYZ_W0.W
	const VectorRegister AddZ_DC_SubY_DC = VectorShuffle(Adds, Subtracts, 2, 0, 1, 0);
	const VectorRegister Row2 = VectorShuffle(AddZ_DC_SubY_DC, DiagonalsXYZ_W0, 0, 2, 2, 3);

	VectorStoreAligned(Row0, &(OutMatrix.M[0][0]));
	VectorStoreAligned(Row1, &(OutMatrix.M[1][0]));
	VectorStoreAligned(Row2, &(OutMatrix.M[2][0]));

	// OutMatrix.M[3][0] = Translation.X;
	// OutMatrix.M[3][1] = Translation.Y;
	// OutMatrix.M[3][2] = Translation.Z;
	// OutMatrix.M[3][3] = 1.0f;
	const VectorRegister Row3 = VectorSet_W1(Translation);
	VectorStoreAligned(Row3, &(OutMatrix.M[3][0]));

	return OutMatrix;
}


FMatrix FTransform::ToMatrixWithScale() const noexcept
{
	FMatrix OutMatrix;
	VectorRegister DiagonalsXYZ;
	VectorRegister Adds;
	VectorRegister Subtracts;

	ToMatrixInternal(DiagonalsXYZ, Adds, Subtracts);
	const VectorRegister DiagonalsXYZ_W0 = VectorSet_W0(DiagonalsXYZ);

	// OutMatrix.M[0][0] = (1.0f - (yy2 + zz2)) * Scale.X;    // Diagonal.X
	// OutMatrix.M[0][1] = (xy2 + wz2) * Scale.X;             // Adds.X
	// OutMatrix.M[0][2] = (xz2 - wy2) * Scale.X;             // Subtracts.Z
	// OutMatrix.M[0][3] = 0.0f;                              // DiagonalsXYZ_W0.W
	const VectorRegister AddX_DC_DiagX_DC = VectorShuffle(Adds, DiagonalsXYZ_W0, 0, 0, 0, 0);
	const VectorRegister SubZ_DC_DiagW_DC = VectorShuffle(Subtracts, DiagonalsXYZ_W0, 2, 0, 3, 0);
	const VectorRegister Row0 = VectorShuffle(AddX_DC_DiagX_DC, SubZ_DC_DiagW_DC, 2, 0, 0, 2);

	// OutMatrix.M[1][0] = (xy2 - wz2) * Scale.Y;             // Subtracts.X
	// OutMatrix.M[1][1] = (1.0f - (xx2 + zz2)) * Scale.Y;    // Diagonal.Y
	// OutMatrix.M[1][2] = (yz2 + wx2) * Scale.Y;             // Adds.Y
	// OutMatrix.M[1][3] = 0.0f;                            // DiagonalsXYZ_W0.W
	const VectorRegister SubX_DC_DiagY_DC = VectorShuffle(Subtracts, DiagonalsXYZ_W0, 0, 0, 1, 0);
	const VectorRegister AddY_DC_DiagW_DC = VectorShuffle(Adds, DiagonalsXYZ_W0, 1, 0, 3, 0);
	const VectorRegister Row1 = VectorShuffle(SubX_DC_DiagY_DC, AddY_DC_DiagW_DC, 0, 2, 0, 2);

	// OutMatrix.M[2][0] = (xz2 + wy2) * Scale.Z;             // Adds.Z
	// OutMatrix.M[2][1] = (yz2 - wx2) * Scale.Z;             // Subtracts.Y
	// OutMatrix.M[2][2] = (1.0f - (xx2 + yy2)) * Scale.Z;    // Diagonals.Z
	// OutMatrix.M[2][3] = 0.0f;                              // DiagonalsXYZ_W0.W
	const VectorRegister AddZ_DC_SubY_DC = VectorShuffle(Adds, Subtracts, 2, 0, 1, 0);
	const VectorRegister Row2 = VectorShuffle(AddZ_DC_SubY_DC, DiagonalsXYZ_W0, 0, 2, 2, 3);

	VectorStoreAligned(Row0, &(OutMatrix.M[0][0]));
	VectorStoreAligned(Row1, &(OutMatrix.M[1][0]));
	VectorStoreAligned(Row2, &(OutMatrix.M[2][0]));

	// OutMatrix.M[3][0] = Translation.X;
	// OutMatrix.M[3][1] = Translation.Y;
	// OutMatrix.M[3][2] = Translation.Z;
	// OutMatrix.M[3][3] = 1.0f;
	const VectorRegister Row3 = VectorSet_W1(Translation);
	VectorStoreAligned(Row3, &(OutMatrix.M[3][0]));

	return OutMatrix;
}

void FTransform::SetFromMatrix(const FMatrix& InMatrix) noexcept
{
	FMatrix M = InMatrix;

	// Get the 3D scale from the matrix
	FVector3D InScale = M.ExtractScaling();
	Scale3D = VectorLoadFloat3_W0(&InScale);

	// If there is negative scaling going on, we handle that here
	if (InMatrix.Determinant() < 0.f)
	{
		// Assume it is along X and modify transform accordingly. 
		// It doesn't actually matter which axis we choose, the 'appearance' will be the same			
		Scale3D = VectorMultiply(Scale3D, GlobalVectorConstants::FloatMinus1_111);
		M.SetAxis(0, -M.GetScaledAxis(EAxis::X));
	}

	FQuat InRotation = FQuat(M);
	Rotation = VectorLoadAligned(&InRotation);
	FVector3D InTranslation = InMatrix.GetOrigin();
	Translation = VectorLoadFloat3_W0(&InTranslation);

	// Normalize rotation
	Rotation = VectorNormalizeQuaternion(Rotation);
}





void FTransform::Blend(const FTransform& Atom1, const FTransform& Atom2, float Alpha) noexcept
{
	if (FMath::Abs(Alpha) <= ZERO_ANIMWEIGHT_THRESH)
	{
		// if blend is all the way for child1, then just copy its bone atoms
		(*this) = Atom1;
	}
	else if (FMath::Abs(Alpha - 1.0f) <= ZERO_ANIMWEIGHT_THRESH)
	{
		// if blend is all the way for child2, then just copy its bone atoms
		(*this) = Atom2;
	}
	else
	{
		// Simple linear interpolation for translation and scale.			
		ScalarRegister BlendWeight = ScalarRegister(Alpha);

		Translation = FMath::Lerp(Atom1.Translation, Atom2.Translation, BlendWeight.Value);
		Scale3D = FMath::Lerp(Atom1.Scale3D, Atom2.Scale3D, BlendWeight.Value);

		VectorRegister VRotation = VectorLerpQuat(Atom1.Rotation, Atom2.Rotation, BlendWeight.Value);

		// ..and renormalize
		Rotation = VectorNormalizeQuaternion(VRotation);
	}
}

void FTransform::BlendWith(const FTransform& OtherAtom, float Alpha) noexcept
{
	if (Alpha > ZERO_ANIMWEIGHT_THRESH)
	{
		if (Alpha >= 1.f - ZERO_ANIMWEIGHT_THRESH)
		{
			// if blend is all the way for child2, then just copy its bone atoms
			(*this) = OtherAtom;
		}
		else
		{
			// Simple linear interpolation for translation and scale.				
			ScalarRegister BlendWeight = ScalarRegister(Alpha);
			Translation = FMath::Lerp(Translation, OtherAtom.Translation, BlendWeight.Value);

			Scale3D = FMath::Lerp(Scale3D, OtherAtom.Scale3D, BlendWeight.Value);

			VectorRegister VRotation = VectorLerpQuat(Rotation, OtherAtom.Rotation, BlendWeight.Value);

			// ..and renormalize
			Rotation = VectorNormalizeQuaternion(VRotation);

		}
	}
}



void FTransform::Mirror(EAxis MirrorAxis, EAxis FlipAxis) noexcept
{
	// We do convert to Matrix for mirroring. 
	FMatrix M = ToMatrixWithScale();
	M.Mirror(MirrorAxis, FlipAxis);
	SetFromMatrix(M);
}







FVector3D FTransform::GetSafeScaleReciprocal(const FVector3D& InScale, float Tolerance) noexcept
{
	FVector3D SafeReciprocalScale;
	if (FMath::Abs(InScale.X) <= Tolerance)
	{
		SafeReciprocalScale.X = 0.f;
	}
	else
	{
		SafeReciprocalScale.X = 1 / InScale.X;
	}

	if (FMath::Abs(InScale.Y) <= Tolerance)
	{
		SafeReciprocalScale.Y = 0.f;
	}
	else
	{
		SafeReciprocalScale.Y = 1 / InScale.Y;
	}

	if (FMath::Abs(InScale.Z) <= Tolerance)
	{
		SafeReciprocalScale.Z = 0.f;
	}
	else
	{
		SafeReciprocalScale.Z = 1 / InScale.Z;
	}

	return SafeReciprocalScale;
}

void FTransform::Multiply(FTransform* OutTransform, const FTransform* A, const FTransform* B) noexcept
{
	//	When Q = quaternion, S = single scalar scale, and T = translation
	//	QST(A) = Q(A), S(A), T(A), and QST(B) = Q(B), S(B), T(B)

	//	QST (AxB) 

	// QST(A) = Q(A)*S(A)*P*-Q(A) + T(A)
	// QST(AxB) = Q(B)*S(B)*QST(A)*-Q(B) + T(B)
	// QST(AxB) = Q(B)*S(B)*[Q(A)*S(A)*P*-Q(A) + T(A)]*-Q(B) + T(B)
	// QST(AxB) = Q(B)*S(B)*Q(A)*S(A)*P*-Q(A)*-Q(B) + Q(B)*S(B)*T(A)*-Q(B) + T(B)
	// QST(AxB) = [Q(B)*Q(A)]*[S(B)*S(A)]*P*-[Q(B)*Q(A)] + Q(B)*S(B)*T(A)*-Q(B) + T(B)

	//	Q(AxB) = Q(B)*Q(A)
	//	S(AxB) = S(A)*S(B)
	//	T(AxB) = Q(B)*S(B)*T(A)*-Q(B) + T(B)

	if (Private_AnyHasNegativeScale(A->Scale3D, B->Scale3D))
	{
		// @note, if you have 0 scale with negative, you're going to lose rotation as it can't convert back to quat
		MultiplyUsingMatrixWithScale(OutTransform, A, B);
	}
	else
	{
		const VectorRegister QuatA = A->Rotation;
		const VectorRegister QuatB = B->Rotation;
		const VectorRegister TranslateA = A->Translation;
		const VectorRegister TranslateB = B->Translation;
		const VectorRegister ScaleA = A->Scale3D;
		const VectorRegister ScaleB = B->Scale3D;

		// RotationResult = B.Rotation * A.Rotation
		OutTransform->Rotation = VectorQuaternionMultiply2(QuatB, QuatA);

		// TranslateResult = B.Rotate(B.Scale * A.Translation) + B.Translate
		const VectorRegister ScaledTransA = VectorMultiply(TranslateA, ScaleB);
		const VectorRegister RotatedTranslate = VectorQuaternionRotateVector(QuatB, ScaledTransA);
		OutTransform->Translation = VectorAdd(RotatedTranslate, TranslateB);

		// ScaleResult = Scale.B * Scale.A
		OutTransform->Scale3D = VectorMultiply(ScaleA, ScaleB);;
	}
}

void FTransform::BlendFromIdentityAndAccumulate(FTransform& FinalAtom, FTransform& SourceAtom, const ScalarRegister& BlendWeight) noexcept
{
	const VectorRegister Const0001 = GlobalVectorConstants::Float0001;
	const VectorRegister ConstNegative0001 = VectorSubtract(VectorZero(), Const0001);
	const VectorRegister VOneMinusAlpha = VectorSubtract(VectorOne(), BlendWeight.Value);
	const VectorRegister DefaultScale = MakeVectorRegister(1.f, 1.f, 1.f, 0.f);

	// Blend rotation
	// To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
	// const float Bias = (|A.B| >= 0 ? 1 : -1)
	// BlendedAtom.Rotation = (B * Alpha) + (A * (Bias * (1.f - Alpha)));
	// BlendedAtom.Rotation.QuaternionNormalize();
	// Note: A = (0,0,0,1), which simplifies things a lot; only care about sign of B.W now, instead of doing a dot product
	const VectorRegister RotationB = SourceAtom.Rotation;

	const VectorRegister QuatRotationDirMask = VectorCompareGE(RotationB, VectorZero());
	const VectorRegister BiasTimesA = VectorSelect(QuatRotationDirMask, Const0001, ConstNegative0001);
	const VectorRegister RotateBTimesWeight = VectorMultiply(RotationB, BlendWeight.Value);
	const VectorRegister UnnormalizedRotation = VectorMultiplyAdd(BiasTimesA, VOneMinusAlpha, RotateBTimesWeight);

	// Normalize blended rotation ( result = (Q.Q >= 1e-8) ? (Q / |Q|) : (0,0,0,1) )
	const VectorRegister BlendedRotation = VectorNormalizeSafe(UnnormalizedRotation, Const0001);

	// FinalAtom.Rotation = BlendedAtom.Rotation * FinalAtom.Rotation;
	FinalAtom.Rotation = VectorQuaternionMultiply2(BlendedRotation, FinalAtom.Rotation);

	// Blend translation and scale
	// BlendedAtom.Translation = Lerp(Zero, SourceAtom.Translation, Alpha);
	// BlendedAtom.Scale = Lerp(0, SourceAtom.Scale, Alpha);
	const VectorRegister BlendedTranslation = FMath::Lerp(VectorZero(), SourceAtom.Translation, BlendWeight.Value);
	const VectorRegister BlendedScale3D = FMath::Lerp(VectorZero(), SourceAtom.Scale3D, BlendWeight.Value);

	// Apply translation and scale to final atom
	// FinalAtom.Translation += BlendedAtom.Translation
	// FinalAtom.Scale *= BlendedAtom.Scale
	FinalAtom.Translation = VectorAdd(FinalAtom.Translation, BlendedTranslation);
	FinalAtom.Scale3D = VectorMultiply(FinalAtom.Scale3D, VectorAdd(DefaultScale, BlendedScale3D));
}






void FTransform::MultiplyUsingMatrixWithScale(FTransform* OutTransform, const FTransform* A, const FTransform* B) noexcept
{
	ConstructTransformFromMatrixWithDesiredScale(A->ToMatrixWithScale(), B->ToMatrixWithScale(), VectorMultiply(A->Scale3D, B->Scale3D), *OutTransform);
}

void FTransform::ConstructTransformFromMatrixWithDesiredScale(const FMatrix& AMatrix, const FMatrix& BMatrix, const VectorRegister& DesiredScale, FTransform& OutTransform) noexcept
{
	// the goal of using M is to get the correct orientation
	// but for translation, we still need scale
	FMatrix M = AMatrix * BMatrix;
	M.RemoveScaling();

	// apply negative scale back to axes
	FVector3D SignedScale;
	VectorStoreFloat3(VectorSign(DesiredScale), &SignedScale);

	M.SetAxis(0, SignedScale.X * M.GetScaledAxis(EAxis::X));
	M.SetAxis(1, SignedScale.Y * M.GetScaledAxis(EAxis::Y));
	M.SetAxis(2, SignedScale.Z * M.GetScaledAxis(EAxis::Z));

	// @note: if you have negative with 0 scale, this will return rotation that is identity
	// since matrix loses that axes
	FQuat Rotation = FQuat(M);
	Rotation.Normalize();

	// set values back to output
	OutTransform.Scale3D = DesiredScale;
	OutTransform.Rotation = VectorLoadAligned(&Rotation);

	// technically I could calculate this using FTransform but then it does more quat multiplication 
	// instead of using Scale in matrix multiplication
	// it's a question of between RemoveScaling vs using FTransform to move translation
	FVector3D Translation = M.GetOrigin();
	OutTransform.Translation = VectorLoadFloat3_W0(&Translation);
}

void FTransform::GetRelativeTransformUsingMatrixWithScale(FTransform* OutTransform, const FTransform* Base, const FTransform* Relative) noexcept
{
	// the goal of using M is to get the correct orientation
	// but for translation, we still need scale
	FMatrix AM = Base->ToMatrixWithScale();
	FMatrix BM = Relative->ToMatrixWithScale();

	// get combined scale
	// Scale = S(A)/S(B)
	static ScalarRegister STolerance(SMALL_NUMBER);
	VectorRegister VSafeScale3D = VectorSet_W0(GetSafeScaleReciprocal(Relative->Scale3D, STolerance));
	VectorRegister VScale3D = VectorMultiply(Base->Scale3D, VSafeScale3D);
	ConstructTransformFromMatrixWithDesiredScale(AM, BM.Inverse(), VScale3D, *OutTransform);
}

