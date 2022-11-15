
#pragma once

#include "GenericPlatform.h"

#include "TypeTraits/TypeSpecifier.h"

#include <string.h>




/*
	Wrapper around standard memory operations.
*/
struct ENGINE_API FMemory
{
	static FORCEINLINE void* Memmove(void* Dest, const void* Src, size_t Count)
	{
		return memmove(Dest, Src, Count);
	}

	static FORCEINLINE int32 Memcmp(const void* Buf1, const void* Buf2, size_t Count)
	{
		return memcmp(Buf1, Buf2, Count);
	}

	static FORCEINLINE void* Memset(void* Dest, uint8 Char, size_t Count)
	{
		return memset(Dest, Char, Count);
	}

	static FORCEINLINE void* Memzero(void* Dest, size_t Count)
	{
		return memset(Dest, 0, Count);
	}

	template<class T>
	static FORCEINLINE void Memzero(T& Src)
	{
		static_assert(!TIsPointer<T>::Value, "For pointers use the two parameters function");
		Memzero(&Src, sizeof(T));
	}

	static FORCEINLINE void* Memcpy(void* Dest, const void* Src, size_t Count)
	{
		return memcpy(Dest, Src, Count);
	}

	template<class T>
	static FORCEINLINE void Memcpy(T& Dest, const T& Src)
	{
		static_assert(!TIsPointer<T>::Value, "For pointers use the three parameters function");
		Memcpy(&Dest, &Src, sizeof(T));
	}

	static FORCEINLINE void* Realloc(void* ptr, size_t NewSize)
	{
		return realloc(ptr, NewSize);
	}

	static FORCEINLINE void Free(void* ptr)
	{
		free(ptr);
	}



	/*...................................memory mapped i/o functions........................................*/

	static FORCEINLINE void MMIOWrite8(void* P, uint8 Data)
	{
		*(volatile uint8*)(P) = Data;
	}

	static FORCEINLINE uint8 MMIORead8(const void* P)
	{
		return *(volatile uint8*)(P);
	}

	static FORCEINLINE void MMIOWrite16(void* P, uint16 Data)
	{
		*(volatile uint16*)(P) = Data;
	}

	static FORCEINLINE uint16 MMIORead16(const void* P)
	{
		return *(volatile uint16*)(P);
	}

	static FORCEINLINE void MMIOWrite32(void* P, uint32 Data)
	{
		*(volatile uint32*)(P) = Data;
	}

	static FORCEINLINE uint32 MMIORead32(const void* P)
	{
		return *(volatile uint32*)(P);
	}

	static FORCEINLINE void MMIOWrite64(void* P, uint64 Data)
	{
		*(volatile uint64*)(P) = Data;
	}

	static FORCEINLINE uint64 MMIORead64(const void* P)
	{
		return *(volatile uint64*)(P);
	}

	static FORCEINLINE void MMIOReadN(void* Dest, const volatile void* Src, size_t Bytes)
	{
		volatile uint8* s = (volatile uint8*)Src;
		uint8* d = (uint8*)Dest;
		while( Bytes-- )
		{
			*d++ = *s++;
		}	
	}

	/*......................................................................................................*/
};
