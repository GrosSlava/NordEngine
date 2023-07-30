// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "EngineMemory.h"
#include "MemoryOps.h"
#include "AssertionMacros.h"
#include "InitializerList.h"
#include "MoveSemantic.h"

#include <algorithm>




/**
	Engine version of std::vector.
	It can be faster because it does not use exceptions and can be optimized for specific operations.
*/
template<typename T>
struct ENGINE_API TArray
{
public:

	TArray() = default;
	FORCEINLINE TArray(uint32 InitSize) : Size(InitSize), Capacity(InitSize) { Data = static_cast<T*>(FMemory::Malloc(sizeof(T) * InitSize)); }
	FORCEINLINE TArray(const TArray& Other) : Size(Other.Size), Capacity(Other.Size)
	{
		Data = static_cast<T*>(FMemory::Malloc(sizeof(T) * Size));
		CopyAssignItems(Data, Other.Data, Other.Size);
	}
	FORCEINLINE TArray(TArray&& Other) noexcept : Data(Other.Data), Size(Other.Size), Capacity(Other.Size)
	{
		Other.Data = nullptr;
		Other.Size = 0;
		Other.Capacity = 0;
	}
	FORCEINLINE TArray(TInitializerList<T> InitList) : Size(InitList.Size()), Capacity(InitList.Size())
	{
		Data = static_cast<T*>(FMemory::Malloc(sizeof(T) * Size));
		CopyAssignItems(Data, InitList.begin(), Size);
	}
	~TArray() { Reset(); }


public:

	FORCEINLINE TArray& operator=(const TArray& Other)
	{
		if( &Other == this ) return *this;

		DestructItems(Data, this->Size);

		Reserve(Other.Size);
		CopyAssignItems(Data, Other.Data, Other.Size);

		return *this;
	}
	FORCEINLINE TArray& operator=(TArray&& Other) noexcept
	{
		DestructItems(Data, this->Size);
		FMemory::Free(Data);

		this->Data = Other.Data;
		this->Size = Other.Size;
		this->Capacity = Other.Capacity;

		Other.Data = nullptr;
		Other.Size = 0;
		Other.Capacity = 0;

		return *this;
	}
	FORCEINLINE TArray& operator=(TInitializerList<T> InitList)
	{
		DestructItems(Data, this->Size);

		Reserve(InitList.Size());
		CopyAssignItems(Data, InitList.begin(), Other.Size);

		return *this;
	}

public:

	FORCEINLINE T& operator[](uint32 Index) { return At(Index); }
	FORCEINLINE const T& operator[](uint32 Index) const { return At(Index); }

public:

	FORCEINLINE T* begin() noexcept { return Data; }
	FORCEINLINE const T* begin() const noexcept { return Data; }
	FORCEINLINE T* end() noexcept { return Data + Size; }
	FORCEINLINE const T* end() const noexcept { return Data + Size; }

public:

	/**
		Allocate memory for give count of elements.
	*/
	FORCEINLINE void Reserve(uint32 NewSize)
	{
		if( NewSize > Capacity )
		{
			Capacity = NewSize * 2; // do not reduce the growth factor

			if( Data != nullptr )
			{
				Data = static_cast<T*>(FMemory::Realloc(Data, sizeof(T) * Capacity));
			}
			else
			{
				Data = static_cast<T*>(FMemory::Malloc(sizeof(T) * Capacity));
			}
		}
	}
	/**
		Requests the removal of unused capacity.
	*/
	FORCEINLINE void ShrinkToFit()
	{
		if( Size == 0 )
		{
			Reset();
		}
		else
		{
			Data = static_cast<T*>(FMemory::Realloc(Data, sizeof(T) * Size));
			Capacity = Size;
		}
	}
	/**
		Empty array without physical resizing to 0.
	*/
	FORCEINLINE void Clear()
	{
		DestructItems(Data, Size);
		Size = 0;
	}
	/**
		Physically risize to 0 size.
	*/
	FORCEINLINE void Reset()
	{
		Clear();
		if( Data )
		{
			FMemory::Free(Data);
		}
		Capacity = 0;
		Data = nullptr;
	}

public:

	/**
		Access element at index.
	*/
	FORCEINLINE T& At(uint32 Index)
	{
		check(Index < Size);
		return Data[Index];
	}
	/**
		Access element at index.
	*/
	FORCEINLINE const T& At(uint32 Index) const
	{
		check(Index < Size);
		return Data[Index];
	}
	/**
		@return First element.
	*/
	FORCEINLINE T& Front() { return At(0); }
	FORCEINLINE const T& Front() const { return At(0); }
	/**
		@return Last element.
	*/
	FORCEINLINE T& Back() { return At(Size - 1); }
	FORCEINLINE const T& Back() const { return At(Size - 1); }
	/**
		@return Array in C-style.
	*/
	FORCEINLINE T* GetData() noexcept { return Data; }
	FORCEINLINE const T* GetData() const noexcept { return Data; }
	/**
		@return Count of elements in the array.
	*/
	FORCEINLINE uint32 GetSize() const noexcept { return Size; }
	/**
		@return Count of elements in the array.
	*/
	FORCEINLINE uint32 Num() const noexcept { return Size; }
	/**
		@return Capacity of the array.
	*/
	FORCEINLINE uint32 GetCapacity() const noexcept { return Capacity; }
	/**
		Check that array is empty.
	*/
	FORCEINLINE bool IsEmpty() const noexcept { return Size == 0; }

public:

	/**
		Add Elem to the end of array.
	*/
	FORCEINLINE void PushBack(T Elem)
	{
		++Size;
		Reserve(Size);

		Back() = MoveTemp(Elem);
	}
	/**
		Add Elem to the begin of array.
	*/
	FORCEINLINE void PushFront(T Elem)
	{
		++Size;
		Reserve(Size);

		FMemory::MemMove(Data + 1, Data, sizeof(T) * (Size - 1));
		begin() = MoveTemp(Elem);
	}
	/**
		Add Elem to the end of array.
	*/
	FORCEINLINE void Add(T Elem) { PushBack(MoveTemp(Elem)); }
	/**
		Add Elem to the end of the array, unless it is already in the array.
	*/
	FORCEINLINE void AddUnique(T Elem)
	{
		if( Contains(Elem) ) return;
		Add(MoveTemp(Elem));
	}

	/**
		Remove last elem.
	*/
	FORCEINLINE void PopBack()
	{
		check(Size > 0);
		--Size;
		DestructItem(end());
	}
	/**
		Remove first elem.
	*/
	FORCEINLINE void PopFront()
	{
		check(Size > 0);
		--Size;
		DestructItem(begin());

		FMemory::MemCpy(Data, Data + 1, sizeof(T) * Size);
	}

	/**
		Add Elem at position index and shift rigth other elements.
	*/
	FORCEINLINE void Insert(uint32 Index, T Elem)
	{
		check(Index < Size);
		++Size;
		Reserve(Size);

		FMemory::MemMove(Data + Index + 1, Data + Index, sizeof(T) * (Size - Index - 1));
		At(Index) = MoveTemp(Elem);
	}

	/**
		Add Elem after stored elem at Index.
		New elem will be at position = Index + 1
	*/
	FORCEINLINE void AddAfter(uint32 Index, T Elem)
	{
		check(Index < Size);
		++Size;
		Reserve(Size);

		FMemory::MemMove(Data + Index + 2, Data + Index + 1, sizeof(T) * (Size - Index - 2));
		At(Index + 1) = MoveTemp(Elem);
	}

	/**
		Remove element at Index.

		@param Index - The index of the first element to remove.
	*/
	FORCEINLINE void RemoveAt(uint32 Index)
	{
		check(Index < Size);
		--Size;
		DestructItem(Data + Index);

		FMemory::MemCpy(Data + Index, Data + Index + 1, sizeof(T) * (Size - Index));
	}
	/**
		Remove element at Index.

		@param Index - The index of the first element to remove.
		@param Count - The number of elements to remove.
	*/
	FORCEINLINE void RemoveAt(uint32 Index, uint32 Count)
	{
		check(Index < Size);
		if( Index + Count > Size ) Count = Size - Index;
		if( Count == 0 ) return;

		Size -= Count;
		DestructItems(Data + Index, Count);

		FMemory::MemCpy(Data + Index, Data + Index + Count, sizeof(T) * (Size - Index));
	}
	/**
		Remove element at index and swapping it with last element.

		@param Index - The index of the first element to remove.
	*/
	FORCEINLINE void RemoveAtSwap(uint32 Index)
	{
		check(Index < Size);
		--Size;
		DestructItem(Data + Index);

		if( Size == 0 ) return;
		FMemory::MemCpy(Data + Index, Data + Size, sizeof(T));
	}
	/**
		Remove all elements where Lambda returns true
		e.g [&](const T& Elem){ return Elem == 0; }
	*/
	template<typename Predicate>
	FORCEINLINE void RemoveAll(Predicate NeedToRemove)
	{
		const T* p = end() - 1;
		while( p >= begin() )
		{
			if( NeedToRemove(*p) )
			{
				RemoveAt(p - begin());
			}
			--p;
		}
	}
	/**
		Remove all elements where Lambda returns true using swapping.
		e.g [&](const T& Elem){ return Elem == 0; }
	*/
	template<typename Predicate>
	FORCEINLINE void RemoveAllSwap(Predicate NeedToRemove)
	{
		const T* p = end() - 1;
		while( p >= begin() )
		{
			if( NeedToRemove(*p) )
			{
				RemoveAtSwap(p - begin());
			}
			--p;
		}
	}

	/**
		Try to find Elem in array by operator==
	*/
	FORCEINLINE bool Contains(const T& Elem) const noexcept { return Find(Elem) != -1; }
	/**
		@return index of first found Elem by operator==. If not found return -1
	*/
	FORCEINLINE uint32 Find(const T& Elem) const noexcept
	{
		const T* p = begin();
		while( p != end() )
		{
			if( *p == Elem )
			{
				return static_cast<uint32>(p - begin());
			}
			++p;
		}
		return -1;
	}

	/**
		Sort array by Predicate.
		e.g [&](const T& A, const T& B){ return A < B; }
	*/
	template<typename Predicate>
	FORCEINLINE void Sort(Predicate Compare) noexcept
	{
		std::sort(begin(), end(), Compare);
	}



private:

	/**
		Data array.
	*/
	T* Data = nullptr;
	/**
		Count of elements in the array.
	*/
	uint32 Size = 0;
	/**
		Allocated memory for elements count.
	*/
	uint32 Capacity = 0;
};