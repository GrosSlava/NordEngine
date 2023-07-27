// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "GenericPlatformString.h"
#include "EngineMemory.h"




struct ENGINE_API FStringBuffer
{
public:

	FStringBuffer() = default;
	FStringBuffer(const FStringBuffer& Other) = delete;
	FORCEINLINE FStringBuffer(FStringBuffer&& Other) noexcept : str(Other.str), StringLength(Other.StringLength), BufferSize(Other.BufferSize) { Other.Clear(); }
	FORCEINLINE explicit FStringBuffer(uint32 Size) noexcept { Resize(Size); }
	FORCEINLINE ~FStringBuffer() noexcept { Reset(); }


public:

	FStringBuffer& operator=(const FStringBuffer& Other) = delete;
	FORCEINLINE FStringBuffer& operator=(FStringBuffer&& Other) noexcept
	{
		if( str != nullptr )
		{
			FMemory::Free(str);
		}

		str = Other.str;
		StringLength = Other.StringLength;
		BufferSize = Other.BufferSize;

		Other.Clear();

		return *this;
	}

public:

	FORCEINLINE TCHAR& operator[](uint32 Index) { return str[Index]; }
	FORCEINLINE TCHAR operator[](uint32 Index) const { return str[Index]; }

public:

	FORCEINLINE const TCHAR* begin() const noexcept { return str; }
	FORCEINLINE TCHAR* begin() noexcept { return str; }
	FORCEINLINE const TCHAR* end() const noexcept { return str + StringLength; }
	FORCEINLINE TCHAR* end() noexcept { return str + StringLength; }

public:

	/**
		@return char at given position in the buffer by pointer.
	*/
	FORCEINLINE TCHAR* GetAtByPointer(uint32 Index) const { return &str[Index]; }

	/**
		Resize string by new length and null terminate it.
	*/
	FORCEINLINE void Resize(uint32 NewStringLength) noexcept
	{
		if( StringLength == NewStringLength ) return;

		StringLength = NewStringLength;
		Reserve(StringLength * 2);

		str[StringLength] = '\0';
	}
	/**
		Allocate memory for buffer.
	*/
	FORCEINLINE void Reserve(uint32 NewBufferSize) noexcept
	{
		if( NewBufferSize <= BufferSize ) return;
		
		BufferSize = NewBufferSize;

		if( str != nullptr )
		{
			str = static_cast<TCHAR*>(FMemory::Realloc(str, BufferSize + 1));
		}
		else
		{
			str = static_cast<TCHAR*>(FMemory::Malloc(BufferSize + 1));
		}
	}
	/**
		Manual set by buffer.
	*/
	FORCEINLINE void SetBuffer(TCHAR* NewBuffer) noexcept
	{
		StringLength = BufferSize = FPlatformString::Strlen(NewBuffer);

		if( str != nullptr )
		{
			FMemory::Free(str);
		}

		str = NewBuffer;
	}
	FORCEINLINE void SetBuffer(TCHAR* NewBuffer, uint32 Size) noexcept
	{
		StringLength = BufferSize = Size;

		if( str != nullptr )
		{
			FMemory::Free(str);
		}

		str = NewBuffer;
	}
	/**
		It will delete stored string.
	*/
	FORCEINLINE void Reset() noexcept
	{
		if( str != nullptr )
		{
			FMemory::Free(str);
		}

		Clear();
	}
	/**
		It will not delete stored string.
	*/
	FORCEINLINE void Clear() noexcept
	{
		str = nullptr;
		StringLength = 0;
		BufferSize = 0;
	}

public:

	FORCEINLINE TCHAR* const GetBuffer() const noexcept { return str; }
	FORCEINLINE uint32 GetLength() const noexcept { return StringLength; }
	FORCEINLINE uint32 GetBufferSize() const noexcept { return BufferSize; }



private:

	/**
		String buffer.
	*/
	TCHAR* str = nullptr;
	/**
		Current string length.
	*/
	uint32 StringLength = 0;
	/**
		String buffer capacity.
	*/
	uint32 BufferSize = 0;
};