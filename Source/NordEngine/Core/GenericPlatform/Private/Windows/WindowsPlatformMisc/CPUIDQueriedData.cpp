// Copyright Nord Engine. All Rights Reserved.
#include "GenericPlatform.h"
#if PLATFORM_WINDOWS

#include "Windows/WindowsPlatformMisc/CPUIDQueriedData.h"

#include "EngineMemory.h"

#include <intrin.h>
#include <excpt.h>





FCPUIDQueriedData FCPUIDQueriedData::CPUIDStaticCache;



bool FCPUIDQueriedData::CheckForCPUIDInstruction()
{
	__try
	{
		int Args[4];
		__cpuid(Args, 0);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
	return true;
}

void FCPUIDQueriedData::GetCPUVendor(ANSICHAR (&OutBuffer)[12 + 1])
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

	FMemory::MemCpy(OutBuffer, VendorResult.Buffer, sizeof(VendorResult.Buffer));
}

void FCPUIDQueriedData::GetCPUBrand(ANSICHAR (&OutBrandString)[0x40])
{
	// @see for more information http://msdn.microsoft.com/en-us/library/vstudio/hskdteyh(v=vs.100).aspx
	ANSICHAR BrandString[0x40] = {0};
	int32 CPUInfo[4] = {-1};
	const size_t CPUInfoSize = sizeof(CPUInfo);

	__cpuid(CPUInfo, 0x80000000);
	const uint32 MaxExtIDs = CPUInfo[0];

	if( MaxExtIDs >= 0x80000004 )
	{
		const uint32 FirstBrandString = 0x80000002;
		const uint32 NumBrandStrings = 3;
		for( uint32 Index = 0; Index < NumBrandStrings; Index++ )
		{
			__cpuid(CPUInfo, FirstBrandString + Index);
			FMemory::MemCpy(BrandString + CPUInfoSize * Index, CPUInfo, CPUInfoSize);
		}
	}

	FMemory::MemCpy(OutBrandString, BrandString, sizeof(BrandString));
}

void FCPUIDQueriedData::QueryCPUInfo(int Args[4])
{
	__cpuid(Args, 1);
}

int32 FCPUIDQueriedData::QueryCacheLineSize()
{
	int32 Result = 1;

	int Args[4];
	__cpuid(Args, 0x80000006);

	Result = Args[2] & 0xFF;

	return Result;
}

#endif // PLATFORM_WINDOWS