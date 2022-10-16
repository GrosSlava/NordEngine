
#pragma once

#include "CoreMinimal.h"

#include "CoreInterfaces/APIListener.h"

#include "TickTimer.h"
#include "PerformanceBlock.h"




class GWorld;
class GBaseWindow;
class GGameInstance;
class GCameraManager;

class GGraphicsEngine;



/*
	Core global states.
*/
enum class ECoreGameState : uint8
{
	NotStarted,
	Initializing,
	InProgress,
	Deinitializing
};




class ENGINE_API GCoreGame final : public IAPIListener
{
	friend GBaseWindow;

	SINGLETON(GCoreGame)

public:

	~GCoreGame();



public:

	/*
		Start game. Core entry point.
	*/
	void Start();

	/*
        Notify all engine systems about tick.
        Call it only from platform main function.
    */
	void Tick();


	FORCEINLINE const FTickTimer& GetTickTimer() const noexcept { return TickTimer; }
	FORCEINLINE const FPerformanceBlock& GetCodePerformance() const noexcept { return CodeBlockPerformance; }
	FORCEINLINE const FPerformanceBlock& GetGraphicsEnginePerformance() const noexcept { return GraphicsEnginePerformance; }


	FORCEINLINE GWorld* GetWorld() const noexcept { return MainWorld; }
	FORCEINLINE GBaseWindow* GetWindow() const noexcept { return MainWindow; }
	FORCEINLINE GGameInstance* GetGameInstance() const noexcept { return GameInstance; }
	FORCEINLINE GCameraManager* GetCameraManager() const noexcept { return CameraManager; }

	FORCEINLINE GGraphicsEngine* GetGraphicsEngine() const noexcept { return GraphicsEngine; }

protected:

	void OnGameStart();
	void OnGameEnd();


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

private:

	void ConstructCoreObjects();
	void InitSubsystems();
	void StartSubsystems();

	void Update();
	void UpdateInputs();
	void UpdateSubsystems();




private:

	/*
		Global game state.
	*/
	ECoreGameState GameState = ECoreGameState::NotStarted;



	GWorld* MainWorld = nullptr;
	GBaseWindow* MainWindow = nullptr;
	GGameInstance* GameInstance = nullptr;
	GCameraManager* CameraManager = nullptr;

	//...........................Engines.............................//

	GGraphicsEngine* GraphicsEngine = nullptr;

	//...............................................................//



	FTickTimer TickTimer;

	//.................Global performance blocks.....................//

	FPerformanceBlock CodeBlockPerformance;
	FPerformanceBlock GraphicsEnginePerformance;

	//...............................................................//
};



FORCEINLINE GCoreGame* GCoreGame::Get() noexcept
{
	static GCoreGame GGame;
	return &GGame;
}