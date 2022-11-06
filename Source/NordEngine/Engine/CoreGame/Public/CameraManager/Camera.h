#pragma once

#include "SpecificationMacros.h"
#include "Transform.h"
#include "Matrix.h"




/*
	Base class of camera for connection between engine and graphics engine.
*/
class GCamera
{
	GENERATED_BODY(GCamera)

public:

	virtual ~GCamera() { }


public:

	FORCEINLINE FVector3D GetCameraForwardVector() const noexcept { return -CameraWorldTransform.GetUnitAxis(EAxis::Z).GetSafeNormal(); }
	FORCEINLINE FVector3D GetCameraRightVector() const noexcept { return CameraWorldTransform.GetUnitAxis(EAxis::X).GetSafeNormal(); }
	FORCEINLINE FVector3D GetCameraUpVector() const noexcept { return CameraWorldTransform.GetUnitAxis(EAxis::Y).GetSafeNormal(); }
	
	
	/*
		Reset all properties to default
	*/
	virtual void ResetCamera();

	FORCEINLINE bool WerePropertyChanged() const noexcept { return CameraPropertiesWereChanged; }
	FORCEINLINE void OnChangesApplied() noexcept { CameraPropertiesWereChanged = false; }



	//..............................Camera properties Get/Set................................//
	
	FORCEINLINE FVector3D GetCameraLocation() const noexcept { return CameraWorldTransform.GetLocation(); }
	FORCEINLINE void SetCameraLocation(const FVector3D& NewLocation) { CameraWorldTransform.SetLocation(NewLocation); }

	FORCEINLINE FQuat GetCameraRotation() const noexcept { return CameraWorldTransform.GetRotation(); }
	FORCEINLINE void SetCameraRotation(const FQuat& NewRotation) { CameraWorldTransform.SetRotation(NewRotation); }

	FORCEINLINE float GetCameraFOV() const noexcept { return FOV; }
	FORCEINLINE void SetFieldOfView(float NewFOV){ FOV = NewFOV; CameraPropertiesWereChanged = true; }

	//.......................................................................................//
	


public:

	static void DeprojectScreenToWorld(uint16 X, uint16 Y, uint16 ScreenWidth, uint16 ScreenHeight, const FMatrix& InvViewProjMatrix, FVector3D& out_WorldOrigin);



protected:

	/*
		Sets to true when some of properties changed. Used in CameraManager to update properties.
	*/
	bool CameraPropertiesWereChanged = false;


	/*
		Camera transform in the world.
	*/
	FTransform CameraWorldTransform;

	//..............................Camera properties................................//
	
	/*
	   Field of view angle in radians
	*/
	float FOV = PI / 4;

	//...............................................................................//
};
