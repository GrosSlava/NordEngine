
#pragma once

#include "GenericPlatform.h"
#include "GenericPlatformTime.h"
#include "SpecificationMacros.h"

#include "EngineMath.h"




class ENGINE_API FTickTimer
{
	GENERATED_BODY(FTickTimer)

public:

	FORCEINLINE FTickTimer() noexcept
	{
		TargetElapsedTicks = TicksPerSecond / 60;

		// Initialize max delta to 1/10 of a second.
		MaxDelta = FPlatformTime::GetQPCFrequency() / 10;
	}



public:

	/*
		Get elapsed ticks since the previous Update call.
	*/
	FORCEINLINE uint64 GetElapsedTicks() const noexcept { return ElapsedTicks; }
	/*
		Get elapsed time since the previous Update call.
	*/
	FORCEINLINE double GetElapsedSeconds() const noexcept { return TicksToSeconds(ElapsedTicks); }

	/*
		Get total ticks since the start of the program.
	*/
	FORCEINLINE uint64 GetTotalTicks() const noexcept { return TotalTicks; }
	/*
		Get total time since the start of the program.
	*/
	FORCEINLINE double GetTotalSeconds() const noexcept { return TicksToSeconds(TotalTicks); }


	/*
		Get total number of updates since start of the program.
	*/
	FORCEINLINE uint32 GetFrameCount() const noexcept { return FrameCount; }
	/*
		Get the current framerate.
	*/
	FORCEINLINE uint32 GetFramesPerSecond() const noexcept { return FramesPerSecond; }


	/*
		Set whether to use fixed or variable timestep mode.
	*/
	FORCEINLINE void SetFixedTimeStep(bool NewIsFixedTimestep) noexcept { IsFixedTimeStep = NewIsFixedTimestep; }

	/*
		Set how often to call Update when in fixed timestep mode.
	*/
	FORCEINLINE void SetTargetElapsedTicks(uint64 TargetElapsed) noexcept { TargetElapsedTicks = TargetElapsed; }
	FORCEINLINE void SetTargetElapsedSeconds(double TargetElapsed) noexcept { TargetElapsedTicks = SecondsToTicks(TargetElapsed); }



	static FORCEINLINE constexpr double TicksToSeconds(uint64 Ticks) noexcept { return static_cast<double>(Ticks) / TicksPerSecond; }
	static FORCEINLINE constexpr uint64 SecondsToTicks(double Seconds) noexcept { return static_cast<uint64>(Seconds * TicksPerSecond); }

public:

	/* 
        After an intentional timing discontinuity (for instance a blocking IO operation)
        call this to avoid having the fixed timestep logic attempt a set of catch-up Update calls.
    */
	FORCEINLINE void ResetElapsedTime() noexcept
	{
		LeftOverTicks = 0;
		FramesPerSecond = 0;
		FramesThisSecond = 0;
		SecondCounter = 0;
	}


	// Update timer state, calling the specified Update function the appropriate number of times.
	template<class C>
	void Tick(C* Executor, void (C::*Update)())
	{
		const uint64 CurrentTime = FPlatformTime::Cycles64();
		const uint64 CurrentPQCFrequency = FPlatformTime::GetQPCFrequency();


		uint64 TimeDelta = CurrentTime - LastTime;

		LastTime = CurrentTime;
		SecondCounter += TimeDelta;

		// Clamp excessively large time deltas (e.g. after paused in the debugger).
		if( TimeDelta > MaxDelta )
		{
			TimeDelta = MaxDelta;
		}

		// Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp.
		TimeDelta *= TicksPerSecond;
		TimeDelta /= CurrentPQCFrequency;



		const uint32 LastFrameCount = FrameCount;

		if( IsFixedTimeStep )
		{
			// Fixed timestep update logic

			// If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
			// the clock to exactly match the target value. This prevents tiny and irrelevant errors
			// from accumulating over time. Without this clamping, a game that requested a 60 fps
			// fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
			// accumulate enough tiny errors that it would drop a frame. It is better to just round
			// small deviations down to zero to leave things running smoothly.

			if( FMath::Abs<int64>(static_cast<int64>(TimeDelta - TargetElapsedTicks)) < TicksPerSecond / 4000 )
			{
				TimeDelta = TargetElapsedTicks;
			}

			LeftOverTicks += TimeDelta;

			while( LeftOverTicks >= TargetElapsedTicks )
			{
				ElapsedTicks = TargetElapsedTicks;
				TotalTicks += TargetElapsedTicks;
				LeftOverTicks -= TargetElapsedTicks;
				++FrameCount;

				(Executor->*Update)();
			}
		}
		else
		{
			// Variable timestep update logic.
			ElapsedTicks = TimeDelta;
			TotalTicks += TimeDelta;
			LeftOverTicks = 0;
			++FrameCount;

			(Executor->*Update)();
		}


		// Track the current framerate.
		if( FrameCount != LastFrameCount )
		{
			++FramesThisSecond;
		}

		if( SecondCounter >= CurrentPQCFrequency )
		{
			FramesPerSecond = FramesThisSecond;
			FramesThisSecond = 0;
			SecondCounter %= CurrentPQCFrequency;
		}
	}




private:

	// Members for tracking the framerate.
	uint32 FrameCount = 0;
	uint32 FramesPerSecond = 0;
	uint32 FramesThisSecond = 0;
	uint64 SecondCounter = 0;

	uint64 LastTime = 0;
	uint64 MaxDelta = 0;


	// Derived timing data uses a canonical tick format.
	uint64 ElapsedTicks = 0;
	uint64 TotalTicks = 0;
	uint64 LeftOverTicks = 0;


	// Members for configuring fixed timestep mode.
	bool IsFixedTimeStep = false;
	uint64 TargetElapsedTicks = 0;

public:

	// Integer format represents time using 10,000,000 ticks per second.
	static const uint64 TicksPerSecond = 10000000;
};
