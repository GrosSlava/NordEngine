
#include "CoreGame/CoreTickLoop.h"

#include "GenericPlatformTime.h"

#include "World/World.h"
#include "CameraManager/CameraManager.h"
#include "GameInput/GameInput.h"

//.......................Engines include....................//

#include "GraphicsEngine/GraphicsEngine.h"

//..........................................................//





GCoreTickLoop::~GCoreTickLoop()
{
}




void GCoreTickLoop::Tick(GCoreObjectsFacade& CoreObjectsFacade)
{
	CurrentTickCoreObjectsFacade = &CoreObjectsFacade;


	TickTimer.Tick(this, &GCoreTickLoop::Update);

	const double LWorkTime = GetTotalWorkTime();
	if( LWorkTime < TickTargetTime )
	{
		FPlatformTime::Sleep(TickTargetTime - LWorkTime);
	}
}

void GCoreTickLoop::Update()
{
	// clang-format off
	GameBlockPerformance.CacheCodePerformance([this]()
	{
		//....Start engines critical section.....//
		if( CurrentTickCoreObjectsFacade->GetGraphicsEngine() != nullptr )
		{
			CurrentTickCoreObjectsFacade->GetGraphicsEngine()->BeginGameLogicSection();
		}
		//.......................................//

		UpdateInputs();
		UpdateSubsystems();

		//....Stop engines critical section......//
		if( CurrentTickCoreObjectsFacade->GetGraphicsEngine() != nullptr )
		{
			CurrentTickCoreObjectsFacade->GetGraphicsEngine()->EndGameLogicSection();
		}
		//.......................................//
	});
	// clang-format on


	//............Engines tick work................//

	// clang-format off
	GraphicsEnginePerformance.CacheCodePerformance([this]()	
	{ 
		if( CurrentTickCoreObjectsFacade->GetGraphicsEngine() != nullptr ) CurrentTickCoreObjectsFacade->GetGraphicsEngine()->Render(CurrentTickCoreObjectsFacade->GetWorld()); 
	});
	// clang-format on

	//.............................................//
}

void GCoreTickLoop::UpdateInputs()
{
	if( CurrentTickCoreObjectsFacade->GetGraphicsEngine() != nullptr )
	{
		CurrentTickCoreObjectsFacade->GetGraphicsEngine()->BroadcastEvents();
	}

	GGameInput::Get()->BroadcastInputs();
}

void GCoreTickLoop::UpdateSubsystems()
{
	const double DeltaTime = TickTimer.GetElapsedSeconds();


	if( CurrentTickCoreObjectsFacade->GetWorld() != nullptr )
	{
		CurrentTickCoreObjectsFacade->GetWorld()->Tick(DeltaTime);
	}
	if( CurrentTickCoreObjectsFacade->GetCameraManager() != nullptr )
	{
		CurrentTickCoreObjectsFacade->GetCameraManager()->Tick(DeltaTime);
	}
}



void GCoreTickLoop::SetFPSLock(uint16 FPS)
{
	if( FPSLock == FPS ) return;


	FPSLock = FPS;

	if( FPSLock > 0)
	{
		TickTargetTime = 1000.0 / FPSLock;
	}
	else
	{
		TickTargetTime = -1;
	}
}

void GCoreTickLoop::ResetElapsedTime()
{
	TickTimer.ResetElapsedTime();
}
