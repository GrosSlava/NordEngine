
#pragma once

#include "CoreMinimal.h"

#include "CoreInterfaces/APIListener.h"



class IDeviceResourcesAdapter;

class GWorld;



/*
	Engine for graphics. 
	@see IDeviceResourcesAdapter.
*/
class ENGINE_API GGraphicsEngine : public IAPIListener
{
	friend class GCoreObjectsFacade;

	GENERATED_BODY(GGraphicsEngine)
	NONCOPYABLE(GGraphicsEngine)

public:

	GGraphicsEngine() { }
	virtual ~GGraphicsEngine();



public:

	virtual void BroadcastEvents();
	virtual void Render(GWorld* World);

	void SetNewDeviceResourcesAdapter(IDeviceResourcesAdapter* Adapter);
	FORCEINLINE IDeviceResourcesAdapter* GetDeviceResourcesAdapter() const noexcept { return DeviceResourcesAdapter; }
	

	void BeginGameLogicSection();
	void EndGameLogicSection();

	void SetVSyncEnabled(bool Enable);
	FORCEINLINE bool GetVSyncEnable() const noexcept { return VSyncEnabled; }

public:

	FORCEINLINE bool GetGraphicsGameWasStarted() const noexcept { return GraphicsGameWasStarted; }

protected:

	virtual void OnGameStart();
	virtual void OnGameEnd();


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

protected:

	virtual void ApplySettingsToAdapter();




protected:

	IDeviceResourcesAdapter* DeviceResourcesAdapter = nullptr;

	bool VSyncEnabled = false;
	uint16 FrameRateLimit = 0;

	bool GraphicsGameWasStarted = false;
};