// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "EngineMemory.h"
#include "MemoryOps.h"
#include "AssertionMacros.h"
#include "InitializerList.h"
#include "MoveSemantic.h"
#include "TypeHash.h"




template<typename K>
struct TSetEntry
{
	K Key;
	uint64 Hash;
};

template<typename K>
struct ENGINE_API TSetBucket
{
public:

	TSetBucket() = default;
	~TSetBucket() = default;


public:

	TSetEntry<K>* begin() noexcept { return Data; }
	const TSetEntry<K>* begin() const noexcept { return Data; }
	TSetEntry<K>* end() noexcept { return Data + Size; }
	const TSetEntry<K>* end() const noexcept { return Data + Size; }

public:

	FORCEINLINE bool IsValid() const noexcept { return Data != nullptr; }
	FORCEINLINE bool IsEmpty() const noexcept { return Size == 0; }
	FORCEINLINE uint32 GetSize() const noexcept { return Size; }
	FORCEINLINE const TSetEntry<K>* GetData() const noexcept { return Data; }
	FORCEINLINE TSetEntry<K>* GetData() noexcept { return Data; }

public:

	FORCEINLINE bool Insert(K&& Key, uint64 Hash)
	{
		TSetEntry<K>* TergetEntry = begin();
		TSetEntry<K>* EntryEnd = end();
		while( TergetEntry != EntryEnd )
		{
			if( TergetEntry->Hash == Hash ) break;
			++TergetEntry;
		}

		if( TergetEntry == EntryEnd )
		{
			++Size;
			Reserve(Size);

			TergetEntry = end() - 1;

			TergetEntry->Key = MoveTemp(Key);
			TergetEntry->Hash = Hash;

			return true;
		}
		else
		{
			TergetEntry->Key = MoveTemp(Key);
			TergetEntry->Hash = Hash;

			return false;
		}
	}

	FORCEINLINE TSetEntry<K>* Find(uint64 Hash) noexcept
	{
		const TSetEntry<K>* Entry = begin();
		const TSetEntry<K>* EntryEnd = end();
		while( Entry != EntryEnd )
		{
			if( Entry->Hash == Hash ) return Entry;
			++Entry;
		}

		return nullptr;
	}
	FORCEINLINE const TSetEntry<K>* Find(uint64 Hash) const noexcept
	{
		TSetEntry<K>* Entry = begin();
		TSetEntry<K>* EntryEnd = end();
		while( Entry != EntryEnd )
		{
			if( Entry->Hash == Hash ) return Entry;
			++Entry;
		}

		return nullptr;
	}

	FORCEINLINE bool Contains(uint64 Hash) const noexcept { return Find(Hash) != nullptr; }

	FORCEINLINE void Remove(uint64 Hash)
	{
		TSetEntry<K>* Entry = begin();
		TSetEntry<K>* EntryEnd = end();
		while( Entry != EntryEnd )
		{
			if( Entry->Hash == Hash ) break;
			++Entry;
		}
		if( Entry == EntryEnd ) return;

		DestructItem(Entry->Key);

		--Size;
		FMemory::MemCpy(Entry, Entry + 1, sizeof(TSetEntry<K>) * (end() - Entry));
	}

	FORCEINLINE void Clear()
	{
		Size = 0;
		Capacity = 0;
		if( Data )
		{
			FMemory::Free(Data);
			Data = nullptr;
		}
	}

	FORCEINLINE void ClearWithDestructors()
	{
		DestructItems(Data, Size);
		Clear();
	}

	FORCEINLINE void Reserve(uint32 NewCapacity)
	{
		if( NewCapacity <= Capacity ) return;

		Capacity = NewCapacity * 2;
		if( Data )
		{
			Data = static_cast<TSetEntry<K>*>(FMemory::Realloc(Data, sizeof(TSetEntry<K>) * Capacity));
		}
		else
		{
			Data = static_cast<TSetEntry<K>*>(FMemory::Malloc(sizeof(TSetEntry<K>) * Capacity));
		}
	}



private:

	uint32 Size = 0;
	uint32 Capacity = 0;
	TSetEntry<K>* Data = nullptr;
};


/**
	Engine version of std::unordered_set.
	It can be faster because it does not use exceptions and can be optimized for specific operations.

	@see GetTypeHash.
*/
template<typename K>
struct ENGINE_API TSet
{
private:

	static const uint64 InitialSize = 17;

public:

	TSet() = default;
	FORCEINLINE TSet(const TSet& Other) : Size(Other.Size), FillCount(Other.FillCount)
	{
		Data = static_cast<TSetBucket<K>*>(malloc(sizeof(TSetBucket<K>) * Size));

		TSetBucket<K>* MyData = Data;
		TSetBucket<K>* MyDataEnd = Data + Size;
		TSetBucket<K>* OtherData = Other.Data;
		while( MyData != MyDataEnd )
		{
			MyData->Reserve(OtherData->GetSize());
			FMemory::MemCpy(MyData->GetData(), OtherData->GetData(), sizeof(TSetEntry<K>) * OtherData->GetSize());
			++MyData;
			++OtherData;
		}
	}
	FORCEINLINE TSet(TSet&& Other) noexcept : Data(Other.Data), Size(Other.Size), FillCount(Other.FillCount)
	{
		Other.Data = nullptr;
		Other.Size = 0;
		Other.FillCount = 0;
	}
	FORCEINLINE TSet(TInitializerList<K> InitList)
	{
		Resize(InitialSize * (InitList.Size() / InitialSize + 1));
		for( const K& Key : InitList )
		{
			Insert(Key);
		}
	}
	~TSet() { Reset(); }


public:

	FORCEINLINE TSet& operator=(const TSet& Other) = delete;
	FORCEINLINE TSet& operator=(TSet&& Other) noexcept
	{
		Reset();

		this->Data = Other.Data;
		this->Size = Other.Size;
		this->FillCount = Other.FillCount;

		Other.Data = nullptr;
		Other.Size = 0;
		Other.FillCount = 0;

		return *this;
	}
	FORCEINLINE TSet& operator=(TInitializerList<K> InitList)
	{
		Clear();

		Resize(InitialSize * (InitList.Size() / InitialSize + 1));
		for( const K& Key : InitList )
		{
			Insert(Key);
		}

		return *this;
	}

public:

	/**
		Check that set is empty.
	*/
	FORCEINLINE bool IsEmpty() const noexcept { return Size == 0; }
	/**
		@return count of set buckets.
		@note Size is not equal to real elements count in set.
	*/
	FORCEINLINE uint32 GetSize() const noexcept { return Size; }

public:

	/**
		Insert Key.
		If Key already in set then will override old data.

		@param Key - The key to insert.
	*/
	FORCEINLINE void Insert(K Key)
	{
		if( IsEmpty() )
		{
			Resize(InitialSize);
		}
		else if( FillCount * 2 > Size )
		{
			Resize(Size * 2);
		}

		const uint64 Hash = GetTypeHash(Key);
		return InsertHash(Hash, MoveTemp(Key));
	}

	/**
		Try to find key by key.
		If key not in map will generate assert error.
		@see Contains.

		@return reference to key.
	*/
	FORCEINLINE K& FindRef(const K& Key) noexcept
	{
		const uint64 Hash = GetTypeHash(Key);
		return FindKeyRefHash(Hash);
	}

	/**
		Check that Key is in map.
	*/
	FORCEINLINE bool Contains(const K& Key) const noexcept
	{
		if( IsEmpty() ) return false;
		const uint64 Hash = GetTypeHash(Key);
		return ContainsHash(Hash);
	}

	/**
		Try to remove Key from the map.
		If Key not in map then will have no effect. 
	*/
	FORCEINLINE void Remove(const K& Key)
	{
		if( IsEmpty() ) return;
		const uint64 Hash = GetTypeHash(Key);
		RemoveHash(Hash);
	}

	/**
		Empty map without physical resizing to 0.
	*/
	FORCEINLINE void Clear()
	{
		for( uint32 i = 0; i < Size; ++i )
		{
			Data[i].ClearWithDestructors();
		}
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
		Size = 0;
	}

private:

	FORCEINLINE void InsertHash(uint64 Hash, K&& Key)
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		FillCount += Data[BucketIndex].Insert(MoveTemp(Key), Hash);
	}
	FORCEINLINE V& FindKeyRefHash(uint64 Hash) noexcept
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		TSetEntry<K>* Entry = Data[BucketIndex].Find(Hash);
		check(Entry);
		return Entry->Key;
	}
	FORCEINLINE bool ContainsHash(uint64 Hash) const noexcept
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		return Data[BucketIndex].Contains(Hash);
	}
	FORCEINLINE void RemoveHash(uint64 Hash)
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		TSetBucket<K>& Bucket = Data[BucketIndex];
		if( !Bucket.IsEmpty() )
		{
			Bucket.Remove(Hash);
			FillCount -= Bucket.IsEmpty();
		}
	}

	FORCEINLINE uint64 GetBucketIndex(uint64 Hash) const noexcept
	{
		check(Size != 0);
		const uint64 BucketIndex = Hash % Size;
	}

	void Resize(uint32 NewSize)
	{
		if( NewSize <= Size ) return;

		TSetBucket<K>* NewData = static_cast<TSetBucket<K>*>(FMemory::Malloc(sizeof(TSetBucket<K>) * NewSize));
		DefaultConstructItems(NewData, NewSize);

		FillCount = 0;
		if( Data )
		{
			for( uint32 i = 0; i < Size; ++i )
			{
				for( TSetEntry<K>& Entry : Data[i] )
				{
					const uint64 BucketIndex = GetBucketIndex(Entry.Hash);
					FillCount += NewData[BucketIndex].Insert(MoveTemp(Entry.Key), Entry.Hash);
				}

				Data[i].Clear();
			}
			FMemory::Free(Data);
		}
		Size = NewSize;
		Data = NewData;
	}



private:

	/**
		Set buckets.
	*/
	TSetBucket<K>* Data = nullptr;
	/**
		Count of buckets.
	*/
	uint32 Size = 0;
	/**
		Count of non-empty buckets.
	*/
	uint32 FillCount = 0;
};