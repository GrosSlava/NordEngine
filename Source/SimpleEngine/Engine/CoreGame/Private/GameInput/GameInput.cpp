
#pragma once

#include "GameInput/GameInput.h"





GGameInput::GGameInput()
{
}




void GGameInput::BroadcastInputs()
{
	if( !IsInputSystemEnabled ) return;


	for( int i = 0; i < static_cast<int>(EKey::KeyCount); ++i )
	{
		KeyBinds[i].BroadcastCurrentAction(CurrentInputMode);
	}

	for( int i = 0; i < static_cast<uint8>(EMouseKey::ButtonCount); ++i )
	{
		MouseButtonBind[i].BroadcastCurrentAction(CurrentInputMode);
	}
}




void GGameInput::OnKeyDown(EKey KeyCode)
{
	KeyBinds[static_cast<uint8>(KeyCode)].OnActionHappend(EKeyBindType::KeyDown);
}

void GGameInput::OnKeyUp(EKey KeyCode)
{
	KeyBinds[static_cast<uint8>(KeyCode)].OnActionHappend(EKeyBindType::KeyUp);
}



void GGameInput::OnLeftMouseButtonDown(uint16 X, uint16 Y)
{
	MouseButtonBind[static_cast<uint8>(EMouseKey::Left)].OnActionHappend(EMouseKeyBindType::KeyDown, X, Y);

	RegisterLeftMouseButtonClickPos(X, Y);
}

void GGameInput::OnLeftMouseButtonDoubleClick(uint16 X, uint16 Y)
{
	MouseButtonBind[static_cast<uint8>(EMouseKey::Left)].OnActionHappend(EMouseKeyBindType::DoubleClick, X, Y);

	RegisterLeftMouseButtonClickPos(X, Y);
}

void GGameInput::OnLeftMouseButtonUp(uint16 X, uint16 Y)
{
	MouseButtonBind[static_cast<uint8>(EMouseKey::Left)].OnActionHappend(EMouseKeyBindType::KeyUp, X, Y);
}



void GGameInput::OnRightMouseButtonDown(uint16 X, uint16 Y)
{
	MouseButtonBind[static_cast<uint8>(EMouseKey::Right)].OnActionHappend(EMouseKeyBindType::KeyDown, X, Y);

	RegisterRightMouseButtonClickPos(X, Y);
}

void GGameInput::OnRightMouseButtonDoubleClick(uint16 X, uint16 Y)
{
	MouseButtonBind[static_cast<uint8>(EMouseKey::Right)].OnActionHappend(EMouseKeyBindType::DoubleClick, X, Y);

	RegisterRightMouseButtonClickPos(X, Y);
}

void GGameInput::OnRightMouseButtonUp(uint16 X, uint16 Y)
{
	MouseButtonBind[static_cast<uint8>(EMouseKey::Right)].OnActionHappend(EMouseKeyBindType::KeyUp, X, Y);
}



void GGameInput::OnMiddleMouseButtonDown(uint16 X, uint16 Y)
{
	MouseButtonBind[static_cast<uint8>(EMouseKey::Middle)].OnActionHappend(EMouseKeyBindType::KeyDown, X, Y);

	RegisterMiddleMouseButtonClickPos(X, Y);
}

void GGameInput::OnMiddleMouseButtonDoubleClick(uint16 X, uint16 Y)
{
	MouseButtonBind[static_cast<uint8>(EMouseKey::Middle)].OnActionHappend(EMouseKeyBindType::DoubleClick, X, Y);

	RegisterMiddleMouseButtonClickPos(X, Y);
}

void GGameInput::OnMiddleMouseButtonUp(uint16 X, uint16 Y)
{
	MouseButtonBind[static_cast<uint8>(EMouseKey::Middle)].OnActionHappend(EMouseKeyBindType::KeyUp, X, Y);
}



void GGameInput::OnMouseMoved(uint16 NewX, uint16 NewY)
{
	if( !IsInputSystemEnabled ) return;

	// clang-format off
	MouseMovingBind.Broadcast
	(
		NewX, NewY, [this]
		(TBaseDelegateHandler<uint16, uint16>* LDelegate)
		{ 
			return FKeyInputInfoHelper::CanInputHandlerBroadcast(static_cast<FInputDelegateHandlerData*>(LDelegate->GetStructuredData()), CurrentInputMode); 
		}
	);
	// clang-format on

	RegisterMouseMovePos(NewX, NewY);
}

void GGameInput::OnMouseWheelScrolled(int Delta)
{
	if( !IsInputSystemEnabled ) return;

	// clang-format off
	MouseWheelScrolledBind.Broadcast
	(
		Delta, [this]
		(TBaseDelegateHandler<int>* LDelegate)
		{ 
			return FKeyInputInfoHelper::CanInputHandlerBroadcast(static_cast<FInputDelegateHandlerData*>(LDelegate->GetStructuredData()), CurrentInputMode);
		}
	);
	// clang-format on
}