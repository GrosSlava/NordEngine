
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/DeviceResourcesAdapter.h"
#include "GraphicsEngine/SceneView.h"

#include "World/World.h"

#include <algorithm>





GGraphicsEngine::~GGraphicsEngine()
{
	if( DeviceResourcesAdapter != nullptr )
	{
		DeviceResourcesAdapter->PreGameDestroy();
		delete DeviceResourcesAdapter;
		DeviceResourcesAdapter = nullptr;
	}
}





void GGraphicsEngine::SetNewDeviceResourcesAdapter(IDeviceResourcesAdapter* Adapter)
{
	if( Adapter == DeviceResourcesAdapter ) return;


	if( DeviceResourcesAdapter != nullptr )
	{
		delete DeviceResourcesAdapter;
	}

	DeviceResourcesAdapter = Adapter;
	ApplySettingsToAdapter();

	if( GraphicsGameWasStarted && DeviceResourcesAdapter != nullptr  )
	{
		DeviceResourcesAdapter->OnGameStart();
	}
}

void GGraphicsEngine::ApplySettingsToAdapter()
{
	if( DeviceResourcesAdapter == nullptr ) return;

	DeviceResourcesAdapter->SetVSyncEnabled(VSyncEnabled);
}



void GGraphicsEngine::Render(GWorld* World)
{
	if( !GraphicsGameWasStarted || IsRenderingNow || World == nullptr || DeviceResourcesAdapter == nullptr ) return;
	IsRenderingNow = true;


	FSceneView SceneView;


	// Prepare 2D objects
	for( const std::shared_ptr<ISpawnableObject2D> LSceneObject : World->Get2DObjects() )
	{
		if( !LSceneObject->GetIsVisible() || LSceneObject->GetSceneView() == nullptr ) continue;

		F2DView L2DView;

		L2DView.DrawableView = LSceneObject->GetSceneView()->GetView();
		L2DView.Origin = LSceneObject->GetOrigin();
		L2DView.WorldLocation = LSceneObject->GetWorldLocation();
		L2DView.Rotation = LSceneObject->GetWorldRotation();
		L2DView.WorldScale = LSceneObject->GetWorldScale();
		L2DView.LayerIndex = LSceneObject->GetLayer();

		SceneView.View2D.push_back(L2DView);
	}
	std::sort(SceneView.View2D.begin(), SceneView.View2D.end(), [](const F2DView& A, const F2DView& B) { return A.LayerIndex < B.LayerIndex; });


	// Prepare 3D objects
	for( const std::shared_ptr<ISpawnableObject3D> LSceneObject : World->Get3DObjects() )
	{
		if( !LSceneObject->GetIsVisible() ) continue;

		F3DView L3DView;

		//TODO

		SceneView.View3D.push_back(L3DView);
	}


	// Render scene
	DeviceResourcesAdapter->Render(SceneView);
	IsRenderingNow = false;
}




void GGraphicsEngine::OnGameStart()
{
	if( GraphicsGameWasStarted ) return;
	GraphicsGameWasStarted = true;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnGameStart();
}

void GGraphicsEngine::OnGameEnd()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->PreGameDestroy();

	GraphicsGameWasStarted = false;
}





void GGraphicsEngine::SetVSyncEnabled(bool Enable)
{
	if( VSyncEnabled == Enable ) return;

	VSyncEnabled = Enable;
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->SetVSyncEnabled(Enable);
}





//..............................................IAPIListener interface.......................................................//

void GGraphicsEngine::OnActivated()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnActivated();
}

void GGraphicsEngine::OnDeactivated()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnDeactivated();
}

void GGraphicsEngine::OnSuspending()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnSuspending();
}

void GGraphicsEngine::OnResuming()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnResuming();
}

void GGraphicsEngine::OnWindowMoved()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowMoved();
}

void GGraphicsEngine::OnWindowSizeChanged()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowSizeChanged();
}

void GGraphicsEngine::OnWindowTitleChanged()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowTitleChanged();
}

void GGraphicsEngine::OnWindowIconChanged()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowIconChanged();
}

void GGraphicsEngine::OnWindowCursorChanged()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowCursorChanged();
}

void GGraphicsEngine::OnWindowMouseCursorVisibilityChanged()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowMouseCursorVisibilityChanged();
}

void GGraphicsEngine::OnWindowMouseCursorGrabbingChanged()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowMouseCursorGrabbingChanged();
}

//...........................................................................................................................//


//..................................................ISubEngine interface.....................................................//

void GGraphicsEngine::BroadcastEvents()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->BroadcastEvents();
}

void GGraphicsEngine::BeginGameLogicSection()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->BeginGameLogicSection();
}

void GGraphicsEngine::EndGameLogicSection()
{
	if( !GraphicsGameWasStarted ) return;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->EndGameLogicSection();
}

//...........................................................................................................................//