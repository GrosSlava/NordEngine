// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"




/**
	TIsCharType.
*/
template<typename T>
struct TIsCharType
{
	enum
	{
		Value = false
	};
};
// clang-format off
template<> struct TIsCharType<ANSICHAR>	{ enum { Value = true }; };
template<> struct TIsCharType<WIDECHAR>	{ enum { Value = true }; };
// clang-format on