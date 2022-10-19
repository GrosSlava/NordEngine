
#pragma once

#include "GenericPlatform.h"

#include "TypeTraits/TypeTraits.h"
#include "TypeTraits/TypeSpecifier.h"
#include "TypeTraits/IsEnum.h"
#include "TypeTraits/AndOrNot.h"
#include "TypeTraits/EnableIf.h"
#include "TypeTraits/IsArithmetic.h"
#include "TypeTraits/CallTraits.h"
#include "TypeCompatibleBytes.h"

#include "EngineMemory.h"




template<typename T, typename U>
struct TMoveSupportTraitsBase
{
	// Param type is not an const lvalue reference, which means it's pass-by-value, so we should just provide a single type for copying.
	// Move overloads will be ignored due to SFINAE.
	typedef U Copy;
};

template<typename T>
struct TMoveSupportTraitsBase<T, const T&>
{
	// Param type is a const lvalue reference, so we can provide an overload for moving.
	typedef const T& Copy;
	typedef T&& Move;
};

/*
	This traits class is intended to be used in pairs to allow efficient and correct move-aware overloads for generic types.
	For example:
 
	template <typename T>
	void Func(typename TMoveSupportTraits<T>::Copy Obj)
	{
		// Copy Obj here
	}
 
	template <typename T>
	void Func(typename TMoveSupportTraits<T>::Move Obj)
	{
		// Move from Obj here as if it was passed as T&&
	}
 
	Structuring things in this way will handle T being a pass-by-value type (e.g. ints, floats, other 'small' types) which
	should never have a reference overload.
*/
template<typename T>
struct TMoveSupportTraits : TMoveSupportTraitsBase<T, typename TCallTraits<T>::ParamType>
{
};







/*
	MoveTemp will cast a reference to an rvalue reference.
	This is equivalent of std::move except that it will not compile when passed an rvalue or
	const object, because we would prefer to be informed when MoveTemp will have no effect.
*/
template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
	typedef typename TRemoveReference<T>::Type CastType;

	// Validate that we're not being passed an rvalue or a const object - the former is redundant, the latter is almost certainly a mistake
	static_assert(TIsLValueReferenceType<T>::Value, "MoveTemp called on an rvalue");
	static_assert(!TAreTypesEqual<CastType&, const CastType&>::Value, "MoveTemp called on a const object");

	return (CastType &&) Obj;
}

/*
	MoveTemp will cast a reference to an rvalue reference.
	This is equivalent of std::move.  It doesn't static assert like MoveTemp, because it is useful in
	templates or macros where it's not obvious what the argument is, but you want to take advantage of move semantics
	where you can but not stop compilation.
*/
template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& MoveTempIfPossible(T&& Obj)
{
	typedef typename TRemoveReference<T>::Type CastType;
	return (CastType &&) Obj;
}





/*
	CopyTemp will enforce the creation of an rvalue which can bind to rvalue reference parameters.
	Unlike MoveTemp, the source object will never be modifed. (i.e. a copy will be made)
	There is no std:: equivalent.
*/
template<typename T>
FORCEINLINE T CopyTemp(T& Val)
{
	return const_cast<const T&>(Val);
}

template<typename T>
FORCEINLINE T CopyTemp(const T& Val)
{
	return Val;
}

template<typename T>
FORCEINLINE T&& CopyTemp(T&& Val)
{
	// If we already have an rvalue, just return it unchanged, rather than needlessly creating yet another rvalue from it.
	return MoveTemp(Val);
}





/*
	This is used to provide type specific behavior for a copy which cannot change the value of B.
*/
template<typename T>
FORCEINLINE void Move(T& A, typename TMoveSupportTraits<T>::Copy B)
{
	// Destruct the previous value of A.
	A.~T();

	// Use placement new and a copy constructor so types with const members will work.
	new(&A) T(B);
}

/*
	This is used to provide type specific behavior for a move which may change the value of B. 
*/
template<typename T>
FORCEINLINE void Move(T& A, typename TMoveSupportTraits<T>::Move B)
{
	// Destruct the previous value of A.
	A.~T();

	// Use placement new and a copy constructor so types with const members will work.
	new(&A) T(MoveTemp(B));
}







/*
	A traits class which specifies whether a Swap of a given type should swap the bits or use a traditional value-based swap.
*/
template<typename T>
struct TUseBitwiseSwap
{
	// We don't use bitwise swapping for 'register' types because this will force them into memory and be slower.
	enum
	{
		Value = !TOrValue<TIsEnum<T>, TIsPointer<T>, TIsArithmetic<T>> ::Value
	};
};


/*
	Swap two values.  Assumes the types are trivially relocatable.
*/
template<typename T>
FORCEINLINE typename TEnableIf<TUseBitwiseSwap<T>::Value>::Type Swap(T& A, T& B)
{
	if( LIKELY(&A != &B) )
	{
		TTypeCompatibleBytes<T> Temp;
		FMemory::Memcpy(&Temp, &A, sizeof(T));
		FMemory::Memcpy(&A, &B, sizeof(T));
		FMemory::Memcpy(&B, &Temp, sizeof(T));
	}
}

template<typename T>
FORCEINLINE typename TEnableIf<!TUseBitwiseSwap<T>::Value>::Type Swap(T& A, T& B)
{
	T Temp = MoveTemp(A);
	A = MoveTemp(B);
	B = MoveTemp(Temp);
}

template<typename T>
FORCEINLINE void Exchange(T& A, T& B)
{
	Swap(A, B);
}





/*
	Forward will cast a reference to an rvalue reference.
	This is equivalent of std::forward.
*/
template<typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type& Obj)
{
	return (T &&) Obj;
}

template<typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type&& Obj)
{
	return (T &&) Obj;
}
