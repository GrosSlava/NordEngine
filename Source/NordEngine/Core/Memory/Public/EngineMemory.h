// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#include <string.h>




/**
	Wrapper around standard memory operations.
*/
struct ENGINE_API FMemory
{
	static FORCEINLINE void* MemMove(void* Dest, const void* Src, SIZE_T Count) { return memmove(Dest, Src, Count); }
	static FORCEINLINE int32 MemCmp(const void* Buf1, const void* Buf2, SIZE_T Count) { return memcmp(Buf1, Buf2, Count); }
	static FORCEINLINE void* MemSet(void* Dest, uint8 Char, SIZE_T Count) { return memset(Dest, Char, Count); }
	static FORCEINLINE void* MemZero(void* Dest, SIZE_T Count) { return memset(Dest, 0, Count); }
	static FORCEINLINE void* MemCpy(void* Dest, const void* Src, SIZE_T Count) { return memcpy(Dest, Src, Count); }

	static FORCEINLINE void* Malloc(SIZE_T Size) { return malloc(Size); }
	static FORCEINLINE void* Realloc(void* Ptr, SIZE_T NewSize) { return realloc(Ptr, NewSize); }
	static FORCEINLINE void Free(void* Ptr) { free(Ptr); }

	static FORCEINLINE void Write8(void* P, uint8 Data) { *static_cast<uint8*>(P) = Data; }
	static FORCEINLINE uint8 Read8(const void* P) { return *static_cast<const uint8*>(P); }

	static FORCEINLINE void Write16(void* P, uint16 Data) { *static_cast<uint16*>(P) = Data; }
	static FORCEINLINE uint16 Read16(const void* P) { return *static_cast<const uint16*>(P); }

	static FORCEINLINE void Write32(void* P, uint32 Data) { *static_cast<uint32*>(P) = Data; }
	static FORCEINLINE uint32 Read32(const void* P) { return *static_cast<const uint32*>(P); }

	static FORCEINLINE void Write64(void* P, uint64 Data) { *static_cast<uint64*>(P) = Data; }
	static FORCEINLINE uint64 Read64(const void* P) { return *static_cast<const uint64*>(P); }

	static FORCEINLINE void ReadN(void* Dest, const void* Src, SIZE_T Bytes)
	{
		const uint8* s = static_cast<const uint8*>(Src);
		uint8* d = static_cast<uint8*>(Dest);
		while( Bytes-- )
		{
			*d++ = *s++;
		}
	}
};
