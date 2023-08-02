// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "EngineMemory.h"
#include "MemoryOps.h"
#include "AssertionMacros.h"
#include "InitializerList.h"
#include "MoveSemantic.h"
#include "KeyValuePair.h"
#include "TypeHash.h"




template<typename K, typename V>
struct TMapEntry
{
	TKeyValuePair<K, V> KeyValue;
	uint64 Hash;
};

template<typename K, typename V>
struct ENGINE_API TMapBucket
{
public:

	TMapBucket() = default;
	~TMapBucket() = default;


public:

	TMapEntry<K, V>* begin() noexcept { return Data; }
	const TMapEntry<K, V>* begin() const noexcept { return Data; }
	TMapEntry<K, V>* end() noexcept { return Data + Size; }
	const TMapEntry<K, V>* end() const noexcept { return Data + Size; }

public:

	FORCEINLINE bool IsValid() const noexcept { return Data != nullptr; }
	FORCEINLINE bool IsEmpty() const noexcept { return Size == 0; }
	FORCEINLINE uint32 GetSize() const noexcept { return Size; }
	FORCEINLINE const TMapEntry<K, V>* GetData() const noexcept { return Data; }
	FORCEINLINE TMapEntry<K, V>* GetData() noexcept { return Data; }

public:

	FORCEINLINE bool Insert(K&& Key, V&& Value, uint64 Hash)
	{
		TMapEntry<K, V>* TergetEntry = begin();
		TMapEntry<K, V>* EntryEnd = end();
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

			TergetEntry->KeyValue.Key = MoveTemp(Key);
			TergetEntry->KeyValue.Value = MoveTemp(Value);
			TergetEntry->Hash = Hash;

			return true;
		}
		else
		{
			TergetEntry->KeyValue.Key = MoveTemp(Key);
			TergetEntry->KeyValue.Value = MoveTemp(Value);
			TergetEntry->Hash = Hash;

			return false;
		}
	}

	FORCEINLINE TMapEntry<K, V>* Find(uint64 Hash) noexcept
	{
		const TMapEntry<K, V>* Entry = begin();
		const TMapEntry<K, V>* EntryEnd = end();
		while( Entry != EntryEnd )
		{
			if( Entry->Hash == Hash ) return Entry;
			++Entry;
		}

		return nullptr;
	}
	FORCEINLINE const TMapEntry<K, V>* Find(uint64 Hash) const noexcept
	{
		TMapEntry<K, V>* Entry = begin();
		TMapEntry<K, V>* EntryEnd = end();
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
		TMapEntry<K, V>* Entry = begin();
		TMapEntry<K, V>* EntryEnd = end();
		while( Entry != EntryEnd )
		{
			if( Entry->Hash == Hash ) break;
			++Entry;
		}
		if( Entry == EntryEnd ) return;

		DestructItem(Entry->KeyValue);

		--Size;
		FMemory::MemCpy(Entry, Entry + 1, sizeof(TMapEntry<K, V>) * (end() - Entry));
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
			Data = static_cast<TMapEntry<K, V>*>(FMemory::Realloc(Data, sizeof(TMapEntry<K, V>) * Capacity));
		}
		else
		{
			Data = static_cast<TMapEntry<K, V>*>(FMemory::Malloc(sizeof(TMapEntry<K, V>) * Capacity));
		}
	}



private:

	uint32 Size = 0;
	uint32 Capacity = 0;
	TMapEntry<K, V>* Data = nullptr;
};


/**
	Engine version of std::unordered_map.
	It can be faster because it does not use exceptions and can be optimized for specific operations.

	@see GetTypeHash.
*/
template<typename K, typename V>
struct ENGINE_API TMap
{
private:

	static const uint64 InitialSize = 17;

public:

	TMap() = default;
	FORCEINLINE TMap(const TMap& Other) : Size(Other.Size), FillCount(Other.FillCount)
	{
		Data = static_cast<TMapBucket<K, V>*>(malloc(sizeof(TMapBucket<K, V>) * Size));

		TMapBucket<K, V>* MyData = Data;
		TMapBucket<K, V>* MyDataEnd = Data + Size;
		TMapBucket<K, V>* OtherData = Other.Data;
		while( MyData != MyDataEnd )
		{
			MyData->Reserve(OtherData->GetSize());
			FMemory::MemCpy(MyData->GetData(), OtherData->GetData(), sizeof(TMapEntry<K, V>) * OtherData->GetSize());
			++MyData;
			++OtherData;
		}
	}
	FORCEINLINE TMap(TMap&& Other) noexcept : Data(Other.Data), Size(Other.Size), FillCount(Other.FillCount)
	{
		Other.Data = nullptr;
		Other.Size = 0;
		Other.FillCount = 0;
	}
	FORCEINLINE TMap(TInitializerList<TKeyValuePair<K, V>> InitList)
	{
		Resize(InitialSize * (InitList.Size() / InitialSize + 1));
		for( const TKeyValuePair<K, V>& Pair : InitList )
		{
			Insert(Pair);
		}
	}
	~TMap() { Reset(); }


public:

	FORCEINLINE TMap& operator=(const TMap& Other) = delete;
	FORCEINLINE TMap& operator=(TMap&& Other) noexcept
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
	FORCEINLINE TMap& operator=(TInitializerList<TKeyValuePair<K, V>> InitList)
	{
		Clear();

		Resize(InitialSize * (InitList.Size() / InitialSize + 1));
		for( const TKeyValuePair<K, V>& Pair : InitList )
		{
			Insert(Pair);
		}

		return *this;
	}

public:

	FORCEINLINE V& operator[](const K& Key)
	{
		const uint64 Hash = GetTypeHash(Key);

		if( IsEmpty() )
		{
			InsertHash(Hash, V());
			return FindValueRefHash(Hash);
		}
		else
		{
			TMapEntry<K, V>* Entry = Data[GetBucketIndex(Hash)].Find(Hash);
			if( Entry == nullptr )
			{
				InsertHash(Hash, V());
				return FindValueRefHash(Hash);
			}
			else
			{
				return Entry->KeyValue.Value;
			}
		}
	}

public:

	/**
		Check that map is empty.
	*/
	FORCEINLINE bool IsEmpty() const noexcept { return Size == 0; }
	/**
		@return count of map buckets.
		@note Size is not equal to real elements count in map.
	*/
	FORCEINLINE uint32 GetSize() const noexcept { return Size; }

public:

	/**
		Insert Value under Key.
		If Key already in map then will override old data.

		@param Key - The key to insert.
		@param Value - THe value to insert.
	*/
	FORCEINLINE void Insert(K Key, V Value)
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
		return InsertHash(Hash, MoveTemp(Key), MoveTemp(Value));
	}
	/**
		Insert Value under Key.
		If Key already in map then will override old data.
	*/
	FORCEINLINE void Insert(const TKeyValuePair<K, V>& Pair) { Insert(Pair.Key, Pair.Value); }

	/**
		Try to find value by key.
		If key not in map will generate assert error.
		@see Contains.

		@return copy of value.
	*/
	FORCEINLINE V Find(const K& Key) const noexcept
	{
		const uint64 Hash = GetTypeHash(Key);
		return FindValueHash(Hash);
	}
	/**
		Try to find value by key.
		If key not in map will generate assert error.
		@see Contains.

		@return reference to value.
	*/
	FORCEINLINE V& FindRef(const K& Key) noexcept
	{
		const uint64 Hash = GetTypeHash(Key);
		return FindValueRefHash(Hash);
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

	FORCEINLINE void InsertHash(uint64 Hash, K&& Key, V&& Value)
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		FillCount += Data[BucketIndex].Insert(MoveTemp(Key), MoveTemp(Value), Hash);
	}
	FORCEINLINE TKeyValuePair<K, V> FindHash(uint64 Hash) const noexcept
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		TMapEntry<K, V>* Entry = Data[BucketIndex].Find(Hash);
		check(Entry);
		return Entry->KeyValue;
	}
	FORCEINLINE V FindValueHash(uint64 Hash) const noexcept
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		TMapEntry<K, V>* Entry = Data[BucketIndex].Find(Hash);
		check(Entry);
		return Entry->KeyValue.Value;
	}
	FORCEINLINE V& FindValueRefHash(uint64 Hash) noexcept
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		TMapEntry<K, V>* Entry = Data[BucketIndex].Find(Hash);
		check(Entry);
		return Entry->KeyValue.Value;
	}
	FORCEINLINE bool ContainsHash(uint64 Hash) const noexcept
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		return Data[BucketIndex].Contains(Hash);
	}
	FORCEINLINE void RemoveHash(uint64 Hash)
	{
		const uint64 BucketIndex = GetBucketIndex(Hash);

		TMapBucket<K, V>& Bucket = Data[BucketIndex];
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

		TMapBucket<K, V>* NewData = static_cast<TMapBucket<K, V>*>(FMemory::Malloc(sizeof(TMapBucket<K, V>) * NewSize));
		DefaultConstructItems(NewData, NewSize);

		FillCount = 0;
		if( Data )
		{
			for( uint32 i = 0; i < Size; ++i )
			{
				for( TMapEntry<K, V>& Entry : Data[i] )
				{
					const uint64 BucketIndex = GetBucketIndex(Entry.Hash);
					FillCount += NewData[BucketIndex].Insert(MoveTemp(Entry.Key), MoveTemp(Entry.Value), Entry.Hash);
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
		Map buckets.
	*/
	TMapBucket<K, V>* Data = nullptr;
	/**
		Count of buckets.
	*/
	uint32 Size = 0;
	/**
		Count of non-empty buckets.
	*/
	uint32 FillCount = 0;
};