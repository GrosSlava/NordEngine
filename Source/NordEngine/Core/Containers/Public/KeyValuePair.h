
#pragma once




/*
	Helper class to make it easy to use key/value pairs with a container.
*/
template<typename KeyType, typename ValueType>
struct TKeyValuePair
{
public:

	TKeyValuePair() { }
	TKeyValuePair(const KeyType& InKey, const ValueType& InValue) : Key(InKey), Value(InValue) { }
	TKeyValuePair(const KeyType& InKey) : Key(InKey) { }



public:

	bool operator==(const TKeyValuePair& Other) const { return Key == Other.Key; }
	bool operator!=(const TKeyValuePair& Other) const { return Key != Other.Key; }
	bool operator<(const TKeyValuePair& Other) const { return Key < Other.Key; }
	inline bool operator()(const TKeyValuePair& A, const TKeyValuePair& B) const { return A.Key < B.Key; }


public:

	KeyType Key;
	ValueType Value;
};