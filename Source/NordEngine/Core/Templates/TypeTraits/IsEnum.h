
#pragma once

#include "TypeTraits/AndOrNot.h"




template<typename T>
struct TIsEnum
{
	enum
	{
		Value = __is_enum(T)
	};
};




namespace IsEnumClass_Private
{
template<typename T>
struct TIsEnumConvertibleToInt
{
	static char (&Resolve(int))[2];
	static char Resolve(...);

	enum
	{
		Value = sizeof(Resolve(T())) - 1
	};
};
} // namespace IsEnumClass_Private

/*
	Traits class which tests if a type is arithmetic.
*/
template<typename T>
struct TIsEnumClass
{
	enum
	{
		Value = TAndValue< __is_enum(T), TNot<IsEnumClass_Private::TIsEnumConvertibleToInt<T>> >::Value
	};
};
