
#pragma once

#include "CoreMinimal.h"

#include "GraphicsEngine/SceneView/SceneView.h"




class GBaseWindow;


/*
	Adapter interface of hardware or third-party renderer.
*/
class IDeviceResourcesAdapter
{
public:

	virtual ~IDeviceResourcesAdapter() { }



public:

	virtual void InitAdapter(GBaseWindow* Window) = 0;

	virtual void BeginGameLogicSection() = 0;
	virtual void EndGameLogicSection() = 0;

	virtual void BroadcastEvents() = 0;
	virtual void Render(const FSceneView& SceneView) = 0;

	virtual void SetVSyncEnabled(bool Enable) = 0;
	virtual void SetFrameRateLimit(uint16 Limit) = 0;
	virtual void SetCameraFOV(float VerticalFovInRadians) = 0;


	virtual void OnDeviceLost() = 0;
	virtual void OnDeviceRestored() = 0;

	virtual void OnGameStart() = 0;
	virtual void PreGameDestroy() = 0;

	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;
	virtual void OnSuspending() = 0;
	virtual void OnResuming() = 0;
	virtual void OnWindowMoved() = 0;
	virtual void OnWindowTitleChanged(const std::string& Text) = 0;
	virtual void OnWindowSizeChanged(uint16 Width, uint16 Height) = 0;
	virtual void OnWindowFullScreenChanged(bool Enable) = 0;
	virtual void OnWindowIconChanged(const std::string& IconPath) = 0;
	virtual void OnWindowCursorChanged(const std::string& CursorPath) = 0;
	virtual void OnWindowShowMouseCursorChanged(bool Show) = 0;
};

/*
	2D specialized adapter for renderer.
*/
class IDeviceResourcesAdapter2D : public IDeviceResourcesAdapter
{
public:

	virtual void SetCameraLocation(const FVector2D& NewLocation) = 0;
	virtual void SetCameraRotation(float NewRotation) = 0;
};