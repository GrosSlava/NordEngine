
#pragma once

#include "CoreMinimal.h"

#if !PLATFORM_WINDOWS
#error PLATFORM_WINDOWS not defined!
#endif

#include "Window/BaseWindow.h"
#include "CoreGame/CoreObjectsFactory.h"




/*
	Builder for WinWindow.
	@see FWindowBuilder, GCoreObjectsFactory.
*/
class ENGINE_API FWinWindowBuilder : public FWindowBuilder
{
	GENERATED_BODY_CHILD(FWinWindowBuilder, FWinWindowBuilder)

public:

	FWinWindowBuilder() { }
	virtual ~FWinWindowBuilder() { }

public:

	virtual GBaseWindow* ConstructWindow(int WindowStyle) override;
};


/*
	Window implementation for Windows.
	@see GBaseWindow, GCoreGame.
*/
class ENGINE_API WinWindow : public GBaseWindow
{
	GENERATED_BODY_CHILD(WinWindow, GBaseWindow)
	NONCOPYABLE(WinWindow)

public:

	WinWindow(int InWindowStyle) : Super(InWindowStyle) { }
	~WinWindow();



public:

	virtual void Construct() override;
	virtual FWindowHandle::WindowHandle GetWindowHandle() const override;

	virtual void GetWindowSize(uint16& Width, uint16& Height) const override;
	virtual void GetFullScreenSize(uint16& Width, uint16& Height) const override;
	virtual void SetWindowSize(uint16 Width, uint16 Height) override;
	virtual void SetWindowFullScreen(bool Enable) override;
	virtual void SetWindowPosition(uint16 X, uint16 Y) override;
	virtual void GetWindowPosition(uint16& X, uint16& Y) const override;

	virtual void SetWindowTitle(const std::string& Text) override;
	virtual void SetWindowIcon(const std::string& IconPath) override;
	virtual void SetWindowCursor(const std::string& CursorPath) override;

	virtual void SetMouseCursorVisible(bool Visible) override;
	virtual bool IsMouseCursorVisible() const override;
	virtual void SetMouseCursorGrabbed(bool Grab) override;
	virtual bool IsMouseCursorGrabbed() const override;
	virtual void SetMousePosition(uint16 X, uint16 Y) override;
	virtual void GetMousePosition(uint16& X, uint16& Y) const override;
	virtual void SetMousePositionRelative(uint16 X, uint16 Y) override;
	virtual void GetMousePositionRelative(uint16& X, uint16& Y) const override;

	virtual void RequestFocus() override;
	virtual bool HasFocus() const override;

protected:

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnWinWindowCreated(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	void GrabCursore(bool Grab);
	void SetTracking(bool Track);

public:

	/*
		Show error message box.
		@param ErrorTitle - C-style string of title.
		@param ErrorMessage - C-style string of error message.
	*/
	FORCEINLINE void ErrorMessageBox(const TCHAR* ErrorTitle, const TCHAR* ErrorMessage) { MessageBox(NULL, ErrorMessage, ErrorTitle, MB_APPLMODAL | MB_SETFOREGROUND | MB_ICONERROR); }




protected:

	/*
		Window handle.
	*/
	HWND m_hWnd = nullptr;

	/*
		Handler for window small icon.
	*/
	HICON hWindowSmallIcon = nullptr;
	/*
		Handler for window big icon.
	*/
	HICON hWindowBigIcon = nullptr;
	/*
		Handler for window cursor icon.
	*/
	HCURSOR hCursor = nullptr;

private:

	/*
		Window width before fullscreen mode.
	*/
	uint16 CachedWindowWidth = 600;
	/*
		Window height before fullscreen mode.
	*/
	uint16 CachedWindowHeight = 600;

	/*
		Current cursor visibility state.
	*/
	bool IsCursorVisible = true;
	/*
		Current cursor grabbings state.
	*/
	bool IsCursorGrabbed = false;
	/*
		Mouse is inside the window or not.
	*/
	bool IsMouseInside = false;
};
