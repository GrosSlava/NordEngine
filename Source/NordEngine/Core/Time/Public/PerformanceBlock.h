
#pragma once

#include "GenericPlatform.h"

#include <chrono>




/*
	Helper struct to manage code performance.
*/
struct ENGINE_API FPerformanceBlock
{
public:

	template<typename LAMBDA>
	void FORCEINLINE CacheCodePerformance(LAMBDA L)
	{
		WorkTime = TimePerformance<LAMBDA>(L); 
	}

	template<typename LAMBDA>
	static FORCEINLINE double TimePerformance(LAMBDA L)
	{
		const auto LCodeTimeStart = std::chrono::system_clock::now();

		L();

		const auto LCodeTimeEnd = std::chrono::system_clock::now();
		return std::chrono::duration<double, std::milli>(LCodeTimeEnd - LCodeTimeStart).count();
	}

public:

	/*
		@return last cached code work time in milliseconds. 
	*/
	FORCEINLINE double GetWorkTime() const noexcept { return WorkTime; }
	/*
		Clear cache.
	*/
	FORCEINLINE void Reset() { WorkTime = 0.0; }




private:

	/*
		Working time in milliseconds.
	*/
	double WorkTime = 0.0;
};

