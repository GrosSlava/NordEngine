
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/DeviceResourcesAdapter.h"
#include "GraphicsEngine/RenderableObject.h"
#include "GraphicsEngine/SceneView/SceneView.h"

#include "World/World.h"

#include <algorithm>





GGraphicsEngine::~GGraphicsEngine()
{
	if( DeviceResourcesAdapter != nullptr )
	{
		DeviceResourcesAdapter->PreGameDestroy();
		delete DeviceResourcesAdapter;
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
	DeviceResourcesAdapter->SetFrameRateLimit(FrameRateLimit);
}





void GGraphicsEngine::BeginGameLogicSection()
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->BeginGameLogicSection();
}

void GGraphicsEngine::EndGameLogicSection()
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->EndGameLogicSection();
}


void GGraphicsEngine::SetVSyncEnabled(bool Enable)
{
	if( VSyncEnabled == Enable ) return;

	VSyncEnabled = Enable;
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->SetVSyncEnabled(Enable);
}

void GGraphicsEngine::SetFrameRateLimit(uint16 Limit)
{
	if( FrameRateLimit == Limit ) return;

	FrameRateLimit = Limit;
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->SetFrameRateLimit(Limit);
}




void GGraphicsEngine::BroadcastEvents()
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->BroadcastEvents();
}

void GGraphicsEngine::Render(GWorld* World)
{
	if( World == nullptr || DeviceResourcesAdapter == nullptr ) return;


	//TODO optimize
	FSceneView SceneView;
	for( const std::shared_ptr<ISpawnableObject> LSceneObject : World->GetAllSceneObjects() )
	{
		if( ISpawnableObject2D* As2DObject = dynamic_cast<ISpawnableObject2D*>(LSceneObject.get()) )
		{
			if( IRenderableObject2D* AsRenderable = dynamic_cast<IRenderableObject2D*>(LSceneObject.get()) )
			{
				if( !AsRenderable->GetIsVisible() ) continue;

				if( AsRenderable->GetSceneView() != nullptr )
				{
					F2DView L2DView;

					L2DView.DrawableView = AsRenderable->GetSceneView()->GetView();
					L2DView.Origin = As2DObject->GetOrigin();
					L2DView.WorldLocation = As2DObject->GetWorldLocation();
					L2DView.Rotation = As2DObject->GetWorldRotation();
					L2DView.WorldScale = As2DObject->GetWorldScale();
					L2DView.LayerIndex = As2DObject->GetLayer();
					SceneView.View2D.push_back(L2DView);
				}
			}
		}
	}

	std::sort(SceneView.View2D.begin(), SceneView.View2D.end(), [](const F2DView& A, const F2DView& B) { return A.LayerIndex < B.LayerIndex; });


	DeviceResourcesAdapter->Render(SceneView);
}


void GGraphicsEngine::OnGameStart()
{
	GraphicsGameWasStarted = true;

	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnGameStart();
}

void GGraphicsEngine::OnGameEnd()
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->PreGameDestroy();

	GraphicsGameWasStarted = false;
}





//..............................................IAPIListener interface.......................................................//

void GGraphicsEngine::OnActivated()
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnActivated();
}

void GGraphicsEngine::OnDeactivated()
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnDeactivated();
}

void GGraphicsEngine::OnSuspending()
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnSuspending();
}

void GGraphicsEngine::OnResuming()
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnResuming();
}

void GGraphicsEngine::OnWindowMoved()
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowMoved();
}

void GGraphicsEngine::OnWindowTitleChanged(const std::string& Text)
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowTitleChanged(Text);
}

void GGraphicsEngine::OnWindowSizeChanged(uint16 Width, uint16 Height)
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowSizeChanged(Width, Height);
}

void GGraphicsEngine::OnWindowFullScreenChanged(bool Enable)
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowFullScreenChanged(Enable);
}

void GGraphicsEngine::OnWindowIconChanged(const std::string& IconPath)
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowIconChanged(IconPath);
}

void GGraphicsEngine::OnWindowCursorChanged(const std::string& CursorPath)
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowCursorChanged(CursorPath);
}

void GGraphicsEngine::OnWindowShowMouseCursorChanged(bool Show)
{
	if( DeviceResourcesAdapter != nullptr ) DeviceResourcesAdapter->OnWindowShowMouseCursorChanged(Show);
}

//...........................................................................................................................//