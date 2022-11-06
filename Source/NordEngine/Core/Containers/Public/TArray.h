#pragma once

#include "TCycleArray.h"
#include "SpecificationMacros.h"



template<typename T>
class TArray;






template<typename T>
struct TArrayIterator
{

	using TArray = TArray<T>;


public:

	FORCEINLINE TArrayIterator(const TArray* InArray, uint32 InSubArrayIndex, uint32 InElemBatchIndex, uint32 InBatchSize) noexcept :
		MyArray(InArray), SubArrayIndex(InSubArrayIndex), BatchSize(InBatchSize),
		StartArray(&InArray->SubArrays[InSubArrayIndex]->DataArray[0]),
		StartElemInBatch(&InArray->SubArrays[InSubArrayIndex]->GetFirst()),
		Elem(&InArray->SubArrays[InSubArrayIndex]->GetAt(InElemBatchIndex)),
		EndElemInBatch(&InArray->SubArrays[InSubArrayIndex]->GetLast()),
		EndArray(&InArray->SubArrays[InSubArrayIndex]->DataArray[InBatchSize - 1])
	{

	}



public:

	FORCEINLINE T& operator*() noexcept { return *Elem; }

	FORCEINLINE TArrayIterator& operator++() noexcept
	{

		if (Elem == EndElemInBatch)
		{
			++SubArrayIndex;

			StartArray = &MyArray->SubArrays[SubArrayIndex]->DataArray[0];
			StartElemInBatch = &MyArray->SubArrays[SubArrayIndex]->GetFirst();
			EndElemInBatch = &MyArray->SubArrays[SubArrayIndex]->GetLast();
			EndArray = &MyArray->SubArrays[SubArrayIndex]->DataArray[BatchSize - 1];

			Elem = StartElemInBatch;
		}
		else
		{
			if (Elem == EndArray)
			{
				Elem = StartElemInBatch;
			}
			else
			{
				++Elem;
			}
		}


		return *this;
	}
	FORCEINLINE TArrayIterator operator++(int) noexcept { TArrayIterator Tmp = *this; ++(*this); return Tmp; }


	FORCEINLINE TArrayIterator& operator--() noexcept
	{
		if (Elem == StartElemInBatch)
		{
			--SubArrayIndex;

			StartArray = &MyArray->SubArrays[SubArrayIndex]->DataArray[0];
			StartElemInBatch = &MyArray->SubArrays[SubArrayIndex]->GetFirst();
			EndElemInBatch = &MyArray->SubArrays[SubArrayIndex]->GetLast();
			EndArray = &MyArray->SubArrays[SubArrayIndex]->DataArray[BatchSize - 1];

			Elem = EndElemInBatch;
		}
		else
		{
			if (Elem == StartArray)
			{
				Elem = EndElemInBatch;
			}
			else
			{
				--Elem;
			}
		}

		return *this;
	}
	FORCEINLINE TArrayIterator operator--(int) noexcept { TArrayIterator Tmp = *this; --(*this); return Tmp; }




	FORCEINLINE friend bool operator== (const TArrayIterator& A, const TArrayIterator& B) noexcept
	{
		return A.Elem == B.Elem;
	}
	FORCEINLINE friend bool operator!= (const TArrayIterator& A, const TArrayIterator& B) noexcept
	{
		return A.Elem != B.Elem;
	}

	FORCEINLINE friend bool operator< (const TArrayIterator& A, const TArrayIterator& B) noexcept
	{
		return A.SubArrayIndex < B.SubArrayIndex && A.Elem < B.Elem;
	}
	FORCEINLINE friend bool operator<= (const TArrayIterator& A, const TArrayIterator& B) noexcept
	{
		return A.SubArrayIndex <= B.SubArrayIndex && A.Elem <= B.Elem;
	}

	FORCEINLINE friend bool operator> (const TArrayIterator& A, const TArrayIterator& B) noexcept
	{
		return A.SubArrayIndex > B.SubArrayIndex && A.Elem > B.Elem;
	}
	FORCEINLINE friend bool operator>= (const TArrayIterator& A, const TArrayIterator& B) noexcept
	{
		return A.SubArrayIndex >= B.SubArrayIndex && A.Elem >= B.Elem;
	}


private:

	const TArray* MyArray = nullptr;

	uint32 SubArrayIndex = 0;

	T* StartArray = nullptr;
	T* StartElemInBatch = nullptr;
	T* Elem = nullptr;
	T* EndElemInBatch = nullptr;
	T* EndArray = nullptr;

	uint32 BatchSize = 0;

};





template<typename T>
class TArray
{
	template<typename T>
	friend struct TArrayIterator;

	using TArrayIterator = TArrayIterator<T>;

	NONCOPYABLE(TArray)

public:

	TArray() = delete;

	/* InBatchSize - use only precalculated sizes */
	FORCEINLINE TArray(uint32 InBatchSize = Array_SmallDataSize);
	FORCEINLINE ~TArray();


public:

	/*
		Index < 0 means get from back.
		Index == -1 means get last element
	*/
	FORCEINLINE T& operator[](int Index);

	/*
		Index < 0 means get from back.
		Index == -1 means get last element
	*/
	FORCEINLINE T operator[](int Index) const;



public:

	FORCEINLINE TArrayIterator begin() const noexcept
	{
		return TArrayIterator(this, 0, 0, BatchSize);
	}

	FORCEINLINE TArrayIterator end() const noexcept
	{
		if (LastElemBatchIndex == BatchSize - 1)
		{
			return TArrayIterator(this, LastUsedSubArrayIndex + 1, 0, BatchSize);
		}
		else
		{
			return TArrayIterator(this, LastUsedSubArrayIndex, LastElemBatchIndex + 1, BatchSize);
		}
	}



public:

	/*
		Add Elem to the end of array
	*/
	FORCEINLINE void PushBack(T Elem) noexcept;

	/*
		Add Elem to the begin of array
	*/
	FORCEINLINE void PushFront(T Elem) noexcept;

	/*
		Remove last elem and return it
		@return popped element
	*/
	T PopBack();

	/*
		Remove first elem and return it
		@return popped element
	*/
	T PopFront();





	/*
		Add Elem after stared elem at Index.
		New elem will be at position = Index + 1
	*/
	void AddAfter(uint32 Index, T Elem);

	/*
		Index < 0 remove from back.
		Index == -1 means remove last element
	*/
	void RemoveAt(int Index);

	/*
		Remove element at index and swapping it with last element
	*/
	void RemoveAtSwap(uint32 Index);



	/*
		Lambda return true if A < B
		e.g [&](const T& A, const T& B){ return A < B; }
	*/
	template<typename Predicate>
	void Sort(Predicate Compare);

	/*
		Remove all elements where Lambda return true
		e.g [&](const T& Elem){ return Elem == 0; }
	*/
	template<typename Predicate>
	void RemoveAll(Predicate NeedToRemove);



	/*
		Try to find Elem in array by ==
	*/
	bool Contains(const T& Elem) const;
	/*
		@return index of first found Elem by ==. If not found return -1
	*/
	int Find(const T& Elem) const;




	/*
		@return C-style standart array. It will be a copy of this array in stndart form.
		You will need to delete them by delete[]
	*/
	T* GetConstantSizeArray() const noexcept;




	/*
		Empty array without physical resizing to 0
	*/
	FORCEINLINE void Empty() noexcept;

	/*
		Physically risize to 0 size
	*/
	FORCEINLINE void Reset() noexcept;



	/*
		@return Count of elements
	*/
	FORCEINLINE uint32 Num() const noexcept { return ElemNum; }




private:

	FORCEINLINE void IncrementElemNum();
	FORCEINLINE void DecrementElemNum();

	FORCEINLINE uint32 GetNextSubArrayCount(uint32 CurrentSubArrayCount) const;
	void RegisterNewSubArray();
	void ClearMemory();



protected:

	uint32 SubArraysNum = 0;
	TCycleArray<T>** SubArrays = nullptr;

private:

	uint32 BatchSize = Array_SmallDataSize;
	uint32 LastUsedSubArrayIndex = 0;
	uint32 ElemNum = 0;

	int LastElemBatchIndex = -1;
	T* LastElem = nullptr;
	T* LastSubArrayAfterEnd = nullptr;
	T* LastSubArrayPreStart = nullptr;

};



//................................................................................................................................................//


template<typename T>
FORCEINLINE void TArray<T>::IncrementElemNum()
{
	++ElemNum; 
	++LastElemBatchIndex; 
	if (LastElemBatchIndex == BatchSize)
	{
		++LastUsedSubArrayIndex; 
			
		if (LastUsedSubArrayIndex >= SubArraysNum)
		{
			RegisterNewSubArray(); 
		}
		else
		{
			SubArrays[LastUsedSubArrayIndex]->Reset(); 
		}
				
			LastElemBatchIndex = 0; 
			LastSubArrayPreStart = &SubArrays[LastUsedSubArrayIndex]->DataArray[-1]; 
			LastElem = LastSubArrayPreStart + 1; 
			LastSubArrayAfterEnd = &SubArrays[LastUsedSubArrayIndex]->DataArray[BatchSize]; 
	}
	else
	{
		if (LastElem == LastSubArrayAfterEnd)
		{
			LastElem = LastSubArrayPreStart + 1; 
		}
	}
}

template<typename T>
FORCEINLINE void TArray<T>::DecrementElemNum()
{
	--ElemNum; 
	if (LastElemBatchIndex == -1)
	{
			
		if (LastUsedSubArrayIndex > 0)
		{
			--LastUsedSubArrayIndex; 
				
			LastElemBatchIndex = BatchSize - 1; 
			LastSubArrayPreStart = &SubArrays[LastUsedSubArrayIndex]->DataArray[-1]; 
			LastElem = &SubArrays[LastUsedSubArrayIndex]->GetAt(BatchSize - 1); 
			LastSubArrayAfterEnd = &SubArrays[LastUsedSubArrayIndex]->DataArray[BatchSize]; 
		}
		else
		{
			LastSubArrayPreStart = &SubArrays[0]->DataArray[-1]; 
			LastElem = LastSubArrayPreStart; 
			LastSubArrayAfterEnd = &SubArrays[0]->DataArray[BatchSize]; 
			SubArrays[0]->Reset(); 
		}
	}
	else
	{
		--LastElemBatchIndex; 
		if (LastElem == LastSubArrayPreStart)
		{
			LastElem = LastSubArrayAfterEnd - 1; 
		}
	}
}









template<typename T>
FORCEINLINE TArray<T>::TArray(uint32 InBatchSize) :
	BatchSize(InBatchSize)
{
	RegisterNewSubArray();

	LastSubArrayPreStart = &SubArrays[0]->DataArray[-1];
	LastElem = LastSubArrayPreStart;
	LastSubArrayAfterEnd = &SubArrays[0]->DataArray[BatchSize];

}

template<typename T>
FORCEINLINE TArray<T>::~TArray()
{
	ClearMemory();

	ElemNum = 0;
	LastUsedSubArrayIndex = 0;
	LastElemBatchIndex = -1;
	SubArraysNum = 0;
}






template<typename T>
FORCEINLINE T& TArray<T>::operator[](int Index)
{
	if (Index < 0) Index += ElemNum;


	const uint32 LSubArrayIndex = Index / BatchSize;
	return SubArrays[LSubArrayIndex]->GetAt(Index - LSubArrayIndex * BatchSize);

}

template<typename T>
FORCEINLINE T TArray<T>::operator[](int Index) const
{
	if (Index < 0) Index += ElemNum;


	const uint32 LSubArrayIndex = Index / BatchSize;
	return SubArrays[LSubArrayIndex]->GetAt(Index - LSubArrayIndex * BatchSize);
}









template<typename T>
FORCEINLINE void TArray<T>::PushBack(T Elem) noexcept
{
	++LastElem;
	IncrementElemNum();

	*LastElem = Elem;

}

template<typename T>
FORCEINLINE void TArray<T>::PushFront(T Elem) noexcept
{
	if (ElemNum == 0)
	{
		PushBack(Elem);
		return;
	}

	//we dont need to increment last elem pointer because it will be compensated when last SubArray move ram left
	IncrementElemNum();



	T PreviuosLastElem = Elem;
	T TmpLastElemInButch;
	for (uint32 i = 0; i <= LastUsedSubArrayIndex; ++i)
	{
		auto LSubArray = SubArrays[i];

		TmpLastElemInButch = LSubArray->GetLast();
		LSubArray->MoveRamLeft();
		LSubArray->GetFirst() = PreviuosLastElem;
		PreviuosLastElem = TmpLastElemInButch;
	}


	//if it is new array. Ram moved on last iteration and first elem is at last array index
	if (LastElemBatchIndex == 0)
	{
		LastElem = &SubArrays[LastUsedSubArrayIndex]->DataArray[BatchSize - 1];
	}
}



template<typename T>
FORCEINLINE T TArray<T>::PopBack()
{
	T LLastElem = *LastElem;

	--LastElem;
	DecrementElemNum();

	return LLastElem;
}

template<typename T>
FORCEINLINE T TArray<T>::PopFront()
{
	T FirstElem = SubArrays[0]->GetFirst();
	SubArrays[0]->MoveRamRight();

	for (uint32 i = 1; i <= LastUsedSubArrayIndex; ++i)
	{
		SubArrays[i - 1]->GetLast() = SubArrays[i]->GetFirst();
		SubArrays[i]->MoveRamRight();
	}

	//we dont need to decrement last elem pointer
	DecrementElemNum();

	return FirstElem;
}








template<typename T>
FORCEINLINE void TArray<T>::AddAfter(uint32 Index, T Elem)
{

	if (ElemNum == 0 || Index >= ElemNum - 1)
	{
		PushBack(Elem);
		return;
	}


	const uint32 SubArrayIndex = (Index + 1) / BatchSize;
	const uint32 LNewElementBatchIndex = (Index + 1) - SubArrayIndex * BatchSize;

	auto LMainSubArray = SubArrays[SubArrayIndex];

	T LastElemInButch = LMainSubArray->GetLast();

	if (LNewElementBatchIndex >= BatchSize / 2)
	{
		for (int i = (int)BatchSize - 2; i >= (int)LNewElementBatchIndex; --i)
		{
			LMainSubArray->GetAt(i + 1) = LMainSubArray->GetAt(i);
		}
	}
	else
	{
		LMainSubArray->MoveRamLeft();
		for (int i = 0; i <= (int)LNewElementBatchIndex - 1; ++i)
		{
			LMainSubArray->GetAt(i) = LMainSubArray->GetAt(i - 1);
		}
	}

	LMainSubArray->GetAt(LNewElementBatchIndex) = Elem;


	IncrementElemNum();


	T TmpLastElemInButch;
	for (uint32 i = SubArrayIndex + 1; i <= LastUsedSubArrayIndex; ++i)
	{
		auto LSubArray = SubArrays[i];

		TmpLastElemInButch = LSubArray->GetLast();
		LSubArray->MoveRamLeft();
		LSubArray->GetFirst() = LastElemInButch;
		LastElemInButch = TmpLastElemInButch;
	}


	//if it is new array. Ram moved on last iteration and first elem is at last array index
	if (LastElemBatchIndex == 0)
	{
		LastElem = &SubArrays[LastUsedSubArrayIndex]->DataArray[BatchSize - 1];
	}

}



template<typename T>
FORCEINLINE void TArray<T>::RemoveAt(int Index)
{
	if (Index >= (int)ElemNum || -Index >= (int)ElemNum) return;

	if (Index < 0) Index += ElemNum;


	const uint32 SubArrayIndex = Index / BatchSize;
	const uint32 LRemoveElementBatchIndex = Index - SubArrayIndex * BatchSize;

	auto LMainSubArray = SubArrays[SubArrayIndex];

	if (LRemoveElementBatchIndex >= BatchSize / 2)
	{
		for (uint32 i = LRemoveElementBatchIndex; i < BatchSize - 1; ++i)
		{
			LMainSubArray->GetAt(i) = LMainSubArray->GetAt(i + 1);
		}
	}
	else
	{
		for (uint32 i = LRemoveElementBatchIndex; i > 0; --i)
		{
			LMainSubArray->GetAt(i) = LMainSubArray->GetAt(i - 1);
		}
		LMainSubArray->MoveRamRight();
	}



	for (uint32 i = SubArrayIndex + 1; i <= LastUsedSubArrayIndex; ++i)
	{
		SubArrays[i - 1]->GetLast() = SubArrays[i]->GetFirst();
		SubArrays[i]->MoveRamRight();
	}

	DecrementElemNum();

}

template<typename T>
FORCEINLINE void TArray<T>::RemoveAtSwap(uint32 Index)
{
	if (Index >= ElemNum) return;


	const uint32 LSubArrayIndex = Index / BatchSize;

	SubArrays[LSubArrayIndex]->GetAt(Index - LSubArrayIndex * BatchSize) = *LastElem;


	--LastElem;
	DecrementElemNum();

}







template<typename T>
template<typename Predicate>
FORCEINLINE void TArray<T>::Sort(Predicate Compare)
{
	//TODO
}


template<typename T>
template<typename Predicate>
FORCEINLINE void TArray<T>::RemoveAll(Predicate NeedToRemove)
{
	if (ElemNum == 0) return;


	for (int SubArrayIndex = LastUsedSubArrayIndex; SubArrayIndex >= 0; --SubArrayIndex)
	{
		auto LMainSubArray = SubArrays[SubArrayIndex];

		for (int LElemIndex = (SubArrayIndex == LastUsedSubArrayIndex) ? LastElemBatchIndex : BatchSize - 1; LElemIndex >= 0; --LElemIndex)
		{

			if (!NeedToRemove(LMainSubArray->GetAt(LElemIndex))) continue;



			if (LElemIndex >= (int)BatchSize / 2)
			{
				for (uint32 i = LElemIndex; i < BatchSize - 1; ++i)
				{
					LMainSubArray->GetAt(i) = LMainSubArray->GetAt(i + 1);
				}
			}
			else
			{
				for (uint32 i = LElemIndex; i > 0; --i)
				{
					LMainSubArray->GetAt(i) = LMainSubArray->GetAt(i - 1);
				}
				LMainSubArray->MoveRamRight();
			}



			for (uint32 i = SubArrayIndex + 1; i <= LastUsedSubArrayIndex; ++i)
			{
				SubArrays[i - 1]->GetLast() = SubArrays[i]->GetFirst();
				SubArrays[i]->MoveRamRight();
			}

			DecrementElemNum();


		}

	}

}




template<typename T>
FORCEINLINE bool TArray<T>::Contains(const T& Elem) const
{
	return Find(Elem) != -1;
}

template<typename T>
FORCEINLINE int TArray<T>::Find(const T& Elem) const
{
	if (ElemNum == 0) return -1;


	for (uint32 SubArrayIndex = 0; SubArrayIndex <= LastUsedSubArrayIndex; ++SubArrayIndex)
	{
		auto LMainSubArray = SubArrays[SubArrayIndex];
		uint32 LastElemIndex = (SubArrayIndex == LastUsedSubArrayIndex) ? LastElemBatchIndex : BatchSize - 1;

		for (uint32 LElemIndex = 0; LElemIndex <= LastElemIndex; ++LElemIndex)
		{
			if (LMainSubArray->GetAt(LElemIndex) == Elem) return SubArrayIndex * BatchSize + LElemIndex;
		}
	}

	return -1;
}








template<typename T>
FORCEINLINE T* TArray<T>::GetConstantSizeArray() const noexcept
{
	T* StandartArray = new T[ElemNum];

	if (ElemNum == 0) return StandartArray;


	uint32 RealIndex = 0;
	for (uint32 SubArrayIndex = 0; SubArrayIndex <= LastUsedSubArrayIndex; ++SubArrayIndex)
	{
		auto LMainSubArray = SubArrays[SubArrayIndex];
		uint32 LastElemIndex = (SubArrayIndex == LastUsedSubArrayIndex) ? LastElemBatchIndex : BatchSize - 1;

		for (uint32 LElemIndex = 0; LElemIndex <= LastElemIndex; ++LElemIndex)
		{
			StandartArray[RealIndex] = LMainSubArray->GetAt(LElemIndex);
			++RealIndex;
		}
	}


	return StandartArray;
}









template<typename T>
FORCEINLINE void TArray<T>::Empty() noexcept
{
	ElemNum = 0;
	LastUsedSubArrayIndex = 0;
	LastElemBatchIndex = -1;

	LastSubArrayPreStart = &SubArrays[0]->DataArray[-1];
	LastElem = LastSubArrayPreStart;
	LastSubArrayAfterEnd = &SubArrays[0]->DataArray[BatchSize];

	SubArrays[0]->Reset();
}

template<typename T>
FORCEINLINE void TArray<T>::Reset() noexcept
{
	ClearMemory();

	SubArraysNum = 0;

	RegisterNewSubArray();
	Empty();
}










template<typename T>
FORCEINLINE uint32 TArray<T>::GetNextSubArrayCount(uint32 CurrentSubArrayCount) const
{
	if (CurrentSubArrayCount == 0) return 1;

	return CurrentSubArrayCount * 2;
}

template<typename T>
FORCEINLINE void TArray<T>::RegisterNewSubArray()
{
	const uint32 NewSubArrayCount = GetNextSubArrayCount(SubArraysNum);
	const uint32 AddCount = NewSubArrayCount - SubArraysNum;

	TCycleArray<T>** LNewSubArrays = new TCycleArray<T>*[NewSubArrayCount + 1];//+1 for create buffer zone for end iterator
	TCycleArray<T>* LSubArrays = new TCycleArray<T>[AddCount];


	for (uint32 i = 0; i < SubArraysNum; ++i)
	{
		LNewSubArrays[i] = SubArrays[i];
	}

	for (uint32 i = 0; i < AddCount; ++i)
	{
		LSubArrays[i].Resize(BatchSize);
		LNewSubArrays[SubArraysNum + i] = &LSubArrays[i];
	}


	SubArraysNum = NewSubArrayCount;

	if (SubArrays != nullptr) delete[] SubArrays;
	SubArrays = LNewSubArrays;

}

template<typename T>
FORCEINLINE void TArray<T>::ClearMemory()
{
	
	uint32 i = 0;
	while (i < SubArraysNum)
	{
		delete[] SubArrays[i];
		i = GetNextSubArrayCount(i);
	}

	
	delete[] SubArrays;
	SubArrays = nullptr;
}




