
#pragma once

#include "GenericPlatform.h"
#include "EngineMemory.h"

#include <intrin.h>
#include <excpt.h>




/*
	Class that caches __cpuid queried data.
*/
class FCPUIDQueriedData
{
public:

	FCPUIDQueriedData() : 
		bHasCPUIDInstruction(CheckForCPUIDInstruction()), Vendor()
	{
		if (bHasCPUIDInstruction)
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

	/*
		Checks if this CPU supports __cpuid instruction.
	 
		@returns True if this CPU supports __cpuid instruction. False otherwise.
	*/
	static bool HasCPUIDInstruction()
	{
		return CPUIDStaticCache.bHasCPUIDInstruction;
	}

	/*
		Gets pre-cached CPU vendor name.
	 
		@returns CPU vendor name.
	*/
	static const ANSICHAR(&GetVendor())[12 + 1]
	{
		return CPUIDStaticCache.Vendor;
	}

	/*
		Gets pre-cached CPU brand string.
	
		@returns CPU brand string.
	*/
	static const ANSICHAR(&GetBrand())[0x40]
	{
		return CPUIDStaticCache.Brand;
	}

	/*
		Gets __cpuid CPU info.
	
		@returns CPU info unsigned int queried using __cpuid.
	*/
	static uint32 GetCPUInfo()
	{
		return CPUIDStaticCache.CPUInfo;
	}

	/*
		Gets __cpuid CPU info.
	
		@returns CPU info unsigned int queried using __cpuid.
	*/
	static uint32 GetCPUInfo2()
	{
		return CPUIDStaticCache.CPUInfo2;
	}

	/*
		Gets cache line size.
	
		@returns Cache line size.
	*/
	static int32 GetCacheLineSize()
	{
		return CPUIDStaticCache.CacheLineSize;
	}


private:

	/*
		Checks if __cpuid instruction is present on current machine.
	 
		@returns True if this CPU supports __cpuid instruction. False otherwise.
	*/
	static bool CheckForCPUIDInstruction()
	{
		__try
		{
			int Args[4];
			__cpuid(Args, 0);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
		return true;
	}

	/*
		Queries Vendor name using __cpuid instruction.
	 
		@returns CPU vendor name.
	*/
	static void GetCPUVendor(ANSICHAR(&OutBuffer)[12 + 1])
	{
		union
		{
			char Buffer[12 + 1];
			struct
			{
				int dw0;
				int dw1;
				int dw2;
			} Dw;
		} VendorResult;

		int Args[4];
		__cpuid(Args, 0);

		VendorResult.Dw.dw0 = Args[1];
		VendorResult.Dw.dw1 = Args[3];
		VendorResult.Dw.dw2 = Args[2];
		VendorResult.Buffer[12] = 0;

		FMemory::Memcpy(OutBuffer, VendorResult.Buffer);
	}

	/*
		Queries brand string using __cpuid instruction.
	 
		@returns CPU brand string.
	*/
	static void GetCPUBrand(ANSICHAR(&OutBrandString)[0x40])
	{
		// @see for more information http://msdn.microsoft.com/en-us/library/vstudio/hskdteyh(v=vs.100).aspx
		ANSICHAR BrandString[0x40] = { 0 };
		int32 CPUInfo[4] = { -1 };
		const size_t CPUInfoSize = sizeof(CPUInfo);

		__cpuid(CPUInfo, 0x80000000);
		const uint32 MaxExtIDs = CPUInfo[0];

		if (MaxExtIDs >= 0x80000004)
		{
			const uint32 FirstBrandString = 0x80000002;
			const uint32 NumBrandStrings = 3;
			for (uint32 Index = 0; Index < NumBrandStrings; Index++)
			{
				__cpuid(CPUInfo, FirstBrandString + Index);
				FMemory::Memcpy(BrandString + CPUInfoSize * Index, CPUInfo, CPUInfoSize);
			}
		}

		FMemory::Memcpy(OutBrandString, BrandString);
	}

	/*
		Queries CPU info using __cpuid instruction.
	 
		@returns CPU info unsigned int queried using __cpuid.
	*/
	static void QueryCPUInfo(int Args[4])
	{
		__cpuid(Args, 1);
	}

	/*
		Queries cache line size using __cpuid instruction.
	 
		@returns Cache line size.
	*/
	static int32 QueryCacheLineSize()
	{
		int32 Result = 1;

		int Args[4];
		__cpuid(Args, 0x80000006);

		Result = Args[2] & 0xFF;

		return Result;
	}


private:

	/* Static field with pre-cached __cpuid data. */
	static FCPUIDQueriedData CPUIDStaticCache;

	/* If machine has CPUID instruction. */
	bool bHasCPUIDInstruction;

	/* Vendor of the CPU. */
	ANSICHAR Vendor[12 + 1];

	/* CPU brand. */
	ANSICHAR Brand[0x40];

	/* CPU info from __cpuid. */
	uint32 CPUInfo = 0;
	uint32 CPUInfo2 = 0;

	/* CPU cache line size. */
	int32 CacheLineSize = 64;
};


/* Static initialization of data to pre-cache __cpuid queries. */
FCPUIDQueriedData FCPUIDQueriedData::CPUIDStaticCache;

