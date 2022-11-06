#pragma once

#include "GenericPlatform.h"

#include "AssertionMacros.h"
#include <forward_list> //TODO replace by engine container
#include <thread>




namespace GarbageCollector_Private
{
	/*
		Helper struct to handle object references.
	*/
	struct FAddrHandler
	{
	public:

		FORCEINLINE FAddrHandler() noexcept { }
		FORCEINLINE FAddrHandler(SIZE_T InAddr, uint32 InCount) noexcept : Addr(InAddr), Count(InCount) { }
		FORCEINLINE FAddrHandler(SIZE_T InAddr) noexcept : Addr(InAddr) { }
		FORCEINLINE FAddrHandler(const void* Ptr, uint32 InCount) noexcept : Addr((SIZE_T)Ptr), Count(InCount) { }
		FORCEINLINE FAddrHandler(const void* Ptr) noexcept : Addr((SIZE_T)Ptr) { }


	public:

		/*
			Check that this handler represent any object.
		*/
		FORCEINLINE bool IsValid() const noexcept { return Addr != 0; }
		/*
			Check that this object is a candidate to killing by any Garbage Collector.
			@return true if this handler has object, but it has no references.
		*/
		FORCEINLINE bool IsPendingToKill() const noexcept { return Count == 0 && Addr != 0; }
		/*
			Check that we have only one ref.
		*/
		FORCEINLINE bool IsUnique() const noexcept { return Count == 1; }

	public:

		/*
			Object's memory address in integral type.
		*/
		SIZE_T Addr = 0;
		/*
			Number of references to an object in the program.
		*/
		uint32 Count = 0;
	};

	FORCEINLINE bool operator==(const FAddrHandler& Lhs, const FAddrHandler& Rhs) noexcept
	{
		return Lhs.Addr == Rhs.Addr;
	}
	FORCEINLINE bool operator!=(const FAddrHandler& Lhs, const FAddrHandler& Rhs) noexcept
	{
		return Lhs.Addr != Rhs.Addr;
	}
	FORCEINLINE bool operator>(const FAddrHandler& Lhs, const FAddrHandler& Rhs) noexcept
	{
		return Lhs.Addr > Rhs.Addr;
	}
	FORCEINLINE bool operator>=(const FAddrHandler& Lhs, const FAddrHandler& Rhs) noexcept
	{
		return Lhs.Addr >= Rhs.Addr;
	}
	FORCEINLINE bool operator<(const FAddrHandler& Lhs, const FAddrHandler& Rhs) noexcept
	{
		return Lhs.Addr < Rhs.Addr;
	}
	FORCEINLINE bool operator<=(const FAddrHandler& Lhs, const FAddrHandler& Rhs) noexcept
	{
		return Lhs.Addr <= Rhs.Addr;
	}



	/*
		Large bool mask for memory optimization in GC.
		Each bit is 0/1 mask.

		@param T - Container POD-type of the mask elements.
		@param BitsPackSize - mask size.
	*/
	template<typename T, uint32 BitsPackSize>
	struct TGarbageBoolMask
	{
	public:

		FORCEINLINE TGarbageBoolMask()
		{
			BoolMask = new T[BitsPackSize / MaskSize];
			Clear();
		}

		~TGarbageBoolMask()
		{
			if( BoolMask != nullptr ) delete[] BoolMask;
		}


	public:

		/*
			Set mask bit to 1.
			@return bit set success.
		*/
		FORCEINLINE bool SetMaskBit(uint32 Index) noexcept
		{
			const uint32 LMaskIndex = Index / MaskSize;

			// 1 << (Index % 8) - for char(1 byte)
			const T LBitIndexMask = (T)1 << (Index & PackTypeRemainMask);

			//BoolMask[Index / 8] & (1 << (Index % 8)) - for char(1 byte)
			const bool IsSet = BoolMask[LMaskIndex] & LBitIndexMask;
			if( IsSet ) return false;

			//BoolMask[Index / 8] |= (1 << (Index % 8)) - for char(1 byte)
			BoolMask[LMaskIndex] |= LBitIndexMask;

			return true;
		}

		/*
			Set mask bit to 0.
		*/
		FORCEINLINE void ClearMaskBit(uint32 Index) noexcept
		{
			//BoolMask[Index / 8] &= ~(1 << (Index % 8)) - for char(1 byte)
			BoolMask[Index / MaskSize] &= ~((T)1 << (Index & PackTypeRemainMask));
		}

		/*
			Set mask to 0.
		*/
		FORCEINLINE void Clear()
		{
			for( T* LMaskElem = BoolMask + BitsPackSize / MaskSize - 1; LMaskElem >= BoolMask; --LMaskElem )
			{
				*LMaskElem = 0;
			}
		}


	private:

		const static uint32 MaskSize = sizeof(T) * 8;
		const static uint32 PackTypeRemainMask = sizeof(T) * 8 - 1;

		T* BoolMask = nullptr;
	};



	/*
		Helper struct to manage of garbage lists to be cleared.
	*/
	struct FGarbageListsToClearArray
	{
	public:

		FGarbageListsToClearArray() = delete;
		FORCEINLINE FGarbageListsToClearArray(uint32 ListsCount) : 
			MaxListsCount(ListsCount) 
		{ 
			IndexesToClean = new uint32[ListsCount];
		}

		~FGarbageListsToClearArray()
		{
			if( IndexesToClean != nullptr ) delete[] IndexesToClean;
		}

	public:

		FORCEINLINE uint32& operator[](uint32 Index)
		{
			check(Index >= 0 && Index < CountOfListsToClean)

			return IndexesToClean[Index];
		}


	public:

		/*
			Add new list index to array.
		*/
		FORCEINLINE void Push(uint32 Index)
		{
			check(Index >= 0 && Index < MaxListsCount)

			IndexesToClean[CountOfListsToClean] = Index;
			++CountOfListsToClean;
		}

		/*
			@return count of lists to clear.
		*/
		FORCEINLINE uint32 Num() const noexcept { return CountOfListsToClean; }

		/*
			Remove all lists from array.
		*/
		FORCEINLINE void Clear() noexcept { CountOfListsToClean = 0; }

		/*
			Check if any list has been pushed.
		*/
		FORCEINLINE bool IsEmpty() const noexcept { return CountOfListsToClean == 0; }



	private:

		/*
			Array of list indexes.
		*/
		uint32* IndexesToClean = nullptr;
		/*
			Count of pushed lists.
		*/
		uint32 CountOfListsToClean = 0;
		/*
			Max count of lists indexes to contain.
		*/
		uint32 MaxListsCount = 0;
	};


	/*
		Helper struct for manage AddrHandlers.
		@see FAddrHandler.
	*/
	struct FAnyPtrMap
	{
	public:

		FORCEINLINE FAnyPtrMap() : 
			ListsIndexesToClean(BitsPackSize)
		{
			ItemMap = new std::forward_list<FAddrHandler>[BitsPackSize];
		}

		~FAnyPtrMap()
		{
			if( ItemMap != nullptr ) delete[] ItemMap;
		}



	public:

		FAddrHandler* AddPtr(const void* Ptr)
		{
			check(Ptr != nullptr)

			const SIZE_T LAddr = (const SIZE_T)Ptr;

			std::forward_list<FAddrHandler>& LItems = ItemMap[GetLocalAddr(LAddr)];
			auto LItemsEnd = LItems.end();

			auto LAddrHandler = std::find(LItems.begin(), LItemsEnd, LAddr);
			if( LAddrHandler != LItemsEnd )
			{
				++LAddrHandler->Count;
				return &LAddrHandler._Ptr->_Myval;
			}
			else
			{
				++TotalObjectsCount;
				LItems.push_front(FAddrHandler(LAddr, 1));
				return &LItems.front();
			}
		}

		FORCEINLINE FAddrHandler* AddNewPtr(const void* Ptr)
		{
			check(Ptr != nullptr)

			const SIZE_T LAddr = (const SIZE_T)Ptr;

			std::forward_list<FAddrHandler>& LItems = ItemMap[GetLocalAddr(LAddr)];

			++TotalObjectsCount;
			LItems.push_front(FAddrHandler(LAddr, 1));
			return &LItems.front();
		}

		FAddrHandler* RemovePtr(const void* Ptr)
		{
			check(Ptr != nullptr)

			const SIZE_T LAddr = (const SIZE_T)Ptr;

			const uint32 LocalAddr = GetLocalAddr(LAddr);
			std::forward_list<FAddrHandler>& LItems = ItemMap[LocalAddr];
			auto LItemsEnd = LItems.end();

			auto LAddrHandler = std::find(LItems.begin(), LItemsEnd, LAddr);
			if( LAddrHandler == LItemsEnd || LAddrHandler->Count == 0 ) return nullptr;

			if( --LAddrHandler->Count == 0 )
			{
				if( ListsIndexesToCleanMask.SetMaskBit(LocalAddr) )
				{
					ListsIndexesToClean.Push(LocalAddr);
				}
			}

			return &LAddrHandler._Ptr->_Myval;
		}

		FORCEINLINE void RemovePtrByHandler(FAddrHandler* PtrHandler)
		{
			if( PtrHandler == nullptr || PtrHandler->Count == 0 || !PtrHandler->IsValid() ) return;

			if( --PtrHandler->Count == 0 )
			{
				const uint32 LocalAddr = GetLocalAddr(PtrHandler->Addr);
				if( ListsIndexesToCleanMask.SetMaskBit(LocalAddr) )
				{
					ListsIndexesToClean.Push(LocalAddr);
				}
			}
		}

		void ClearInvalidPtrs()
		{
			LastGarbagedObjectsCount = 0;
			if( ListsIndexesToClean.IsEmpty() ) return;

			++GarbageCollectionIndex;


			if( GarbageCleaningThreadsCount > 1 && ListsIndexesToClean.Num() > GarbageCleaningThreadsCount )
			{
				std::thread* LThreads = new std::thread[GarbageCleaningThreadsCount];
				uint32* LThreadGarabagedObjectsCount = new uint32[GarbageCleaningThreadsCount];

				const uint32 LStep = ListsIndexesToClean.Num() / GarbageCleaningThreadsCount + 1;
				uint32 L = 0;
				uint32 R = LStep;
				uint32 LThreadIndex = 0;
				for( LThreadIndex; LThreadIndex < GarbageCleaningThreadsCount; ++LThreadIndex )
				{
					if( R > ListsIndexesToClean.Num() ) R = ListsIndexesToClean.Num();
					if( L == R )
					{
						break;
					}

					LThreads[LThreadIndex] = std::thread(
						[this, L, R, LThreadIndex, LThreadGarabagedObjectsCount]()
						{
							uint32 LGarbagedObjectsCount = 0;
							for( uint32 i = L; i < R; ++i )
							{
								ItemMap[ListsIndexesToClean[i]].remove_if(
									[this, &LGarbagedObjectsCount](FAddrHandler& LAddrHandler)
									{
										if( !LAddrHandler.IsPendingToKill() ) return false;

										delete(void*)LAddrHandler.Addr;
										++LGarbagedObjectsCount;
										return true;
									});
							}

							LThreadGarabagedObjectsCount[LThreadIndex] = LGarbagedObjectsCount;
						});

					L = R;
					R += LStep;
				}

				for( uint32 i = 0; i < LThreadIndex; ++i )
				{
					LThreads[i].join();
					LastGarbagedObjectsCount += LThreadGarabagedObjectsCount[i];
				}

				ListsIndexesToCleanMask.Clear();

				delete[] LThreads;
				delete[] LThreadGarabagedObjectsCount;
			}
			else
			{
				for( uint32 i = 0; i < ListsIndexesToClean.Num(); ++i )
				{
					ItemMap[ListsIndexesToClean[i]].remove_if(
						[this](FAddrHandler& LAddrHandler)
						{
							if( !LAddrHandler.IsPendingToKill() ) return false;

							delete(void*)LAddrHandler.Addr;
							++LastGarbagedObjectsCount;
							return true;
						});

					ListsIndexesToCleanMask.ClearMaskBit(ListsIndexesToClean[i]);
				}
			}

			TotalObjectsCount -= LastGarbagedObjectsCount;
			ListsIndexesToClean.Clear();
		}



		FORCEINLINE FAddrHandler* GetPtrHandler(const void* Ptr) const noexcept
		{
			check(Ptr != nullptr)

			const SIZE_T LAddr = (const SIZE_T)Ptr;
			const std::forward_list<FAddrHandler>& LItems = ItemMap[GetLocalAddr(LAddr)];
			auto LItemsEnd = LItems.end();

			auto LAddrHandler = std::find(LItems.begin(), LItemsEnd, LAddr);
			if( LAddrHandler == LItemsEnd ) return nullptr;

			return &LAddrHandler._Ptr->_Myval;
		}

		FORCEINLINE uint32 GetTotalObjectsCount() const noexcept { return TotalObjectsCount; }

		FORCEINLINE uint32 GetLastGarbagedObjectsCount() const noexcept { return LastGarbagedObjectsCount; }

		FORCEINLINE int32 GetGarbageCollectionIndex() const noexcept { return GarbageCollectionIndex; }


		FORCEINLINE uint32 GetGarbageCleaningThreadsCount() const noexcept { return GarbageCleaningThreadsCount; }

		FORCEINLINE void SetGarbageCleaningThreadsCount(uint32 NewThreadCount) noexcept { GarbageCleaningThreadsCount = NewThreadCount; }

	private:

		FORCEINLINE uint32 GetLocalAddr(SIZE_T Addr) const noexcept
		{
			return Addr & BitsPackMask; // Addr % BitsPackSize
		}



	public:

		const static uint32 BitsPackSize = 1048576; //1 MB ~69MB in ram
		const static uint32 BitsPackMask = 0xfffff;	// 20 bits 2^20 = 1048576

		//const static uint32 BitsPackSize = 65536;	//64 KB ~4MB in ram
		//const static uint32 BitsPackMask = 0xffff;	// 16 bits 2^16 = 65536

	private:

		std::forward_list<FAddrHandler>* ItemMap = nullptr;

		FGarbageListsToClearArray ListsIndexesToClean;
		TGarbageBoolMask<SIZE_T, BitsPackSize> ListsIndexesToCleanMask;


		/*
			Total objects count who are under surveillance.
		*/
		uint32 TotalObjectsCount = 0;
		/*
			The number of objects destroyed at the last iteration of ClearInvalidPtrs.
		*/
		uint32 LastGarbagedObjectsCount = 0;
		/*
			Index of ClearInvalidPtrs iteration.
		*/
		int32 GarbageCollectionIndex = 0;
		/*
			Count of threads to async garbage cleaning.
		*/
		uint32 GarbageCleaningThreadsCount = 1;
	};
} // namespace GarbageCollector_Private
