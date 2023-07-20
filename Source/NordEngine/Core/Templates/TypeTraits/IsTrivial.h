// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "TypeTraits/AndOrNot.h"
#include "TypeTraits/IsTriviallyCopyConstructible.h"
#include "TypeTraits/IsTriviallyCopyAssignable.h"
#include "TypeTraits/IsTriviallyDestructible.h"




/**
	Traits class which tests if a type is trivial.
*/
template<typename T>
struct TIsTrivial
{
	enum
	{
		// clang-format off
		Value = TAnd<TIsTriviallyDestructible<T>, TIsTriviallyCopyConstructible<T>,	TIsTriviallyCopyAssignable<T>>::Value
		// clang-format on
	};
};