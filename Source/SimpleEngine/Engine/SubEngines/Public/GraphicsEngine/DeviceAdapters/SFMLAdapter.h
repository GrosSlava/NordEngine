
#pragma once

#include "CoreMinimal.h"

#include "GraphicsEngine/DeviceResourcesAdapter.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include "CoreGame/CoreObjectsFactory.h"




class GBaseWindow;



/*
	Builder for USFMLAdapter2D.
	@see FDeviceResourcesAdapterBuilder, GCoreObjectsFactory.
*/
class FSFMLAdapter2DBuilder : public FDeviceResourcesAdapterBuilder
{
	GENERATED_BODY_CHILD(FSFMLAdapter2DBuilder, FDeviceResourcesAdapterBuilder)

public:

	FSFMLAdapter2DBuilder() { }
	virtual ~FSFMLAdapter2DBuilder() { }

public:

	virtual IDeviceResourcesAdapter* ConstructDeviceResourcesAdapter() override;
};




/*
	2D adapter for SFML API.
	@see IDeviceResourcesAdapter2D, GGraphicsEngine.
*/
class USFMLAdapter2D : public IDeviceResourcesAdapter2D
{
	GENERATED_BODY(USFMLAdapter2D)
	NONCOPYABLE(USFMLAdapter2D)

public:

	USFMLAdapter2D() { }
	~USFMLAdapter2D();



public:

	//..................IAPIListener interface.......................//

	virtual void OnActivated() override;
	virtual void OnDeactivated() override;
	virtual void OnSuspending() override;
	virtual void OnResuming() override;
	virtual void OnWindowMoved() override;
	virtual void OnWindowSizeChanged() override;
	virtual void OnWindowTitleChanged() override;
	virtual void OnWindowIconChanged() override;
	virtual void OnWindowCursorChanged() override;
	virtual void OnWindowMouseCursorVisibilityChanged() override;
	virtual void OnWindowMouseCursorGrabbingChanged() override;

	//...............................................................//

	//....................ISubEngine interface.......................//

	virtual void BroadcastEvents() override;

	virtual void BeginGameLogicSection() override;
	virtual void EndGameLogicSection() override;

	//...............................................................//

	//.............IDeviceResourcesAdapter2D interface...............//

	virtual void InitAdapter(GBaseWindow* Window) override;
	virtual void Render(const FSceneView& SceneView) override;

	virtual void OnGameStart() override;
	virtual void PreGameDestroy() override;
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	virtual void SetVSyncEnabled(bool Enable) override;
	virtual void SetCameraFOV(float VerticalFovInRadians) override;
	virtual void SetCameraLocation(const FVector2D& NewLocation) override;
	virtual void SetCameraRotation(float NewRotation) override;

	//...............................................................//




private:

	/*
		Cached owned platform window.
	*/
	GBaseWindow* MyWindow = nullptr;
	/*
		Cached SFML window.
	*/
	sf::RenderWindow SFMLWindow;
};