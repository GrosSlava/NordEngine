#pragma once

#if 0
#include "GenericPlatform.h"
#include "GenericPlatformMiscInfo.h"
#include <windows.h>

#include "FString.h"



/** Helper struct used to get the string version of the Windows version. */
struct FWindowsOSVersionHelper
{
	enum ErrorCodes
	{
		SUCCEEDED = 0,
		ERROR_UNKNOWNVERSION = 1,
		ERROR_GETPRODUCTINFO_FAILED = 2,
		ERROR_GETVERSIONEX_FAILED = 4,
		ERROR_GETWINDOWSGT62VERSIONS_FAILED = 8,
	};

	static int32 GetOSVersions(FString& OutOSVersion, FString& OutOSSubVersion);
};




struct FWindowsPlatformMisc
{

//Windows implementation of standart functional
public:

	static FString GetOSEnvironmentVariable(const TCHAR* VariableName);
	static void SetOSEnvironmentVariable(const TCHAR* VariableName, const TCHAR* Value);

	static bool SetStoredValue(const FString& InStoreId, const FString& InSectionName, const FString& InKeyName, const FString& InValue);
	static bool GetStoredValue(const FString& InStoreId, const FString& InSectionName, const FString& InKeyName, FString& OutValue);
	static bool DeleteStoredValue(const FString& InStoreId, const FString& InSectionName, const FString& InKeyName);
	static bool DeleteStoredSection(const FString& InStoreId, const FString& InSectionName);


	FORCEINLINE static void MemoryBarrier() { _mm_sfence(); }

	/**
	 * Has the OS execute a command and path pair (such as launch a browser)
	 *
	 * @param ComandType OS hint as to the type of command
	 * @param Command the command to execute
	 * @param CommandLine the commands to pass to the executable
	 *
	 * @return whether the command was successful or not
	 */
	static bool OsExecute(const TCHAR* CommandType, const TCHAR* Command, const TCHAR* CommandLine = NULL);





	static void SetUTF8Output();
	static void LocalPrint(const TCHAR* Message);
	static void RequestExit(bool Force);
	static void CreateGuid(struct FGuid& Result);	
	static uint64 GetFileVersion(const FString& FileName);

	static EAppReturnType MessageBoxExt(EAppMsgType MsgType, const TCHAR* Text, const TCHAR* Caption);




	/*
		if Error == 0 then gets last error
	*/
	static const TCHAR* GetSystemErrorMessage(TCHAR* OutBuffer, uint32 BufferCount, int32 Error);
	static uint32 GetLastError();
	static void SetLastError(uint32 ErrorCode);
	static void RaiseException(uint32 ExceptionCode);



	/**
		Returns whether the platform is running on battery power or not.
	*/
	static bool IsRunningOnBattery();

	static bool Is64bitOperatingSystem();
	static void GetOSVersions(FString& OutOSVersionLabel, FString& OutOSSubVersionLabel);
	static FString GetOSVersion();
	
	static FString GetDefaultLanguage();
	static FString GetDefaultLocale();


	

	static int32 NumberOfCores();
	static int32 NumberOfCoresIncludingHyperthreads();
	static int32 NumberOfWorkerThreadsToSpawn();


	/**
	 * Determines if the cpuid instruction is supported on this processor
	 *
	 * @return	Returns true if cpuid is supported
	 */
	static bool HasCPUIDInstruction();

	static FString GetCPUVendor();
	static FString GetCPUBrand();
	
	/**
	 * Uses cpuid instruction to get the vendor string
	 *
	 * @return	CPU info bitfield
	 *
	 *			Bits 0-3	Stepping ID
	 *			Bits 4-7	Model
	 *			Bits 8-11	Family
	 *			Bits 12-13	Processor type (Intel) / Reserved (AMD)
	 *			Bits 14-15	Reserved
	 *			Bits 16-19	Extended model
	 *			Bits 20-27	Extended family
	 *			Bits 28-31	Reserved
	 */
	static uint32 GetCPUInfo();

	/**
	 * Returns the size of the cache line in bytes.
	 *
	 * @return The cache line size.
	 */
	static int32 GetCacheLineSize();

	static FString GetPrimaryGPUBrand();
	static struct FGPUDriverInfo GetGPUDriverInfo(const FString& DeviceDescription);

	static bool GetDiskTotalAndFreeSpace(const FString& InPath, uint64& TotalNumberOfBytes, uint64& NumberOfFreeBytes);



	/**
	 * Gets Visual Studio common tools path.
	 *
	 * @param Version Version of VS to get (11 - 2012, 12 - 2013).
	 * @param OutData Output parameter with common tools path.
	 *
	 * @return Returns if succeeded.
	 */
	static bool GetVSComnTools(int32 Version, FString& OutData);

	/**
	 * Gets a globally unique ID the represents a particular operating system install.
	 */
	static FString GetOperatingSystemId();

	static EConvertibleLaptopMode GetConvertibleLaptopMode();





	/**
	* @return Windows path separator.
	*/
	static const TCHAR* GetDefaultPathSeparator();

	static FString GetPathToEngineExe();
	static bool IsValidAbsolutePathFormat(const FString& Path);


//Windows specific functional
public:

	/**
	 * Determines if we are running on the Windows version or newer
	 *
	 * See the 'Remarks' section of https://msdn.microsoft.com/en-us/library/windows/desktop/ms724833(v=vs.85).aspx
	 * for a list of MajorVersion/MinorVersion version combinations for Microsoft Windows.
	 *
	 * @return	Returns true if the current Windows version if equal or newer than MajorVersion
	 */
	static bool VerifyWindowsVersion(uint32 MajorVersion, uint32 MinorVersion, uint32 BuildNumber = 0);

	/**
	 * Provides a simpler interface for fetching and cleanup of registry value queries
	 *
	 * @param	InKey		The Key (folder) in the registry to search under
	 * @param	InSubKey	The Sub Key (folder) within the main Key to look for
	 * @param	InValueName	The Name of the Value (file) withing the Sub Key to look for
	 * @param	OutData		The Data entered into the Value
	 *
	 * @return	true, if it successfully found the Value
	 */
	static bool QueryRegKey(const HKEY InKey, const TCHAR* InSubKey, const TCHAR* InValueName, FString& OutData);

};


typedef FWindowsPlatformMisc FPlatformMisc;
#endif