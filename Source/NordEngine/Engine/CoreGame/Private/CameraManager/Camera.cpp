#if 0
#include "Camera.h"





void GCamera::ResetCamera()
{
	SetCameraLocation(FVector3D(0.0f, 0.0f, 0.0f));
	SetCameraRotation(FQuat(0.0f, 0.0f, 0.0f, 1.0f));
	SetFieldOfView(PI / 4);
}




void GCamera::DeprojectScreenToWorld(uint16 X, uint16 Y, uint16 ScreenWidth, uint16 ScreenHeight, const FMatrix& InvViewProjMatrix, FVector3D& out_WorldOrigin)
{
	// Get the pixel coordinates into 0..1 normalized coordinates within the constrained view rectangle
	const float NormalizedX = X / ((float)ScreenWidth);
	const float NormalizedY = Y / ((float)ScreenHeight);

	// Get the pixel coordinates into -1..1 projection space
	const float ScreenSpaceX = (NormalizedX - 0.5f) * 2.0f;
	const float ScreenSpaceY = ((1.0f - NormalizedY) - 0.5f) * 2.0f;

	// The start of the ray trace is defined to be at mousex,mousey,1 in projection space (z=1 is near, z=0 is far - this gives us better precision)
	// To get the direction of the ray trace we need to use any z between the near and the far plane, so let's use (mousex, mousey, 0.5)
	const FVector4D RayStartProjectionSpace = FVector4D(ScreenSpaceX, ScreenSpaceY, 1.0f, 1.0f);


	// Projection (changing the W coordinate) is not handled by the FMatrix transforms that work with vectors, so multiplications
	// by the projection matrix should use homogeneous coordinates (i.e. FPlane).
	const FVector4D HGRayStartWorldSpace = InvViewProjMatrix.TransformFVector4(RayStartProjectionSpace);
	FVector3D RayStartWorldSpace(HGRayStartWorldSpace.X, HGRayStartWorldSpace.Y, HGRayStartWorldSpace.Z);


	// divide vectors by W to undo any projection and get the 3-space coordinate
	if( HGRayStartWorldSpace.W != 0.0f )
	{
		RayStartWorldSpace /= HGRayStartWorldSpace.W;
	}

	// Finally, store the results in the outputs
	out_WorldOrigin = RayStartWorldSpace;
}
#endif