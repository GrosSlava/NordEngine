
#include "CoreGame/CoreGame.h"
#include "CoreGame/CoreObjectsFactory.h"

#include "Window/BaseWindow.h"
#include "GraphicsEngine/GraphicsEngine.h"

#include "GameSettings/GameSettings.h"





GCoreGame::GCoreGame()
{
}

GCoreGame::~GCoreGame()
{
	OnGameEnd();
}





void GCoreGame::Start()
{
	if( GameState != ECoreGameState::NotStarted ) return;
	GameState = ECoreGameState::Initializing;


	GGameSettings::Get()->LoadSettings();
	CoreObjectsFacade.ConstructCoreObjects();


	if( CoreObjectsFacade.GetWindow() != nullptr )
	{
		//Window will call OnGameStart after construct
		CoreObjectsFacade.GetWindow()->Construct();
	}
	else
	{
		OnGameStart();
	}
}

void GCoreGame::OnGameStart()
{
	if( GameState != ECoreGameState::Initializing ) return;


	CoreObjectsFacade.InitSubsystems();
	CoreObjectsFacade.StartSubsystems();

	GameState = ECoreGameState::InProgress;


	GGameSettings* LGameSettings = GGameSettings::Get();
	LGameSettings->ApplyGameSettings();
	LGameSettings->SaveGameSettings();
}

void GCoreGame::OnGameEnd()
{
	if( GameState == ECoreGameState::NotStarted ) return;
	GameState = ECoreGameState::Deinitializing;


	CoreObjectsFacade.Clear();

	GCoreObjectsFactory::Get()->Clear();
	GameState = ECoreGameState::NotStarted;
}




void GCoreGame::Tick()
{
	if( GameState != ECoreGameState::InProgress ) return;


	if( FPSLockIsDirty )
	{
		CoreTickLoop.SetFPSLock(VSyncEnabled ? 0 : FPSLock);
		if( CoreObjectsFacade.GetGraphicsEngine() != nullptr ) CoreObjectsFacade.GetGraphicsEngine()->SetVSyncEnabled(VSyncEnabled);
		
		FPSLockIsDirty = false;
	}

	CoreTickLoop.Tick(CoreObjectsFacade);
}





void GCoreGame::SetFPSLock(uint16 FPS)
{
	FPSLock = FPS;
	FPSLockIsDirty = true;
}

void GCoreGame::SetVSyncEnabled(bool Enable)
{
	VSyncEnabled = Enable;
	FPSLockIsDirty = true;
}





//..............................................IAPIListener interface.......................................................//

void GCoreGame::OnActivated()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnActivated();
}

void GCoreGame::OnDeactivated()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnDeactivated();
}

void GCoreGame::OnSuspending()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnSuspending();
}

void GCoreGame::OnResuming()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnResuming();
	CoreTickLoop.ResetElapsedTime();
}

void GCoreGame::OnWindowMoved()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnWindowMoved();
}

void GCoreGame::OnWindowSizeChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnWindowSizeChanged();
}

void GCoreGame::OnWindowTitleChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnWindowTitleChanged();
}

void GCoreGame::OnWindowIconChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnWindowIconChanged();
}

void GCoreGame::OnWindowCursorChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnWindowCursorChanged();
}

void GCoreGame::OnWindowMouseCursorVisibilityChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnWindowMouseCursorVisibilityChanged();
}

void GCoreGame::OnWindowMouseCursorGrabbingChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	CoreObjectsFacade.OnWindowMouseCursorGrabbingChanged();
}

//...........................................................................................................................//