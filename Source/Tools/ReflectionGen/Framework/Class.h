// CppReflectionGen framework.
#pragma once

#include "Ptr.h"




#define CLASS_BUILDER_BODY(TypeName)               \
protected:                                         \
                                                   \
	TypeName() = default;                          \
                                                   \
public:                                            \
                                                   \
	~TypeName() = default;                         \
                                                   \
public:                                            \
                                                   \
	TypeName(TypeName&&) = delete;                 \
	TypeName(const TypeName&) = delete;            \
	TypeName& operator=(const TypeName&) = delete; \
	TypeName& operator=(TypeName&&) = delete;      \
                                                   \
public:                                            \
                                                   \
	static TypeName* Get() noexcept                \
	{                                              \
		static TypeName ClassBuilder;              \
		return &ClassBuilder;                      \
	}

template<typename T>
class TClassBuilder
{
	CLASS_BUILDER_BODY(TClassBuilder)

public:

	virtual void* Create() const = 0;
	virtual TPtr<void> CreatePtr() const = 0;
};

template<typename T>
struct TClass
{
public:

	TClass() { Builder = TClassBuilder<T>::Get(); }
	template<typename V>
	TClass(TClass<V> Other)
	{
		this->Builder = Other.GetBuilder();
	}
	~TClass() = default;


public:

	inline T* Instantiate() const { return static_cast<T*>(Builder->Create(Result)); }
	inline TPtr<T> InstantiatePtr() const { return Builder->CreatePtr(); }

public:

	inline const TClassBuilder<T>* GetBuilder() const noexcept { return Builder; }

public:

	template<typename V>
	TClass<T>& operator=(TClass<V> Other)
	{
		this->Builder = Other.GetBuilder();
		return *this;
	}



private:

	const TClassBuilder<T>* Builder = nullptr;
};

template<typename U, typename V>
inline bool operator==(TClass<U> A, TClass<V> B) noexcept
{
	return A.GetBuilder() == B.GetBuilder();
}
template<typename U, typename V>
inline bool operator!=(TClass<U> A, TClass<V> B) noexcept
{
	return A.GetBuilder() != B.GetBuilder();
}