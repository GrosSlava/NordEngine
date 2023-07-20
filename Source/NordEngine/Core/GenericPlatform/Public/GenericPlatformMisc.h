// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	Enumerates the modes a convertible laptop can be in.
*/
enum class EConvertibleLaptopMode
{
	/* Not a convertible laptop. */
	NotSupported,

	/* Laptop arranged as a laptop. */
	Laptop,

	/* Laptop arranged as a tablet. */
	Tablet
};

/**
	Device orientations for screens. e.g. Landscape, Portrait, etc.
*/
enum class EDeviceScreenOrientation
{
	/* The orientation is not known */
	Unknown,

	/* The orientation is portrait with the home button at the bottom */
	Portrait,

	/* The orientation is portrait with the home button at the top */
	PortraitUpsideDown,

	/* The orientation is landscape with the home button at the right side */
	LandscapeLeft,

	/* The orientation is landscape with the home button at the left side */
	LandscapeRight,

	/* The orientation is as if place on a desk with the screen upward */
	FaceUp,

	/* The orientation is as if place on a desk with the screen downward */
	FaceDown,

	/* The orientation is portrait, oriented upright with the sensor */
	PortraitSensor,

	/* The orientation is landscape, oriented upright with the sensor */
	LandscapeSensor,
};

/**
	Enumerates supported message dialog button types.
*/
enum class EAppMsgType
{
	Ok,
	YesNo,
	OkCancel,
	YesNoCancel,
	CancelRetryContinue,
	YesNoYesAllNoAll,
	YesNoYesAllNoAllCancel,
	YesNoYesAll,
};

/**
	Enumerates message dialog return types.
*/
enum class EAppReturnType
{
	No,
	Yes,
	YesAll,
	NoAll,
	Cancel,
	Ok,
	Retry,
	Continue,
};

/**
	Enumerates the network connection typesþ
*/
enum class ENetworkConnectionType
{
	Unknown,
	None,
	AirplaneMode,
	Cell,
	WiFi,
	WiMAX,
	Bluetooth,
	Ethernet,
};




// clang-format off
// FPlatformMisc will be defined.
#if WIN32 || WIN64
	#include "Windows/WindowsPlatformMisc/WindowsPlatformMisc.h"
#else
	#error "Undefined platform!"
#endif
// clang-format on