// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#if !PLATFORM_WINDOWS
#error PLATFORM_WINDOWS not defined!
#endif

#include "GenericPlatformMiscInfo.h"
#include "FString.h"
#include "Array.h"




struct ENGINE_API FWindowsPlatformMisc
{
	//............................................EnvironmentVariables........................................//

	static FString GetOSEnvironmentVariable(const TCHAR* VariableName);
	static void SetOSEnvironmentVariable(const TCHAR* VariableName, const TCHAR* Value);

	static bool SetStoredValue(const FString& InStoreId, const FString& InSectionName, const FString& InKeyName, const FString& InValue);
	static bool GetStoredValue(const FString& InStoreId, const FString& InSectionName, const FString& InKeyName, FString& OutValue);
	static bool DeleteStoredValue(const FString& InStoreId, const FString& InSectionName, const FString& InKeyName);
	static bool DeleteStoredSection(const FString& InStoreId, const FString& InSectionName);

	//........................................................................................................//

	//............................................. Misc routines.............................................//

	/**
		Has the OS execute a command and path pair (such as launch a browser).
	 
		@param ComandType - OS hint as to the type of command.
		@param Command - The command to execute.
		@param CommandLine - The commands to pass to the executable.
		@return whether the command was successful or not.
	*/
	static bool OsExecute(const TCHAR* CommandType, const TCHAR* Command, const TCHAR* CommandLine = nullptr);
	static void RequestExit(bool Force);

	static void MemoryBarrier();

	static void SetUTF8Output();
	static void LocalPrint(const TCHAR* Message);
	static EAppReturnType MessageBoxExt(EAppMsgType MsgType, const TCHAR* Text, const TCHAR* Caption);

	static void StackTrace(TArray<FStackFrame>& OutStackFrames);
	static void DefaultShowStackTrace(const FString& Caption, uint32 SkipTopFrames = 0);

	//........................................................................................................//

	//.............................................Errors.....................................................//

	/**
		if Error == 0 then gets last error.
	*/
	static const TCHAR* GetSystemErrorMessage(TCHAR* OutBuffer, uint32 BufferCount, int32 Error);
	static uint32 GetLastError();
	static void SetLastError(uint32 ErrorCode);
	static void RaiseException(uint32 ExceptionCode);

	//........................................................................................................//

	//...........................................System info..................................................//

	static bool IsRunningOnBattery();
	static bool Is64bitOperatingSystem();

	static void GetOSVersions(FString& OutOSVersionLabel, FString& OutOSSubVersionLabel);
	static FString GetOSVersion();
	static FString GetOperatingSystemId();

	static FString GetDefaultLanguage();
	static FString GetDefaultLocale();

	static EConvertibleLaptopMode GetConvertibleLaptopMode();

	static int32 NumberOfCores();
	static int32 NumberOfCoresIncludingHyperthreads();
	static int32 NumberOfWorkerThreadsToSpawn();

	static bool HasCPUIDInstruction();
	static FString GetCPUVendor();
	static FString GetCPUBrand();
	/**
		Uses cpuid instruction to get the vendor string
	 
		@return	CPU info bitfield
	 			Bits 0-3	Stepping ID
	 			Bits 4-7	Model
	 			Bits 8-11	Family
	 			Bits 12-13	Processor type (Intel) / Reserved (AMD)
	 			Bits 14-15	Reserved
	 			Bits 16-19	Extended model
	 			Bits 20-27	Extended family
	 			Bits 28-31	Reserved
	*/
	static uint32 GetCPUInfo();
	static int32 GetCacheLineSize();

	static FString GetPrimaryGPUBrand();

	static bool GetDiskTotalAndFreeSpace(const FString& InPath, uint64& TotalNumberOfBytes, uint64& NumberOfFreeBytes);

	//........................................................................................................//

	//...........................................File system..................................................//

	static const TCHAR* GetDefaultPathSeparator();
	static uint64 GetFileVersion(const FString& FileName);
	static FString GetPathToEngineExe();
	static bool IsValidAbsolutePathFormat(const FString& Path);

	//........................................................................................................//
};

using FPlatformMisc = FWindowsPlatformMisc;
