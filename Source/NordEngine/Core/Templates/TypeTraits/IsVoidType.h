// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	TIsVoidType
*/
template<typename T>
struct TIsVoidType
{
	enum
	{
		Value = false
	};
};
// clang-format off
template<> struct TIsVoidType<void>					{ enum { Value = true }; };
template<> struct TIsVoidType<void const>			{ enum { Value = true }; };
template<> struct TIsVoidType<void volatile>		{ enum { Value = true }; };
template<> struct TIsVoidType<void const volatile>	{ enum { Value = true }; };
// clang-format on