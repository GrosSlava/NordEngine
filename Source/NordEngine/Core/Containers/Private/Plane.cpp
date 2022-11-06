
#include "Plane.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"


FPlane::FPlane(const FVector4D& V) noexcept : FVector3D(V),
	 W(V.W)
{

}




FPlane FPlane::TransformBy(const FMatrix& M) const
{
	const FMatrix tmpTA = M.TransposeAdjoint();
	const float DetM = M.Determinant();
	return this->TransformByUsingAdjointT(M, DetM, tmpTA);
}

FPlane FPlane::TransformByUsingAdjointT(const FMatrix& M, float DetM, const FMatrix& TA) const
{
	FVector3D newNorm = TA.TransformVector(*this).GetSafeNormal();

	if (DetM < 0.f)
	{
		newNorm *= -1.0f;
	}

	return FPlane(M.TransformPosition(*this * W), newNorm);
}
