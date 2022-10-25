
#pragma once

#include "TypeTraits/AndOrNot.h"
#include "TypeTraits/IsArithmetic.h"
#include "TypeTraits/IsPODType.h"




/*
	Call traits helpers.
*/
template<typename T, bool TypeIsSmall>
struct TCallTraitsParamTypeHelper
{
	typedef const T& ParamType;
	typedef const T& ConstParamType;
};

template<typename T>
struct TCallTraitsParamTypeHelper<T, true>
{
	typedef const T ParamType;
	typedef const T ConstParamType;
};

template<typename T>
struct TCallTraitsParamTypeHelper<T*, true>
{
	typedef T* ParamType;
	typedef const T* ConstParamType;
};



/*-----------------------------------------------------------------------------
	TCallTraits
 
	Same call traits as boost, though not with as complete a solution.
 
	The main member to note is ParamType, which specifies the optimal
	form to pass the type as a parameter to a function.
 
	Has a small-value optimization when a type is a POD type and as small as a pointer.
-----------------------------------------------------------------------------*/

/*
	Base class for call traits. Used to more easily refine portions when specializing.
*/
template<typename T>
struct TCallTraitsBase
{
private:

	enum
	{
		//(sizeof(T) <= sizeof(void*) and IsPODType) or IsArithmetic
		PassByValue = TOr<TAndValue<(sizeof(T) <= sizeof(void*)), TIsPODType<T>>, TIsArithmetic<T>>::Value
	};

public:

	typedef T ValueType;
	typedef T& Reference;
	typedef const T& ConstReference;
	typedef typename TCallTraitsParamTypeHelper<T, PassByValue>::ParamType ParamType;
	typedef typename TCallTraitsParamTypeHelper<T, PassByValue>::ConstParamType ConstPointerType;
};

/*
	TCallTraits.
*/
template<typename T>
struct TCallTraits : public TCallTraitsBase<T>
{
};

// Fix reference-to-reference problems.
template<typename T>
struct TCallTraits<T&>
{
	typedef T& ValueType;
	typedef T& Reference;
	typedef const T& ConstReference;
	typedef T& ParamType;
	typedef T& ConstPointerType;
};



// Array types
template<typename T, size_t N>
struct TCallTraits<T[N]>
{
private:

	typedef T ArrayType[N];

public:

	typedef const T* ValueType;
	typedef ArrayType& Reference;
	typedef const ArrayType& ConstReference;
	typedef const T* const ParamType;
	typedef const T* const ConstPointerType;
};


// const array types
template<typename T, size_t N>
struct TCallTraits<const T[N]>
{
private:

	typedef const T ArrayType[N];

public:

	typedef const T* ValueType;
	typedef ArrayType& Reference;
	typedef const ArrayType& ConstReference;
	typedef const T* const ParamType;
	typedef const T* const ConstPointerType;
};
