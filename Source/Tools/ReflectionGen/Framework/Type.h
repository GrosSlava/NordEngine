// CppReflectionGen framework.
#pragma once

#include <cstdint>




using TypeID = uintptr_t;

template<typename T>
struct TType
{
public:

	TType(TType&&) = delete;
	TType(const TType&) = delete;
	TType& operator=(const TType&) = delete;
	TType& operator=(TType&&) = delete;

private:

	TType() = default;

public:

	static TType* Get() noexcept
	{
		static TType Type;
		return &Type;
	}

	static TypeID ID() noexcept { return reinterpret_cast<TypeID>(Get()); }
	static T* Cast(void* V) noexcept { return static_cast<T*>(V); }
};


template<>
struct TType<void>
{
public:

	TType(TType&&) = delete;
	TType(const TType&) = delete;
	TType& operator=(const TType&) = delete;
	TType& operator=(TType&&) = delete;

private:

	TType() = default;

public:

	static TypeID ID() noexcept { return 0; }
};
