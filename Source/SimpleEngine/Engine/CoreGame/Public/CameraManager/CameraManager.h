
#pragma once

#include "CoreMinimal.h"




class GCamera;


/*
	Base class for camera manager.
	You can inherit it and set it in the game settings.
	Created by GGameSettings.
	@see GCamera, GGameSettings, GCoreGame.
*/
class ENGINE_API GCameraManager
{
	friend class GCoreGame;

	GENERATED_BODY(GCameraManager)
	NONCOPYABLE(GCameraManager)

public:

	GCameraManager() { }
	virtual ~GCameraManager() { }



public:

	virtual void SetActiveCamera(std::shared_ptr<GCamera> NewCamera);

protected:

	virtual void Tick(double DeltaTime);
	virtual void UpdateCameraProperties();




protected:

	std::shared_ptr<GCamera> ActiveCamera = nullptr;
};