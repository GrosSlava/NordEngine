// Copyright Nord Engine. All Rights Reserved.
#pragma once
#define GEN_PLATFORM_H
#include "Build.h"




template<typename T32BITS, typename T64BITS, int PointerSize>
struct SelectIntPointerType
{
	// nothing here are is it an error if the partial specializations fail
};
template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 8>
{
	// Select the 64 bit type.
	using TIntPointer = T64BITS;
};
template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 4>
{
	// Select the 32 bit type.
	using TIntPointer = T32BITS;
};

/**
	Generic types for almost all compilers and platforms.
*/
struct FGenericPlatformTypes
{
	//~ Unsigned base types

	// 8-bit unsigned integer
	using uint8 = unsigned char;
	// 16-bit unsigned integer
	using uint16 = unsigned short int;
	// 32-bit unsigned integer
	using uint32 = unsigned int;
	// 64-bit unsigned integer
	using uint64 = unsigned long long;

	//~ Signed base types.

	// 8-bit signed integer
	using int8 = signed char;
	// 16-bit signed integer
	using int16 = signed short int;
	// 32-bit signed integer
	using int32 = signed int;
	// 64-bit signed integer
	using int64 = signed long long;

	//~ Character types

	// An ANSI character. 8-bit fixed-width representation of 7-bit characters.
	using ANSICHAR = char;
	// A wide character. In-memory only. ?-bit fixed-width representation of the platform's natural wide character set. Could be different sizes on different platforms.
	using WIDECHAR = wchar_t;
	// A switchable character. In-memory only. Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.
#if UNICODE
	using TCHAR = WIDECHAR;
#else
	using TCHAR = ANSICHAR;
#endif

	//~ Pointer types

	// Unsigned int. The same size as a pointer.
	using UPTRINT = SelectIntPointerType<uint32, uint64, sizeof(void*)>::TIntPointer;
	// Signed int. The same size as a pointer.
	using PTRINT = SelectIntPointerType<int32, int64, sizeof(void*)>::TIntPointer;
	// Unsigned int. The same size as a pointer.
	using SIZE_T = UPTRINT;
	// Signed int. The same size as a pointer.
	using SSIZE_T = PTRINT;
	// The type of the NULL constant.
	using TYPE_OF_NULL = int32;
	// The type of the C++ nullptr keyword.
	using TYPE_OF_NULLPTR = decltype(nullptr);
};




// clang-format off
// FPlatformTypes will be defined.
#if WIN32 || WIN64
	#include "Windows/WindowsPlatform.h"
#else
	#error "Undefined platform!"
#endif

#include "GenericPlatformUtils.h"
#include "GenericPlatformCompilerSetup.h"
// clang-format on




// clang-format off
// define all other platforms to be zero
#ifndef PLATFORM_WINDOWS
	#define PLATFORM_WINDOWS 0
#endif
#ifndef PLATFORM_MICROSOFT
	#define PLATFORM_MICROSOFT 0
#endif
#ifndef PLATFORM_LINUX
	#define PLATFORM_LINUX 0
#endif
#ifndef PLATFORM_LINUXAARCH64
	#define PLATFORM_LINUXAARCH64 0
#endif
#ifndef PLATFORM_UNIX
	#define PLATFORM_UNIX 0
#endif
#ifndef PLATFORM_FREEBSD
	#define PLATFORM_FREEBSD 0
#endif
#ifndef PLATFORM_MAC
	#define PLATFORM_MAC 0
	#define PLATFORM_MAC_X86 0
	#define PLATFORM_MAC_ARM64 0
#endif
#ifndef PLATFORM_APPLE
	#define PLATFORM_APPLE 0
#endif
#ifndef PLATFORM_ANDROID
	#define PLATFORM_ANDROID 0
#endif
#ifndef PLATFORM_ANDROID_ARM
	#define PLATFORM_ANDROID_ARM 0
#endif
#ifndef PLATFORM_ANDROID_ARM64
	#define PLATFORM_ANDROID_ARM64 0
#endif
#ifndef PLATFORM_ANDROID_X86
	#define PLATFORM_ANDROID_X86 0
#endif
#ifndef PLATFORM_ANDROID_X64
	#define PLATFORM_ANDROID_X64 0
#endif
#ifndef PLATFORM_ANDROID_VULKAN
	#define PLATFORM_ANDROID_VULKAN 0
#endif
#ifndef PLATFORM_ANDROIDGL4
	#define PLATFORM_ANDROIDGL4 0
#endif
#ifndef PLATFORM_IOS
	#define PLATFORM_IOS 0
#endif
#ifndef PLATFORM_XBOXONE
	#define PLATFORM_XBOXONE 0
#endif
#ifndef PLATFORM_PS4
	#define PLATFORM_PS4 0
#endif
#ifndef PLATFORM_SWITCH
	#define PLATFORM_SWITCH 0
#endif
// clang-format on

// clang-format off
#ifndef PLATFORM_CPU_X86_FAMILY
#if( defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__amd64__) || defined(__x86_64__) )
	#define PLATFORM_CPU_X86_FAMILY 1
#else
	#define PLATFORM_CPU_X86_FAMILY 0
#endif
#endif
// clang-format on

// clang-format off
#ifndef PLATFORM_CPU_ARM_FAMILY
#if( defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64) )
	#define PLATFORM_CPU_ARM_FAMILY 1
#else
	#define PLATFORM_CPU_ARM_FAMILY 0
#endif
#endif
// clang-format on

// clang-format off
#ifndef PLATFORM_HAS_CPUID
#if defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
	#define PLATFORM_HAS_CPUID 1
#else
	#define PLATFORM_HAS_CPUID 0
#endif
#endif
// clang-format on

// clang-format off
// Base defines, must define these for the platform, there are no defaults
#ifndef PLATFORM_DESKTOP
	#error "PLATFORM_DESKTOP must be defined"
#endif
#ifndef PLATFORM_64BITS
	#error "PLATFORM_64BITS must be defined"
#endif
#ifndef PLATFORM_32BITS
	#error "PLATFORM_32BITS must be defined"
#endif
// clang-format on

// clang-format off
// Base defines, these have defaults
#ifndef USING_ADDRESS_SANITISER
	#define USING_ADDRESS_SANITISER 0
#endif
#ifndef PLATFORM_LITTLE_ENDIAN
	#define PLATFORM_LITTLE_ENDIAN 0
#endif
#ifndef PLATFORM_EXCEPTIONS_DISABLED
	#define PLATFORM_EXCEPTIONS_DISABLED !PLATFORM_DESKTOP
#endif
#ifndef PLATFORM_SEH_EXCEPTIONS_DISABLED
	#define PLATFORM_SEH_EXCEPTIONS_DISABLED 0
#endif
#ifndef PLATFORM_ENABLE_VECTORINTRINSICS
	#define PLATFORM_ENABLE_VECTORINTRINSICS 0
#endif
#ifndef PLATFORM_MAYBE_HAS_SSE4_1
	#define PLATFORM_MAYBE_HAS_SSE4_1 0
#endif
#ifndef PLATFORM_ALWAYS_HAS_SSE4_1
	#define PLATFORM_ALWAYS_HAS_SSE4_1 0
#endif
#ifndef PLATFORM_MAYBE_HAS_AVX
	#define PLATFORM_MAYBE_HAS_AVX 0
#endif
#ifndef PLATFORM_ALWAYS_HAS_AVX
	#define PLATFORM_ALWAYS_HAS_AVX 0
#endif
#ifndef PLATFORM_ALWAYS_HAS_FMA3
	#define PLATFORM_ALWAYS_HAS_FMA3 0
#endif
#ifndef PLATFORM_WEAKLY_CONSISTENT_MEMORY
	#define PLATFORM_WEAKLY_CONSISTENT_MEMORY PLATFORM_CPU_ARM_FAMILY
#endif
#ifndef PLATFORM_HAS_128BIT_ATOMICS
	#define PLATFORM_HAS_128BIT_ATOMICS 0
#endif
#ifndef PLATFORM_SUPPORTS_COLORIZED_OUTPUT_DEVICE
	#define PLATFORM_SUPPORTS_COLORIZED_OUTPUT_DEVICE PLATFORM_DESKTOP
#endif
#ifndef PLATFORM_COMPILER_HAS_IF_CONSTEXPR
	#define PLATFORM_COMPILER_HAS_IF_CONSTEXPR 0
#endif
// clang-format on

// clang-format off
// Engine features support
#ifndef PLATFORM_SUPPORTS_TEXTURE_STREAMING
	#define PLATFORM_SUPPORTS_TEXTURE_STREAMING 1
#endif
#ifndef PLATFORM_SUPPORTS_VIRTUAL_TEXTURE_STREAMING
	#define PLATFORM_SUPPORTS_VIRTUAL_TEXTURE_STREAMING 0
#endif
#ifndef PLATFORM_SUPPORTS_VIRTUAL_TEXTURES
	#define PLATFORM_SUPPORTS_VIRTUAL_TEXTURES 0
#endif
#ifndef PLATFORM_SUPPORTS_VARIABLE_RATE_SHADING
	#define PLATFORM_SUPPORTS_VARIABLE_RATE_SHADING 0
#endif
#ifndef PLATFORM_SUPPORTS_GEOMETRY_SHADERS
	#define PLATFORM_SUPPORTS_GEOMETRY_SHADERS 1
#endif
#ifndef PLATFORM_SUPPORTS_TESSELLATION_SHADERS
	#define PLATFORM_SUPPORTS_TESSELLATION_SHADERS 1
#endif
#ifndef PLATFORM_SUPPORTS_BORDERLESS_WINDOW
	#define PLATFORM_SUPPORTS_BORDERLESS_WINDOW 0
#endif
#ifndef PLATFORM_SUPPORTS_MULTIPLE_NATIVE_WINDOWS
	#define PLATFORM_SUPPORTS_MULTIPLE_NATIVE_WINDOWS 1
#endif
#ifndef PLATFORM_HAS_TOUCH_MAIN_SCREEN
	#define PLATFORM_HAS_TOUCH_MAIN_SCREEN 0
#endif
// clang-format on

// clang-format off
// TCHAR/WCHAR
#ifndef PLATFORM_TCHAR_IS_1_BYTE
	#define PLATFORM_TCHAR_IS_1_BYTE 0
#endif
#ifndef PLATFORM_TCHAR_IS_4_BYTES
	#define PLATFORM_TCHAR_IS_4_BYTES 0
#endif
#ifndef PLATFORM_TCHAR_IS_CHAR16
	#define PLATFORM_TCHAR_IS_CHAR16 0
#endif
#ifndef PLATFORM_WCHAR_IS_4_BYTES
	#define PLATFORM_WCHAR_IS_4_BYTES 0
#endif
// clang-format om




// An 8-bit unsigned integer.
using uint8 = FPlatformTypes::uint8;
// A 16-bit unsigned integer.
using uint16 = FPlatformTypes::uint16;
// A 32-bit unsigned integer.
using uint32 = FPlatformTypes::uint32;
// A 64-bit unsigned integer.
using uint64 = FPlatformTypes::uint64;

// An 8-bit signed integer.
using int8 = FPlatformTypes::int8;
// A 16-bit signed integer.
using int16 = FPlatformTypes::int16;
// A 32-bit signed integer.
using int32 = FPlatformTypes::int32;
// A 64-bit signed integer.
using int64 = FPlatformTypes::int64;

// An ANSI character. Normally a signed type.
using ANSICHAR = FPlatformTypes::ANSICHAR;
// A wide character. Normally a signed type.
using WIDECHAR = FPlatformTypes::WIDECHAR;
// Either ANSICHAR or WIDECHAR, depending on whether the platform supports wide characters or the requirements of the licensee.
using TCHAR = FPlatformTypes::TCHAR;

// An unsigned integer the same size as a pointer
using UPTRINT = FPlatformTypes::UPTRINT;
// A signed integer the same size as a pointer
using PTRINT = FPlatformTypes::PTRINT;
// An unsigned integer the same size as a pointer, the same as UPTRINT
using SIZE_T = FPlatformTypes::SIZE_T;
// An integer the same size as a pointer, the same as PTRINT
using SSIZE_T = FPlatformTypes::SSIZE_T;
// The type of the NULL constant.
using TYPE_OF_NULL = FPlatformTypes::TYPE_OF_NULL;
// The type of the C++ nullptr keyword.
using TYPE_OF_NULLPTR = FPlatformTypes::TYPE_OF_NULLPTR;



/**
	Test the global types.
*/
namespace TypeTests
{
template<typename A, typename B>
struct TAreTypesEqual
{
	static const bool Value = false;
};
template<typename T>
struct TAreTypesEqual<T, T>
{
	static const bool Value = true;
};

static_assert(!PLATFORM_TCHAR_IS_4_BYTES || sizeof(TCHAR) == 4, "TCHAR size must be 4 bytes.");
static_assert(PLATFORM_TCHAR_IS_4_BYTES || sizeof(TCHAR) == 2, "TCHAR size must be 2 bytes.");

static_assert(!PLATFORM_WCHAR_IS_4_BYTES || sizeof(wchar_t) == 4, "wchar_t size must be 4 bytes.");
static_assert(PLATFORM_WCHAR_IS_4_BYTES || sizeof(wchar_t) == 2, "wchar_t size must be 2 bytes.");

static_assert(PLATFORM_32BITS || PLATFORM_64BITS, "Type tests pointer size failed.");
static_assert(PLATFORM_32BITS != PLATFORM_64BITS, "Type tests pointer exclusive failed.");
static_assert(!PLATFORM_64BITS || sizeof(void*) == 8, "Pointer size is 64bit, but pointers are short.");
static_assert(PLATFORM_64BITS || sizeof(void*) == 4, "Pointer size is 32bit, but pointers are long.");

static_assert(char(-1) < char(0), "Unsigned char type test failed.");

static_assert((!TAreTypesEqual<ANSICHAR, WIDECHAR>::Value), "ANSICHAR and WIDECHAR should be different types.");
static_assert((TAreTypesEqual<TCHAR, ANSICHAR>::Value == true || TAreTypesEqual<TCHAR, WIDECHAR>::Value == true), "TCHAR should either be ANSICHAR or WIDECHAR.");

static_assert(sizeof(uint8) == 1, "uint8 type size test failed.");
static_assert(int32(uint8(-1)) == 0xFF, "uint8 type sign test failed.");

static_assert(sizeof(uint16) == 2, "uint16 type size test failed.");
static_assert(int32(uint16(-1)) == 0xFFFF, "uint16 type sign test failed.");

static_assert(sizeof(uint32) == 4, "uint32 type size test failed.");
static_assert(int64(uint32(-1)) == int64(0xFFFFFFFF), "uint32 type sign test failed.");

static_assert(sizeof(uint64) == 8, "uint64 type size test failed.");
static_assert(uint64(-1) > uint64(0), "uint64 type sign test failed.");


static_assert(sizeof(int8) == 1, "int8 type size test failed.");
static_assert(int32(int8(-1)) == -1, "int8 type sign test failed.");

static_assert(sizeof(int16) == 2, "int16 type size test failed.");
static_assert(int32(int16(-1)) == -1, "int16 type sign test failed.");

static_assert(sizeof(int32) == 4, "int32 type size test failed.");
static_assert(int64(int32(-1)) == int64(-1), "int32 type sign test failed.");

static_assert(sizeof(int64) == 8, "int64 type size test failed.");
static_assert(int64(-1) < int64(0), "int64 type sign test failed.");

static_assert(sizeof(ANSICHAR) == 1, "ANSICHAR type size test failed.");
static_assert(int32(ANSICHAR(-1)) == -1, "ANSICHAR type sign test failed.");

static_assert(sizeof(WIDECHAR) == 2 || sizeof(WIDECHAR) == 4, "WIDECHAR type size test failed.");

static_assert(sizeof(PTRINT) == sizeof(void*), "PTRINT type size test failed.");
static_assert(PTRINT(-1) < PTRINT(0), "PTRINT type sign test failed.");

static_assert(sizeof(UPTRINT) == sizeof(void*), "UPTRINT type size test failed.");
static_assert(UPTRINT(-1) > UPTRINT(0), "UPTRINT type sign test failed.");

static_assert(sizeof(SIZE_T) == sizeof(void*), "SIZE_T type size test failed.");
static_assert(SIZE_T(-1) > SIZE_T(0), "SIZE_T type sign test failed.");
} // namespace TypeTests

#undef GEN_PLATFORM_H