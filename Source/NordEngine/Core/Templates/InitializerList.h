// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"




/**
	Std::initializer_list analog.
*/
template<typename T>
struct TInitializerList
{
public:

	FORCEINLINE constexpr TInitializerList() noexcept : First(nullptr), Last(nullptr) { }
	FORCEINLINE constexpr TInitializerList(const T* InFirst, const T* InLast) noexcept : First(InFirst), Last(InLast) { }


public:

	constexpr const T* begin() const noexcept { return First; }
	constexpr const T* end() const noexcept { return Last; }

public:

	FORCEINLINE constexpr SIZE_T Size() const noexcept { return static_cast<SIZE_T>(Last - First); }



private:

	const T* First = nullptr;
	const T* Last = nullptr;
};