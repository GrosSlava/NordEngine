// CppReflectionGen framework.
#pragma once

#include <cstdlib>




class BaseClassAllocator;

using UseID_t = unsigned long long int;



namespace PtrWrapperPrivate
{
template<typename From, typename To>
struct PointerConvert
{
	static To* Convert(From* P) noexcept { return P; }
};

template<typename To>
struct PointerConvert<void, To>
{
	static To* Convert(void* P) noexcept { return static_cast<To*>(P); }
};
} // namespace PtrWrapperPrivate

struct PtrContext
{
	UseID_t UseID = 0;
	BaseClassAllocator* Allocator = nullptr;
};

template<typename T>
struct TPtrWrapper
{
public:

	TPtrWrapper() = default;
	template<typename V>
	TPtrWrapper(const TPtrWrapper<V>& Other) : P(PtrWrapperPrivate::PointerConvert<V, T>::Convert(Other.P)), Context(Other.Context)
	{
	}
	TPtrWrapper(const TPtrWrapper<T>& Other) : P(Other.P), Context(Other.Context) { }
	TPtrWrapper(TPtrWrapper<T>&& Other) noexcept : P(Other.P), Context(Other.Context) { Other.Clear(); }
	TPtrWrapper(T* InP, PtrContext* InContext) : P(InP), Context(InContext) { }
	~TPtrWrapper() = default;


public:

	inline bool IsValid() const noexcept { return P != nullptr; }
	inline void Clear() noexcept { P = nullptr; }

public:

	template<typename V>
	TPtrWrapper<T> operator=(const TPtrWrapper<V>& Other)
	{
		this->P = PtrWrapperPrivate::PointerConvert<V, T>::Convert(Other.P);
		this->Context = Other.Context;
		return *this;
	}
	TPtrWrapper<T> operator=(const TPtrWrapper<T>& Other)
	{
		this->P = Other.P;
		this->Context = Other.Context;
		return *this;
	}



public:

	T* P = nullptr;
	PtrContext* Context = nullptr;
};

template<typename T>
struct TObjectWrapper
{
	T Obj;
	PtrContext Context;
};



class BaseClassAllocator
{
public:

	virtual void Deallocate(const TPtrWrapper<void>& Wrapper) = 0;
};

template<typename T>
class TClassAllocator : public BaseClassAllocator
{
private:

	TClassAllocator() { Expand(); }

public:

	~TClassAllocator() = default;

public:

	TClassAllocator(TClassAllocator&&) = delete;
	TClassAllocator(const TClassAllocator&) = delete;
	TClassAllocator& operator=(const TClassAllocator&) = delete;
	TClassAllocator& operator=(TClassAllocator&&) = delete;

public:

	static TClassAllocator* Get() noexcept
	{
		static TClassAllocator Allocator;
		return &Allocator;
	}


public:

	TPtrWrapper<T> Allocate()
	{
		if( FreeElemsCount == 0 )
		{
			Expand();
			if( FreeElemsCount == 0 ) return TPtrWrapper<T>();
		}

		--FreeElemsCount;
		return FreeElemsStack[FreeElemsCount];
	}

	virtual void Deallocate(const TPtrWrapper<void>& Wrapper) override
	{
		if( Wrapper.Context->Allocator != this ) return;

		Wrapper.Context->UseID++;

		FreeElemsStack[FreeElemsCount] = Wrapper;
		++FreeElemsCount;
	}

private:

	void Expand()
	{
		const unsigned long long int AdditionalObjectsCount = ElemsCount == 0 ? 512 : ElemsCount;
		const unsigned long long int NewElemsCount = ElemsCount + AdditionalObjectsCount;
		TPtrWrapper<T>* NewElemsStack = static_cast<TPtrWrapper<T>*>(malloc(sizeof(TPtrWrapper<T>) * NewElemsCount));
		if( !NewElemsStack ) return;

		TObjectWrapper<T>* LAdditionalObjectsPool = static_cast<TObjectWrapper<T>*>(malloc(sizeof(TObjectWrapper<T>) * AdditionalObjectsCount));
		if( !LAdditionalObjectsPool ) return;

		if( FreeElemsStack )
		{
			free(FreeElemsStack);
		}
		FreeElemsStack = NewElemsStack;
		ElemsCount = NewElemsCount;

		TPtrWrapper<T>* LInsertionPos = FreeElemsStack;
		for( unsigned long long int i = 0; i < AdditionalObjectsCount; ++i, ++LInsertionPos, ++LAdditionalObjectsPool )
		{
			LAdditionalObjectsPool->Context.UseID = 0;
			LAdditionalObjectsPool->Context.Allocator = this;

			LInsertionPos->P = &(LAdditionalObjectsPool->Obj);
			LInsertionPos->Context = &(LAdditionalObjectsPool->Context);
		}

		FreeElemsCount = AdditionalObjectsCount;
	}



private:

	TPtrWrapper<T>* FreeElemsStack = nullptr;
	unsigned long long int ElemsCount = 0;
	unsigned long long int FreeElemsCount = 0;
};