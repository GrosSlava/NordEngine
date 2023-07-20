// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"

#include "TypeTraits/IsVoidType.h"
#include "TypeTraits/IsEnum.h"
#include "TypeTraits/IsArithmetic.h"
#include "TypeTraits/AndOrNot.h"
#include "TypeTraits/TypeSpecifier.h"
#include "TypeTraits/CallTraits.h"
#include "RemoveCV.h"




/**
	Gets the Nth type in a template parameter pack. N must be less than sizeof...(Types). 
*/
template<int32 N, typename... Types>
struct TNthTypeFromParameterPack;

template<int32 N, typename T, typename... OtherTypes>
struct TNthTypeFromParameterPack<N, T, OtherTypes...>
{
	using Type = typename TNthTypeFromParameterPack<N - 1, OtherTypes...>::Type;
};

template<typename T, typename... OtherTypes>
struct TNthTypeFromParameterPack<0, T, OtherTypes...>
{
	using Type = T;
};



/**
	TIsFundamentalType
*/
template<typename T>
struct TIsFundamentalType
{
	// clang-format off
	enum { Value = TOr<TIsArithmetic<T>, TIsVoidType<T>>::Value };
	// clang-format on
};



/**
	TIsZeroConstructType
*/
template<typename T>
struct TIsZeroConstructType
{
	// clang-format off
	enum { Value = TOr<TIsEnum<T>, TIsArithmetic<T>, TIsPointer<T>>::Value };
	// clang-format on
};



/**
	Helper for array traits. Provides a common base to more easily refine a portion of the traits
	when specializing. Mainly used by MemoryOps.h.
*/
template<typename T>
struct TTypeTraitsBase
{
	typedef typename TCallTraits<T>::ParamType ConstInitType;
	typedef typename TCallTraits<T>::ConstPointerType ConstPointerType;

	// There's no good way of detecting this so we'll just assume it to be true for certain known types and expect
	// users to customize it for their custom types.
	enum
	{
		// clang-format off
		IsBytewiseComparable = TOr<TIsEnum<T>, TIsArithmetic<T>, TIsPointer < T >> ::Value
		// clang-format on
	};
};
/**
	Traits for types.
*/
template<typename T>
struct TTypeTraits : public TTypeTraitsBase<T>
{
};
