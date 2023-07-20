// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	Traits class which tests if a type is POD.
*/
template<typename T>
struct TIsPODType
{
	enum
	{
		Value = __is_pod(T)
	};
};
