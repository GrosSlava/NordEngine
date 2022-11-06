#pragma once

#include "GenericPlatform.h"
#include "GarbageCollectorInternals.h"




/*
	Main garbage manager.
*/
class GGarbageCollector
{
	friend struct FGCPointerBase;

public:

	static FORCEINLINE GGarbageCollector* Get()
	{
		static GGarbageCollector GC;
		return &GC;
	}

private:

	FORCEINLINE GGarbageCollector() { }



public:

	/*
		Force collect garbage.
	*/
	FORCEINLINE void ForceGC() { PtrMap.ClearInvalidPtrs(); }
	/*
		@return total objects count who are under surveillance.
	*/
	FORCEINLINE uint32 GetTotalObjectsCount() const noexcept { return PtrMap.GetTotalObjectsCount(); }
	/*
		@return the number of objects destroyed at the last iteration of garbage collection.
	*/
	FORCEINLINE uint32 GetLastGarbagedObjectsCount() const noexcept { return PtrMap.GetLastGarbagedObjectsCount(); }
	/*
		@return index of the garbage collection iteration.
	*/
	FORCEINLINE int32 GetGarbageCollectionIndex() const noexcept { return PtrMap.GetGarbageCollectionIndex(); }

	/*
		Check that object address is in GarbageCollector.
	*/
	FORCEINLINE bool IsGCObjectValid(const void* Object) const noexcept 
	{ 
		if( Object == nullptr ) return false;
		return PtrMap.GetPtrHandler(Object) != nullptr;
	}
	/*
		Check that object address is in GarbageCollector and hasn't got any refs.
	*/
	FORCEINLINE bool IsGCObjectPendingToKill(const void* Object) const noexcept
	{
		if( Object == nullptr ) return false;
		const GarbageCollector_Private::FAddrHandler* LAddrHandler = PtrMap.GetPtrHandler(Object);
		return LAddrHandler != nullptr && LAddrHandler->IsPendingToKill();
	}
	/*
		Check that object address is in GarbageCollector and has got minimum one ref.
	*/
	FORCEINLINE bool IsGCObjectValidAndNotPendingToKill(const void* Object) const noexcept
	{
		if( Object == nullptr ) return false;
		const GarbageCollector_Private::FAddrHandler* LAddrHandler = PtrMap.GetPtrHandler(Object);
		return LAddrHandler != nullptr && !LAddrHandler->IsPendingToKill();
	}

	/*
		@return count of threads to garbage cleaning.
	*/
	FORCEINLINE uint32 GetGarbageCleaningThreadsCount() const noexcept { return PtrMap.GetGarbageCleaningThreadsCount(); }
	/*
		Set count of threads for garbage async cleaning.
		@param NewThreadCount - Count of threads.
	*/
	FORCEINLINE void SetGarbageCleaningThreadsCount(uint32 NewThreadCount) noexcept { PtrMap.SetGarbageCleaningThreadsCount(NewThreadCount); }


//for FGCPointerBase, no checks to Object nullptr
protected:

	FORCEINLINE GarbageCollector_Private::FAddrHandler* RegisterObjectRef(const void* Object) { return PtrMap.AddPtr(Object); }

	FORCEINLINE GarbageCollector_Private::FAddrHandler* RegisterExactlyNewObjectRef(const void* Object) { return PtrMap.AddNewPtr(Object); }

	FORCEINLINE GarbageCollector_Private::FAddrHandler* UnregisterObjectRef(const void* Object) { return PtrMap.RemovePtr(Object); }

	FORCEINLINE void UnregisterObjectRefByHandler(GarbageCollector_Private::FAddrHandler* PtrHandler) { PtrMap.RemovePtrByHandler(PtrHandler); }

	FORCEINLINE GarbageCollector_Private::FAddrHandler* GetPtrHandler(const void* Object) { return PtrMap.GetPtrHandler(Object); }



private:

	/*
		Storage of object ptrs.
	*/
	GarbageCollector_Private::FAnyPtrMap PtrMap;
};






/*
	Base class for GC pointers.
*/
struct FGCPointerBase
{
public:

	FORCEINLINE FGCPointerBase() { }
	FORCEINLINE FGCPointerBase(const FGCPointerBase& OtherGCPointerBase) noexcept : 
		SyncAddrHandler(OtherGCPointerBase.SyncAddrHandler), SyncIndex(OtherGCPointerBase.SyncIndex) 
	{ }
	FORCEINLINE FGCPointerBase(FGCPointerBase&& OtherGCPointerBase) noexcept : 
		SyncAddrHandler(OtherGCPointerBase.SyncAddrHandler), SyncIndex(OtherGCPointerBase.SyncIndex)
	{
		OtherGCPointerBase.SyncAddrHandler = nullptr;
	}


protected:

	/*
		Initialize from other FGCPointerBase.
	*/
	FORCEINLINE void SyncInitFrom(const FGCPointerBase& Other) noexcept
	{
		SyncAddrHandler = Other.SyncAddrHandler;
		SyncIndex = Other.SyncIndex;
	}

	/*
		@param Ptr - Owned object to sync check.
		@return true if this GCPointer synchronized with GC.
	*/
	FORCEINLINE bool IsSync(const void* Ptr) const noexcept
	{ 
		return SyncAddrHandler != nullptr && GGarbageCollector::Get()->GetGarbageCollectionIndex() == SyncIndex && *SyncAddrHandler == Ptr;
	}

	/*
		Force synchronize this pointer with GC.

		@return synchronization success.
	*/
	FORCEINLINE bool Sync(const void* Ptr) const noexcept
	{
		if( Ptr == nullptr )
		{
			SyncAddrHandler = nullptr;
			return false;
		}

		GGarbageCollector* LGC = GGarbageCollector::Get();
		if( SyncAddrHandler != nullptr && LGC->GetGarbageCollectionIndex() == SyncIndex && *SyncAddrHandler == Ptr ) return true;

		SyncAddrHandler = LGC->GetPtrHandler(Ptr);
		SyncIndex = LGC->GetGarbageCollectionIndex();

		return SyncAddrHandler != nullptr;
	}

	/*
		@param Ptr - Owned object to synchronized register in GC.
	*/
	FORCEINLINE void SyncRegister(const void* Ptr)
	{
		if( Ptr == nullptr )
		{
			SyncAddrHandler = nullptr;
			return;
		}

		GGarbageCollector* LGC = GGarbageCollector::Get();
		if( SyncAddrHandler != nullptr && LGC->GetGarbageCollectionIndex() == SyncIndex && *SyncAddrHandler == Ptr )
		{
			++SyncAddrHandler->Count;
		}
		else
		{
			SyncAddrHandler = LGC->RegisterObjectRef(Ptr);
			SyncIndex = LGC->GetGarbageCollectionIndex();
		}
	}

	/*
		@param Ptr - Owned object which is exactly new for GC.
	*/
	FORCEINLINE void SyncRegisterExactlyNew(const void* Ptr)
	{
		if( Ptr == nullptr )
		{
			SyncAddrHandler = nullptr;
			return;
		}

		GGarbageCollector* LGC = GGarbageCollector::Get();

		SyncAddrHandler = LGC->RegisterExactlyNewObjectRef(Ptr);
		SyncIndex = LGC->GetGarbageCollectionIndex();
	}

	/*
		@param Ptr - owned object to synchronized unregister in GC.
	*/
	FORCEINLINE void SyncUnregister(const void* Ptr)
	{
		if( Ptr == nullptr )
		{
			SyncAddrHandler = nullptr;
			return;
		}

		GGarbageCollector* LGC = GGarbageCollector::Get();
		if( SyncAddrHandler != nullptr && LGC->GetGarbageCollectionIndex() == SyncIndex && *SyncAddrHandler == Ptr )
		{
			LGC->UnregisterObjectRefByHandler(SyncAddrHandler);
		}
		else
		{
			//if Count == 0 SyncAddrHandler will be nullptr
			SyncAddrHandler = LGC->UnregisterObjectRef(Ptr);
		}
	}

	/*
		Check that object is in GC and pending to kill.
	*/
	FORCEINLINE bool SyncIsPendingToKill(const void* Ptr) const noexcept
	{
		if( Ptr == nullptr )
		{
			SyncAddrHandler = nullptr;
			return false;
		}

		GGarbageCollector* LGC = GGarbageCollector::Get();
		if( SyncAddrHandler != nullptr && LGC->GetGarbageCollectionIndex() == SyncIndex && *SyncAddrHandler == Ptr ) return SyncAddrHandler->IsPendingToKill();

		SyncAddrHandler = LGC->GetPtrHandler(Ptr);
		SyncIndex = LGC->GetGarbageCollectionIndex();

		return SyncAddrHandler != nullptr && SyncAddrHandler->IsPendingToKill();
	}

	/*
		Check that object is in GC and not pending to kill.
	*/
	FORCEINLINE bool SyncIsValidAndNotPendingToKill(const void* Ptr) const noexcept
	{
		if( Ptr == nullptr )
		{
			SyncAddrHandler = nullptr;
			return false;
		}

		GGarbageCollector* LGC = GGarbageCollector::Get();
		if( SyncAddrHandler != nullptr && LGC->GetGarbageCollectionIndex() == SyncIndex && *SyncAddrHandler == Ptr ) return !SyncAddrHandler->IsPendingToKill();

		SyncAddrHandler = LGC->GetPtrHandler(Ptr);
		SyncIndex = LGC->GetGarbageCollectionIndex();

		return SyncAddrHandler != nullptr && !SyncAddrHandler->IsPendingToKill();
	}


private:

	mutable GarbageCollector_Private::FAddrHandler* SyncAddrHandler = nullptr;
	mutable int32 SyncIndex = -1;
};




template<typename T> struct TGCPointer;
template<typename T> struct TWeakGCPointer;


template<typename T>
struct TGCPointer : public FGCPointerBase
{
public:

	FORCEINLINE TGCPointer() { }
	FORCEINLINE TGCPointer(T* InObject) :
		Object(InObject)
	{ 
		SyncRegister(InObject);
	}
	FORCEINLINE TGCPointer(T* InObject, bool IsNew) :
		Object(InObject)
	{
		if( IsNew )
		{
			SyncRegisterExactlyNew(InObject);
		}
		else
		{
			SyncRegister(InObject);
		}
	}
	FORCEINLINE TGCPointer(const TGCPointer<T>& OtherGCPointer) : FGCPointerBase(OtherGCPointer),
		Object(OtherGCPointer.Object)
	{ 
		SyncRegister(Object);
	}
	FORCEINLINE TGCPointer(TGCPointer<T>&& OtherGCPointer) noexcept : FGCPointerBase(OtherGCPointer),
		Object(OtherGCPointer.Object) 
	{ 
		OtherGCPointer.Object = nullptr;
	}
	FORCEINLINE TGCPointer(const TWeakGCPointer<T>& WeakGCPointer) : FGCPointerBase(WeakGCPointer),
		Object(WeakGCPointer.Object)
	{ 
		SyncRegister(Object);
	}
	FORCEINLINE TGCPointer(TWeakGCPointer<T>&& WeakGCPointer) noexcept : FGCPointerBase(WeakGCPointer),
		Object(WeakGCPointer.Object) 
	{ 
		SyncRegister(Object);
		WeakGCPointer.Object = nullptr;
	}

	~TGCPointer() { SyncUnregister(Object); }


public:

	FORCEINLINE operator bool() const { return IsValid(); }

	FORCEINLINE T& operator*() const { return *Object; }

	FORCEINLINE T* operator->() const { return Object; }

	FORCEINLINE TGCPointer<T>& operator=(const TGCPointer<T>& InGCPointer)
	{
		if( InGCPointer.Object == Object ) return *this;

		SyncUnregister(Object);
		Object = InGCPointer.Object;
		SyncInitFrom(InGCPointer);
		SyncRegister(Object);

		return *this;
	}
	FORCEINLINE TGCPointer<T>& operator=(TGCPointer<T>&& InGCPointer)
	{
		SyncUnregister(Object);
		Object = InGCPointer.Object;
		SyncInitFrom(InGCPointer);
		InGCPointer.Object = nullptr;

		return *this;
	}
	FORCEINLINE TGCPointer<T>& operator=(const TWeakGCPointer<T>& InWeakGCPointer)
	{
		if( InWeakGCPointer.Object == Object ) return *this;

		SyncUnregister(Object);
		Object = InWeakGCPointer.Object;
		SyncInitFrom(InWeakGCPointer);
		SyncRegister(Object);

		return *this;
	}
	FORCEINLINE TGCPointer<T>& operator=(TWeakGCPointer<T>&& InWeakGCPointer)
	{
		SyncUnregister(Object);
		Object = InWeakGCPointer.Object;
		SyncInitFrom(InWeakGCPointer);
		InWeakGCPointer.Object = nullptr;

		return *this;
	}
	FORCEINLINE TGCPointer<T>& operator=(T* Ptr)
	{
		if( Ptr == Object ) return *this;

		SyncUnregister(Object);
		Object = Ptr;
		SyncRegister(Object);

		return *this;
	}


	template<typename T>
	friend bool operator==(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator!=(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator>(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator>=(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator<(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator<=(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;


	template<typename T>
	friend bool operator==(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator!=(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator>(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator>=(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator<(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator<=(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;


	template<typename T>
	friend bool operator==(const TGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator!=(const TGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator>(const TGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator>=(const TGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator<(const TGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator<=(const TGCPointer<T>& Lhs, T* Rhs) noexcept;



public:

	FORCEINLINE bool IsValid() const noexcept
	{
		//if Object not in GC or Object == nullptr then we will be not sync
		//Note that this type of object can't be pending to kill, because we have minimum one ref to it.
		return Sync(Object);
	}

	FORCEINLINE T* Get() const noexcept
	{ 
		return Object;
	}

	FORCEINLINE T* GetChecked() const noexcept
	{
		if( !IsValid() )
		{
			Object = nullptr;
		}
		return Object;
	}

	FORCEINLINE void Reset()
	{
		SyncUnregister(Object);
		Object = nullptr;
	}



private:

	/*
		Owned object.
	*/
	T* Object = nullptr;
};

template<typename T>
FORCEINLINE bool operator==(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object == Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator!=(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object != Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator>(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object > Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator>=(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object >= Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator<(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object < Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator<=(const TGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object <= Rhs.Object;
}

template<typename T>
FORCEINLINE bool operator==(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object == Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator!=(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object != Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator>(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object > Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator>=(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object >= Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator<(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object < Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator<=(const TGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object <= Rhs.Object;
}

template<typename T>
FORCEINLINE bool operator==(const TGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object == Rhs;
}
template<typename T>
FORCEINLINE bool operator!=(const TGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object != Rhs;
}
template<typename T>
FORCEINLINE bool operator>(const TGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object > Rhs;
}
template<typename T>
FORCEINLINE bool operator>=(const TGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object >= Rhs;
}
template<typename T>
FORCEINLINE bool operator<(const TGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object < Rhs;
}
template<typename T>
FORCEINLINE bool operator<=(const TGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object <= Rhs;
}


template<typename T>
struct TWeakGCPointer : public FGCPointerBase
{
public:

	FORCEINLINE TWeakGCPointer() { }
	FORCEINLINE TWeakGCPointer(const TWeakGCPointer<T>& OtherWeakGCPointer) : FGCPointerBase(OtherWeakGCPointer),
		Object(OtherWeakGCPointer.Object)
	{ 
		Sync(Object);
	}
	FORCEINLINE TWeakGCPointer(TWeakGCPointer<T>&& OtherWeakGCPointer) noexcept : FGCPointerBase(OtherWeakGCPointer),
		Object(OtherWeakGCPointer.Object) 
	{ 
		Sync(Object);
		OtherWeakGCPointer.Object = nullptr;
	}
	FORCEINLINE TWeakGCPointer(const TGCPointer<T>& OtherGCPointer) : FGCPointerBase(OtherGCPointer),
		Object(OtherGCPointer.Object)
	{ 
		Sync(Object);
	}
	TWeakGCPointer(TGCPointer<T>&& OtherGCPointer) = delete;
	
	~TWeakGCPointer() { }


public:

	FORCEINLINE operator bool() const { return IsValid(); }

	FORCEINLINE T& operator*() const { return *Object; }

	FORCEINLINE T* operator->() const { return Object; }

	FORCEINLINE TWeakGCPointer<T>& operator=(const TGCPointer<T>& InGCPointer)
	{
		if( InGCPointer.Object == Object ) return *this;

		Object = InGCPointer.Object;
		SyncInitFrom(InGCPointer);
		Sync(Object);

		return *this;
	}
	TWeakGCPointer<T>& operator=(TGCPointer<T>&& InGCPointer) = delete;
	FORCEINLINE TWeakGCPointer<T>& operator=(const TWeakGCPointer<T>& InWeakGCPointer)
	{
		if( InWeakGCPointer.Object == Object ) return *this;

		Object = InWeakGCPointer.Object;
		SyncInitFrom(InWeakGCPointer);

		return *this;
	}
	FORCEINLINE TWeakGCPointer<T>& operator=(TWeakGCPointer<T>&& InWeakGCPointer)
	{
		Object = InWeakGCPointer.Object;
		SyncInitFrom(InWeakGCPointer);
		InWeakGCPointer.Object = nullptr;

		return *this;
	}
	TWeakGCPointer<T>& operator=(T* Ptr) = delete;


	template<typename T>
	friend bool operator==(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator!=(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator>(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator>=(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator<(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator<=(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept;


	template<typename T>
	friend bool operator==(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator!=(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator>(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator>=(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator<(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;

	template<typename T>
	friend bool operator<=(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept;


	template<typename T>
	friend bool operator==(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator!=(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator>(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator>=(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator<(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept;

	template<typename T>
	friend bool operator<=(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept;



public:

	FORCEINLINE bool IsValid() const noexcept
	{
		return SyncIsValidAndNotPendingToKill(Object);
	}

	FORCEINLINE T* Get() const noexcept
	{ 
		return Object;
	}

	FORCEINLINE T* GetChecked() const noexcept
	{
		if( !IsValid() )
		{
			Object = nullptr;
		}
		return Object;
	}

	FORCEINLINE void Reset()
	{
		Object = nullptr;
	}



private:

	/*
		Owned object.
	*/
	T* Object = nullptr;
};

template<typename T>
FORCEINLINE bool operator==(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object == Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator!=(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object != Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator>(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object > Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator>=(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object >= Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator<(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object < Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator<=(const TWeakGCPointer<T>& Lhs, const TWeakGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object <= Rhs.Object;
}

template<typename T>
FORCEINLINE bool operator==(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object == Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator!=(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object != Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator>(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object > Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator>=(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object >= Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator<(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object < Rhs.Object;
}
template<typename T>
FORCEINLINE bool operator<=(const TWeakGCPointer<T>& Lhs, const TGCPointer<T>& Rhs) noexcept
{
	return Lhs.Object <= Rhs.Object;
}

template<typename T>
FORCEINLINE bool operator==(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object == Rhs;
}
template<typename T>
FORCEINLINE bool operator!=(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object != Rhs;
}
template<typename T>
FORCEINLINE bool operator>(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object > Rhs;
}
template<typename T>
FORCEINLINE bool operator>=(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object >= Rhs;
}
template<typename T>
FORCEINLINE bool operator<(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object < Rhs;
}
template<typename T>
FORCEINLINE bool operator<=(const TWeakGCPointer<T>& Lhs, T* Rhs) noexcept
{
	return Lhs.Object <= Rhs;
}





template<typename T, typename... Args>
FORCEINLINE TGCPointer<T> MakeGCPointer(...)
{
	return TGCPointer<T>(new T(Args...), true);
}
