// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#include "TypeTraits/IsEnum.h"
#include "TypeTraits/EnableIf.h"




/**
	Combines two hash values to get a third.
	Note - this function is not commutative.
*/
FORCEINLINE uint32 HashCombine(uint32 A, uint32 C) noexcept
{
	uint32 B = 0x9e3779b9;
	A += B;

	// clang-format off
	A -= B; A -= C; A ^= (C >> 13);
	B -= C; B -= A; B ^= (A << 8);
	C -= A; C -= B; C ^= (B >> 13);
	A -= B; A -= C; A ^= (C >> 12);
	B -= C; B -= A; B ^= (A << 16);
	C -= A; C -= B; C ^= (B >> 5);
	A -= B; A -= C; A ^= (C >> 3);
	B -= C; B -= A; B ^= (A << 10);
	C -= A; C -= B; C ^= (B >> 15);
	// clang-format on;

	return C;
}

//............Hash functions for common types..........................//

FORCEINLINE uint64 GetTypeHash(const uint8 A) noexcept
{
	return A;
}

FORCEINLINE uint64 GetTypeHash(const int8 A) noexcept
{
	return A;
}

FORCEINLINE uint64 GetTypeHash(const uint16 A) noexcept
{
	return A;
}

FORCEINLINE uint64 GetTypeHash(const int16 A) noexcept
{
	return A;
}

FORCEINLINE uint64 GetTypeHash(const int32 A) noexcept
{
	return A;
}

FORCEINLINE uint64 GetTypeHash(const uint32 A) noexcept
{
	return A;
}

FORCEINLINE uint64 GetTypeHash(const uint64 A) noexcept
{
	return A;
}

FORCEINLINE uint64 GetTypeHash(const int64 A) noexcept
{
	return A;
}

FORCEINLINE uint64 GetTypeHash(float Value) noexcept
{
	return *(uint32*)&Value;
}

FORCEINLINE uint64 GetTypeHash(double Value) noexcept
{
	return *(uint64*)&Value;
}

FORCEINLINE uint64 GetTypeHash(const void* Value) noexcept
{
	return reinterpret_cast<UPTRINT>(Value);
}

FORCEINLINE uint64 GetTypeHash(void* Value) noexcept
{
	return reinterpret_cast<UPTRINT>(Value);
}

template<typename EnumType>
FORCEINLINE typename TEnableIf<TIsEnum<EnumType>::Value, uint64>::Type GetTypeHash(EnumType E) noexcept
{
	return GetTypeHash((__underlying_type(EnumType))E);
}

//.....................................................................//