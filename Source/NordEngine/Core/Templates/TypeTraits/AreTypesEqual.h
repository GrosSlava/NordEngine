// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	Tests whether two typenames refer to the same type.
*/
template<typename A, typename B>
struct TAreTypesEqual;

template<typename, typename>
struct TAreTypesEqual
{
	enum
	{
		Value = false
	};
};

template<typename A>
struct TAreTypesEqual<A, A>
{
	enum
	{
		Value = true
	};
};



/**
	TIsSame
 
	Implementation of std::is_same trait.
*/
template<typename A, typename B>
struct TIsSame
{
	enum
	{
		Value = false
	};
};
template<typename T>
struct TIsSame<T, T>
{
	enum
	{
		Value = true
	};
};