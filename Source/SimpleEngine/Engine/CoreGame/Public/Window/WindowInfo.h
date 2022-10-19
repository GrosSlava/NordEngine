
#pragma once

#include "CoreMinimal.h"




namespace EWindowStyle
{
enum
{
	None = 0,		   // No border / title bar (this flag and all others are mutually exclusive)
	Titlebar = 1 << 0, // Title bar + fixed border
	Resize = 1 << 1,   // Title bar + resizable border + maximize button
	Close = 1 << 2,	   // Title bar + close button

	Default = Titlebar | Resize | Close // Default window style
};
} // namespace EWindowStyle


namespace FWindowHandle
{
// clang-format off
#if PLATFORM_WINDOWS
	// Window handle is HWND (HWND__*) on Windows
	typedef HWND WindowHandle;
	const static WindowHandle EmptyWindowHandle = nullptr;
#elif PLATFORM_LINUX || PLATFORM_LINUXAARCH64 || PLATFORM_FREEBSD || PLATFORM_UNIX
	// Window handle is Window (unsigned long) on Unix - X11
	typedef unsigned long WindowHandle;
	const static WindowHandle EmptyWindowHandle = 0;
#elif PLATFORM_MAC || PLATFORM_MAC_X86 || PLATFORM_MAC_ARM64
	// Window handle is NSWindow or NSView (void*) on Mac OS X - Cocoa
	typedef void* WindowHandle;
	const static WindowHandle EmptyWindowHandle = nullptr;
#elif PLATFORM_IOS
	// Window handle is UIWindow (void*) on iOS - UIKit
	typedef void* WindowHandle;
	const static WindowHandle EmptyWindowHandle = nullptr;
#elif PLATFORM_ANDROID
	// Window handle is ANativeWindow* (void*) on Android
	typedef void* WindowHandle;
	const static WindowHandle EmptyWindowHandle = nullptr;
#endif
// clang-format on
} // namespace FWindowHandle