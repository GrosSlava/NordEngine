// CppReflectionGen framework.
#pragma once

#include "ClassAllocator.h"
#include <new>




template<typename T>
struct TPtr
{
public:

	TPtr() = default;
	template<typename V>
	TPtr(const TPtr<V>& Other) : UseID(Other.GetUseID())
	{
		Other.GetPtrWrapper(this->PtrWrapper);
	}
	TPtr(const TPtr<T>& Other) : PtrWrapper(Other.PtrWrapper), UseID(Other.UseID) { }
	TPtr(TPtr<T>&& Other) noexcept : PtrWrapper(Other.PtrWrapper), UseID(Other.UseID) { Other.Clear(); }
	TPtr(const TPtrWrapper<T>& Wrapper, UseID_t InUseID) : PtrWrapper(Wrapper), UseID(InUseID) { }
	~TPtr() { Clear(); }


public:

	inline T* Get() noexcept { return PtrWrapper.P; }
	inline const T* Get() const noexcept { return PtrWrapper.P; }
	inline bool IsValid() const noexcept { return PtrWrapper.IsValid() && UseID == PtrWrapper.Context->UseID; }

public:

	template<typename V>
	inline void GetPtrWrapper(TPtrWrapper<V>& OutWrapper) const noexcept
	{
		OutWrapper = PtrWrapper;
	}
	inline TPtrWrapper<T> GetPtrWrapper() const noexcept { return PtrWrapper; }
	inline UseID_t GetUseID() const noexcept { return UseID; }

public:

	template<typename... Args>
	TPtr<T> Create(Args... args)
	{
		PtrWrapper = TClassAllocator<T>::Get()->Allocate();
		if( !PtrWrapper.IsValid() ) return *this;
		UseID = PtrWrapper.Context->UseID;

		try
		{
			new(Get()) T(args...);
		}
		catch( ... )
		{
			Clear();
			throw;
		}

		return *this;
	}
	void Destroy()
	{
		if( !IsValid() ) return;
		Get()->~T();
		PtrWrapper.Context->Allocator->Deallocate(PtrWrapper);
		Clear();
	}
	inline void Clear() noexcept { PtrWrapper.Clear(); }

	template<typename V>
	inline TPtr<V> CastTo()
	{
		TPtrWrapper<V> NewWrapper(dynamic_cast<V*>(Get()), PtrWrapper.Context);
		return TPtr<V>(NewWrapper, UseID);
	}

public:

	explicit operator bool() const noexcept { return IsValid(); }
	T* operator->() noexcept { return Get(); }

	template<typename V>
	TPtr<T> operator=(const TPtr<V>& Other)
	{
		Other.GetPtrWrapper(this->PtrWrapper);
		this->UseID = Other.GetUseID();
		return *this;
	}
	TPtr<T> operator=(const TPtr<T>& Other)
	{
		this->PtrWrapper = Other.PtrWrapper;
		this->UseID = Other.UseID;
		return *this;
	}



private:

	TPtrWrapper<T> PtrWrapper;
	UseID_t UseID = -1;
};

template<typename U, typename V>
inline bool operator==(const TPtr<U>& A, const TPtr<V>& B) noexcept
{
	return A.Get() == B.Get();
}
template<typename U, typename V>
inline bool operator!=(const TPtr<U>& A, const TPtr<V>& B) noexcept
{
	return A.Get() != B.Get();
}

template<typename U, typename V>
inline bool operator==(const TPtr<U>& A, V* B) noexcept
{
	return A.Get() == B;
}
template<typename U, typename V>
inline bool operator!=(const TPtr<U>& A, V* B) noexcept
{
	return A.Get() != B;
}

template<typename U, typename V>
inline bool operator==(U* A, const TPtr<V>& B) noexcept
{
	return A == B.Get();
}
template<typename U, typename V>
inline bool operator!=(U* A, const TPtr<V>& B) noexcept
{
	return A != B.Get();
}
