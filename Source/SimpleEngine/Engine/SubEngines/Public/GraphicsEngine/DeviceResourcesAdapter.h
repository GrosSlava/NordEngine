
#pragma once

#include "CoreMinimal.h"

#include "CoreInterfaces/APIListener.h"
#include "CoreInterfaces/SubEngine.h"

#include "GraphicsEngine/SceneView.h"




class GBaseWindow;


/*
	Adapter interface for hardware or third-party render API.
	Can't be implemented directly.
	@see IDeviceResourcesAdapter2D, IDeviceResourcesAdapter3D.
*/
class IDeviceResourcesAdapter : public IAPIListener, public ISubEngine
{
public:

	virtual ~IDeviceResourcesAdapter() { }



public:

	/*
		Initialize adapter by platform specific Window.
	*/
	virtual void InitAdapter(GBaseWindow* Window) = 0;

	/*
		Request rendering.
		@param SceneView - prepared view from all scene objects.
	*/
	virtual void Render(const FSceneView& SceneView) = 0;


	/*
		Event to start graphics logic.
	*/
	virtual void OnGameStart() = 0;
	/*
		Event to end graphics logic.
	*/
	virtual void PreGameDestroy() = 0;

	/*
		Hardware device was lost.
	*/
	virtual void OnDeviceLost() = 0;
	/*
		Hardware device was restored.
	*/
	virtual void OnDeviceRestored() = 0;


	/*
		Enable/disable vertical sync.
	*/
	virtual void SetVSyncEnabled(bool Enable) = 0;
	/*
		Change camera field of view.
	*/
	virtual void SetCameraFOV(float VerticalFovInRadians) = 0;
};




/*
	2D specialized adapter for renderer.
*/
class IDeviceResourcesAdapter2D : public IDeviceResourcesAdapter
{
public:

	/*
		Set virtual rendering 2D camera location.
	*/
	virtual void SetCameraLocation(const FVector2D& NewLocation) = 0;
	/*
		Set virtual rendering 2D camera rotation.
		@param NewRotation - angle in [TODO FAngle]
	*/
	virtual void SetCameraRotation(float NewRotation) = 0;
};

/*
	3D specialized adapter for renderer.
*/
class IDeviceResourcesAdapter3D : public IDeviceResourcesAdapter
{
public:

	//TODO
};