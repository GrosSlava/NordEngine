#pragma once

#include "GenericPlatform.h"
#include "GenericPlatformUtils.h"




#define Array_VerySmallDataSize			16
#define Array_SmallDataSize				128
#define Array_MediumDataSize			1024
#define Array_LargeDataSize				16384
#define Array_ExtreamlyLargeDataSize	131072



/*
	Ram moving can be used for easy pop from start or back without copy or moving data
*/

template<typename T>
class TCycleArray
{
	template<typename T>
	friend class TArray;

	template<typename T>
	friend struct TArrayIterator;


public:

	FORCEINLINE ~TCycleArray();


public:

	FORCEINLINE T& operator[](uint32 Index);
	FORCEINLINE T operator[](uint32 Index) const;

public:

	FORCEINLINE T& GetAt(uint32 Index);
	FORCEINLINE T& GetFirst() noexcept;
	FORCEINLINE T& GetLast() noexcept;


	FORCEINLINE void MoveRamLeft() noexcept;
	FORCEINLINE void MoveRamRight() noexcept;


	FORCEINLINE int GetLIndex() const noexcept;
	FORCEINLINE int GetRIndex() const noexcept;

	FORCEINLINE void Reset() noexcept;
	FORCEINLINE void Resize(uint32 NewSize) noexcept;


private:

	uint32 Size = 0;

	uint32 LIndex = 0;
	T* DataArray = nullptr;


};



//................................................................................................................................................//


template<typename T>
FORCEINLINE TCycleArray<T>::~TCycleArray()
{
	delete[] DataArray;
}



template<typename T>
FORCEINLINE T& TCycleArray<T>::operator[](uint32 Index)
{
	Index += LIndex;
	return (Index >= Size) ? DataArray[Index - Size] : DataArray[Index];
}

template<typename T>
FORCEINLINE T TCycleArray<T>::operator[](uint32 Index) const
{
	Index += LIndex;
	return (Index >= Size) ? DataArray[Index - Size] : DataArray[Index];
}

template<typename T>
FORCEINLINE T& TCycleArray<T>::GetAt(uint32 Index)
{
	Index += LIndex;
	return (Index >= Size) ? DataArray[Index - Size] : DataArray[Index];

}

template<typename T>
FORCEINLINE T& TCycleArray<T>::GetFirst() noexcept
{
	return DataArray[LIndex];
}

template<typename T>
FORCEINLINE T& TCycleArray<T>::GetLast() noexcept
{
	return DataArray[GetRIndex()];
}








template<typename T>
FORCEINLINE void TCycleArray<T>::MoveRamLeft() noexcept
{
	LIndex = GetRIndex();
}

template<typename T>
FORCEINLINE void TCycleArray<T>::MoveRamRight() noexcept
{
	++LIndex;
	if (LIndex == Size) LIndex = 0;
}





template<typename T>
FORCEINLINE int TCycleArray<T>::GetLIndex() const noexcept
{
	return LIndex;
}

template<typename T>
FORCEINLINE int TCycleArray<T>::GetRIndex() const noexcept
{
	return (LIndex == 0) ? (Size - 1) : LIndex - 1;
}





template<typename T>
FORCEINLINE void TCycleArray<T>::Reset() noexcept
{
	LIndex = 0;

}

template<typename T>
FORCEINLINE void TCycleArray<T>::Resize(uint32 NewSize) noexcept
{
	Size = NewSize;

	if (DataArray != nullptr) delete[] DataArray;
	
	DataArray = new T[NewSize];
}

