// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "MoveSemantic.h"




/**
	Helper class to make it easy to use key/value pairs with a container.
*/
template<typename KeyType, typename ValueType>
struct TKeyValuePair
{
public:

	TKeyValuePair() = default;
	FORCEINLINE TKeyValuePair(KeyType InKey, ValueType InValue) noexcept : Key(MoveTemp(InKey)), Value(MoveTemp(InValue)) { }
	FORCEINLINE TKeyValuePair(KeyType&& InKey, ValueType&& InValue) noexcept : Key(MoveTemp(InKey)), Value(MoveTemp(InValue)) { }
	FORCEINLINE TKeyValuePair(KeyType InKey) noexcept : Key(MoveTemp(InKey)) { }
	FORCEINLINE TKeyValuePair(KeyType&& InKey) noexcept : Key(MoveTemp(InKey)) { }
	~TKeyValuePair() = default;


public:

	FORCEINLINE bool operator==(const TKeyValuePair& Other) const noexcept { return Key == Other.Key; }
	FORCEINLINE bool operator!=(const TKeyValuePair& Other) const noexcept { return Key != Other.Key; }
	FORCEINLINE bool operator<(const TKeyValuePair& Other) const noexcept { return Key < Other.Key; }
	FORCEINLINE bool operator()(const TKeyValuePair& A, const TKeyValuePair& B) const noexcept { return A.Key < B.Key; }



public:

	KeyType Key;
	ValueType Value;
};