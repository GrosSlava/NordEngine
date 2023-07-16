// CppReflectionGen framework.
#pragma once

#include "Type.h"
#include "Property.h"




struct TFunction
{
public:

	TFunction() = delete;
	TFunction(unsigned int InArgsCount) : ArgsCount(InArgsCount)
	{
		Args = new Property*[ArgsCount];
		for( unsigned int i = 0; i < ArgsCount; ++i )
		{
			Args[i] = nullptr;
		}
	}
	TFunction(const TFunction& Other) = delete;
	TFunction(TFunction&& Other) noexcept : ArgsCount(Other.ArgsCount), Args(Other.Args), Return(Other.Return)
	{
		Other.ArgsCount = 0;
		Other.Args = nullptr;
		Other.Return = nullptr;
	}
	~TFunction()
	{
		if( Return ) delete Return;

		if( ArgsCount != 0 )
		{
			for( unsigned int i = 0; i < ArgsCount; ++i )
			{
				if( Args[i] ) delete Args[i];
			}
			delete[] Args;
		}
	}


public:

	inline unsigned int GetArgsCount() const noexcept { return ArgsCount; }
	inline Property* GetArg(unsigned int Index) const { return Args[Index]; }
	inline Property* GetReturn() const noexcept { return Return; }

public:

	template<typename T>
	inline void SetArg(T V, unsigned int Index)
	{
		if( Args[Index] ) delete Args[Index];
		Args[Index] = new TProperty<T>(V);
	}
	template<typename T>
	inline void SetReturn()
	{
		if( Return ) delete Return;
		Return = new TProperty<T>();
	}
	template<>
	inline void SetReturn<void>()
	{
		if( Return ) delete Return;
		Return = nullptr;
	}

	template<typename R, typename... Args>
	bool CheckSignature() const noexcept
	{
		if( sizeof...(Args) != ArgsCount ) return false;
		if( !CheckReturnType<R>() ) return false;

		unsigned int i = 0;
		bool LSuccess = true;

		// clang-format off
		(
			[&]
			{
				LSuccess &= CheckArgType<Args>(i);
				++i;
			}(),
		...);
		// clang-format on

		return LSuccess;
	}
	template<typename T>
	inline bool CheckArgType(unsigned int Index) const noexcept
	{
		return Args[Index] && Args[Index]->IsA<T>();
	}
	template<typename T>
	inline bool CheckReturnType() const noexcept
	{
		return (Return && Return->IsA<T>()) || (!Return && TType<T>::ID() == 0);
	}


private:

	unsigned int ArgsCount = 0;
	Property** Args = nullptr;
	Property* Return = nullptr;
};


template<typename R, typename... Args>
TFunction MakeFunction(Args... Values)
{
	TFunction LFunction(sizeof...(Args));

	LFunction.SetReturn<R>();

	unsigned int i = 0;
	// clang-format off
	(
		[&]
		{
			LFunction.SetArg(Values, i);
			++i;
		}(),
	...);
	// clang-format on

	return static_cast<TFunction&&>(LFunction);
}