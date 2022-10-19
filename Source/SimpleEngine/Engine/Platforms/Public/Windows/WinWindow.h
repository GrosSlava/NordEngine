
#pragma once

#include "CoreMinimal.h"

#if !PLATFORM_WINDOWS
	#error PLATFORM_WINDOWS not defined!
#endif

#include "Window/BaseWindow.h"
#include "CoreObjectsFactory.h"




class ENGINE_API FWinWindowBuilder : public FWindowBuilder
{
	GENERATED_BODY_CHILD(FWinWindowBuilder, FWinWindowBuilder)

public:

	FWinWindowBuilder() { }
	virtual ~FWinWindowBuilder() { }

public:

	virtual GBaseWindow* ConstructWindow(uint16 Width, uint16 Height, int WindowStyle) override;
};



class ENGINE_API WinWindow : public GBaseWindow
{
	GENERATED_BODY_CHILD(WinWindow, GBaseWindow)
	NONCOPYABLE(WinWindow)

public:

	WinWindow(uint16 InWidth, uint16 InHeight, int InWindowStyle) : Super(InWidth, InHeight, InWindowStyle)
	{ 
		
	}
	~WinWindow() { }



public:

	virtual void Construct() override;

	virtual FWindowHandle::WindowHandle GetWindowHandle() const override;
	virtual void GetFullScreenSize(uint16& Width, uint16& Height) const override;

	virtual void SetWindowTitle(const std::string& Text) override;
	virtual void SetWindowSize(uint16 Width, uint16 Height) override;
	virtual void SetWindowFullScreen(bool Enable) override;
	virtual void SetWindowIcon(const std::string& IconPath) override;
	virtual void SetWindowCursor(const std::string& CursorPath) override;
	virtual void SetShowMouseCursor(bool Show) override;

protected:

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnWinWindowCreated(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:

	FORCEINLINE void ErrorMessageBox(const TCHAR* ErrorTitle, const TCHAR* ErrorMessage) 
	{
		MessageBox(NULL, ErrorMessage, ErrorTitle, MB_APPLMODAL | MB_SETFOREGROUND | MB_ICONERROR); 
	}




protected:

	HWND m_hWnd = nullptr;
	TRACKMOUSEEVENT m_TrackMouseEvent;

	HICON hWindowSmallIcon = nullptr;
	HICON hWindowBigIcon = nullptr;
	HCURSOR hCursor = nullptr;
};
