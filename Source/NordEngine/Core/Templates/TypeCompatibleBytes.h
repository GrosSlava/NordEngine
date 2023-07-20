// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"




/**
	Used to declare an untyped array of data with compile-time alignment.
	It needs to use template specialization as the MS_ALIGN and GCC_ALIGN macros require literal parameters.
*/
template<int32 Size, uint32 Alignment>
struct TAlignedBytes; // this intentionally won't compile, we don't support the requested alignment

/**
	Unaligned storage. 
*/
template<int32 Size>
struct TAlignedBytes<Size, 1>
{
	uint8 Pad[Size];
};

/**
	A macro that implements TAlignedBytes for a specific alignment.
*/
#define IMPLEMENT_ALIGNED_STORAGE(Align) \
	template<int32 Size>                 \
	struct TAlignedBytes<Size, Align>    \
	{                                    \
		struct MS_ALIGN(Align) TPadding  \
		{                                \
			uint8 Pad[Size];             \
		};                               \
	};

// Implement TAlignedBytes for these alignments.
IMPLEMENT_ALIGNED_STORAGE(16);
IMPLEMENT_ALIGNED_STORAGE(8);
IMPLEMENT_ALIGNED_STORAGE(4);
IMPLEMENT_ALIGNED_STORAGE(2);

#undef IMPLEMENT_ALIGNED_STORAGE




/**
	An untyped array of data with compile-time alignment and size derived from another type.
*/
template<typename ElementType>
struct TTypeCompatibleBytes : public TAlignedBytes<sizeof(ElementType), alignof(ElementType)>
{
	FORCEINLINE ElementType* GetTypedPtr() { return (ElementType*)this; }
	FORCEINLINE const ElementType* GetTypedPtr() const { return (const ElementType*)this; }
};
