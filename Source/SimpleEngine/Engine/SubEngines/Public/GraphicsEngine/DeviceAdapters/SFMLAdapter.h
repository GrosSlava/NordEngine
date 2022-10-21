
#pragma once

#include "CoreMinimal.h"

#include "GraphicsEngine/DeviceResourcesAdapter.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include "CoreObjectsFactory.h"




class GBaseWindow;



class FSFMLAdapter2DBuilder : public FDeviceResourcesAdapterBuilder
{
	GENERATED_BODY_CHILD(FSFMLAdapter2DBuilder, FDeviceResourcesAdapterBuilder)

public:

	FSFMLAdapter2DBuilder() { }
	virtual ~FSFMLAdapter2DBuilder() { }

public:

	virtual IDeviceResourcesAdapter* ConstructDeviceResourcesAdapter() override;
};





class USFMLAdapter2D : public IDeviceResourcesAdapter2D
{
	GENERATED_BODY(USFMLAdapter2D)
	NONCOPYABLE(USFMLAdapter2D)

public:

	USFMLAdapter2D() { }
	~USFMLAdapter2D();



public:

	virtual void InitAdapter(GBaseWindow* Window) override;

	virtual void BeginGameLogicSection() override;
	virtual void EndGameLogicSection() override;

	virtual void BroadcastEvents() override;
	virtual void Render(const FSceneView& SceneView) override;

	virtual void SetVSyncEnabled(bool Enable) override;
	virtual void SetFrameRateLimit(uint16 Limit) override;
	virtual void SetCameraFOV(float VerticalFovInRadians) override;
	virtual void SetCameraLocation(const FVector2D& NewLocation) override;
	virtual void SetCameraRotation(float NewRotation) override;

	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	virtual void OnGameStart() override;
	virtual void PreGameDestroy() override;

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




private:

	GBaseWindow* MyWindow = nullptr;
	sf::RenderWindow SFMLWindow;
};