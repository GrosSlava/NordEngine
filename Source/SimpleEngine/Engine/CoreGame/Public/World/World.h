
#pragma once

#include "CoreMinimal.h"

#include "World/SpawnableObject.h"




class ENGINE_API GWorld
{
	friend class GCoreObjectsFacade;

	GENERATED_BODY(GWorld)
	NONCOPYABLE(GWorld)

public:

	GWorld() { }
	virtual ~GWorld() { }



public:

	virtual void Tick(double DeltaTime);

	template<class T, typename... args>
	std::shared_ptr<T> SpawnActor2D(const FSceneObject2DSpawnParams& SpwnParams, args... Params);
	

	FORCEINLINE const std::vector<std::shared_ptr<ISpawnableObject>>& GetAllSceneObjects() const noexcept { return SceneObjects; }

	FORCEINLINE bool GetWorldGameWasStarted() const noexcept { return WorldGameWasStarted; }

protected:

	virtual void OnGameStart();
	virtual void OnGameEnd();




private:

	std::vector<std::shared_ptr<ISpawnableObject>> SceneObjects;

	bool WorldGameWasStarted = false;
};





template<class T, typename... args>
FORCEINLINE std::shared_ptr<T> GWorld::SpawnActor2D(const FSceneObject2DSpawnParams& SpwnParams, args... Params)
{
	std::shared_ptr<T> LNewActor = std::make_shared<T>(Params...);
	SceneObjects.push_back(LNewActor);

	LNewActor->PreConstruct();

	LNewActor->SetWorldLocation(SpwnParams.Location);
	LNewActor->SetWorldRotation(SpwnParams.Rotation);
	LNewActor->SetWorldScale(SpwnParams.Scale);
	LNewActor->SetLayer(SpwnParams.LayerIndex);

	LNewActor->OnConstruct();

	if( WorldGameWasStarted )
	{
		LNewActor->BeginPlay();
	}

	return LNewActor;
}
