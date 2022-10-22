
#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
	#error PLATFORM_WINDOWS not defined!
#endif

#include "Windows/WindowsHWrapper.h"






struct ENGINE_API FWindowsPlatformTime
{
public:

	static FORCEINLINE double Seconds()
	{
		LARGE_INTEGER Cycles;
		QueryPerformanceCounter(&Cycles);

		return Cycles.QuadPart * GetSecondsPerCycle();
	}

	/* 
		@return QueryPerformanceCounter (QPC).
	*/
	static FORCEINLINE uint32 Cycles()
	{
		LARGE_INTEGER Cycles;
		QueryPerformanceCounter(&Cycles);
		return (uint32)Cycles.QuadPart;
	}

	/* 
		@return QueryPerformanceCounter (QPC).
	*/
	static FORCEINLINE uint64 Cycles64()
	{
		LARGE_INTEGER Cycles;
		QueryPerformanceCounter(&Cycles);
		return Cycles.QuadPart;
	}

	static double GetSecondsPerCycle();

	static uint64 GetQPCFrequency();




	/* Converts cycles to milliseconds. */
	static FORCEINLINE float ToMilliseconds(const uint32 Cycles) { return GetSecondsPerCycle() * 1000.0 * Cycles; }

	/* Converts cycles to seconds. */
	static FORCEINLINE float ToSeconds(const uint32 Cycles) { return GetSecondsPerCycle() * Cycles; }

	static void Sleep(uint32 Milliseconds); 
	


	static void SystemTime(int32& Year, int32& Month, int32& DayOfWeek, int32& Day, int32& Hour, int32& Min, int32& Sec, int32& MSec);
};



typedef FWindowsPlatformTime FPlatformTime;