
#pragma once

#include "CoreMinimal.h"




/*
	Input mode.
*/
enum class EInputMode : uint8
{
	/* Only for gamelay characters when no focus to widget. */
	GameOnly,
	/* Only for widgets, when have focus to it. */
	UIOnly,
	/* For both: gameplay characters and widgets. */
	GameAndUI
};

/*
	Type of input event calling.
*/
enum class EInputCallType : uint8
{
	/* Event will happen every frame if its input is active. */
	EveryFrameWhenActive,
	/* Event will happen once at the moment when the physical input is activated/deactivated. */
	OnlyWhenHappends
};





/*
	Input data staged in input delegate.
*/
struct FInputDelegateHandlerData
{
public:

	FORCEINLINE FInputDelegateHandlerData(EInputMode InInputMode, EInputCallType InInputCallType) : InputMode(InInputMode), InputCallType(InInputCallType) { }



public:

	EInputMode InputMode;
	EInputCallType InputCallType;
	bool IsInputEnabled = true;
};



/*
	Helper struct for input types.
*/
struct FKeyInputInfoHelper
{
public:

	/*
		@return true if InputData marks that input can be broadcasted.
	*/
	static FORCEINLINE bool CanInputHandlerBroadcast(FInputDelegateHandlerData* InputData, EInputMode CurrentInputMode) noexcept
	{
		if( InputData == nullptr ) return false;

		return InputData->IsInputEnabled && IsInputableMode(InputData->InputMode, CurrentInputMode);
	}

	/*
		@return true if Mode can be used for CurrentInputMode
	*/
	static FORCEINLINE bool IsInputableMode(EInputMode Mode, EInputMode CurrentInputMode) noexcept
	{
		// clang-format off
		return Mode == CurrentInputMode || Mode == EInputMode::GameAndUI || CurrentInputMode == EInputMode::GameAndUI;
		// clang-format on
	}
};





//..............................................Keyboard.............................................................//

enum class EKeyBindType : uint8
{
	KeyDown = 0,
	KeyUp = 1,
	
	Count
};


struct FKeyInputInfo
{
public:

	template<class TObject>
	FORCEINLINE void BindKey(EKeyBindType KeyBindType, EInputMode InputMode, EInputCallType InputCallType, TObject* Object, void (TObject::*Method)())
	{
		if( KeyBindType == EKeyBindType::Count ) return;

		auto LDelegate = new TDelegateHandler<TObject>(Object, Method);
		LDelegate->SetStructuredData(new FInputDelegateHandlerData(InputMode, InputCallType));

		KeyBindDelegates[static_cast<uint8>(KeyBindType)].AddEventHandler(LDelegate);
	}

	template<class TObject>
	FORCEINLINE void UnbindKey(EKeyBindType KeyBindType, TObject* Object, void (TObject::*Method)())
	{
		if( KeyBindType == EKeyBindType::Count ) return;

		KeyBindDelegates[static_cast<uint8>(KeyBindType)].RemoveEventHandler(Object, Method);
	}




	FORCEINLINE void OnActionHappend(EKeyBindType Action)
	{
		CurrentAction = Action;

		IsActionHappend = true;
	}

	FORCEINLINE void BroadcastCurrentAction(EInputMode CurrentInputMode)
	{
		if( CurrentAction == EKeyBindType::Count ) return;

		// clang-format off
		KeyBindDelegates[static_cast<uint8>(CurrentAction)].Broadcast
		(
			[this, CurrentInputMode](TBaseDelegateHandler<>* LDelegate)
			{
				FInputDelegateHandlerData* LDelegateData = static_cast<FInputDelegateHandlerData*>(LDelegate->GetStructuredData());

				if( !FKeyInputInfoHelper::CanInputHandlerBroadcast(LDelegateData, CurrentInputMode) ) return false;

				if(
					(LDelegateData->InputCallType == EInputCallType::OnlyWhenHappends && !IsActionHappend) ||
					(LDelegateData->InputCallType == EInputCallType::EveryFrameWhenActive && !GetIsActive()) 
					) return false;

				return true;
			}
		);
		// clang-format on

		IsActionHappend = false;
	}



	FORCEINLINE bool GetIsActive() const noexcept { return CurrentAction == EKeyBindType::KeyDown; }




private:

	bool IsActionHappend = false;
	EKeyBindType CurrentAction = EKeyBindType::Count;

	TDelegate<> KeyBindDelegates[static_cast<uint8>(EKeyBindType::Count)];
};

//...................................................................................................................//





//.................................................Mouse.............................................................//

enum class EMouseKeyBindType : uint8
{
	KeyDown = 0,
	KeyUp = 1,
	DoubleClick = 2,
	
	Count
};


struct FMouseKeyInputInfo
{
public:

	template<class TObject>
	FORCEINLINE void BindKey(EMouseKeyBindType MouseKeyBindType, EInputMode InputMode, EInputCallType InputCallType, TObject* Object, void (TObject::*Method)(uint16, uint16))
	{
		if( MouseKeyBindType == EMouseKeyBindType::Count ) return;

		auto LDelegate = new TDelegateHandler<TObject, uint16, uint16>(Object, Method);
		LDelegate->SetStructuredData(new FInputDelegateHandlerData(InputMode, InputCallType));

		MouseKeyBindDelegates[static_cast<uint8>(MouseKeyBindType)].AddEventHandler(LDelegate);
	}

	template<class TObject>
	FORCEINLINE void UnbindKey(EMouseKeyBindType MouseKeyBindType, TObject* Object, void (TObject::*Method)(uint16, uint16))
	{
		if( MouseKeyBindType == EMouseKeyBindType::Count ) return;

		MouseKeyBindDelegates[static_cast<uint8>(MouseKeyBindType)].RemoveEventHandler(Object, Method);
	}




	FORCEINLINE void OnActionHappend(EMouseKeyBindType Action, uint16 X, uint16 Y)
	{
		CurrentAction = Action;
		StoredX = X;
		StoredY = Y;

		IsActionHappend = true;
	}

	FORCEINLINE void BroadcastCurrentAction(EInputMode CurrentInputMode)
	{
		if( CurrentAction == EMouseKeyBindType::Count ) return;

		// clang-format off
		MouseKeyBindDelegates[(uint8)CurrentAction].Broadcast
		(
			StoredX, StoredY,
			[this, CurrentInputMode](TBaseDelegateHandler<uint16, uint16>* LDelegate)
			{
				FInputDelegateHandlerData* LDelegateData = static_cast<FInputDelegateHandlerData*>(LDelegate->GetStructuredData());

				if( !FKeyInputInfoHelper::CanInputHandlerBroadcast(LDelegateData, CurrentInputMode) ) return false;

				if(
					(LDelegateData->InputCallType == EInputCallType::OnlyWhenHappends && !IsActionHappend) ||
					(LDelegateData->InputCallType == EInputCallType::EveryFrameWhenActive && !GetIsActive()) 
					) return false;

				return true;
			}
		);
		// clang-format on

		IsActionHappend = false;
	}



	FORCEINLINE bool GetIsActive() const noexcept { return CurrentAction == EMouseKeyBindType::KeyDown; }




private:

	bool IsActionHappend = false;
	EMouseKeyBindType CurrentAction = EMouseKeyBindType::Count;
	uint16 StoredX = 0;
	uint16 StoredY = 0;


	TDelegate<uint16, uint16> MouseKeyBindDelegates[static_cast<uint8>(EMouseKeyBindType::Count)];
};

//...................................................................................................................//