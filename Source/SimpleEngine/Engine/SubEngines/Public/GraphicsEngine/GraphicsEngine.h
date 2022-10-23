
#pragma once

#include "CoreMinimal.h"

#include "CoreInterfaces/APIListener.h"
#include "CoreInterfaces/SubEngine.h"




class IDeviceResourcesAdapter;

class GWorld;



/*
	Engine for graphics. 
	@see IDeviceResourcesAdapter.
*/
class ENGINE_API GGraphicsEngine : public IAPIListener, public ISubEngine
{
	friend class GCoreObjectsFacade;

	GENERATED_BODY(GGraphicsEngine)
	NONCOPYABLE(GGraphicsEngine)

public:

	GGraphicsEngine() { }
	virtual ~GGraphicsEngine();



public:

	/*
		Render all objects from World.
		@param World - World to render.
	*/
	virtual void Render(GWorld* World);

	/*
		Set adapter for rendering API or hardware interface.
	*/
	void SetNewDeviceResourcesAdapter(IDeviceResourcesAdapter* Adapter);
	/*
		@return current rendering adapter.
	*/
	FORCEINLINE IDeviceResourcesAdapter* GetDeviceResourcesAdapter() const noexcept { return DeviceResourcesAdapter; }



	/*
		Enable/disable vertical sync.
	*/
	void SetVSyncEnabled(bool Enable);
	/*
		@return vertical sync enabled or not.
	*/
	FORCEINLINE bool GetVSyncEnable() const noexcept { return VSyncEnabled; }


	/*
		@return true if GraphicsEngine has started the game.
	*/
	FORCEINLINE bool GetGraphicsGameWasStarted() const noexcept { return GraphicsGameWasStarted; }

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


protected:

	/*
		Event to start graphics logic.
	*/
	virtual void OnGameStart();
	/*
		Event to end graphics logic.
	*/
	virtual void OnGameEnd();

protected:

	/*
		Apply current graphics settings to adapter.
	*/
	virtual void ApplySettingsToAdapter();




protected:

	/*
		Adapter to communicate with rendering API or hardware interface.
	*/
	IDeviceResourcesAdapter* DeviceResourcesAdapter = nullptr;


	/*
		VSync enable state.
	*/
	bool VSyncEnabled = false;


	/*
		Marks that the GraphicsEngine has started the game.
	*/
	bool GraphicsGameWasStarted = false;
	/*
		Rendering critical section marker.
	*/
	bool IsRenderingNow = false;
};