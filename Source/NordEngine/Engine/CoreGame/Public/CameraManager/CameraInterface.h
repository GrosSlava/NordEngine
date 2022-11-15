
#pragma once

#include "CoreMinimal.h"




class IDeviceResourcesAdapter;



/*
	Interface of low-level camera.
	Can't be implemented directly.
	@see ICamera2D, ICamera3D.
*/
class ENGINE_API ICamera
{
public:

	virtual ~ICamera() { }



public:

	/*
		Reset all properties to default.
	*/
	virtual void ResetCamera() = 0;
	/*
		Apply all visual properties for rendering.
	*/
	virtual void ApplyVisualProperties(IDeviceResourcesAdapter* DeviceResourcesAdapter) = 0;

	/*
		@return camera field of view.
		[TODO FAngle]
	*/
	virtual float GetCameraFOV() const = 0; 
	/*
		Set camera field of view.
		[TODO FAngle]
	*/
	virtual void SetFieldOfView(float NewFOV) = 0;
};




/*
	2D specialized camera interface.
*/
class ENGINE_API ICamera2D
{
public:

	/*
		@return unit vector for camera right direction.
	*/
	virtual FVector2D GetCameraRightVector() const = 0;
	/*
		@return unit vector for camera up direction.
	*/
	virtual FVector2D GetCameraUpVector() const = 0;
	/*
		@return camera location in 2D world space.
	*/
	virtual FVector2D GetCameraLocation() const = 0;
	/*
		Set camera location in 2D world space.
	*/
	virtual void SetCameraLocation(const FVector2D& NewLocation) = 0;
	/*
		@return camera rotation angle.
		[TODO FAngle]
	*/
	virtual float GetCameraRotation() const = 0;
	/*
		Set camera rotation angle.
		[TODO FAngle]
	*/
	virtual void SetCameraRotation(float NewRotation) = 0;
};

/*
	3D specialized camera interface.
*/
class ENGINE_API ICamera3D
{
public:
	/*
		@return unit vector for camera right direction.
	*/
	virtual FVector3D GetCameraRightVector() const = 0;
	/*
		@return unit vector for camera up direction.
	*/
	virtual FVector3D GetCameraUpVector() const = 0;
	/*
		@return unit vector for camera forward direction.
	*/
	virtual FVector3D GetCameraForwardVector() const = 0;
	/*
		@return camera location in 3D world space.
	*/
	virtual FVector3D GetCameraLocation() const = 0;
	/*
		Set camera location in 3D world space.
	*/
	virtual void SetCameraLocation(const FVector3D& NewLocation) = 0;
	/*
		@return camera rotation quaternion.
	*/
	virtual FQuat GetCameraRotation() const = 0;
	/*
		Set camera rotation quaternion.
	*/
	virtual void SetCameraRotation(const FQuat& NewRotation) = 0;
};