
#include "CoreGame/CoreObjectsFacade.h"

#include "World/World.h"
#include "Window/BaseWindow.h"
#include "GameInstance/GameInstance.h"
#include "CameraManager/CameraManager.h"

//.......................Engines include....................//

#include "GraphicsEngine/DeviceResourcesAdapter.h"
#include "GraphicsEngine/GraphicsEngine.h"

//..........................................................//

#include "GameSettings/GameSettings.h"





GCoreObjectsFacade::~GCoreObjectsFacade()
{
	Clear();
}




void GCoreObjectsFacade::ConstructCoreObjects()
{
	GGameSettings* LGameSettings = GGameSettings::Get();


	// clear all core objects
	Clear();


	//....................Construct engines..................//

	GraphicsEngine = LGameSettings->ConstructGraphicsEngine();

	//.......................................................//


	MainWorld = LGameSettings->ConstructWorld();
	GameInstance = LGameSettings->ConstructGameInstance();
	CameraManager = LGameSettings->ConstructCameraManager();

	MainWindow = LGameSettings->ConstructWindow();
}

void GCoreObjectsFacade::InitSubsystems()
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

void GCoreObjectsFacade::StartSubsystems()
{
	//...........Start engines.........//

	if( GraphicsEngine != nullptr )
	{
		GraphicsEngine->OnGameStart();
	}

	//.................................//


	// start world at the end
	if( MainWorld != nullptr )
	{
		MainWorld->OnGameStart();
	}
}

void GCoreObjectsFacade::Clear()
{
	// end world first
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


	// delete window at the end
	if( MainWindow != nullptr )
	{
		delete MainWindow;
		MainWindow = nullptr;
	}
}





//..............................................IAPIListener interface.......................................................//

void GCoreObjectsFacade::OnActivated()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnActivated();
}

void GCoreObjectsFacade::OnDeactivated()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnDeactivated();
}

void GCoreObjectsFacade::OnSuspending()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnSuspending();
}

void GCoreObjectsFacade::OnResuming()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnResuming();
}

void GCoreObjectsFacade::OnWindowMoved()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowMoved();
}

void GCoreObjectsFacade::OnWindowSizeChanged()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowSizeChanged();
}

void GCoreObjectsFacade::OnWindowTitleChanged()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowTitleChanged();
}

void GCoreObjectsFacade::OnWindowIconChanged()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowIconChanged();
}

void GCoreObjectsFacade::OnWindowCursorChanged()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowCursorChanged();
}

void GCoreObjectsFacade::OnWindowMouseCursorVisibilityChanged()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowMouseCursorVisibilityChanged();
}

void GCoreObjectsFacade::OnWindowMouseCursorGrabbingChanged()
{
	if( GraphicsEngine != nullptr ) GraphicsEngine->OnWindowMouseCursorGrabbingChanged();
}

//...........................................................................................................................//