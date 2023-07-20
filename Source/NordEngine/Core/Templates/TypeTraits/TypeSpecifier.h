// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	Traits class which tests if a type is const.
*/
template<typename T>
struct TIsConst
{
	static constexpr bool Value = false;
};
template<typename T>
struct TIsConst<const T>
{
	static constexpr bool Value = true;
};



/**
	Returns a non-const reference type as const.
*/
template<typename T>
constexpr inline const T& AsConst(T& Ref)
{
	return Ref;
}
/**
	Disallowed for rvalue references because it cannot extend their lifetime.
*/
template<typename T>
void AsConst(const T&& Ref) = delete;
/**
	Returns a non-const reference type as const.
	This overload is only required until the pointer overloads are removed.
*/
template<typename T, size_t N>
constexpr inline const T (&AsConst(T (&Array)[N]))[N]
{
	return Array;
}



// clang-format off

/**
	TRemoveConst<> is modeled after boost's implementation.  It allows you to take a templatized type
	such as 'const Foo*' and use const_cast to convert that type to 'Foo*' without knowing about Foo.
 
	MutablePtr = const_cast< RemoveConst< ConstPtrType >::Type >( ConstPtr );
*/
template<class T> struct TRemoveConst			{ typedef T Type; };
template<class T> struct TRemoveConst<const T>	{ typedef T Type; };

/**
	TIsReferenceType
*/
template<typename T> struct TIsReferenceType		{ enum{ Value = false }; };
template<typename T> struct TIsReferenceType<T&>	{ enum{	Value = true }; };
template<typename T> struct TIsReferenceType<T&&>	{ enum{ Value = true}; };

/**
	TIsLValueReferenceType
*/
template<typename T> struct TIsLValueReferenceType		{ enum{ Value = false }; };
template<typename T> struct TIsLValueReferenceType<T&>	{ enum{ Value = true }; };

/**
	TIsRValueReferenceType
*/
template<typename T> struct TIsRValueReferenceType		{ enum{ Value = false }; };
template<typename T> struct TIsRValueReferenceType<T&&>	{ enum{ Value = true}; };

/**
	TRValueToLValueReference converts any rvalue reference type into the equivalent lvalue reference, otherwise returns the same type.
*/
template<typename T> struct TRValueToLValueReference		{ typedef T Type; };
template<typename T> struct TRValueToLValueReference<T&&>	{ typedef T& Type; };

/**
	TRemoveReference<type> will remove any references from a type.
*/
template<typename T> struct TRemoveReference		{ typedef T Type; };
template<typename T> struct TRemoveReference<T&>	{ typedef T Type; };
template<typename T> struct TRemoveReference<T&&>	{ typedef T Type; };

/**
	Traits class which tests if a type is a pointer.
*/
template<typename T> struct TIsPointer						{ enum{ Value = false }; };
template<typename T> struct TIsPointer<T*>					{ enum{ Value = true }; };
template<typename T> struct TIsPointer<const T>				{ enum{ Value = TIsPointer<T>::Value }; };
template<typename T> struct TIsPointer<volatile T>			{ enum{ Value = TIsPointer<T>::Value }; };
template<typename T> struct TIsPointer<const volatile T>	{ enum{ Value = TIsPointer<T>::Value }; };

/**
	Removes one level of pointer from a type, e.g.:
 
	TRemovePointer<      int32  >::Type == int32
	TRemovePointer<      int32* >::Type == int32
	TRemovePointer<      int32**>::Type == int32*
	TRemovePointer<const int32* >::Type == const int32
*/
template<typename T> struct TRemovePointer		{ typedef T Type;};
template<typename T> struct TRemovePointer<T*>	{ typedef T Type; };

// clang-format on