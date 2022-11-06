#if 0
#include "Windows/WindowsPlatformMisc/WindowsPlatformMisc.h"
#include "GenericPlatformDriver.h"
#include "Windows/WindowsPlatformMisc/CPUIDQueriedData.h"
#include "Windows/WindowsResources.h"

#include "CodeAnalysis.h"


#include <io.h>
#include <fcntl.h>

#pragma comment(lib, "Version.lib")






#include "Windows/WindowsPlatformMisc/WindowsOsVersionHelper.h"

int32 FWindowsOSVersionHelper::GetOSVersions(FString& OutOSVersionLabel, FString& OutOSSubVersionLabel)
{
	TCHAR OSVersionLabel[128];
	TCHAR OSSubVersionLabel[128];

	OSVersionLabel[0] = 0;
	OSSubVersionLabel[0] = 0;

	int32 Result = GetOSVersionsHelper(OSVersionLabel, _countof(OSVersionLabel), OSSubVersionLabel, _countof(OSSubVersionLabel));

	OutOSVersionLabel = OSVersionLabel;
	OutOSSubVersionLabel = OSSubVersionLabel;

	return Result;
}







FString FWindowsPlatformMisc::GetOSEnvironmentVariable(const TCHAR* VariableName)
{
	// Allocate the data for the string. Loop in case the variable happens to change while running, or the buffer isn't large enough.
	FString Buffer;
	for (uint32 Length = 128;;)
	{
		Buffer.SetNumUninitialized(Length);

		Length = GetEnvironmentVariableW(VariableName, Buffer.GetBuffer(), Buffer.Length());
		if (Length == 0)
		{
			Buffer.Clear();
			break;
		}
		else if (Length < Buffer.Length())
		{
			Buffer.SetLength(Length);
			break;
		}
	}

	return Buffer;
}

void FWindowsPlatformMisc::SetOSEnvironmentVariable(const TCHAR* VariableName, const TCHAR* Value)
{
	if (!SetEnvironmentVariable(VariableName, Value))
	{
		TCHAR ErrorMessage[512];
		GetSystemErrorMessage(ErrorMessage, 512, 0);
		MessageBoxExt(EAppMsgType::Ok, ErrorMessage, TEXT("Error"));
	}
}



bool FWindowsPlatformMisc::SetStoredValue(const FString& InStoreId, const FString& InSectionName, const FString& InKeyName, const FString& InValue)
{
	if (InStoreId.IsEmpty() || InSectionName.IsEmpty() || InKeyName.IsEmpty()) return false;


	FString FullRegistryKey = FString(TEXT("Software")) + TEXT("/") + InStoreId + TEXT("/") + InSectionName;
	FullRegistryKey = FullRegistryKey.Replace(TEXT("/"), TEXT("\\")); // we use forward slashes, but the registry needs back slashes

	HKEY hKey;
	HRESULT Result = RegCreateKeyEx(HKEY_CURRENT_USER, FullRegistryKey.GetStr(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr);
	if (Result == ERROR_SUCCESS)
	{
		Result = RegSetValueEx(hKey, InKeyName.GetStr(), 0, REG_SZ, (const BYTE*)InValue.GetStr(), (InValue.Length() + 1) * sizeof(TCHAR));
		RegCloseKey(hKey);
	}

	if (Result != ERROR_SUCCESS)
	{
		TCHAR ErrorBuffer[1024];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, Result, 0, ErrorBuffer, 1024, nullptr);
		return false;
	}

	return true;
}

bool FWindowsPlatformMisc::GetStoredValue(const FString& InStoreId, const FString& InSectionName, const FString& InKeyName, FString& OutValue)
{
	if (InStoreId.IsEmpty() || InSectionName.IsEmpty() || InKeyName.IsEmpty()) return false;


	FString FullRegistryKey = FString(TEXT("Software")) + TEXT("/") + InStoreId + TEXT("/") + InSectionName;
	FullRegistryKey = FullRegistryKey.Replace(TEXT("/"), TEXT("\\")); // we use forward slashes, but the registry needs back slashes

	return QueryRegKey(HKEY_CURRENT_USER, FullRegistryKey.GetStr(), InKeyName.GetStr(), OutValue);
}

bool FWindowsPlatformMisc::DeleteStoredValue(const FString& InStoreId, const FString& InSectionName, const FString& InKeyName)
{
	// Deletes values in reg keys and also deletes the owning key if it becomes empty
	if (InStoreId.IsEmpty() || InSectionName.IsEmpty() || InKeyName.IsEmpty()) return false;
	

	FString FullRegistryKey = FString(TEXT("Software")) + TEXT("/") + InStoreId + TEXT("/") + InSectionName;
	FullRegistryKey = FullRegistryKey.Replace(TEXT("/"), TEXT("\\")); // we use forward slashes, but the registry needs back slashes

	HKEY hKey;
	HRESULT Result = RegOpenKeyEx(HKEY_CURRENT_USER, FullRegistryKey.GetStr(), 0, KEY_WRITE | KEY_READ, &hKey);
	if (Result == ERROR_SUCCESS)
	{
		Result = RegDeleteValue(hKey, InKeyName.GetStr());

		// Query for sub-keys in the open key
		TCHAR CheckKeyName[256];
		DWORD CheckKeyNameLength = sizeof(CheckKeyName) / sizeof(CheckKeyName[0]);
		HRESULT EnumResult = RegEnumKeyEx(hKey, 0, CheckKeyName, &CheckKeyNameLength, NULL, NULL, NULL, NULL);
		bool bZeroSubKeys = EnumResult != ERROR_SUCCESS;

		// Query for a remaining value in the open key
		wchar_t CheckValueName[256];
		DWORD CheckValueNameLength = sizeof(CheckValueName) / sizeof(CheckValueName[0]);
		EnumResult = RegEnumValue(hKey, 0, CheckValueName, &CheckValueNameLength, NULL, NULL, NULL, NULL);
		bool bZeroValues = EnumResult != ERROR_SUCCESS;

		RegCloseKey(hKey);

		if (bZeroSubKeys && bZeroValues)
		{
			// No more values - delete the section
			RegDeleteKey(HKEY_CURRENT_USER, FullRegistryKey.GetStr());
		}
	}

	return Result == ERROR_SUCCESS;
}

bool FWindowsPlatformMisc::DeleteStoredSection(const FString& InStoreId, const FString& InSectionName)
{
	if (InStoreId.IsEmpty() || InSectionName.IsEmpty()) return false;


	FString FullRegistryKey = FString(TEXT("Software")) + TEXT("/") + InStoreId + TEXT("/") + InSectionName;
	FullRegistryKey = FullRegistryKey.Replace(TEXT("/"), TEXT("\\")); // we use forward slashes, but the registry needs back slashes

	return RegDeleteTree(HKEY_CURRENT_USER, FullRegistryKey.GetStr()) == ERROR_SUCCESS;
}



bool FWindowsPlatformMisc::OsExecute(const TCHAR* CommandType, const TCHAR* Command, const TCHAR* CommandLine)
{
	HINSTANCE hApp = ShellExecute(
									NULL,
									CommandType,
									Command,
									CommandLine,
									NULL,
									SW_SHOWNORMAL
								);


	return hApp > (HINSTANCE)32;
}






void FWindowsPlatformMisc::SetUTF8Output()
{
	CA_SUPPRESS(6031)
	_setmode(_fileno(stdout), _O_U8TEXT);
}

void FWindowsPlatformMisc::LocalPrint(const TCHAR* Message)
{
	OutputDebugString(Message);
}

void FWindowsPlatformMisc::RequestExit(bool Force)
{
	if (Force)
	{
		TerminateProcess(GetCurrentProcess(), 0);
	}
	else
	{
		// Tell the platform specific code we want to exit cleanly from the main loop.
		PostQuitMessage(0);
	}
}

void FWindowsPlatformMisc::CreateGuid(FGuid& Result)
{
	CoCreateGuid((GUID*)&Result);
}

uint64 FWindowsPlatformMisc::GetFileVersion(const FString& FileName)
{
	DWORD VersionInfoSize = GetFileVersionInfoSize(FileName.GetStr(), NULL);
	if (VersionInfoSize == 0) return 0;
	

	uint8* VersionInfo = new uint8[VersionInfoSize];
	if (GetFileVersionInfo(FileName.GetStr(), NULL, VersionInfoSize, VersionInfo))
	{
		VS_FIXEDFILEINFO* FileInfo;
		UINT FileInfoLen;
		if (VerQueryValue(VersionInfo, TEXT("\\"), (LPVOID*)&FileInfo, &FileInfoLen))
		{
			delete[] VersionInfo;
			return (uint64(FileInfo->dwFileVersionMS) << 32) | uint64(FileInfo->dwFileVersionLS);
		}
	}

	delete[] VersionInfo;
	return 0;
}




#define HOTKEY_YES			100
#define HOTKEY_NO			101
#define HOTKEY_CANCEL		102

/**
 * Helper global variables, used in MessageBoxDlgProc for set message text.
 */
static TCHAR* GMessageBoxText = NULL;
static TCHAR* GMessageBoxCaption = NULL;
/**
 * Used by MessageBoxDlgProc to indicate whether a 'Cancel' button is present and
 * thus 'Esc should be accepted as a hotkey.
 */
static bool GCancelButtonEnabled = false;



/**
 * Calculates button position and size, localize button text.
 * @param HandleWnd handle to dialog window
 * @param Text button text to localize
 * @param DlgItemId dialog item id
 * @param PositionX current button position (x coord)
 * @param PositionY current button position (y coord)
 * @return true if succeeded
 */
static bool SetDlgItem(HWND HandleWnd, const TCHAR* Text, int32 DlgItemId, int32* PositionX, int32* PositionY)
{
	SIZE SizeButton;

	HDC DC = CreateCompatibleDC(NULL);
	GetTextExtentPoint32(DC, Text, wcslen(Text), &SizeButton);
	DeleteDC(DC);
	DC = NULL;

	SizeButton.cx += 14;
	SizeButton.cy += 8;

	HWND Handle = GetDlgItem(HandleWnd, DlgItemId);
	if (Handle)
	{
		*PositionX -= (SizeButton.cx + 5);
		SetWindowPos(Handle, HWND_TOP, *PositionX, *PositionY - SizeButton.cy, SizeButton.cx, SizeButton.cy, 0);
		SetDlgItemText(HandleWnd, DlgItemId, Text);

		return true;
	}

	return false;
}

/**
 * Callback for MessageBoxExt dialog (allowing for Yes to all / No to all )
 * @return		One of EAppReturnType::Yes, EAppReturnType::YesAll, EAppReturnType::No, EAppReturnType::NoAll, EAppReturnType::Cancel.
 */
PTRINT CALLBACK MessageBoxDlgProc(HWND HandleWnd, uint32 Message, WPARAM WParam, LPARAM LParam)
{
	switch (Message)
	{

	case WM_INITDIALOG:
	{
		// Sets most bottom and most right position to begin button placement
		RECT Rect;
		POINT Point;

		GetWindowRect(HandleWnd, &Rect);
		Point.x = Rect.right;
		Point.y = Rect.bottom;
		ScreenToClient(HandleWnd, &Point);

		int32 PositionX = Point.x - 8;
		int32 PositionY = Point.y - 10;

		SetDlgItem(HandleWnd, TEXT("Cancel"), IDC_CANCEL, &PositionX, &PositionY);
		SetDlgItem(HandleWnd, TEXT("No to All"), IDC_NOTOALL, &PositionX, &PositionY);
		SetDlgItem(HandleWnd, TEXT("No"), IDC_NO_B, &PositionX, &PositionY);
		SetDlgItem(HandleWnd, TEXT("Yes to All"), IDC_YESTOALL, &PositionX, &PositionY);
		SetDlgItem(HandleWnd, TEXT("Yes"), IDC_YES, &PositionX, &PositionY);

		SetDlgItemText(HandleWnd, IDC_MESSAGE, GMessageBoxText);
		SetWindowText(HandleWnd, GMessageBoxCaption);

		// If parent window exist, get it handle and make it foreground.
		HWND ParentWindow = GetTopWindow(HandleWnd);
		if (ParentWindow)
		{
			SetWindowPos(ParentWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		SetForegroundWindow(HandleWnd);
		SetWindowPos(HandleWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		RegisterHotKey(HandleWnd, HOTKEY_YES, 0, 'Y');
		RegisterHotKey(HandleWnd, HOTKEY_NO, 0, 'N');
		if (GCancelButtonEnabled)
		{
			RegisterHotKey(HandleWnd, HOTKEY_CANCEL, 0, VK_ESCAPE);
		}

		// Windows are foreground, make them not top most.
		SetWindowPos(HandleWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		if (ParentWindow)
		{
			SetWindowPos(ParentWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		return true;
	}

	case WM_DESTROY:
	{
		UnregisterHotKey(HandleWnd, HOTKEY_YES);
		UnregisterHotKey(HandleWnd, HOTKEY_NO);
		if (GCancelButtonEnabled)
		{
			UnregisterHotKey(HandleWnd, HOTKEY_CANCEL);
		}

		return true;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(WParam))
		{

		case IDC_YES:
		{
			EndDialog(HandleWnd, (INT_PTR)EAppReturnType::Yes);
			break;
		}

		case IDC_YESTOALL:
		{
			EndDialog(HandleWnd, (INT_PTR)EAppReturnType::YesAll);
			break;
		}

		case IDC_NO_B:
		{
			EndDialog(HandleWnd, (INT_PTR)EAppReturnType::No);
			break;
		}

		case IDC_NOTOALL:
		{
			EndDialog(HandleWnd, (INT_PTR)EAppReturnType::NoAll);
			break;
		}

		case IDC_CANCEL:
		{
			if (GCancelButtonEnabled)
			{
				EndDialog(HandleWnd, (INT_PTR)EAppReturnType::Cancel);
			}

			break;
		}

		}

		break;
	}

	case WM_HOTKEY:
	{
		switch (WParam)
		{

		case HOTKEY_YES:
		{
			EndDialog(HandleWnd, (INT_PTR)EAppReturnType::Yes);
			break;
		}
			
		case HOTKEY_NO:
		{
			EndDialog(HandleWnd, (INT_PTR)EAppReturnType::No);
			break;
		}
		
		case HOTKEY_CANCEL:
		{
			if (GCancelButtonEnabled)
			{
				EndDialog(HandleWnd, (INT_PTR)EAppReturnType::Cancel);
			}

			break;
		}
			
		}

		break;
	}

	default:
		return false;

	}

	return true;
}

/**
 * Displays extended message box allowing for YesAll/NoAll
 * @return 3 - YesAll, 4 - NoAll, -1 for Fail
 */
int MessageBoxExtInternal(EAppMsgType MsgType, HWND HandleWnd, const TCHAR* Text, const TCHAR* Caption)
{
	GMessageBoxText = (TCHAR*)Text;
	GMessageBoxCaption = (TCHAR*)Caption;

	switch (MsgType)
	{

	case EAppMsgType::YesNoYesAllNoAll:
	{
		GCancelButtonEnabled = false;
		return (int)DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_YESNO2ALL), HandleWnd, MessageBoxDlgProc);
	}
	case EAppMsgType::YesNoYesAllNoAllCancel:
	{
		GCancelButtonEnabled = true;
		return (int)DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_YESNO2ALLCANCEL), HandleWnd, MessageBoxDlgProc);
	}
	case EAppMsgType::YesNoYesAll:
	{
		GCancelButtonEnabled = false;
		return (int)DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_YESNOYESTOALL), HandleWnd, MessageBoxDlgProc);
	}

	}

	return -1;
}

EAppReturnType FWindowsPlatformMisc::MessageBoxExt(EAppMsgType MsgType, const TCHAR* Text, const TCHAR* Caption)
{

	HWND ParentWindow = (HWND)NULL;
	switch (MsgType)
	{

	case EAppMsgType::Ok:
	{
		MessageBox(ParentWindow, Text, Caption, MB_OK | MB_SYSTEMMODAL);
		return EAppReturnType::Ok;
	}
	case EAppMsgType::YesNo:
	{
		int32 Return = MessageBox(ParentWindow, Text, Caption, MB_YESNO | MB_SYSTEMMODAL);
		return Return == IDYES ? EAppReturnType::Yes : EAppReturnType::No;
	}
	case EAppMsgType::OkCancel:
	{
		int32 Return = MessageBox(ParentWindow, Text, Caption, MB_OKCANCEL | MB_SYSTEMMODAL);
		return Return == IDOK ? EAppReturnType::Ok : EAppReturnType::Cancel;
	}
	case EAppMsgType::YesNoCancel:
	{
		int32 Return = MessageBox(ParentWindow, Text, Caption, MB_YESNOCANCEL | MB_ICONQUESTION | MB_SYSTEMMODAL);
		return Return == IDYES ? EAppReturnType::Yes : (Return == IDNO ? EAppReturnType::No : EAppReturnType::Cancel);
	}
	case EAppMsgType::CancelRetryContinue:
	{
		int32 Return = MessageBox(ParentWindow, Text, Caption, MB_CANCELTRYCONTINUE | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_SYSTEMMODAL);
		return Return == IDCANCEL ? EAppReturnType::Cancel : (Return == IDTRYAGAIN ? EAppReturnType::Retry : EAppReturnType::Continue);
	}

	case EAppMsgType::YesNoYesAllNoAll:
	{
		return (EAppReturnType)MessageBoxExtInternal(EAppMsgType::YesNoYesAllNoAll, ParentWindow, Text, Caption);
		//These return codes just happen to match up with ours.
		// return 0 for No, 1 for Yes, 2 for YesToAll, 3 for NoToAll
		break;
	}
	case EAppMsgType::YesNoYesAllNoAllCancel:
	{
		return (EAppReturnType)MessageBoxExtInternal(EAppMsgType::YesNoYesAllNoAllCancel, ParentWindow, Text, Caption);
		//These return codes just happen to match up with ours.
		// return 0 for No, 1 for Yes, 2 for YesToAll, 3 for NoToAll, 4 for Cancel
		break;
	}
	case EAppMsgType::YesNoYesAll:
	{
		return (EAppReturnType)MessageBoxExtInternal(EAppMsgType::YesNoYesAll, ParentWindow, Text, Caption);
		//These return codes just happen to match up with ours.
		// return 0 for No, 1 for Yes, 2 for YesToAll
		break;
	}


	default:
		break;

	}


	return EAppReturnType::Cancel;
}








const TCHAR* FWindowsPlatformMisc::GetSystemErrorMessage(TCHAR* OutBuffer, uint32 BufferCount, int32 Error)
{
	if (OutBuffer == nullptr || BufferCount == 0) return nullptr;

	*OutBuffer = TEXT('\0');
	if (Error == 0)
	{
		Error = GetLastError();
	}

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), OutBuffer, BufferCount, NULL);
	TCHAR* Found = (TCHAR*)FMicrosoftPlatformString::Strchr(OutBuffer, TEXT('\r'));
	if (Found)
	{
		*Found = TEXT('\0');
	}

	Found = (TCHAR*)FMicrosoftPlatformString::Strchr(OutBuffer, TEXT('\n'));
	if (Found)
	{
		*Found = TEXT('\0');
	}

	return OutBuffer;
}

uint32 FWindowsPlatformMisc::GetLastError()
{
	return (uint32)::GetLastError();
}

void FWindowsPlatformMisc::SetLastError(uint32 ErrorCode)
{
	::SetLastError((DWORD)ErrorCode);
}

void FWindowsPlatformMisc::RaiseException(uint32 ExceptionCode)
{
	/** This is the last place to gather memory stats before exception. */

	::RaiseException(ExceptionCode, 0, 0, NULL);
}






bool FWindowsPlatformMisc::IsRunningOnBattery()
{
	SYSTEM_POWER_STATUS Status;
	GetSystemPowerStatus(&Status);

	switch (Status.BatteryFlag)
	{

	case 4://	"Critical-the battery capacity is at less than five percent"
	case 2://	"Low-the battery capacity is at less than 33 percent"
	case 1://	"High-the battery capacity is at more than 66 percent"
	case 8://	"Charging"
		return true;
	case 128://	"No system battery" - desktop, NB: UPS don't count as batteries under Windows
	case 255://	"Unknown status-unable to read the battery flag information"
	default:
		return false;

	}

	return false;
}



bool FWindowsPlatformMisc::Is64bitOperatingSystem()
{

#pragma warning( push )
#pragma warning( disable: 4191 )	// unsafe conversion from 'type of expression' to 'type required'

	typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	BOOL bIsWoW64Process = 0;
	if (fnIsWow64Process != NULL)
	{
		if (fnIsWow64Process(GetCurrentProcess(), &bIsWoW64Process) == 0)
		{
			bIsWoW64Process = 0;
		}
	}

#pragma warning( pop )
	return bIsWoW64Process == 1;
}

void FWindowsPlatformMisc::GetOSVersions(FString& OutOSVersionLabel, FString& OutOSSubVersionLabel)
{
	static struct FOSVersionsInitializer
	{
		FOSVersionsInitializer()
		{
			OSVersionLabel[0] = 0;
			OSSubVersionLabel[0] = 0;
			GetOSVersionsHelper(OSVersionLabel, _countof(OSVersionLabel), OSSubVersionLabel, _countof(OSSubVersionLabel));
		}

		TCHAR OSVersionLabel[128];
		TCHAR OSSubVersionLabel[128];
	} OSVersionsInitializer;

	OutOSVersionLabel = OSVersionsInitializer.OSVersionLabel;
	OutOSSubVersionLabel = OSVersionsInitializer.OSSubVersionLabel;
}

FString FWindowsPlatformMisc::GetOSVersion()
{
	static struct FOSVersionInitializer
	{
		FOSVersionInitializer()
		{
			CachedOSVersion[0] = 0;
			if (!GetOSVersionHelper(CachedOSVersion, _countof(CachedOSVersion)))
			{
				CachedOSVersion[0] = 0;
			}
		}

		TCHAR CachedOSVersion[128];
	} OSVersionInitializer;

	return OSVersionInitializer.CachedOSVersion;
}




FString FWindowsPlatformMisc::GetDefaultLanguage()
{
	// Only use GetUserPreferredUILanguages on Windows 8+ as older versions didn't always have language packs available
	if (VerifyWindowsVersion(6, 2))
	{
		ULONG NumLanguages = 0;
		ULONG LangBufferSize = 0;
		if (GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &NumLanguages, nullptr, &LangBufferSize))
		{
			FString LangBuffer(LangBufferSize);

			if (GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &NumLanguages, LangBuffer.GetBuffer(), &LangBufferSize))
			{
				// GetUserPreferredUILanguages returns a list where each item is null terminated, so this produces a string containing only the first item
				return LangBuffer;
			}
		}
	}

	return GetDefaultLocale();
}

FString FWindowsPlatformMisc::GetDefaultLocale()
{
	WCHAR LocaleName[LOCALE_NAME_MAX_LENGTH];
	if (GetUserDefaultLocaleName(LocaleName, LOCALE_NAME_MAX_LENGTH))
	{
		return FString(LocaleName);
	}

	return TEXT("en");
}






int32 FWindowsPlatformMisc::NumberOfCores()
{
	//TODO

	return 0;
}

int32 FWindowsPlatformMisc::NumberOfCoresIncludingHyperthreads()
{
	//TODO

	return 0;
}

int32 FWindowsPlatformMisc::NumberOfWorkerThreadsToSpawn()
{
	static int32 MaxServerWorkerThreads = 4;
	static int32 MaxWorkerThreads = 26;

	int32 LNumberOfCores = NumberOfCores();
	int32 LNumberOfCoresIncludingHyperthreads = NumberOfCoresIncludingHyperthreads();
	int32 LNumberOfThreads = 0;

	if (NumberOfCoresIncludingHyperthreads > NumberOfCores)
	{
		LNumberOfThreads = LNumberOfCoresIncludingHyperthreads - 2;
	}
	else
	{
		LNumberOfThreads = LNumberOfCores - 1;
	}

	int32 MaxWorkerThreadsWanted = MaxWorkerThreads;
	// need to spawn at least one worker thread (see FTaskGraphImplementation)
	return FMath::Max(FMath::Min(LNumberOfThreads, MaxWorkerThreadsWanted), 2);
}




bool FWindowsPlatformMisc::HasCPUIDInstruction()
{
	return FCPUIDQueriedData::HasCPUIDInstruction();
}

FString FWindowsPlatformMisc::GetCPUVendor()
{
	return FCPUIDQueriedData::GetVendor();
}

FString FWindowsPlatformMisc::GetCPUBrand()
{
	return FCPUIDQueriedData::GetBrand();
}

uint32 FWindowsPlatformMisc::GetCPUInfo()
{
	return FCPUIDQueriedData::GetCPUInfo();
}

int32 FWindowsPlatformMisc::GetCacheLineSize()
{
	return FCPUIDQueriedData::GetCacheLineSize();
}



FString FWindowsPlatformMisc::GetPrimaryGPUBrand()
{
	static FString PrimaryGPUBrand;
	if (PrimaryGPUBrand.IsEmpty())
	{
		// Find primary display adapter and get the device name.
		PrimaryGPUBrand = TEXT("GenericGPUBrand");

		DISPLAY_DEVICE DisplayDevice;
		DisplayDevice.cb = sizeof(DisplayDevice);
		DWORD DeviceIndex = 0;

		while (EnumDisplayDevices(0, DeviceIndex, &DisplayDevice, 0))
		{
			if ((DisplayDevice.StateFlags & (DISPLAY_DEVICE_ATTACHED_TO_DESKTOP | DISPLAY_DEVICE_PRIMARY_DEVICE)) > 0)
			{
				PrimaryGPUBrand = DisplayDevice.DeviceString;
				break;
			}

			FMemory::Memzero(DisplayDevice);
			DisplayDevice.cb = sizeof(DisplayDevice);
			++DeviceIndex;
		}
	}

	return PrimaryGPUBrand;
}

FGPUDriverInfo FWindowsPlatformMisc::GetGPUDriverInfo(const FString& DeviceDescription)
{
	//TODO
	return FGPUDriverInfo();
}





bool FWindowsPlatformMisc::GetDiskTotalAndFreeSpace(const FString& InPath, uint64& TotalNumberOfBytes, uint64& NumberOfFreeBytes)
{
	//TODO
	// We need to convert the path to make sure it is formatted with windows style Drive e.g. "C:\"
	const FString ValidatedPath = InPath; //FPaths::ConvertRelativePathToFull(InPath).Replace(TEXT("/"), TEXT("\\"));


	if (ValidatedPath.Length() >= 3 && ValidatedPath[1] == ':' && ValidatedPath[2] == '\\')
	{
		return !!GetDiskFreeSpaceEx(ValidatedPath.GetStr(), nullptr, reinterpret_cast<ULARGE_INTEGER*>(&TotalNumberOfBytes), reinterpret_cast<ULARGE_INTEGER*>(&NumberOfFreeBytes));
	}

	return false;
}







bool FWindowsPlatformMisc::GetVSComnTools(int32 Version, FString& OutData)
{

	FString ValueName = FString::Printf(TEXT("%d.0"), Version);

	FString IDEPath;
	if (!QueryRegKey(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7"), ValueName.GetStr(), IDEPath))
	{
		if (!QueryRegKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7"), ValueName.GetStr(), IDEPath))
		{
			if (!QueryRegKey(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VS7"), ValueName.GetStr(), IDEPath))
			{
				if (!QueryRegKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\SxS\\VS7"), ValueName.GetStr(), IDEPath))
				{
					return false;
				}
			}
		}
	}

	//TODO
	//OutData = FPaths::ConvertRelativePathToFull(FPaths::Combine(*IDEPath, L"Common7", L"Tools"));
	return true;
}



FString FWindowsPlatformMisc::GetOperatingSystemId()
{
	FString Result;
	// more info on this key can be found here: http://stackoverflow.com/questions/99880/generating-a-unique-machine-id
	QueryRegKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Cryptography"), TEXT("MachineGuid"), Result);
	return Result;
}

EConvertibleLaptopMode FWindowsPlatformMisc::GetConvertibleLaptopMode()
{
	if (!VerifyWindowsVersion(6, 2))
	{
		return EConvertibleLaptopMode::NotSupported;
	}

	if (GetSystemMetrics(SM_CONVERTIBLESLATEMODE) == 0)
	{
		return EConvertibleLaptopMode::Tablet;
	}

	return EConvertibleLaptopMode::Laptop;
}







const TCHAR* FWindowsPlatformMisc::GetDefaultPathSeparator()
{
	return TEXT("\\");
}

FString FWindowsPlatformMisc::GetPathToEngineExe()
{
	WCHAR Path[512];
	const UINT PathSize = _countof(Path);

	DWORD size = GetModuleFileName(nullptr, Path, PathSize);
	if (size == 0 || size == PathSize)
	{
		// Method failed or path was truncated.
		return L"";
	}


	WCHAR* LastSlash = wcsrchr(Path, L'\\');
	if (LastSlash)
	{
		*(LastSlash + 1) = L'\0';
	}

	return Path;
}

bool FWindowsPlatformMisc::IsValidAbsolutePathFormat(const FString& Path)
{
	//TODO

	/*bool bIsValid = true;
	const FString OnlyPath = FPaths::GetPath(Path);
	if (OnlyPath.IsEmpty())
	{
		bIsValid = false;
	}

	// Must begin with a drive letter
	if (bIsValid && !FChar::IsAlpha(OnlyPath[0]))
	{
		bIsValid = false;
	}

	// On Windows the path must be absolute, i.e: "D:/" or "D:\\"
	if (bIsValid && !(Path.Find(TEXT(":/")) == 1 || Path.Find(TEXT(":\\")) == 1))
	{
		bIsValid = false;
	}

	// Find any unnamed directory changes
	if (bIsValid && (Path.Find(TEXT("//")) != INDEX_NONE) || (Path.Find(TEXT("\\/")) != INDEX_NONE) || (Path.Find(TEXT("/\\")) != INDEX_NONE) || (Path.Find(TEXT("\\\\")) != INDEX_NONE))
	{
		bIsValid = false;
	}

	// ensure there's no further instances of ':' in the string
	if (bIsValid && !(Path.Find(TEXT(":"), ESearchCase::IgnoreCase, ESearchDir::FromStart, 2) == INDEX_NONE))
	{
		bIsValid = false;
	}

	return bIsValid;*/

	return false;
}








bool FWindowsPlatformMisc::VerifyWindowsVersion(uint32 MajorVersion, uint32 MinorVersion, uint32 BuildNumber)
{
	OSVERSIONINFOEX Version;
	Version.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	Version.dwMajorVersion = MajorVersion;
	Version.dwMinorVersion = MinorVersion;
	Version.dwBuildNumber = BuildNumber;

	ULONGLONG ConditionMask = 0;
	ConditionMask = VerSetConditionMask(ConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
	ConditionMask = VerSetConditionMask(ConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
	ConditionMask = VerSetConditionMask(ConditionMask, VER_BUILDNUMBER, VER_GREATER_EQUAL);

	return !!VerifyVersionInfo(&Version, VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER, ConditionMask);
}

bool FWindowsPlatformMisc::QueryRegKey(const HKEY InKey, const TCHAR* InSubKey, const TCHAR* InValueName, FString& OutData)
{
	bool bSuccess = false;

	// Redirect key depending on system
	for (int32 RegistryIndex = 0; RegistryIndex < 2 && !bSuccess; ++RegistryIndex)
	{
		HKEY Key = 0;
		const uint32 RegFlags = (RegistryIndex == 0) ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;
		if (RegOpenKeyEx(InKey, InSubKey, 0, KEY_READ | RegFlags, &Key) == ERROR_SUCCESS)
		{
			DWORD Size = 0;
			// First, we'll call RegQueryValueEx to find out how large of a buffer we need
			if ((RegQueryValueEx(Key, InValueName, NULL, NULL, NULL, &Size) == ERROR_SUCCESS) && Size)
			{
				// Allocate a buffer to hold the value and call the function again to get the data
				char* Buffer = new char[Size];
				if (RegQueryValueEx(Key, InValueName, NULL, NULL, (LPBYTE)Buffer, &Size) == ERROR_SUCCESS)
				{
					const uint32 Length = (Size / sizeof(TCHAR)) - 1;
					OutData = FString((TCHAR*)Buffer, Length);
					bSuccess = true;
				}
				delete[] Buffer;
			}
			RegCloseKey(Key);
		}
	}

	return bSuccess;
}
#endif
