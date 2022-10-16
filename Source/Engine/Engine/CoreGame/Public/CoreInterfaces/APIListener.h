
#pragma once

#include "CoreMinimal.h"




/*
	Platform API events interface.
*/
class ENGINE_API IAPIListener
{
public:

	/*
        Game is becoming active window.
    */
	virtual void OnActivated() = 0;
	/* 
        Game is becoming background window.
    */
	virtual void OnDeactivated() = 0;
	/* 
        Game is being power - suspended(or minimized).
    */
	virtual void OnSuspending() = 0;
	/* 
        Game is being power-resumed (or returning from minimize).
    */
	virtual void OnResuming() = 0;
	/* 
        Game window is being moved. 
    */
	virtual void OnWindowMoved() = 0;
	/*
		Game window title changed.
	*/
	virtual void OnWindowTitleChanged(const std::string& Text) = 0;
	/* 
        Game window is being resized.
    */
	virtual void OnWindowSizeChanged(uint16 Width, uint16 Height) = 0;
	/*
		Window fullscreen was toggled.
	*/
	virtual void OnWindowFullScreenChanged(bool Enable) = 0;
	/*
		Game window icon was changed.
	*/
	virtual void OnWindowIconChanged(const std::string& IconPath) = 0;
	/*
		Game window cursor icon was changed. 
	*/
	virtual void OnWindowCursorChanged(const std::string& CursorPath) = 0;
	/*
		Game cursor showing was changed.
	*/
	virtual void OnWindowShowMouseCursorChanged(bool Show) = 0;
};