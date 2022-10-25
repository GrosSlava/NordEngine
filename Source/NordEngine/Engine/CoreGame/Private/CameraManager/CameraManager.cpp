
#include "CameraManager/CameraManager.h"

#include "CoreGame/CoreGame.h"





void GCameraManager::Tick(double DeltaTime)
{
	UpdateCameraProperties();
}

void GCameraManager::UpdateCameraProperties()
{
	if( ActiveCamera == nullptr ) return;


	//GCoreGame::Get()->GetGraphicsEngine()->GetDeviceResourcesAdapter()->SetCameraLocation(ActiveCamera->GetCameraLocation());
	//GCoreGame::Get()->GetGraphicsEngine()->GetDeviceResourcesAdapter()->SetCameraRotation(ActiveCamera->GetCameraRotation());


	//if( ActiveCamera->WerePropertyChanged() )
	//{
		//....................Put camera properties to graphics engine................//

		//GCoreGame::Get()->GetGraphicsEngine()->GetDeviceResourcesAdapter()->SetCameraFOV(ActiveCamera->GetCameraFOV());

		//............................................................................//

		//ActiveCamera->OnChangesApplied();
	//}
}



void GCameraManager::SetActiveCamera(std::shared_ptr<GCamera> NewCamera)
{
	ActiveCamera = NewCamera;
}