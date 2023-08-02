// Copyright Nord Engine. All Rights Reserved.
#pragma once

#ifndef WINDOWS_H_WRAPPER
#error Do not include PreWindowsAPI directly!
#endif

#if !PLATFORM_WINDOWS
#error PLATFORM_WINDOWS not defined!
#endif




// Re-enable warnings
THIRD_PARTY_INCLUDES_END

// Restore the struct packing setting
PRAGMA_POP_PLATFORM_DEFAULT_PACKING

// Hide Windows-only types
#undef INT
#undef UINT
#undef DWORD
#undef FLOAT
#undef TRUE
#undef FALSE

// Undo any Windows defines.
#undef uint8
#undef uint16
#undef uint32
#undef int32
#undef float
#undef CDECL
#undef PF_MAX
#undef PlaySound
#undef DrawText
#undef CaptureStackBackTrace
#undef MemoryBarrier
#undef DeleteFile
#undef MoveFile
#undef CopyFile
#undef CreateDirectory
#undef GetCurrentTime
#undef SendMessage
#undef LoadString
#undef UpdateResource
#undef FindWindow
#undef GetObject
#undef CreateFont
#undef CreateDesktop
#undef GetMessage
#undef PostMessage
#undef GetCommandLine
#undef GetProp
#undef SetPort
#undef SetProp
#undef GetFileAttributes
#undef ReportEvent
#undef GetClassName
#undef GetClassInfo
#undef Yield
#undef IMediaEventSink
#undef GetTempFileName
#undef GetFreeSpace

// Undefine all the atomics.
#undef InterlockedIncrement
#undef InterlockedDecrement
#undef InterlockedAdd
#undef InterlockedExchange
#undef InterlockedExchangeAdd
#undef InterlockedCompareExchange
#undef InterlockedCompareExchangePointer
#undef InterlockedExchange64
#undef InterlockedExchangeAdd64
#undef InterlockedCompareExchange64
#undef InterlockedIncrement64
#undef InterlockedDecrement64
#undef InterlockedAnd
#undef InterlockedOr
#undef InterlockedXor

// Restore any previously defined macros
#undef TEXT
#pragma pop_macro("TEXT")
