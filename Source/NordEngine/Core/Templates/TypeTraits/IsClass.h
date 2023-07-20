// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	Determines if T is a struct/class type.
*/
template<typename T>
struct TIsClass
{
private:

	template<typename U>
	static uint16 Func(int U::*);
	template<typename U>
	static uint8 Func(...);

public:

	enum
	{
		Value = !__is_union(T) && sizeof(Func<T>(0)) - 1
	};
};



/**
	Chooses between two different classes based on a boolean.
*/
template<bool Predicate, typename TrueClass, typename FalseClass>
class TChooseClass;

template<typename TrueClass, typename FalseClass>
class TChooseClass<true, TrueClass, FalseClass>
{
public:

	typedef TrueClass Result;
};

template<typename TrueClass, typename FalseClass>
class TChooseClass<false, TrueClass, FalseClass>
{
public:

	typedef FalseClass Result;
};