// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	Tests is a type is a function.
*/
template<typename T>
struct TIsFunction
{
	enum
	{
		Value = false
	};
};

template<typename RetType, typename... Params>
struct TIsFunction<RetType(Params...)>
{
	enum
	{
		Value = true
	};
};
