
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
	friend class GCoreGame;

	GENERATED_BODY(GGraphicsEngine)
	NONCOPYABLE(GGraphicsEngine)

public:

	GGraphicsEngine() { }
	virtual ~GGraphicsEngine();



public:

	void SetNewDeviceResourcesAdapter(IDeviceResourcesAdapter* Adapter);
	FORCEINLINE IDeviceResourcesAdapter* GetDeviceResourcesAdapter() const noexcept { return DeviceResourcesAdapter; }
	

	void BeginGameLogicSection();
	void EndGameLogicSection();

	void SetVSyncEnabled(bool Enable);
	FORCEINLINE bool GetVSyncEnable() const noexcept { return VSyncEnabled; }

	void SetFrameRateLimit(uint16 Limit);
	FORCEINLINE uint16 GetFrameRateLimit() const noexcept { return FrameRateLimit; }

public:

	FORCEINLINE bool GetGraphicsGameWasStarted() const noexcept { return GraphicsGameWasStarted; }

protected:

	virtual void BroadcastEvents();
	virtual void Render(GWorld* World);

	virtual void OnGameStart();
	virtual void OnGameEnd();


	//..................IAPIListener interface.......................//

	virtual void OnActivated() override;
	virtual void OnDeactivated() override;
	virtual void OnSuspending() override;
	virtual void OnResuming() override;
	virtual void OnWindowMoved() override;
	virtual void OnWindowTitleChanged(const std::string& Text) override;
	virtual void OnWindowSizeChanged(uint16 Width, uint16 Height) override;
	virtual void OnWindowFullScreenChanged(bool Enable) override;
	virtual void OnWindowIconChanged(const std::string& IconPath) override;
	virtual void OnWindowCursorChanged(const std::string& CursorPath) override;
	virtual void OnWindowShowMouseCursorChanged(bool Show) override;

	//...............................................................//

protected:

	virtual void ApplySettingsToAdapter();




protected:

	IDeviceResourcesAdapter* DeviceResourcesAdapter = nullptr;

	bool VSyncEnabled = false;
	uint16 FrameRateLimit = 0;

	bool GraphicsGameWasStarted = false;
};