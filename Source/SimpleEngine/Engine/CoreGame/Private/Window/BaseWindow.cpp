
#include "Window/BaseWindow.h"

#include "CoreGame.h"
#include "GameInput/GameInput.h"





void GBaseWindow::Construct()
{
	//TODO on child

	OnCreated();
}

FWindowHandle::WindowHandle GBaseWindow::GetWindowHandle() const
{
	//TODO on child

	return FWindowHandle::EmptyWindowHandle;
}



void GBaseWindow::GetWindowSize(uint16& Width, uint16& Height) const
{
	Width = 0;
	Height = 0;

	//TODO on child
}

void GBaseWindow::GetFullScreenSize(uint16& Width, uint16& Height) const
{
	Width = 0;
	Height = 0;
	
	//TODO on child
}

void GBaseWindow::SetWindowSize(uint16 Width, uint16 Height)
{
	//TODO on child

	OnWindowSizeChanged();
}

void GBaseWindow::SetWindowFullScreen(bool Enable)
{
	//TODO on child

	OnWindowSizeChanged();
}

void GBaseWindow::SetWindowPosition(uint16 X, uint16 Y)
{
	//TODO on child

	OnWindowMoved();
}

void GBaseWindow::GetWindowPosition(uint16& X, uint16& Y) const
{
	X = 0;
	Y = 0;

	//TODO on child
}



void GBaseWindow::SetWindowTitle(const std::string& Text)
{
	CurrentTitle = Text;

	//TODO on child

	OnWindowTitleChanged();
}

void GBaseWindow::SetWindowIcon(const std::string& IconPath)
{
	//TODO on child

	OnWindowIconChanged();
}

void GBaseWindow::SetWindowCursor(const std::string& CursorPath)
{
	//TODO on child

	OnWindowCursorChanged();
}



void GBaseWindow::SetMouseCursorVisible(bool Visible)
{
	//TODO on child

	OnWindowMouseCursorVisibilityChanged();
}

bool GBaseWindow::IsMouseCursorVisible() const
{
	return false;
}

void GBaseWindow::SetMouseCursorGrabbed(bool Grab)
{
	//TODO on child

	OnWindowMouseCursorGrabbingChanged();
}

bool GBaseWindow::IsMouseCursorGrabbed() const
{
	return false;
}



void GBaseWindow::RequestFocus()
{
	//TODO on child
}

bool GBaseWindow::HasFocus() const
{
	//TODO on child

	return false;
}





//..............................Messages from API(window).................................//

void GBaseWindow::OnCreated()
{
	GCoreGame::Get()->OnGameStart();
}

void GBaseWindow::OnActivated()
{
	GCoreGame::Get()->OnActivated();
}

void GBaseWindow::OnDeactivated()
{
	GCoreGame::Get()->OnDeactivated();
}

void GBaseWindow::OnPaint()
{
	GCoreGame::Get()->Tick();
}

void GBaseWindow::OnDestroy()
{
	GCoreGame::Get()->OnGameEnd();
}



void GBaseWindow::OnWindowMoved()
{
	GCoreGame::Get()->OnWindowMoved();
}

void GBaseWindow::OnWindowSuspending()
{
	GCoreGame::Get()->OnSuspending();
}

void GBaseWindow::OnWindowResuming()
{
	GCoreGame::Get()->OnResuming();
}



void GBaseWindow::OnWindowSizeChanged()
{
	GCoreGame::Get()->OnWindowSizeChanged();
}

void GBaseWindow::OnWindowTitleChanged()
{
	GCoreGame::Get()->OnWindowTitleChanged();
}

void GBaseWindow::OnWindowIconChanged()
{
	GCoreGame::Get()->OnWindowIconChanged();
}

void GBaseWindow::OnWindowCursorChanged()
{
	GCoreGame::Get()->OnWindowCursorChanged();
}

void GBaseWindow::OnWindowMouseCursorVisibilityChanged()
{
	GCoreGame::Get()->OnWindowMouseCursorVisibilityChanged();
}

void GBaseWindow::OnWindowMouseCursorGrabbingChanged()
{
	GCoreGame::Get()->OnWindowMouseCursorGrabbingChanged();
}




//..........................Inputs...................................//

void GBaseWindow::OnKeyDown(EKey KeyCode)
{
	GGameInput::Get()->OnKeyDown(KeyCode);
}

void GBaseWindow::OnKeyUp(EKey KeyCode)
{
	GGameInput::Get()->OnKeyUp(KeyCode);
}


void GBaseWindow::OnLeftMouseButtonDown(uint16 X, uint16 Y)
{
	GGameInput::Get()->OnLeftMouseButtonDown(X, Y);
}

void GBaseWindow::OnLeftMouseButtonDoubleClick(uint16 X, uint16 Y)
{
	GGameInput::Get()->OnLeftMouseButtonDoubleClick(X, Y);
}

void GBaseWindow::OnLeftMouseButtonUp(uint16 X, uint16 Y)
{
	GGameInput::Get()->OnLeftMouseButtonUp(X, Y);
}


void GBaseWindow::OnRightMouseButtonDown(uint16 X, uint16 Y)
{
	GGameInput::Get()->OnRightMouseButtonDown(X, Y);
}

void GBaseWindow::OnRightMouseButtonDoubleClick(uint16 X, uint16 Y)
{
	GGameInput::Get()->OnRightMouseButtonDoubleClick(X, Y);
}

void GBaseWindow::OnRightMouseButtonUp(uint16 X, uint16 Y)
{
	GGameInput::Get()->OnRightMouseButtonUp(X, Y);
}


void GBaseWindow::OnMiddleMouseButtonDown(uint16 X, uint16 Y)
{
	GGameInput::Get()->OnMiddleMouseButtonDown(X, Y);
}

void GBaseWindow::OnMiddleMouseButtonDoubleClick(uint16 X, uint16 Y)
{
	GGameInput::Get()->OnMiddleMouseButtonDoubleClick(X, Y);
}

void GBaseWindow::OnMiddleMouseButtonUp(uint16 X, uint16 Y)
{
	GGameInput::Get()->OnMiddleMouseButtonUp(X, Y);
}


void GBaseWindow::OnMouseMoved(uint16 NewX, uint16 NewY)
{
	GGameInput::Get()->OnMouseMoved(NewX, NewY);
}

void GBaseWindow::OnMouseWheelScrolled(int Delta)
{
	GGameInput::Get()->OnMouseWheelScrolled(Delta);
}


void GBaseWindow::OnMouseLeavedFromScreen()
{
	GGameInput::Get()->SetInputSystemEnabled(false);
}

void GBaseWindow::OnMouseEnterToScreen()
{
	GGameInput::Get()->SetInputSystemEnabled(true);
}

//...................................................................//


//........................................................................................//
