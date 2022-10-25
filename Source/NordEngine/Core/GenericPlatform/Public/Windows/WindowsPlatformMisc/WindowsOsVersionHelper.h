
#pragma once



#if 0 //TODO
bool GetOSVersionHelper(TCHAR* OutString, int32 Length)
{
	// Get system info
	SYSTEM_INFO SystemInfo;
	const TCHAR* Architecture;
	if (FPlatformMisc::Is64bitOperatingSystem())
	{
		Architecture = TEXT("64bit");
		GetNativeSystemInfo(&SystemInfo);
	}
	else
	{
		Architecture = TEXT("32bit");
		GetSystemInfo(&SystemInfo);
	}

	OSVERSIONINFOEX OsVersionInfo = { 0 };
	OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);



#pragma warning(push)
#pragma warning(disable : 4996) // 'function' was declared deprecated

	CA_SUPPRESS(28159)
		if (GetVersionEx((LPOSVERSIONINFO)&OsVersionInfo))

#pragma warning(pop)

		{
			FString::Snprintf(OutString, Length, TEXT("%d.%d.%d.%d.%d.%s"), OsVersionInfo.dwMajorVersion, OsVersionInfo.dwMinorVersion, OsVersionInfo.dwBuildNumber, OsVersionInfo.wProductType, OsVersionInfo.wSuiteMask, Architecture);
			return true;
		}



	return false;
}








int32 GetOSVersionsHelper(TCHAR* OutOSVersionLabel, int32 OSVersionLabelLength, TCHAR* OutOSSubVersionLabel, int32 OSSubVersionLabelLength)
{

	int32 ErrorCode = (int32)FWindowsOSVersionHelper::SUCCEEDED;


	// Get system info
	SYSTEM_INFO SystemInfo;
	if (FPlatformMisc::Is64bitOperatingSystem())
	{
		GetNativeSystemInfo(&SystemInfo);
	}
	else
	{
		GetSystemInfo(&SystemInfo);
	}



	OSVERSIONINFOEX OsVersionInfo = { 0 };
	OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	FString OSVersionLabel = TEXT("Windows (unknown version)");
	FString OSSubVersionLabel = TEXT("");

#pragma warning(push)
#pragma warning(disable : 4996) // 'function' was declared deprecated

	CA_SUPPRESS(28159)
		if (GetVersionEx((LPOSVERSIONINFO)&OsVersionInfo))
#pragma warning(pop)

		{
			bool bIsInvalidVersion = false;

			switch (OsVersionInfo.dwMajorVersion)
			{
			case 5:
				switch (OsVersionInfo.dwMinorVersion)
				{
				case 0:
					OSVersionLabel = TEXT("Windows 2000");
					if (OsVersionInfo.wProductType == VER_NT_WORKSTATION)
					{
						OSSubVersionLabel = TEXT("Professional");
					}
					else
					{
						if (OsVersionInfo.wSuiteMask & VER_SUITE_DATACENTER)
						{
							OSSubVersionLabel = TEXT("Datacenter Server");
						}
						else if (OsVersionInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
						{
							OSSubVersionLabel = TEXT("Advanced Server");
						}
						else
						{
							OSSubVersionLabel = TEXT("Server");
						}
					}
					break;
				case 1:
					OSVersionLabel = TEXT("Windows XP");
					if (OsVersionInfo.wSuiteMask & VER_SUITE_PERSONAL)
					{
						OSSubVersionLabel = TEXT("Home Edition");
					}
					else
					{
						OSSubVersionLabel = TEXT("Professional");
					}
					break;
				case 2:
					if (GetSystemMetrics(SM_SERVERR2))
					{
						OSVersionLabel = TEXT("Windows Server 2003 R2");
					}
					else if (OsVersionInfo.wSuiteMask & VER_SUITE_STORAGE_SERVER)
					{
						OSVersionLabel = TEXT("Windows Storage Server 2003");
					}
					else if (OsVersionInfo.wSuiteMask & VER_SUITE_WH_SERVER)
					{
						OSVersionLabel = TEXT("Windows Home Server");
					}
					else if (OsVersionInfo.wProductType == VER_NT_WORKSTATION && SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					{
						OSVersionLabel = TEXT("Windows XP");
						OSSubVersionLabel = TEXT("Professional x64 Edition");
					}
					else
					{
						OSVersionLabel = TEXT("Windows Server 2003");
					}
					break;
				default:
					ErrorCode |= (int32)FWindowsOSVersionHelper::ERROR_UNKNOWNVERSION;
				}
				break;
			case 6:
				switch (OsVersionInfo.dwMinorVersion)
				{
				case 0:
					if (OsVersionInfo.wProductType == VER_NT_WORKSTATION)
					{
						OSVersionLabel = TEXT("Windows Vista");
					}
					else
					{
						OSVersionLabel = TEXT("Windows Server 2008");
					}
					break;
				case 1:
					if (OsVersionInfo.wProductType == VER_NT_WORKSTATION)
					{
						OSVersionLabel = TEXT("Windows 7");
					}
					else
					{
						OSVersionLabel = TEXT("Windows Server 2008 R2");
					}
					break;
				case 2:
					if (OsVersionInfo.wProductType == VER_NT_WORKSTATION)
					{
						OSVersionLabel = TEXT("Windows 8");
					}
					else
					{
						OSVersionLabel = TEXT("Windows Server 2012");
					}
					break;
				case 3:
					if (OsVersionInfo.wProductType == VER_NT_WORKSTATION)
					{
						OSVersionLabel = TEXT("Windows 8.1");
					}
					else
					{
						OSVersionLabel = TEXT("Windows Server 2012 R2");
					}
					break;
				default:
					ErrorCode |= (int32)FWindowsOSVersionHelper::ERROR_UNKNOWNVERSION;
					break;
				}
				break;
			case 10:
				switch (OsVersionInfo.dwMinorVersion)
				{
				case 0:
					if (OsVersionInfo.wProductType == VER_NT_WORKSTATION)
					{
						OSVersionLabel = TEXT("Windows 10");
					}
					else
					{
						OSVersionLabel = TEXT("Windows Server 2019");
					}

					// For Windows 10, get the release number and append that to the string too (eg. 1709 = Fall Creators Update). There doesn't seem to be any good way to get
					// this other than grabbing an entry from the registry.
					{
						FString ReleaseId;
						if (FWindowsPlatformMisc::QueryRegKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), TEXT("ReleaseId"), ReleaseId))
						{
							OSVersionLabel += FString::Printf(TEXT(" (Release %s)"), ReleaseId.GetStr());
						}
					}

					break;
				default:
					ErrorCode |= (int32)FWindowsOSVersionHelper::ERROR_UNKNOWNVERSION;
					break;
				}
				break;
			default:
				ErrorCode |= FWindowsOSVersionHelper::ERROR_UNKNOWNVERSION;
				break;
			}

			if (OsVersionInfo.dwMajorVersion >= 6)
			{
#pragma warning( push )
#pragma warning( disable: 4191 )	// unsafe conversion from 'type of expression' to 'type required'
				typedef BOOL(WINAPI* LPFN_GETPRODUCTINFO)(DWORD, DWORD, DWORD, DWORD, PDWORD);
				LPFN_GETPRODUCTINFO fnGetProductInfo = (LPFN_GETPRODUCTINFO)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
#pragma warning( pop )
				if (fnGetProductInfo != NULL)
				{
					DWORD Type;
					fnGetProductInfo(OsVersionInfo.dwMajorVersion, OsVersionInfo.dwMinorVersion, 0, 0, &Type);

					switch (Type)
					{
					case PRODUCT_ULTIMATE:
						OSSubVersionLabel = TEXT("Ultimate Edition");
						break;
					case PRODUCT_PROFESSIONAL:
						OSSubVersionLabel = TEXT("Professional");
						break;
					case PRODUCT_HOME_PREMIUM:
						OSSubVersionLabel = TEXT("Home Premium Edition");
						break;
					case PRODUCT_HOME_BASIC:
						OSSubVersionLabel = TEXT("Home Basic Edition");
						break;
					case PRODUCT_ENTERPRISE:
						OSSubVersionLabel = TEXT("Enterprise Edition");
						break;
					case PRODUCT_BUSINESS:
						OSSubVersionLabel = TEXT("Business Edition");
						break;
					case PRODUCT_STARTER:
						OSSubVersionLabel = TEXT("Starter Edition");
						break;
					case PRODUCT_CLUSTER_SERVER:
						OSSubVersionLabel = TEXT("Cluster Server Edition");
						break;
					case PRODUCT_DATACENTER_SERVER:
						OSSubVersionLabel = TEXT("Datacenter Edition");
						break;
					case PRODUCT_DATACENTER_SERVER_CORE:
						OSSubVersionLabel = TEXT("Datacenter Edition (core installation)");
						break;
					case PRODUCT_ENTERPRISE_SERVER:
						OSSubVersionLabel = TEXT("Enterprise Edition");
						break;
					case PRODUCT_ENTERPRISE_SERVER_CORE:
						OSSubVersionLabel = TEXT("Enterprise Edition (core installation)");
						break;
					case PRODUCT_ENTERPRISE_SERVER_IA64:
						OSSubVersionLabel = TEXT("Enterprise Edition for Itanium-based Systems");
						break;
					case PRODUCT_SMALLBUSINESS_SERVER:
						OSSubVersionLabel = TEXT("Small Business Server");
						break;
					case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
						OSSubVersionLabel = TEXT("Small Business Server Premium Edition");
						break;
					case PRODUCT_STANDARD_SERVER:
						OSSubVersionLabel = TEXT("Standard Edition");
						break;
					case PRODUCT_STANDARD_SERVER_CORE:
						OSSubVersionLabel = TEXT("Standard Edition (core installation)");
						break;
					case PRODUCT_WEB_SERVER:
						OSSubVersionLabel = TEXT("Web Server Edition");
						break;
					}
				}
				else
				{
					OSSubVersionLabel = TEXT("(type unknown)");
					ErrorCode |= (int32)FWindowsOSVersionHelper::ERROR_GETPRODUCTINFO_FAILED;
				}
			}

#if 0
			// THIS BIT ADDS THE SERVICE PACK INFO TO THE EDITION STRING
			// Append service pack info
			if (OsVersionInfo.szCSDVersion[0] != 0)
			{
				OSSubVersionLabel += FString::Printf(TEXT(" (%s)"), OsVersionInfo.szCSDVersion);
			}
#else
			// THIS BIT USES SERVICE PACK INFO ONLY
			OSSubVersionLabel = OsVersionInfo.szCSDVersion;
#endif
		}
		else
		{
			ErrorCode |= FWindowsOSVersionHelper::ERROR_GETVERSIONEX_FAILED;
		}

	FMicrosoftPlatformString::Strcpy(OutOSVersionLabel, OSVersionLabel.GetStr());
	FMicrosoftPlatformString::Strcpy(OutOSSubVersionLabel, OSSubVersionLabel.GetStr());

	return ErrorCode;
}

#endif