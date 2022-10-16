
#pragma once




/*
	Traits class which tests if a type has a trivial copy assignment operator.
*/
template<typename T>
struct TIsTriviallyCopyAssignable
{
	enum
	{
		Value = __has_trivial_assign(T)
	};
};
