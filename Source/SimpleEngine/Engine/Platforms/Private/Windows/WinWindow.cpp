
#include "GenericPlatform.h"
#if PLATFORM_WINDOWS


#include "Windows/WinWindow.h"

#include "windowsx.h"





GBaseWindow* FWinWindowBuilder::ConstructWindow(int WindowStyle)
{
	return new WinWindow(WindowStyle);
}





#define WINDOW_CLASS_NAME TEXT("WinWindowClass")


WinWindow::~WinWindow()
{
	if( hWindowSmallIcon != nullptr )
	{
		DestroyIcon(hWindowSmallIcon);
		hWindowSmallIcon = nullptr;
	}
	if( hWindowBigIcon != nullptr )
	{
		DestroyIcon(hWindowBigIcon);
		hWindowBigIcon = nullptr;
	}
	if( hCursor != nullptr )
	{
		DestroyIcon(hCursor);
		hCursor = nullptr;
	}

	UnregisterClass(WINDOW_CLASS_NAME, GetModuleHandle(nullptr));
}





void WinWindow::Construct()
{
	//.......................Register window class.....................//
	WNDCLASSEX LWindowClass;
	LWindowClass.cbSize = sizeof(WNDCLASSEX);
	LWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	LWindowClass.lpfnWndProc = &WinWindow::WndProc;
	LWindowClass.cbClsExtra = 0;
	LWindowClass.cbWndExtra = 0;
	LWindowClass.hInstance = GetModuleHandle(nullptr);
	LWindowClass.hIcon = LoadIcon(LWindowClass.hInstance, IDI_APPLICATION);
	LWindowClass.hIconSm = LoadIcon(LWindowClass.hInstance, IDI_APPLICATION);
	LWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	LWindowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	LWindowClass.lpszMenuName = nullptr;
	LWindowClass.lpszClassName = WINDOW_CLASS_NAME;

	if( !RegisterClassEx(&LWindowClass) )
	{
		ErrorMessageBox(TEXT("Fatal error"), TEXT("Call to RegisterClassEx failed!"));
		return;
	}
	//.................................................................//


	//.......................Create window.............................//

	int LWinwowStyle = 0;
	if( Style == EWindowStyle::None )
	{
		LWinwowStyle |= WS_POPUP;
	}
	else
	{
		if( Style & EWindowStyle::Titlebar ) LWinwowStyle |= WS_CAPTION | WS_MINIMIZEBOX;
		if( Style & EWindowStyle::Resize ) LWinwowStyle |= WS_THICKFRAME | WS_MAXIMIZEBOX;
		if( Style & EWindowStyle::Close ) LWinwowStyle |= WS_SYSMENU;
	}

	// clang-format off
	HWND hWnd = CreateWindowEx
	(
		WS_EX_OVERLAPPEDWINDOW, 
		WINDOW_CLASS_NAME, 
		TEXT(""), LWinwowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		600, 600, 
		nullptr,
		nullptr, 
		GetModuleHandle(nullptr), 
		this
	);
	// clang-format on

	if( !hWnd )
	{
		ErrorMessageBox(TEXT("Fatal Error"), TEXT("Call to CreateWindowEx failed!"));
		return;
	}

	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);

	//.................................................................//
}

FWindowHandle::WindowHandle WinWindow::GetWindowHandle() const
{
	return m_hWnd;
}




void WinWindow::GetWindowSize(uint16& Width, uint16& Height) const
{
	RECT LRect;
	GetClientRect(m_hWnd, &LRect);

	Width = static_cast<uint16>(LRect.right - LRect.left);
	Height = static_cast<uint16>(LRect.bottom - LRect.top);
}

void WinWindow::GetFullScreenSize(uint16& Width, uint16& Height) const
{
	Width = GetSystemMetrics(SM_CXSCREEN);
	Height = GetSystemMetrics(SM_CYSCREEN);
}

void WinWindow::SetWindowSize(uint16 Width, uint16 Height)
{
	// SetWindowPos wants the total size of the window (including title bar and borders), so we have to compute it
	RECT LRect = {0, 0, Width, Height};
	AdjustWindowRect(&LRect, static_cast<DWORD>(GetWindowLongPtr(m_hWnd, GWL_STYLE)), false);

	CachedWindowWidth = LRect.right - LRect.left;
	CachedWindowHeight = LRect.bottom - LRect.top;

	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, CachedWindowWidth, CachedWindowHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void WinWindow::SetWindowFullScreen(bool Enable)
{
	if( !Enable )
	{
		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, 0);

		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, CachedWindowWidth, CachedWindowHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	else
	{
		GetWindowSize(CachedWindowWidth, CachedWindowHeight); // cache current size to restore

		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP);
		SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowWindow(m_hWnd, SW_SHOWMAXIMIZED);
	}
}

void WinWindow::SetWindowPosition(uint16 X, uint16 Y)
{
	SetWindowPos(m_hWnd, HWND_TOP, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	if( IsCursorGrabbed ) SetMouseCursorGrabbed(true);
}

void WinWindow::GetWindowPosition(uint16& X, uint16& Y) const
{
	RECT LRect;
	GetWindowRect(m_hWnd, &LRect);

	X = LRect.left;
	Y = LRect.top;
}



void WinWindow::SetWindowTitle(const std::string& Text)
{
	CurrentTitle = Text;
	SetWindowTextA(m_hWnd, Text.c_str());

	OnWindowTitleChanged();
}

void WinWindow::SetWindowIcon(const std::string& IconPath)
{
	if( IconPath.empty() ) return;


	if( hWindowSmallIcon != nullptr )
	{
		DestroyIcon(hWindowSmallIcon);
	}
	if( hWindowBigIcon != nullptr )
	{
		DestroyIcon(hWindowBigIcon);
	}

	hWindowSmallIcon = static_cast<HICON>(LoadImageA(GetModuleHandle(nullptr), IconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	hWindowBigIcon = static_cast<HICON>(LoadImageA(GetModuleHandle(nullptr), IconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));

	if( !hWindowSmallIcon )
	{
		ErrorMessageBox(TEXT("Fatal Error"), TEXT("Can't load small Icon!"));
		return;
	}
	if( !hWindowBigIcon )
	{
		ErrorMessageBox(TEXT("Fatal Error"), TEXT("Can't load big Icon!"));
		return;
	}

	SendMessageW(m_hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hWindowSmallIcon));
	SendMessageW(m_hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hWindowBigIcon));

	OnWindowIconChanged();
}

void WinWindow::SetWindowCursor(const std::string& CursorPath)
{
	if( CursorPath.empty() ) return;


	if( hCursor != nullptr )
	{
		DestroyCursor(hCursor);
	}

	hCursor = static_cast<HCURSOR>(LoadImageA(GetModuleHandle(nullptr), CursorPath.c_str(), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE));
	if( !hCursor )
	{
		ErrorMessageBox(TEXT("Fatal Error"), TEXT("Can't load cursor!"));
		return;
	}


	SetCursor(hCursor);
	OnWindowCursorChanged();
}



void WinWindow::SetMouseCursorVisible(bool Visible)
{
	if( IsCursorVisible == Visible ) return;


	IsCursorVisible = Visible;
	ShowCursor(IsCursorVisible);

	OnWindowMouseCursorVisibilityChanged();
}

bool WinWindow::IsMouseCursorVisible() const
{
	return IsCursorVisible;
}

void WinWindow::SetMouseCursorGrabbed(bool Grab)
{
	if( IsCursorGrabbed == Grab ) return;


	IsCursorGrabbed = Grab;
	GrabCursore(IsCursorGrabbed);

	OnWindowMouseCursorGrabbingChanged();
}

void WinWindow::GrabCursore(bool Grab)
{
	if( Grab )
	{
		RECT LRect;
		GetClientRect(m_hWnd, &LRect);
		MapWindowPoints(m_hWnd, nullptr, reinterpret_cast<LPPOINT>(&LRect), 2);
		ClipCursor(&LRect);
	}
	else
	{
		ClipCursor(nullptr);
	}
}

bool WinWindow::IsMouseCursorGrabbed() const
{
	return IsCursorGrabbed;
}

void WinWindow::SetMousePosition(uint16 X, uint16 Y)
{
	SetCursorPos(X, Y);
}

void WinWindow::GetMousePosition(uint16& X, uint16& Y) const
{
	POINT LPoint;
	GetCursorPos(&LPoint);

	X = LPoint.x;
	Y = LPoint.y;
}

void WinWindow::SetMousePositionRelative(uint16 X, uint16 Y)
{
	POINT LPoint = {X, Y};
	ClientToScreen(m_hWnd, &LPoint);
	SetCursorPos(LPoint.x, LPoint.y);
}

void WinWindow::GetMousePositionRelative(uint16& X, uint16& Y) const
{
	POINT LPoint;
	GetCursorPos(&LPoint);
	ScreenToClient(m_hWnd, &LPoint);

	X = LPoint.x;
	Y = LPoint.y;
}



void WinWindow::RequestFocus()
{
	// Allow focus stealing only within the same process; compare PIDs of current and foreground window
	DWORD ThisPid, ForegroundPid;
	GetWindowThreadProcessId(m_hWnd, &ThisPid);
	GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundPid);

	if( ThisPid == ForegroundPid )
	{
		// The window requesting focus belongs to the same process as the current window: steal focus
		SetForegroundWindow(m_hWnd);
	}
	else
	{
		// Different process: don't steal focus, but create a taskbar notification ("flash")
		FLASHWINFO info;
		info.cbSize = sizeof(info);
		info.hwnd = m_hWnd;
		info.dwFlags = FLASHW_TRAY;
		info.dwTimeout = 0;
		info.uCount = 3;

		FlashWindowEx(&info);
	}
}

bool WinWindow::HasFocus() const
{
	return m_hWnd == GetForegroundWindow();
}





LRESULT WinWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;

	static WinWindow* LWindow = nullptr;



	switch( message )
	{
	case WM_CREATE:
	{
		LWindow = (WinWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;

		if( LWindow != nullptr )
		{
			LWindow->OnWinWindowCreated(hWnd, message, wParam, lParam);
			LWindow->OnCreated();
		}

		break;
	}

	case WM_ACTIVATEAPP:
	{
		if( wParam )
		{
			if( LWindow != nullptr ) LWindow->OnActivated();
		}
		else
		{
			if( LWindow != nullptr ) LWindow->OnDeactivated();
		}

		break;
	}

	case WM_PAINT:
	{
		if( s_in_sizemove )
		{
			if( LWindow != nullptr ) LWindow->OnPaint();
		}
		else
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}

		break;
	}

	case WM_CLOSE:
	{
		break;
	}

	case WM_DESTROY:
	{
		if( LWindow != nullptr )
		{
			LWindow->OnDestroy();
			PostQuitMessage(0);
		}

		break;
	}



	case WM_SETCURSOR:
	{
		if( LOWORD(lParam) == HTCLIENT )
		{
			if( LWindow != nullptr )
			{
				SetCursor(LWindow->hCursor);
				LWindow->OnWindowCursorChanged();
				break;
			}
		}

		break;
	}

	case WM_SETFOCUS:
	{
		if( LWindow != nullptr )
		{
			// restore/update cursor grabbing
			LWindow->GrabCursore(LWindow->IsCursorGrabbed);
		}

		break;
	}

	case WM_KILLFOCUS:
	{
		if( LWindow != nullptr )
		{
			LWindow->GrabCursore(false);
		}

		break;
	}



	case WM_MOVE:
	{
		if( LWindow != nullptr )
		{
			// restore/update cursor grabbing
			LWindow->GrabCursore(LWindow->IsCursorGrabbed);
			LWindow->OnWindowMoved();
		}

		break;
	}

	case WM_SIZE:
	{
		if( wParam == SIZE_MINIMIZED )
		{
			if( !s_minimized )
			{
				s_minimized = true;
				if( !s_in_suspend )
				{
					if( LWindow != nullptr ) LWindow->OnWindowSuspending();
					s_in_suspend = true;
				}
			}
		}
		else if( s_minimized )
		{
			s_minimized = false;
			if( s_in_suspend )
			{
				if( LWindow != nullptr )
				{
					LWindow->GrabCursore(LWindow->IsCursorGrabbed);
					LWindow->OnWindowResuming();
				}

				s_in_suspend = false;
			}
		}
		else if( !s_in_sizemove )
		{
			if( LWindow != nullptr )
			{
				LWindow->GrabCursore(LWindow->IsCursorGrabbed);
				LWindow->OnWindowSizeChanged();
			}
		}

		break;
	}

	case WM_ENTERSIZEMOVE:
	{
		s_in_sizemove = true;

		if( LWindow != nullptr )
		{
			LWindow->GrabCursore(false);
		}

		break;
	}

	case WM_EXITSIZEMOVE:
	{
		s_in_sizemove = false;

		if( LWindow != nullptr )
		{
			RECT LRect;
			GetClientRect(hWnd, &LRect);

			// restore/update cursor grabbing
			LWindow->GrabCursore(LWindow->IsCursorGrabbed);
			LWindow->OnWindowSizeChanged();
		}

		break;
	}

	case WM_GETMINMAXINFO:
	{
		if( lParam )
		{
			auto info = reinterpret_cast<MINMAXINFO*>(lParam);
			info->ptMinTrackSize.x = 320;
			info->ptMinTrackSize.y = 200;
		}

		break;
	}

	case WM_POWERBROADCAST:
	{
		switch( wParam )
		{
		case PBT_APMQUERYSUSPEND:
		{
			if( !s_in_suspend )
			{
				if( LWindow != nullptr ) LWindow->OnWindowSuspending();
				s_in_suspend = true;
			}

			return true;
		}

		case PBT_APMRESUMESUSPEND:
		{
			if( !s_minimized )
			{
				if( s_in_suspend )
				{
					if( LWindow != nullptr ) LWindow->OnWindowResuming();
					s_in_suspend = false;
				}
			}

			return false;
		}
		}

		break;
	}



	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		if( LWindow != nullptr ) LWindow->OnKeyDown(FKey(wParam));

		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		if( LWindow != nullptr ) LWindow->OnKeyUp(FKey(wParam));

		break;
	}

	case WM_MOUSEWHEEL:
	{
		if( LWindow != nullptr ) LWindow->OnMouseWheelScrolled(GET_WHEEL_DELTA_WPARAM(wParam));

		break;
	}

	case WM_MOUSEMOVE:
	{
		const int16 LX = GET_X_LPARAM(lParam);
		const int16 LY = GET_Y_LPARAM(lParam);

		if( LWindow != nullptr )
		{
			LWindow->OnMouseMoved(LX, LY);


			RECT LArea;
			GetClientRect(LWindow->m_hWnd, &LArea);

			if( (LX < LArea.left) || (LX > LArea.right) || (LY < LArea.top) || (LY > LArea.bottom) )
			{
				if( LWindow->IsMouseInside )
				{
					LWindow->IsMouseInside = false;

					// No longer care for the mouse leaving the window
					LWindow->SetTracking(false);
					LWindow->OnMouseLeavedFromScreen();
				}
			}
			else
			{
				if( !LWindow->IsMouseInside )
				{
					LWindow->IsMouseInside = true;

					// Look for the mouse leaving the window
					LWindow->SetTracking(true);
					LWindow->OnMouseEnterToScreen();
				}
			}
		}

		break;
	}

	case WM_MOUSELEAVE:
	{
		if( LWindow != nullptr )
		{
			LWindow->IsMouseInside = false;
			LWindow->OnMouseLeavedFromScreen();
		}

		break;
	}

	case WM_MOUSEHOVER:
	{
		if( LWindow != nullptr )
		{
			LWindow->OnMouseEnterToScreen();
		}

		break;
	}

	case WM_LBUTTONDOWN:
	{
		if( LWindow != nullptr ) LWindow->OnLeftMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}

	case WM_LBUTTONDBLCLK:
	{
		if( LWindow != nullptr ) LWindow->OnLeftMouseButtonDoubleClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}

	case WM_LBUTTONUP:
	{
		if( LWindow != nullptr ) LWindow->OnLeftMouseButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}

	case WM_RBUTTONDOWN:
	{
		if( LWindow != nullptr ) LWindow->OnRightMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}

	case WM_RBUTTONDBLCLK:
	{
		if( LWindow != nullptr ) LWindow->OnRightMouseButtonDoubleClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}

	case WM_RBUTTONUP:
	{
		if( LWindow != nullptr ) LWindow->OnRightMouseButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}

	case WM_MBUTTONDOWN:
	{
		if( LWindow != nullptr ) LWindow->OnMiddleMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}

	case WM_MBUTTONDBLCLK:
	{
		if( LWindow != nullptr ) LWindow->OnMiddleMouseButtonDoubleClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}

	case WM_MBUTTONUP:
	{
		if( LWindow != nullptr ) LWindow->OnMiddleMouseButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		break;
	}



	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}





void WinWindow::OnWinWindowCreated(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = hWnd;

	SetTracking(true);
}

void WinWindow::SetTracking(bool Track)
{
	TRACKMOUSEEVENT LMouseEvent;
	LMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
	LMouseEvent.dwFlags = Track ? TME_LEAVE : TME_CANCEL;
	LMouseEvent.dwHoverTime = HOVER_DEFAULT; //How long the mouse has to be in the window to trigger a hover event.
	LMouseEvent.hwndTrack = m_hWnd;
	TrackMouseEvent(&LMouseEvent);
}

#endif // PLATFORM_WINDOWS