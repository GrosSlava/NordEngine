
#include "CoreGame.h"

#include "World/World.h"
#include "Window/BaseWindow.h"
#include "GameInput/GameInput.h"
#include "GameInstance/GameInstance.h"
#include "CameraManager/CameraManager.h"

//.......................Engines include....................//

#include "GraphicsEngine/DeviceResourcesAdapter.h"
#include "GraphicsEngine/GraphicsEngine.h"

//..........................................................//

#include "GameSettings/GameSettings.h"
#include "CoreObjectsFactory.h"





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

	ConstructCoreObjects();

	if( MainWindow != nullptr )
	{
		//Window will call OnGameStart after construct
		MainWindow->Construct();
	}
}

void GCoreGame::ConstructCoreObjects()
{
	GGameSettings* LGameSettings = GGameSettings::Get();

	//....................Construct engines..................//

	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->OnGameEnd();
		delete GraphicsEngine; // will clear it's resources
	}
	GraphicsEngine = LGameSettings->ConstructGraphicsEngine();

	//.......................................................//


	if( GameInstance != nullptr )
	{
		delete GameInstance;
	}
	GameInstance = LGameSettings->ConstructGameInstance();

	if( CameraManager != nullptr )
	{
		delete CameraManager;
	}
	CameraManager = LGameSettings->ConstructCameraManager();

	if( MainWorld != nullptr )
	{
		MainWorld->OnGameEnd();
		delete MainWorld;
	}
	MainWorld = LGameSettings->ConstructWorld();

	if( MainWindow != nullptr )
	{
		delete MainWindow;
	}
	MainWindow = LGameSettings->ConstructWindow();
}





void GCoreGame::OnGameStart()
{
	if( GameState != ECoreGameState::Initializing ) return;


	InitSubsystems();
	StartSubsystems();

	GameState = ECoreGameState::InProgress;


	GGameSettings* LGameSettings = GGameSettings::Get();
	LGameSettings->ApplyGameSettings(MainWindow);
	LGameSettings->SaveGameSettings();
}

void GCoreGame::InitSubsystems()
{
	GGameSettings* LGameSettings = GGameSettings::Get();

	IDeviceResourcesAdapter* LDeviceAdapter = LGameSettings->ConstructDeviceResourcesAdapter();
	if( LDeviceAdapter != nullptr )
	{
		LDeviceAdapter->InitAdapter(MainWindow);
	}
	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->SetNewDeviceResourcesAdapter(LDeviceAdapter);
	}
}

void GCoreGame::StartSubsystems()
{
	//...........Start engines.........//

	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->OnGameStart();
	}

	//.................................//


	if( MainWorld != nullptr )
	{
		MainWorld->OnGameStart();
	}
}



void GCoreGame::OnGameEnd()
{
	if( GameState == ECoreGameState::NotStarted ) return;
	GameState = ECoreGameState::Deinitializing;


	if( MainWorld != nullptr )
	{
		MainWorld->OnGameEnd();
		delete MainWorld;
		MainWorld = nullptr;
	}

	if( GameInstance != nullptr )
	{
		delete GameInstance;
		GameInstance = nullptr;
	}

	if( CameraManager != nullptr )
	{
		delete CameraManager;
		CameraManager = nullptr;
	}


	//..............Clear engines.................//

	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->OnGameEnd();
		delete GraphicsEngine; // will clear it's resources
		GraphicsEngine = nullptr;
	}

	//............................................//



	if( MainWindow != nullptr )
	{
		delete MainWindow;
		MainWindow = nullptr;
	}

	GCoreObjectsFactory::Get()->Clear();
	GameState = ECoreGameState::NotStarted;
}



void GCoreGame::Tick()
{
	if( GameState != ECoreGameState::InProgress ) return;
	

	// clang-format off
	CodeBlockPerformance.CacheCodePerformance([this]() { TickTimer.Tick(this, &GCoreGame::Update); });
	// clang-format on
	
	//............Engines tick work................//

	// clang-format off
	GraphicsEnginePerformance.CacheCodePerformance([this]()	{ if( GraphicsEngine != nullptr ) GraphicsEngine->Render(MainWorld); });
	// clang-format on

	//.............................................//
}

void GCoreGame::Update()
{
	//....Start engines critical section.....//
	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->BeginGameLogicSection();
	}
	//.......................................//


	UpdateInputs();
	UpdateSubsystems();


	//....Stop engines critical section......//
	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->EndGameLogicSection();
	}
	//.......................................//
}

void GCoreGame::UpdateInputs()
{
	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->BroadcastEvents();
	}

	GGameInput::Get()->BroadcastInputs();
}

void GCoreGame::UpdateSubsystems()
{
	const double DeltaTime = TickTimer.GetElapsedSeconds();


	if( MainWorld != nullptr )
	{
		MainWorld->Tick(DeltaTime);
	}
	if( CameraManager != nullptr )
	{
		CameraManager->Tick(DeltaTime);
	}
}




void GCoreGame::SetFPSLock(uint16 FPS)
{
	if( FPSLock == FPS ) return;


	FPSLock = FPS;

	if( FPSLock == 0 )
	{
		TickTimer.SetFixedTimeStep(false);
	}
	else
	{
		TickTimer.SetFixedTimeStep(true);
		TickTimer.SetTargetElapsedSeconds(1.0 / FPSLock);
	}
	
	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->SetFrameRateLimit(VSyncEnabled ? 0 : FPSLock);
	}
}

void GCoreGame::SetVSyncEnabled(bool Enable)
{
	if( VSyncEnabled == Enable ) return;


	VSyncEnabled = Enable;

	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->SetVSyncEnabled(VSyncEnabled);
		GraphicsEngine->SetFrameRateLimit(VSyncEnabled ? 0 : FPSLock);
	}
}





//..............................................IAPIListener interface.......................................................//

void GCoreGame::OnActivated()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnActivated();
}

void GCoreGame::OnDeactivated()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnDeactivated();
}

void GCoreGame::OnSuspending()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnSuspending();
}

void GCoreGame::OnResuming()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnResuming();
	TickTimer.ResetElapsedTime();
}

void GCoreGame::OnWindowMoved()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowMoved();
}

void GCoreGame::OnWindowSizeChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowSizeChanged();
}

void GCoreGame::OnWindowTitleChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowTitleChanged();
}

void GCoreGame::OnWindowIconChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowIconChanged();
}

void GCoreGame::OnWindowCursorChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowCursorChanged();
}

void GCoreGame::OnWindowMouseCursorVisibilityChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowMouseCursorVisibilityChanged();
}

void GCoreGame::OnWindowMouseCursorGrabbingChanged()
{
	if( GameState != ECoreGameState::InProgress ) return;

	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowMouseCursorGrabbingChanged();
}

//...........................................................................................................................//