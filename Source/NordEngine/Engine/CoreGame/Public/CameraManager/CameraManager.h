
#pragma once

#include "CoreMinimal.h"




class ICamera;


/*
	Base class for camera manager.
	You can inherit it and set it in the game settings.
	Created by GGameSettings.
	@see GCamera, GGameSettings.
*/
class ENGINE_API GCameraManager
{
	GENERATED_BODY(GCameraManager)
	NONCOPYABLE(GCameraManager)

public:

	GCameraManager() { }
	virtual ~GCameraManager() { }



public:

	virtual void Tick(double DeltaTime);

	virtual void SetActiveCamera(std::shared_ptr<ICamera> NewCamera);
	
protected:

	virtual void UpdateCameraProperties();




protected:

	std::shared_ptr<ICamera> ActiveCamera = nullptr;
};