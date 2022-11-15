
#include "CameraManager/CameraManager.h"
#include "CameraManager/CameraInterface.h"

#include "CoreGame/CoreGame.h"
#include "GraphicsEngine/GraphicsEngine.h"





void GCameraManager::Tick(double DeltaTime)
{
	UpdateCameraProperties();
}

void GCameraManager::UpdateCameraProperties()
{
	if( ActiveCamera == nullptr ) return;


	if( GGraphicsEngine* LGraphicsEngine = GCoreGame::Get()->GetCoreObjectsFacade().GetGraphicsEngine() )
	{
		if( IDeviceResourcesAdapter* LAdapter = LGraphicsEngine->GetDeviceResourcesAdapter() )
		{
			ActiveCamera->ApplyVisualProperties(LAdapter);
		}
	}
}



void GCameraManager::SetActiveCamera(std::shared_ptr<ICamera> NewCamera)
{
	ActiveCamera = NewCamera;
}