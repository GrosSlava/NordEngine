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
	FORCEINLINE ~FStringBuffer() { Reset(); }


public:

	FORCEINLINE TCHAR& operator[](uint32 Index) { return str[Index]; }
	FORCEINLINE TCHAR operator[](uint32 Index) const { return str[Index]; }

public:

	FORCEINLINE TCHAR* begin() const noexcept { return str; }
	FORCEINLINE TCHAR* end() const { return &str[StringLength]; }

public:

	/**
		@return char at given position in the buffer by pointer.
	*/
	FORCEINLINE TCHAR* GetAtByPointer(uint32 Index) const { return &str[Index]; }

	/**
		Resize string by new length and null terminate it.
	*/
	void Resize(uint32 NewStringLength)
	{
		if( StringLength == NewStringLength ) return;

		StringLength = NewStringLength;
		if( StringLength > BufferSize )
		{
			BufferSize = StringLength * 2;

			if( str != nullptr )
			{
				str = static_cast<TCHAR*>(FMemory::Realloc(str, BufferSize + 1));
			}
			else
			{
				str = static_cast<TCHAR*>(FMemory::Malloc(BufferSize + 1));
			}
		}

		str[StringLength] = '\0';
	}
	/**
		Manual set by buffer.
	*/
	FORCEINLINE void SetBuffer(TCHAR* NewBuffer)
	{
		StringLength = BufferSize = FPlatformString::Strlen(NewBuffer);

		if( str != nullptr )
		{
			FMemory::Free(str);
		}

		str = NewBuffer;
	}

	/**
		It will delete stored string.
	*/
	FORCEINLINE void Reset()
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
	FORCEINLINE void Clear()
	{
		str = nullptr;
		StringLength = 0;
		BufferSize = 0;
	}

public:

	FORCEINLINE TCHAR* GetBuffer() const noexcept { return str; }
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
	int32 StringLength = 0;
	/**
		String buffer capacity.
	*/
	int32 BufferSize = 0;
};