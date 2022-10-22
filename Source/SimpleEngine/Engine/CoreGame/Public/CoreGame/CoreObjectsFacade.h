
#pragma once

#include "CoreMinimal.h"

#include "CoreInterfaces/APIListener.h"




class GWorld;
class GBaseWindow;
class GGameInstance;
class GCameraManager;

class GGraphicsEngine;



/*
	Facade for all core objects.
	@see GGameSettings, GCoreGame.
*/
class ENGINE_API GCoreObjectsFacade final : public IAPIListener
{
	GENERATED_BODY(GCoreObjectsFacade)

public:

	~GCoreObjectsFacade();



public:

	/*
		Create/recreate all core objects.
	*/
	void ConstructCoreObjects();
	/*
		Init all subsystems.
	*/
	void InitSubsystems();
	/*
		Start subsystems after total initialization.
	*/
	void StartSubsystems();
	/*
		End and clear all subsystems.
	*/
	void Clear();

public:

	FORCEINLINE GWorld* GetWorld() const noexcept { return MainWorld; }
	FORCEINLINE GBaseWindow* GetWindow() const noexcept { return MainWindow; }
	FORCEINLINE GGameInstance* GetGameInstance() const noexcept { return GameInstance; }
	FORCEINLINE GCameraManager* GetCameraManager() const noexcept { return CameraManager; }

	FORCEINLINE GGraphicsEngine* GetGraphicsEngine() const noexcept { return GraphicsEngine; }

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




protected:

	GWorld* MainWorld = nullptr;
	GBaseWindow* MainWindow = nullptr;
	GGameInstance* GameInstance = nullptr;
	GCameraManager* CameraManager = nullptr;

	//...........................Engines.............................//

	GGraphicsEngine* GraphicsEngine = nullptr;

	//...............................................................//
};