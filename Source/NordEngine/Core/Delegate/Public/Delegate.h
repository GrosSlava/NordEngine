
#pragma once

#include "DelegateHandler.h"

#include <vector>




/*
	NOTE! All delegates have AddDelegateHandler(bind) and RemoveDelegateHandler(unbind).
	All your objects should unbind from binded delegates on it's destructor.
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

	FORCEINLINE void AddEventHandler(TBaseDelegateHandler<ParamTypes...>* EventHandler)
	{
		check(EventHandler != nullptr)

		Handlers.push_back(EventHandler);
	}

	template<class TObject>
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...))
	{
		check(Object != nullptr)

		Handlers.push_back(new TDelegateHandler<TObject, ParamTypes...>(Object, Method));
	}


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

	std::vector<TBaseDelegateHandler<ParamTypes...>*> Handlers;

	/* true while Broadcasting. */
	bool IsBroadcasting = false;
	/* Marks that we will clear all invalid handlers at first available moment. */
	bool NeedToClearInvalidHandlers = false;
	/* Count of broadcasts calls at one moment. */
	uint32 BrodcastCallsNum = 0;
};




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

	FORCEINLINE void AddEventHandler(TBaseDelegateHandler<ParamTypes...>* EventHandler)
	{
		check(EventHandler != nullptr)
		if( Handler != nullptr ) return;

		Handler = EventHandler;
	}

	template<class TObject>
	FORCEINLINE void AddEventHandler(TObject* Object, void (TObject::*Method)(ParamTypes...))
	{
		check(Object != nullptr)
		if( Handler != nullptr ) return;

		Handler = new TDelegateHandler<TObject, ParamTypes...>(Object, Method);
	}


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

	FORCEINLINE void Clear() 
	{ 
		RemoveEventHandler();
	}



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

	TBaseDelegateHandler<ParamTypes...>* Handler;

	/* true while Broadcasting. */
	bool IsBroadcasting = false;
	/* Marks that we will clear all invalid handlers at first available moment. */
	bool NeedToClearInvalidHandlers = false;
	/* Count of broadcasts calls at one moment. */
	uint32 BrodcastCallsNum = 0;
};