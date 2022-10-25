
#pragma once

#include "CoreMinimal.h"

#include "CoreInterfaces/APIListener.h"
#include "CoreGame/CoreObjectsFacade.h"
#include "CoreGame/CoreTickLoop.h"




/*
	Core global states.
*/
enum class ECoreGameState : uint8
{
	NotStarted,	   /* Game not started. */
	Initializing,  /* Game is initializing now. */
	InProgress,	   /* Game ready and do game logic. */
	Deinitializing /* Game is over and the data is being cleared. */
};



/*
	Main object of game.
	Provide access to all subsystems and listen events from platform API.
*/
class ENGINE_API GCoreGame final : public IAPIListener
{
	SINGLETON(GCoreGame)
	GENERATED_BODY(GCoreGame)

public:

	~GCoreGame();



public:

	/*
		Start game. Core entry point.
	*/
	void Start();

	/*
		Tick full game.
	*/
	void Tick();


	FORCEINLINE const GCoreObjectsFacade& GetCoreObjectsFacade() const noexcept { return CoreObjectsFacade; }
	FORCEINLINE const GCoreTickLoop& GetCoreTickLoop() const noexcept { return CoreTickLoop; }

	FORCEINLINE uint16 GetFPSLock() const noexcept { return FPSLock; }
	void SetFPSLock(uint16 FPS);
	FORCEINLINE bool GetVSyncEnabled() const noexcept { return VSyncEnabled; }
	void SetVSyncEnabled(bool Enable);

public:

	void OnGameStart();
	void OnGameEnd();

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




private:

	/*
		Global game state.
	*/
	ECoreGameState GameState = ECoreGameState::NotStarted;

	/*
		All core objects.
	*/
	GCoreObjectsFacade CoreObjectsFacade;
	/*
		Core main game tick object.
	*/
	GCoreTickLoop CoreTickLoop;


	/*
		Cached last global FPS lock.
	*/
	uint16 FPSLock = 0;
	/*
		Cached last global VSync enable.
	*/
	bool VSyncEnabled = false;
	/*
		Is FPSLock or VSyncEnabled changed at previous tick.
	*/
	bool FPSLockIsDirty = true;
};



FORCEINLINE GCoreGame* GCoreGame::Get() noexcept
{
	static GCoreGame GGame;
	return &GGame;
}