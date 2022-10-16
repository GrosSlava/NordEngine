
#include "Window/BaseWindow.h"

#include "CoreGame.h"
#include "GameInput/GameInput.h"






void GBaseWindow::Construct()
{
	//TODO on child

	OnCreated();
}





void GBaseWindow::GetFullScreenSize(uint16& Width, uint16& Height) const
{
	Width = 0;
	Height = 0;
	
	//TODO on child
}



void GBaseWindow::SetWindowTitle(const std::string& Text)
{
	CurrentTitle = Text;

	//TODO on child

	OnWindowTitleChanged(Text);
}

void GBaseWindow::SetWindowSize(uint16 Width, uint16 Height)
{
	CurrentWindowWidth = Width;
	CurrentWindowHeight = Height;

	//TODO on child

	OnWindowSizeChanged(Width, Height);
}

void GBaseWindow::SetWindowFullScreen(bool Enable)
{
	//TODO on child

	OnWindowFullScreenChanged(Enable);
}

void GBaseWindow::SetWindowIcon(const std::string& IconPath)
{
	//TODO on child

	OnWindowIconChanged(IconPath);
}

void GBaseWindow::SetWindowCursor(const std::string& CursorPath)
{
	//TODO on child

	OnWindowCursorChanged(CursorPath);
}

void GBaseWindow::SetShowMouseCursor(bool Show)
{
	//TODO on child

	OnWindowShowMouseCursorChanged(Show);
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



void GBaseWindow::OnWindowTitleChanged(const std::string& Text)
{
	GCoreGame::Get()->OnWindowTitleChanged(Text);
}

void GBaseWindow::OnWindowSizeChanged(uint16 Width, uint16 Height)
{
	//window size already changed. Not call SetWindowSize.
	GCoreGame::Get()->OnWindowSizeChanged(Width, Height);
}

void GBaseWindow::OnWindowFullScreenChanged(bool Enable)
{
	GCoreGame::Get()->OnWindowFullScreenChanged(Enable);
}

void GBaseWindow::OnWindowIconChanged(const std::string& IconPath)
{
	GCoreGame::Get()->OnWindowIconChanged(IconPath);
}

void GBaseWindow::OnWindowCursorChanged(const std::string& CursorPath)
{
	GCoreGame::Get()->OnWindowCursorChanged(CursorPath);
}

void GBaseWindow::OnWindowShowMouseCursorChanged(bool Show)
{
	GCoreGame::Get()->OnWindowShowMouseCursorChanged(Show);
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
