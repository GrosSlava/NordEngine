
#pragma once

#include "CoreMinimal.h"

#include "WindowInfo.h"
#include "KeyCodes.h"




/*
	Base class for platform API based window.
*/
class ENGINE_API GBaseWindow
{
	GENERATED_BODY(GBaseWindow)
	NONCOPYABLE(GBaseWindow)

public:

	GBaseWindow() = delete;
	GBaseWindow(uint16 InWidth, uint16 InHeight, int InWindowStyle) : 
		CurrentWindowWidth(InWidth), CurrentWindowHeight(InHeight), Style(InWindowStyle)
	{ 
	}

	virtual ~GBaseWindow() { }



public:

	FORCEINLINE uint16 GetCurrentWindowWidth() const noexcept { return CurrentWindowWidth; }
	FORCEINLINE uint16 GetCurrentWindowHeight() const noexcept { return CurrentWindowHeight; }
	FORCEINLINE int GetWindowStyle() const noexcept { return Style; }

//Window base interface
public:

	virtual void Construct();

	virtual FWindowHandle::WindowHandle GetWindowHandle() const { return FWindowHandle::EmptyWindowHandle; }
	virtual void GetFullScreenSize(uint16& Width, uint16& Height) const;

	virtual void SetWindowTitle(const std::string& Text);
	virtual void SetWindowSize(uint16 Width, uint16 Height);
	virtual void SetWindowFullScreen(bool Enable);
	virtual void SetWindowIcon(const std::string& IconPath);
	virtual void SetWindowCursor(const std::string& CursorPath);
	virtual void SetShowMouseCursor(bool Show);

//Messages from API(window)
protected:

	virtual void OnCreated();
	virtual void OnActivated();
	virtual void OnDeactivated();
	virtual void OnPaint();
	virtual void OnDestroy();

	virtual void OnWindowMoved();
	virtual void OnWindowSuspending();
	virtual void OnWindowResuming();

	virtual void OnWindowTitleChanged(const std::string& Text);
	virtual void OnWindowSizeChanged(uint16 Width, uint16 Height);
	virtual void OnWindowFullScreenChanged(bool Enable);
	virtual void OnWindowIconChanged(const std::string& IconPath);
	virtual void OnWindowCursorChanged(const std::string& CursorPath);
	virtual void OnWindowShowMouseCursorChanged(bool Show);

	//...........................Inputs..........................//

	FORCEINLINE void OnKeyDown(FKey Key) { OnKeyDown(Key.Key); }
	virtual void OnKeyDown(EKey KeyCode);

	FORCEINLINE void OnKeyUp(FKey Key) { OnKeyUp(Key.Key); }
	virtual void OnKeyUp(EKey KeyCode);

	virtual void OnLeftMouseButtonDown(uint16 X, uint16 Y);
	virtual void OnLeftMouseButtonDoubleClick(uint16 X, uint16 Y);
	virtual void OnLeftMouseButtonUp(uint16 X, uint16 Y);

	virtual void OnRightMouseButtonDown(uint16 X, uint16 Y);
	virtual void OnRightMouseButtonDoubleClick(uint16 X, uint16 Y);
	virtual void OnRightMouseButtonUp(uint16 X, uint16 Y);

	virtual void OnMiddleMouseButtonDown(uint16 X, uint16 Y);
	virtual void OnMiddleMouseButtonDoubleClick(uint16 X, uint16 Y);
	virtual void OnMiddleMouseButtonUp(uint16 X, uint16 Y);

	virtual void OnMouseMoved(uint16 NewX, uint16 NewY);
	virtual void OnMouseWheelScrolled(int Delta);

	virtual void OnMouseLeavedFromScreen();
	virtual void OnMouseEnterToScreen();

	//...........................................................//




protected:

	int Style = EWindowStyle::Default;

	std::string CurrentTitle = "";
	uint16 CurrentWindowWidth = 0;
	uint16 CurrentWindowHeight = 0;
};
