
#pragma once




/*
	Traits class which tests if a type has a trivial copy constructor.
*/
template<typename T>
struct TIsTriviallyCopyConstructible
{
	enum
	{
		Value = __has_trivial_copy(T)
	};
};