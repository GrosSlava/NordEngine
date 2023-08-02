// Copyright Nord Engine. All Rights Reserved.
#include "GenericPlatform.h"
#if PLATFORM_WINDOWS

#include "Windows/WindowsPlatformMisc/WindowsStackTraceHelper.h"
#include "Windows/WindowsHWrapper.h"

#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")





bool FWindowsStackTraceHelper::GetStackFrames(TArray<FStackFrame>& OutStackFrames)
{
	DWORD Machine = IMAGE_FILE_MACHINE_AMD64;
	HANDLE Process = GetCurrentProcess();
	HANDLE Thread = GetCurrentThread();

	if( !SymInitialize(Process, nullptr, true) )
	{
		return false;
	}

	SymSetOptions(SYMOPT_LOAD_LINES);

	CONTEXT Context = {};
	Context.ContextFlags = CONTEXT_FULL;
	RtlCaptureContext(&Context);

	STACKFRAME Frame = {};
	Frame.AddrPC.Offset = Context.Rip;
	Frame.AddrPC.Mode = AddrModeFlat;
	Frame.AddrFrame.Offset = Context.Rbp;
	Frame.AddrFrame.Mode = AddrModeFlat;
	Frame.AddrStack.Offset = Context.Rsp;
	Frame.AddrStack.Mode = AddrModeFlat;


	bool IsFirst = true;
	while( StackWalk(Machine, Process, Thread, &Frame, &Context, nullptr, SymFunctionTableAccess, SymGetModuleBase, nullptr) )
	{
		FStackFrame f;
		f.Address = Frame.AddrPC.Offset;


		DWORD64 ModuleBase = SymGetModuleBase(Process, Frame.AddrPC.Offset);

		FString ModuleName;
		ModuleName.Reserve(MAX_PATH);
		if( ModuleBase && GetModuleFileName((HINSTANCE)ModuleBase, ModuleName.GetBuffer(), MAX_PATH) )
		{
			const int32 LastSlash = ModuleName.Find(TEXT("\\/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			f.ModuleName = LastSlash == -1 ? ModuleName : ModuleName.SubStr(LastSlash + 1);
		}
		else
		{
			f.ModuleName = TEXT("Unknown Module");
		}


		DWORD64 Offset = 0;

		char SymbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
		PIMAGEHLP_SYMBOL Symbol = (PIMAGEHLP_SYMBOL)SymbolBuffer;
		Symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL) + 255;
		Symbol->MaxNameLength = 254;

		if( SymGetSymFromAddr(Process, Frame.AddrPC.Offset, &Offset, Symbol) )
		{
			f.SymName = FString::FromAnsi(Symbol->Name);
		}
		else
		{
			f.SymName = TEXT("Unknown Function");
		}


		IMAGEHLP_LINE Line;
		Line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		DWORD Offset_ln = 0;
		if( SymGetLineFromAddr(Process, Frame.AddrPC.Offset, &Offset_ln, &Line) )
		{
			f.FileName = FString::FromAnsi(Line.FileName);
			f.Line = Line.LineNumber;
		}
		else
		{
			f.FileName = TEXT("Unknown File");
			f.Line = 0;
		}


		if( !IsFirst )
		{
			OutStackFrames.PushBack(f);
		}
		IsFirst = false;
	}

	SymCleanup(Process);
	return true;
}

#endif // PLATFORM_WINDOWS