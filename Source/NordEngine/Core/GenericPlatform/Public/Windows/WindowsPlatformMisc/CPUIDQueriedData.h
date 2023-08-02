// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
#error PLATFORM_WINDOWS not defined!
#endif




/**
	Class that caches __cpuid queried data.
*/
class ENGINE_API FCPUIDQueriedData
{
public:

	FCPUIDQueriedData() : HasCPUIDInstruction(CheckForCPUIDInstruction()), Vendor()
	{
		if( HasCPUIDInstruction )
		{
			GetCPUVendor(Vendor);
			GetCPUBrand(Brand);
			int Info[4];
			QueryCPUInfo(Info);
			CPUInfo = Info[0];
			CPUInfo2 = Info[2];
			CacheLineSize = QueryCacheLineSize();
		}
	}


public:

	/**
		Checks if this CPU supports __cpuid instruction.
	 
		@return True if this CPU supports __cpuid instruction. False otherwise.
	*/
	static FORCEINLINE bool GetHasCPUIDInstruction() noexcept { return CPUIDStaticCache.HasCPUIDInstruction; }
	/**
		Gets pre-cached CPU vendor name.
	 
		@return CPU vendor name.
	*/
	static FORCEINLINE const ANSICHAR (&GetVendor() noexcept)[12 + 1] { return CPUIDStaticCache.Vendor; }
	/**
		Gets pre-cached CPU brand string.
	
		@return CPU brand string.
	*/
	static FORCEINLINE const ANSICHAR (&GetBrand() noexcept)[0x40] { return CPUIDStaticCache.Brand; }
	/**
		Gets __cpuid CPU info.
	
		@return CPU info unsigned int queried using __cpuid.
	*/
	static FORCEINLINE uint32 GetCPUInfo() noexcept { return CPUIDStaticCache.CPUInfo; }
	/**
		Gets __cpuid CPU info.
	
		@return CPU info unsigned int queried using __cpuid.
	*/
	static FORCEINLINE uint32 GetCPUInfo2() noexcept { return CPUIDStaticCache.CPUInfo2; }
	/**
		Gets cache line size.
	
		@return Cache line size.
	*/
	static FORCEINLINE int32 GetCacheLineSize() noexcept { return CPUIDStaticCache.CacheLineSize; }

private:

	/**
		Checks if __cpuid instruction is present on current machine.
	 
		@return True if this CPU supports __cpuid instruction. False otherwise.
	*/
	static bool CheckForCPUIDInstruction();

	/**
		Queries Vendor name using __cpuid instruction.
	 
		@return CPU vendor name.
	*/
	static void GetCPUVendor(ANSICHAR (&OutBuffer)[12 + 1]);
	/**
		Queries brand string using __cpuid instruction.
	 
		@return CPU brand string.
	*/
	static void GetCPUBrand(ANSICHAR (&OutBrandString)[0x40]);
	/**
		Queries CPU info using __cpuid instruction.
	 
		@return CPU info unsigned int queried using __cpuid.
	*/
	static void QueryCPUInfo(int Args[4]);
	/**
		Queries cache line size using __cpuid instruction.
	 
		@return Cache line size.
	*/
	static int32 QueryCacheLineSize();



private:

	/**
		Static field with pre-cached __cpuid data. 
	*/
	static FCPUIDQueriedData CPUIDStaticCache;

private:

	/** 
		If machine has CPUID instruction.
	*/
	bool HasCPUIDInstruction = false;
	/** 
		Vendor of the CPU. 
	*/
	ANSICHAR Vendor[12 + 1];
	/**
		CPU brand. 
	*/
	ANSICHAR Brand[0x40];
	/**
		CPU info from __cpuid.
	*/
	uint32 CPUInfo = 0;
	uint32 CPUInfo2 = 0;
	/**
		CPU cache line size.
	*/
	int32 CacheLineSize = 64;
};
