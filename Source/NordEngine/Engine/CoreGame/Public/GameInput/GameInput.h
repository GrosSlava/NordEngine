
#pragma once

#include "CoreMinimal.h"

#include "KeyCodes.h"
#include "GameInputInfo.h"




/*
	Main input object which controls all inputs from platform API.
*/
class ENGINE_API GGameInput final
{
	SINGLETON(GGameInput)

public:

	FORCEINLINE ~GGameInput() { }



public:

	/*
		Populate all accumulated inputs.
		Will be called on each tick in GCoreGame.
	*/
	void BroadcastInputs();


	FORCEINLINE void OnKeyDown(const FKey& Key) { OnKeyDown(Key.Key); }
	void OnKeyDown(EKey KeyCode);

	FORCEINLINE void OnKeyUp(const FKey& Key) { OnKeyUp(Key.Key); }
	void OnKeyUp(EKey KeyCode);

	void OnLeftMouseButtonDown(uint16 X, uint16 Y);
	void OnLeftMouseButtonDoubleClick(uint16 X, uint16 Y);
	void OnLeftMouseButtonUp(uint16 X, uint16 Y);

	void OnRightMouseButtonDown(uint16 X, uint16 Y);
	void OnRightMouseButtonDoubleClick(uint16 X, uint16 Y);
	void OnRightMouseButtonUp(uint16 X, uint16 Y);

	void OnMiddleMouseButtonDown(uint16 X, uint16 Y);
	void OnMiddleMouseButtonDoubleClick(uint16 X, uint16 Y);
	void OnMiddleMouseButtonUp(uint16 X, uint16 Y);

	void OnMouseMoved(uint16 NewX, uint16 NewY);
	void OnMouseWheelScrolled(int Delta);

public:

	FORCEINLINE EInputMode GetCurrentInputMode() const noexcept { return CurrentInputMode; }
	FORCEINLINE void SetInputMode(EInputMode NewInputMode) noexcept { CurrentInputMode = NewInputMode; }

	FORCEINLINE bool GetIsInputSystemEnabled() const noexcept { return IsInputSystemEnabled; }
	FORCEINLINE void SetInputSystemEnabled(bool Enable) noexcept { IsInputSystemEnabled = Enable; }


	FORCEINLINE void GetLastMouseClickPos(uint16& X, uint16& Y) const noexcept
	{
		X = LastMouseClickX;
		Y = LastMouseClickY;
	}
	FORCEINLINE void GetLastLeftMouseClickPos(uint16& X, uint16& Y) const noexcept
	{
		X = LastLeftMouseButtonClickX;
		Y = LastLeftMouseButtonClickY;
	}
	FORCEINLINE void GetLastRightMouseClickPos(uint16& X, uint16& Y) const noexcept
	{
		X = LastRightMouseButtonClickX;
		Y = LastRightMouseButtonClickY;
	}
	FORCEINLINE void GetLastMiddleMouseClickPos(uint16& X, uint16& Y) const noexcept
	{
		X = LastMiddleMouseButtonClickX;
		Y = LastMiddleMouseButtonClickY;
	}
	FORCEINLINE void GetLastMouseMovePos(uint16& X, uint16& Y) const noexcept
	{
		X = LastMousePositionX;
		Y = LastMousePositionY;
	}


	FORCEINLINE bool IsKeyPressed(EKey KeyCode) const noexcept { return KeyBinds[static_cast<uint16>(KeyCode)].GetIsActive(); }
	FORCEINLINE bool IsKeyPressed(const FKey& Key) const noexcept { return IsKeyPressed(Key.Key); }
	//FORCEINLINE bool IsMouseKeyPressed(EMouseKey MouseKey) const noexcept { return MouseButtonBind[static_cast<uint16>(MouseKey)].GetIsActive(); }

public:

	/*
		Hardcode bind key with method.
		NOTE: call unbind on your object destructor.
	*/
	template<class TObject>
	FORCEINLINE void BindKey(EKey KeyCode, EKeyBindType KeyBindType, EInputMode InputMode, EInputCallType InputCallType, TObject* Object, void (TObject::*Method)(void))
	{
		if( KeyBindType == EKeyBindType::Count || KeyCode == EKey::KeyCount ) return;
		KeyBinds[static_cast<uint16>(KeyCode)].BindKey(KeyBindType, InputMode, InputCallType, Object, Method);
	}

	template<class TObject>
	FORCEINLINE void BindKey(const FKey& Key, EKeyBindType KeyBindType, EInputMode InputMode, EInputCallType InputCallType, TObject* Object, void (TObject::*Method)(void))
	{
		BindKey(Key.Key, KeyBindType, InputMode, InputCallType, Object, Method);
	}

	/*
		Hardcode unbind key from method.
	*/
	template<class TObject>
	FORCEINLINE void UnbindKey(EKey KeyCode, EKeyBindType KeyBindType, TObject* Object, void (TObject::*Method)(void))
	{
		if( KeyBindType == EKeyBindType::Count || KeyCode == EKey::KeyCount ) return;
		KeyBinds[static_cast<uint16>(KeyCode)].UnbindKey(KeyBindType, Object, Method);
	}

	template<class TObject>
	FORCEINLINE void UnbindKey(const FKey& Key, EKeyBindType KeyBindType, TObject* Object, void (TObject::*Method)(void))
	{
		UnbindKey(Key.Key, KeyBindType, Object, Method);
	}



	/*
		Hardcode bind mouse key with method
		NOTE: call unbind on your object destructor
	*/
	/*template<class TObject>
	FORCEINLINE void BindMouseKey(EMouseKey MouseKey, EMouseKeyBindType MouseKeyBindType, EInputMode InputMode, EInputCallType InputCallType, TObject* Object, void (TObject::*Method)(uint16, uint16))
	{
		if( MouseKeyBindType == EMouseKeyBindType::Count ) return;
		MouseButtonBind[static_cast<uint16>(MouseKey)].BindKey(MouseKeyBindType, InputMode, InputCallType, Object, Method);
	}*/

	/*
		Hardcode unbind mouse key from method.
	*/
	/*template<class TObject>
	FORCEINLINE void UnbindMouseKey(EMouseKey MouseKey, EMouseKeyBindType MouseKeyBindType, TObject* Object, void (TObject::*Method)(uint16, uint16))
	{
		if( MouseKeyBindType == EMouseKeyBindType::Count ) return;
		MouseButtonBind[static_cast<uint16>(MouseKey)].UnbindKey(MouseKeyBindType, Object, Method);
	}*/



	/*
		Hardcode bind mouse moving with method
		NOTE: call unbind on your object destructor
	*/
	template<class TObject>
	FORCEINLINE void BindMouseMoving(EInputMode InputMode, TObject* Object, void (TObject::*Method)(uint16, uint16))
	{
		auto LDelegate = new TDelegateHandler<TObject, uint16, uint16>(Object, Method);
		LDelegate->SetStructuredData(new FInputDelegateHandlerData(Object, InputMode, EInputCallType::OnlyWhenHeppends));
		MouseMovingBind.AddEventHandler(LDelegate);
	}

	template<class TObject>
	FORCEINLINE void UnbindMouseMoving(TObject* Object, void (TObject::*Method)(uint16, uint16))
	{
		MouseMovingBind.RemoveEventHandler(Object, Method);
	}



	/*
		Hardcode bind mouse wheel scrolling with method
		NOTE: call unbind on your object destructor
	*/
	template<class TObject>
	FORCEINLINE void BindMouseWheelScrool(EInputMode InputMode, TObject* Object, void (TObject::*Method)(int))
	{
		auto LDelegate = new TDelegateHandler<TObject, int>(Object, Method);
		LDelegate->SetStructuredData(new FInputDelegateHandlerData(Object, InputMode, EInputCallType::OnlyWhenHeppends));
		MouseWheelScrolledBind.AddEventHandler(LDelegate);
	}

	/*
		Hardcode unbind mouse wheel scrolling from method.
	*/
	template<class TObject>
	FORCEINLINE void UnbindMouseWheelScrool(TObject* Object, void (TObject::*Method)(int))
	{
		MouseWheelScrolledBind.RemoveEventHandler(Object, Method);
	}

private:

	FORCEINLINE void RegisterLeftMouseButtonClickPos(uint16 X, uint16 Y) noexcept
	{
		LastLeftMouseButtonClickX = LastMouseClickX = X;
		LastLeftMouseButtonClickY = LastMouseClickY = Y;
	}

	FORCEINLINE void RegisterRightMouseButtonClickPos(uint16 X, uint16 Y) noexcept
	{
		LastRightMouseButtonClickX = LastMouseClickX = X;
		LastRightMouseButtonClickY = LastMouseClickY = Y;
	}

	FORCEINLINE void RegisterMiddleMouseButtonClickPos(uint16 X, uint16 Y) noexcept
	{
		LastMiddleMouseButtonClickX = LastMouseClickX = X;
		LastMiddleMouseButtonClickY = LastMouseClickY = Y;
	}

	FORCEINLINE void RegisterMouseMovePos(uint16 X, uint16 Y) noexcept
	{
		LastMousePositionX = X;
		LastMousePositionY = Y;
	}




private:

	EInputMode CurrentInputMode = EInputMode::GameOnly;
	bool IsInputSystemEnabled = true;


	//...................Bind delegates......................//

	FKeyInputInfo KeyBinds[static_cast<uint16>(EKey::KeyCount)];

	//FMouseKeyInputInfo MouseButtonBind[static_cast<uint16>(EMouseKey::ButtonCount)];

	TDelegate<uint16, uint16> MouseMovingBind;
	TDelegate<int> MouseWheelScrolledBind;

	//.......................................................//


	//.......................Cached input....................//

	uint16 LastMouseClickX = 0;
	uint16 LastMouseClickY = 0;

	uint16 LastLeftMouseButtonClickX = 0;
	uint16 LastLeftMouseButtonClickY = 0;

	uint16 LastRightMouseButtonClickX = 0;
	uint16 LastRightMouseButtonClickY = 0;

	uint16 LastMiddleMouseButtonClickX = 0;
	uint16 LastMiddleMouseButtonClickY = 0;

	uint16 LastMousePositionX = 0;
	uint16 LastMousePositionY = 0;

	//.......................................................//
};



FORCEINLINE GGameInput* GGameInput::Get() noexcept
{
	static GGameInput GInput;
	return &GInput;
}
