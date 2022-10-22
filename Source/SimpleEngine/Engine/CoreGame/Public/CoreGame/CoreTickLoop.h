
#pragma once

#include "CoreMinimal.h"

#include "TickTimer.h"
#include "PerformanceBlock.h"

#include "CoreGame/CoreObjectsFacade.h"




/*
	Main game tick object.
	@see GCoreGame, GCoreObjectsFacade.
*/
class ENGINE_API GCoreTickLoop final
{
	GENERATED_BODY(GCoreTickLoop)

public:

	~GCoreTickLoop();



public:

	/*
        Notify all engine systems about tick.
		@param CoreObjectsFacade - facade of core objects to tick.
	*/
	void Tick(GCoreObjectsFacade& CoreObjectsFacade);

public:

	FORCEINLINE const FTickTimer& GetTickTimer() const noexcept { return TickTimer; }
	FORCEINLINE const FPerformanceBlock& GetGamePerformance() const noexcept { return GameBlockPerformance; }
	FORCEINLINE const FPerformanceBlock& GetGraphicsEnginePerformance() const noexcept { return GraphicsEnginePerformance; }

	FORCEINLINE double GetTotalWorkTime() const { return GameBlockPerformance.GetWorkTime() + GraphicsEnginePerformance.GetWorkTime(); }
	FORCEINLINE double GetCurrentFPS() const { return 1000.0 / GetTotalWorkTime(); }
	FORCEINLINE uint16 GetFPSLock() const noexcept { return FPSLock; }
	void SetFPSLock(uint16 FPS);

	void ResetElapsedTime();

private:

	void Update();
	void UpdateInputs();
	void UpdateSubsystems();




private:

	/*
		Tick manager. Collect information about elapsed ticks and FPS, etc...
	*/
	FTickTimer TickTimer;

	//.................Global performance blocks.....................//

	FPerformanceBlock GameBlockPerformance;
	FPerformanceBlock GraphicsEnginePerformance;

	//...............................................................//


	/*
		Current max FPS.
	*/
	uint16 FPSLock = 0;
	/*
		Minimal time for one frame in milliseconds.
	*/
	double TickTargetTime = -1;
	/*
		Objects facade for current tick.
	*/
	GCoreObjectsFacade* CurrentTickCoreObjectsFacade = nullptr;
};