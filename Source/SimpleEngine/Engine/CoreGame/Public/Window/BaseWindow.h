
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
	GBaseWindow(uint16 InWidth, uint16 InHeight, int InWindowStyle) : Style(InWindowStyle) { }

	virtual ~GBaseWindow() { }



public:

	FORCEINLINE int GetWindowStyle() const noexcept { return Style; }
	FORCEINLINE std::string GetWindowTitle() const noexcept { return CurrentTitle; }

public:

	virtual void Construct();
	virtual FWindowHandle::WindowHandle GetWindowHandle() const;

	virtual void GetWindowSize(uint16& Width, uint16& Height) const;
	virtual void GetFullScreenSize(uint16& Width, uint16& Height) const;
	virtual void SetWindowSize(uint16 Width, uint16 Height);
	virtual void SetWindowFullScreen(bool Enable);
	virtual void SetWindowPosition(uint16 X, uint16 Y);
	virtual void GetWindowPosition(uint16& X, uint16& Y) const;

	virtual void SetWindowTitle(const std::string& Text);
	virtual void SetWindowIcon(const std::string& IconPath);
	virtual void SetWindowCursor(const std::string& CursorPath);

	virtual void SetMouseCursorVisible(bool Visible);
	virtual bool IsMouseCursorVisible() const;
	virtual void SetMouseCursorGrabbed(bool Grab);
	virtual bool IsMouseCursorGrabbed() const;

	virtual void RequestFocus();
	virtual bool HasFocus() const;

protected:

	virtual void OnCreated();
	virtual void OnActivated();
	virtual void OnDeactivated();
	virtual void OnPaint();
	virtual void OnDestroy();

	virtual void OnWindowMoved();
	virtual void OnWindowSuspending();
	virtual void OnWindowResuming();

	virtual void OnWindowSizeChanged();
	virtual void OnWindowTitleChanged();
	virtual void OnWindowIconChanged();
	virtual void OnWindowCursorChanged();
	virtual void OnWindowMouseCursorVisibilityChanged();
	virtual void OnWindowMouseCursorGrabbingChanged();

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
};
