
#pragma once

#include "DelegateHandler.h"

#include <vector>




/*
	NOTE! All delegates have AddDelegateHandler(bind) and RemoveDelegateHandler(unbind).
	All your objects should unbind from binded delegates on it's destructor.
*/



/*
	Multicast delegate.
	Support multiple subscribers.
*/
template<typename... ParamTypes>
class TDelegate
{
public:

	FORCEINLINE TDelegate() { }

	~TDelegate()
	{
		for( auto LHandler : Handlers )
		{
			delete LHandler;
		}
	}



public:

	/*
		Add new event handler. Not check for unique.
	*/
	FORCEINLINE void AddEventHandler(TBaseDelegateHandler<ParamTypes...>* EventHandler)
	{
		ensure(EventHandler != nullptr)

		Handlers.push_back(EventHandler);
	}
	/*
		Add new event handler. Not check for unique.
	*/
	template<class TObject>
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...))
	{
		ensure(Object != nullptr)

		Handlers.push_back(new TDelegateHandler<TObject, ParamTypes...>(Object, Method));
	}

	/*
		Remove event handler if it exists.
	*/
	template<class TObject>
	void RemoveEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...))
	{
		ensure(Object != nullptr)

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
			std::remove_if
			(
				Handlers.begin(), Handlers.end(),
				[&](TBaseDelegateHandler<ParamTypes...>* LHandler)
				{
					if( !LHandler->IsValid() )
					{
						delete LHandler;
						return true;
					}
					return false;
				}
			);
		}
	}
	/*
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

			Handlers.clear();
		}
	}


	/*
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
	/*
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

	FORCEINLINE void StartBroadcasting()
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

			std::remove_if
			(
				Handlers.begin(), Handlers.end(),
				[&](const TBaseDelegateHandler<ParamTypes...>* LHandler)
				{
					if( !LHandler->IsValid() )
					{
						delete LHandler;
						return true;
					}
					return false;
				}
			);
		}


		IsBroadcasting = false;
	}
	



private:

	/*
		Array of subscribed objects.
	*/
	std::vector<TBaseDelegateHandler<ParamTypes...>*> Handlers;

	/* 
		True while Broadcasting.
	*/
	bool IsBroadcasting = false;
	/*
		Marks that we will clear all invalid handlers at first available moment.
	*/
	bool NeedToClearInvalidHandlers = false;
	/* 
		Count of broadcasts calls at one moment.
	*/
	uint32 BrodcastCallsNum = 0;
};


/*
	Multicast delegate only for objects with specific type.
	Support multiple subscribers.
*/
template<class TObject, typename... ParamTypes>
class TObjectDelegate
{
public:

	FORCEINLINE TObjectDelegate() { }
	~TObjectDelegate() { }



public:

	/*
		Add new event handler. Not check for unique.
	*/
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::* Method)(ParamTypes...)) 	{ Delegate.AddEventHandler(Object, Method);	}
	/*
		Remove event handler if it exists.
	*/
	FORCEINLINE void RemoveEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...)) { Delegate.RemoveEventHandler(Object, Method); }
	/*
		Remove all event handlers.
	*/
	FORCEINLINE void Clear() { Delegate.Clear(); }

	/*
		Call all subscribed object's methods.
	*/
	FORCEINLINE void Broadcast(ParamTypes... Params) { Delegate.Broadcast(Params...); }
	/*
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

	/*
		Wrapping delegate.
	*/
	TDelegate<ParamTypes...> Delegate;
};




/*
	Single delegate.
	Support one subscriber.
*/
template<typename... ParamTypes>
class TSingleDelegate
{
public:

	FORCEINLINE TSingleDelegate() { }

	~TSingleDelegate()
	{
		if( Handler != nullptr )
		{
			delete Handler;
			Handler = nullptr;
		}
	}



public:

	/*
		Set event handler if it is empty.
	*/
	FORCEINLINE void AddEventHandler(TBaseDelegateHandler<ParamTypes...>* EventHandler)
	{
		ensure(EventHandler != nullptr)
		if( Handler != nullptr ) return;

		Handler = EventHandler;
	}
	/*
		Set event handler if it is empty.
	*/
	template<class TObject>
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...))
	{
		ensure(Object != nullptr)
		if( Handler != nullptr ) return;

		Handler = new TDelegateHandler<TObject, ParamTypes...>(Object, Method);
	}

	/*
		Clear event handler.
	*/
	FORCEINLINE void RemoveEventHandler()
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
	/*
		Clear event handler.
	*/
	FORCEINLINE void Clear() 
	{ 
		RemoveEventHandler();
	}


	/*
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
	/*
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

	FORCEINLINE void StartBroadcasting()
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
			if( Handler != nullptr )
			{
				delete Handler;
				Handler = nullptr;
			}
		}

		IsBroadcasting = false;
	}




private:

	/*
		Current subscriber.
	*/
	TBaseDelegateHandler<ParamTypes...>* Handler;

	/* 
		True while Broadcasting.
	*/
	bool IsBroadcasting = false;
	/* 
		Marks that we will clear all invalid handlers at first available moment. 
	*/
	bool NeedToClearInvalidHandlers = false;
	/* 
		Count of broadcasts calls at one moment.
	*/
	uint32 BrodcastCallsNum = 0;
};


/*
	Single delegate of specific obkect.
	Support one subscriber.
*/
template<class TObject, typename... ParamTypes>
class TObjectSingleDelegate
{
public:

	FORCEINLINE TObjectSingleDelegate() { }
	~TObjectSingleDelegate() { }



public:

	/*
		Set event handler if it is empty.
	*/
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...)) { Delegate.AddEventHandler(Object, Method); }
	/*
		Clear event handler.
	*/
	FORCEINLINE void RemoveEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...)) { Delegate.RemoveEventHandler(Object, Method); }
	/*
		Clear event handler.
	*/
	FORCEINLINE void Clear() { Delegate.Clear(); }

	/*
		Call subscribed object method.
	*/
	FORCEINLINE void Broadcast(ParamTypes... Params) { Delegate.Broadcast(Params...); }
	/*
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

	/*
		Wrapping delegate.
	*/
	TSingleDelegate<ParamTypes...> Delegate;
};