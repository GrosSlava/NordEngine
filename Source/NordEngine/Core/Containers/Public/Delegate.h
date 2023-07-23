// Copyright Nord Engine. All Rights Reserved.
#pragma once

#include "GenericPlatform.h"
#include "AssertionMacros.h"
#include "Array.h"




/*
	NOTE! All delegates have AddDelegateHandler(bind) and RemoveDelegateHandler(unbind).
	All your objects should unbind from binded delegates on it's destructor.
*/


/**
	Wrapper around user data for storing in delegate.
*/
struct FDelegateStructuredData
{
public:

	FDelegateStructuredData() = default;
	~FDelegateStructuredData()
	{
		if( Data ) delete Data;
	}


public:

	/**
		@return current stored data.
	*/
	FORCEINLINE void* GetData() const noexcept { return Data; }
	/**
		Construct new data.
	*/
	template<typename T, typename... Params>
	FORCEINLINE void SetData(Params... Args)
	{
		if( Data ) delete Data;
		Data = new T(Args...);
	}



private:

	/**
		Stored data.
	*/
	void* Data = nullptr;
};

template<typename... ParamTypes>
class TBaseDelegateHandler
{
public:

	TBaseDelegateHandler() = default;
	virtual ~TBaseDelegateHandler() = default;


public:

	virtual bool IsValid() const noexcept = 0;
	virtual void Invalidate() noexcept = 0;
	virtual bool IsEqual(TBaseDelegateHandler<ParamTypes...>* Other) const noexcept = 0;
	virtual void Call(ParamTypes... Params) = 0;

public:

	/**
		Access user data.
	*/
	FORCEINLINE FDelegateStructuredData GetStructuredData() const noexcept { return StructuredData; }
	/**
		Set new user data.
		Old one will be destroyed.
	*/
	FORCEINLINE void SetStructuredData(FDelegateStructuredData NewData) noexcept { StructuredData = NewData; }



private:

	/**
		Stored user data.
	*/
	FDelegateStructuredData StructuredData;
};

template<class TObject, typename... ParamTypes>
class TDelegateHandler : public TBaseDelegateHandler<ParamTypes...>
{
	using MyType = TDelegateHandler<TObject, ParamTypes...>;
	using TMethod = void (TObject::*)(ParamTypes...);

public:

	TDelegateHandler() = delete;
	FORCEINLINE TDelegateHandler(TObject* InObject, TMethod InMethod) : Object(InObject), Method(InMethod) { Invalidate(); }


public:

	virtual bool IsValid() const noexcept override { return !WasInvalidated && Object != nullptr; }
	virtual void Invalidate() noexcept override { WasInvalidated = true; }

	virtual bool IsEqual(TBaseDelegateHandler<ParamTypes...>* Other) const noexcept override
	{
		if( Other == nullptr ) return false;
		MyType* AsDelegateHandler = dynamic_cast<MyType*>(Other);

		return Object == AsDelegateHandler->Object && Method == AsDelegateHandler->Method;
	}

	virtual void Call(ParamTypes... Params) override
	{
		check(Object != nullptr);
		(Object->*Method)(Params...);
	}

public:

	FORCEINLINE TObject* GetObject() const noexcept { return Object; }



private:

	/**
		Object, which method we handle.
	*/
	TObject* Object = nullptr;
	/**
		Object's method? which we handle.
	*/
	TMethod Method;
	/**
		Marks that this handle is invalid.
	*/
	bool WasInvalidated = false;
};




/**
	Multicast delegate.
	Support multiple subscribers.
*/
template<typename... ParamTypes>
class TDelegate
{
public:

	TDelegate() = default;
	~TDelegate()
	{
		for( auto LHandler : Handlers )
		{
			if( LHandler ) delete LHandler;
		}
		Handlers.Reset();
	}


public:

	/**
		Add new event handler. Not check for unique.
	*/
	FORCEINLINE void AddEventHandler(TBaseDelegateHandler<ParamTypes...>* EventHandler)
	{
		check(EventHandler != nullptr)
		Handlers.Add(EventHandler);
	}
	/**
		Add new event handler. Not check for unique.
	*/
	template<class TObject>
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...))
	{
		check(Object != nullptr)
		Handlers.Add(new TDelegateHandler<TObject, ParamTypes...>(Object, Method));
	}

	/**
		Remove event handler if it exists.
	*/
	template<class TObject>
	void RemoveEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...))
	{
		check(Object != nullptr)

		TDelegateHandler<TObject, ParamTypes...> TmpHandler(Object, Method);

		if( IsBroadcasting )
		{
			for( auto LHandler : Handlers )
			{
				if( LHandler->IsEqual(&TmpHandler) )
				{
					LHandler->Invalidate();
				}
			}

			NeedToClearInvalidHandlers = true;
		}
		else
		{
			Handlers.RemoveAllSwap(
				[&](TBaseDelegateHandler<ParamTypes...>* LHandler)
				{
					if( !LHandler->IsValid() )
					{
						delete LHandler;
						return true;
					}
					return false;
				});
		}
	}
	/**
		Remove all event handlers.
	*/
	FORCEINLINE void Clear()
	{
		if( IsBroadcasting )
		{
			for( auto LHandler : Handlers )
			{
				LHandler->Invalidate();
			}

			NeedToClearInvalidHandlers = true;
		}
		else
		{
			for( auto LHandler : Handlers )
			{
				delete LHandler;
			}
			Handlers.Clear();
		}
	}

	/**
		Call all subscribed object's methods.
	*/
	FORCEINLINE void Broadcast(ParamTypes... Params)
	{
		StartBroadcasting();

		for( auto LHandler : Handlers )
		{
			if( LHandler->IsValid() )
			{
				LHandler->Call(Params...);
			}
		}

		EndBroadcasting();
	}
	/**
		Call all subscribed object's methods.

		@param Condition - lambda wich takes Handler and return bool.
		e.g [&](TBaseDelegateHandler<ParamTypes...>* LDelegate){ return true; }
	*/
	template<typename Predicate>
	FORCEINLINE void Broadcast(ParamTypes... Params, Predicate Condition)
	{
		StartBroadcasting();

		for( auto LHandler : Handlers )
		{
			if( LHandler->IsValid() && Condition(LHandler) )
			{
				LHandler->Call(Params...);
			}
		}

		EndBroadcasting();
	}

private:

	FORCEINLINE void StartBroadcasting() noexcept
	{
		IsBroadcasting = true;
		++BrodcastCallsNum;
	}

	void EndBroadcasting()
	{
		--BrodcastCallsNum;
		if( BrodcastCallsNum != 0 ) return;

		if( NeedToClearInvalidHandlers )
		{
			NeedToClearInvalidHandlers = false;

			Handlers.RemoveAllSwap(
				[&](const TBaseDelegateHandler<ParamTypes...>* LHandler)
				{
					if( !LHandler->IsValid() )
					{
						delete LHandler;
						return true;
					}
					return false;
				});
		}

		IsBroadcasting = false;
	}



private:

	/**
		Array of subscribed objects.
	*/
	TArray<TBaseDelegateHandler<ParamTypes...>*> Handlers;

	/**
		True while Broadcasting.
	*/
	bool IsBroadcasting = false;
	/**
		Marks that we will clear all invalid handlers at first available moment.
	*/
	bool NeedToClearInvalidHandlers = false;
	/**
		Count of broadcasts calls at one moment.
	*/
	uint32 BrodcastCallsNum = 0;
};

/**
	Multicast delegate only for objects with specific type.
	Support multiple subscribers.
*/
template<class TObject, typename... ParamTypes>
class TObjectDelegate
{
public:

	TObjectDelegate() = default;
	~TObjectDelegate() = default;


public:

	/**
		Add new event handler. Not check for unique.
	*/
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...)) { Delegate.AddEventHandler(Object, Method); }

	/**
		Remove event handler if it exists.
	*/
	FORCEINLINE void RemoveEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...)) { Delegate.RemoveEventHandler(Object, Method); }
	/**
		Remove all event handlers.
	*/
	FORCEINLINE void Clear() { Delegate.Clear(); }

	/**
		Call all subscribed object's methods.
	*/
	FORCEINLINE void Broadcast(ParamTypes... Params) { Delegate.Broadcast(Params...); }
	/**
		Call all subscribed object's methods.

		@param Condition - lambda wich takes Handler and return bool.
		e.g [&](TBaseDelegateHandler<ParamTypes...>* LDelegate){ return true; }
	*/
	template<typename Predicate>
	FORCEINLINE void Broadcast(ParamTypes... Params, Predicate Condition)
	{
		Delegate.Broadcast<Predicate>(Params..., Condition);
	}



private:

	/**
		Wrapping delegate.
	*/
	TDelegate<ParamTypes...> Delegate;
};



/**
	Single delegate.
	Support one subscriber.
*/
template<typename... ParamTypes>
class TSingleDelegate
{
public:

	TSingleDelegate() = default;
	~TSingleDelegate()
	{
		if( Handler != nullptr )
		{
			delete Handler;
			Handler = nullptr;
		}
	}


public:

	/**
		Set event handler if it is empty.
	*/
	FORCEINLINE void AddEventHandler(TBaseDelegateHandler<ParamTypes...>* EventHandler) noexcept
	{
		check(EventHandler != nullptr)
		if( Handler != nullptr ) return;

		Handler = EventHandler;
	}
	/**
		Set event handler if it is empty.
	*/
	template<class TObject>
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...)) noexcept
	{
		check(Object != nullptr)
		if( Handler != nullptr ) return;

		Handler = new TDelegateHandler<TObject, ParamTypes...>(Object, Method);
	}

	/**
		Clear event handler.
	*/
	FORCEINLINE void RemoveEventHandler() noexcept
	{
		if( Handler == nullptr ) return;

		if( IsBroadcasting )
		{
			Handler->Invalidate();

			NeedToClearInvalidHandlers = true;
		}
		else
		{
			delete Handler;
			Handler = nullptr;
		}
	}
	/**
		Clear event handler.
	*/
	FORCEINLINE void Clear() noexcept { RemoveEventHandler(); }

	/**
		Call subscribed object method.
	*/
	FORCEINLINE void Broadcast(ParamTypes... Params)
	{
		if( Handler == nullptr ) return;

		StartBroadcasting();

		if( Handler->IsValid() )
		{
			Handler->Call(Params...);
		}

		EndBroadcasting();
	}
	/**
		Call subscribed object method.

		@param Condition - lambda wich takes Handler and return bool.
		e.g [&](TBaseDelegateHandler<ParamTypes...>* LDelegate){ return true; }
	*/
	template<typename Predicate>
	FORCEINLINE void Broadcast(ParamTypes... Params, Predicate Condition)
	{
		if( Handler == nullptr ) return;

		StartBroadcasting();

		if( Handler->IsValid() && Condition(Handler) )
		{
			Handler->Call(Params...);
		}

		EndBroadcasting();
	}

private:

	FORCEINLINE void StartBroadcasting() noexcept
	{
		IsBroadcasting = true;
		++BrodcastCallsNum;
	}

	void EndBroadcasting() noexcept
	{
		--BrodcastCallsNum;
		if( BrodcastCallsNum != 0 ) return;

		if( NeedToClearInvalidHandlers )
		{
			NeedToClearInvalidHandlers = false;
			if( Handler != nullptr )
			{
				delete Handler;
				Handler = nullptr;
			}
		}

		IsBroadcasting = false;
	}



private:

	/**
		Current subscriber.
	*/
	TBaseDelegateHandler<ParamTypes...>* Handler;

	/**
		True while Broadcasting.
	*/
	bool IsBroadcasting = false;
	/**
		Marks that we will clear all invalid handlers at first available moment. 
	*/
	bool NeedToClearInvalidHandlers = false;
	/**
		Count of broadcasts calls at one moment.
	*/
	uint32 BrodcastCallsNum = 0;
};

/**
	Single delegate of specific obkect.
	Support one subscriber.
*/
template<class TObject, typename... ParamTypes>
class TObjectSingleDelegate
{
public:

	TObjectSingleDelegate() = default;
	~TObjectSingleDelegate() = default;


public:

	/**
		Set event handler if it is empty.
	*/
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...)) noexcept { Delegate.AddEventHandler(Object, Method); }

	/**
		Clear event handler.
	*/
	FORCEINLINE void RemoveEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...)) noexcept { Delegate.RemoveEventHandler(Object, Method); }

	/**
		Clear event handler.
	*/
	FORCEINLINE void Clear() noexcept { Delegate.Clear(); }

	/**
		Call subscribed object method.
	*/
	FORCEINLINE void Broadcast(ParamTypes... Params) { Delegate.Broadcast(Params...); }
	/**
		Call subscribed object method.

		@param Condition - lambda wich takes Handler and return bool.
		e.g [&](TBaseDelegateHandler<ParamTypes...>* LDelegate){ return true; }
	*/
	template<typename Predicate>
	FORCEINLINE void Broadcast(ParamTypes... Params, Predicate Condition)
	{
		Delegate.Broadcast<Predicate>(Params..., Condition);
	}



private:

	/**
		Wrapping delegate.
	*/
	TSingleDelegate<ParamTypes...> Delegate;
};