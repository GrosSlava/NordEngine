// CppReflectionGen framework.
#pragma once

#include "Type.h"




class Property
{
public:

	virtual unsigned int GetSize() const noexcept = 0;
	virtual bool IsA(TypeID ID) const noexcept = 0;
	virtual TypeID GetTypeID() const noexcept = 0;
	virtual void* Get() noexcept = 0;

public:

	template<typename T>
	inline T GetAs() noexcept
	{
		return *(TType<T>::Cast(Get()));
	}
	template<typename T>
	inline T* GetAsPtr() noexcept
	{
		return TType<T>::Cast(Get());
	}
	template<typename T>
	inline bool IsA() noexcept
	{
		return IsA(TType<T>::ID());
	}
};

template<typename T>
class TProperty : public Property
{
public:

	TProperty() = default;
	explicit TProperty(const T& InV) : Val(InV) { }
	TProperty(T&& InV) noexcept : Val(InV) { }
	TProperty(const TProperty& Other) : Val(Other.Val) { }
	TProperty(TProperty&& Other) noexcept : Val(Other.Val) { }
	template<typename V>
	TProperty(const TProperty<V>& Other) : Val(Other.GetTyped())
	{
	}

public:

	virtual unsigned int GetSize() const noexcept override { return sizeof(T); }
	virtual bool IsA(TypeID ID) const noexcept override { return TType<T>::ID() == ID; }
	virtual TypeID GetTypeID() const noexcept override { return TType<T>::ID(); }
	virtual void* Get() noexcept override { return &Val; }

public:

	inline void Set(const T& NewV) { Val = NewV; }
	inline T GetTyped() const noexcept { return Val; }

public:

	template<typename V>
	TProperty<T> operator=(const TProperty<V>& Other)
	{
		if( this == &Other ) return *this;
		this->Val = Other.GetTyped();
		return *this;
	}
	template<typename V>
	TProperty<T> operator=(const V& NewV)
	{
		this->Val = NewV;
		return *this;
	}
	TProperty<T> operator=(const TProperty<T>& Other)
	{
		if( this == &Other ) return *this;
		this->Val = Other.Val;
		return *this;
	}
	TProperty<T> operator=(const T& NewV)
	{
		this->Val = NewV;
		return *this;
	}



private:

	T Val;
};

struct PropertyHandler
{
public:

	PropertyHandler() = default;
	template<typename T>
	PropertyHandler(const T& Val)
	{
		Prop = new TProperty(Val);
	}
	PropertyHandler(const PropertyHandler& Other) = delete;
	PropertyHandler(PropertyHandler&& Other) noexcept : Prop(Other.Prop) { Other.Prop = nullptr; }
	~PropertyHandler()
	{
		if( Prop ) delete Prop;
	}


public:

	inline bool IsValid() const noexcept { return Prop != nullptr; }
	inline unsigned int GetSize() const noexcept { return Prop->GetSize(); }
	inline bool IsA(TypeID ID) const noexcept { return Prop->IsA(ID); }
	inline TypeID GetTypeID() const noexcept { return Prop->GetTypeID(); }

	template<typename T>
	inline T GetAs() noexcept
	{
		return Prop->GetAs<T>();
	}
	template<typename T>
	inline bool IsA() noexcept
	{
		return Prop->IsA<T>();
	}

public:

	PropertyHandler operator=(const PropertyHandler& Other) = delete;
	template<typename T>
	PropertyHandler operator=(const T& NewV)
	{
		if( Prop ) delete Prop;
		Prop = new TProperty(Val);
		return *this;
	}



private:

	Property* Prop = nullptr;
};



template<typename U, typename V>
inline bool operator==(const TProperty<U>& A, const TProperty<V>& B) noexcept
{
	return A.GetTyped() == B.GetTyped();
}
template<typename U, typename V>
inline bool operator!=(const TProperty<U>& A, const TProperty<V>& B) noexcept
{
	return A.GetTyped() != B.GetTyped();
}
template<typename U, typename V>
inline bool operator<(const TProperty<U>& A, const TProperty<V>& B) noexcept
{
	return A.GetTyped() < B.GetTyped();
}
template<typename U, typename V>
inline bool operator<=(const TProperty<U>& A, const TProperty<V>& B) noexcept
{
	return A.GetTyped() <= B.GetTyped();
}
template<typename U, typename V>
inline bool operator>(const TProperty<U>& A, const TProperty<V>& B) noexcept
{
	return A.GetTyped() > B.GetTyped();
}
template<typename U, typename V>
inline bool operator>=(const TProperty<U>& A, const TProperty<V>& B) noexcept
{
	return A.GetTyped() >= B.GetTyped();
}

template<typename U, typename V>
inline bool operator==(const TProperty<U>& A, const V& B) noexcept
{
	return A.GetTyped() == B;
}
template<typename U, typename V>
inline bool operator!=(const TProperty<U>& A, const V& B) noexcept
{
	return A.GetTyped() != B;
}
template<typename U, typename V>
inline bool operator<(const TProperty<U>& A, const V& B) noexcept
{
	return A.GetTyped() < B;
}
template<typename U, typename V>
inline bool operator<=(const TProperty<U>& A, const V& B) noexcept
{
	return A.GetTyped() <= B;
}
template<typename U, typename V>
inline bool operator>(const TProperty<U>& A, const V& B) noexcept
{
	return A.GetTyped() > B;
}
template<typename U, typename V>
inline bool operator>=(const TProperty<U>& A, const V& B) noexcept
{
	return A.GetTyped() >= B;
}

template<typename U, typename V>
inline bool operator==(const U& A, const TProperty<V>& B) noexcept
{
	return A == B.GetTyped();
}
template<typename U, typename V>
inline bool operator!=(const U& A, const TProperty<V>& B) noexcept
{
	return A != B.GetTyped();
}
template<typename U, typename V>
inline bool operator<(const U& A, const TProperty<V>& B) noexcept
{
	return A < B.GetTyped();
}
template<typename U, typename V>
inline bool operator<=(const U& A, const TProperty<V>& B) noexcept
{
	return A <= B.GetTyped();
}
template<typename U, typename V>
inline bool operator>(const U& A, const TProperty<V>& B) noexcept
{
	return A > B.GetTyped();
}
template<typename U, typename V>
inline bool operator>=(const U& A, const TProperty<V>& B) noexcept
{
	return A >= B.GetTyped();
}