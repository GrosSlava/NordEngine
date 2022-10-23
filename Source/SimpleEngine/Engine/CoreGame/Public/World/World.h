
#pragma once

#include "CoreMinimal.h"

#include "World/SpawnableObject.h"




/*
	Main class for containing all scene objects.
	You can inherit it and set it in the game settings.
*/
class ENGINE_API GWorld
{
	friend class GCoreObjectsFacade;

	GENERATED_BODY(GWorld)
	NONCOPYABLE(GWorld)

public:

	GWorld() { }
	virtual ~GWorld() { }



public:

	template<class T, typename... args>
	std::shared_ptr<T> SpawnActor2D(const FSceneObject2DSpawnParams& SpwnParams, args... Params);
	template<class T, typename... args>
	std::shared_ptr<T> SpawnActor3D(const FSceneObject3DSpawnParams& SpwnParams, args... Params);

	/*
		Do L for all spawned objects.
		@param L - lambda [](std::shared_ptr<ISpawnableObject> LObject)
	*/
	template<typename LAMBDA>
	void IterateOverActors(LAMBDA L);

public:

	/*
		Populate tick for all scene objects.
	*/
	virtual void Tick(double DeltaTime);


	/*
		Destroy and remove all objects from world.
	*/
	void DestroyAllObjects();


	/*
		@return all spawned 2D objects.
	*/
	FORCEINLINE const std::vector<std::shared_ptr<ISpawnableObject2D>>& Get2DObjects() const noexcept { return SceneObjects2D; }
	/*
		@return all spawned 3D objects.
	*/
	FORCEINLINE const std::vector<std::shared_ptr<ISpawnableObject3D>>& Get3DObjects() const noexcept { return SceneObjects3D; }
	
	/*
		@return total spawned objects count(including invalid objects).
	*/
	FORCEINLINE int GetObjectsCount() const noexcept { return SceneObjects2D.size() + SceneObjects3D.size(); }


	/*
		@return true if world has started the game.
	*/
	FORCEINLINE bool GetWorldGameWasStarted() const noexcept { return WorldGameWasStarted; }

protected:

	/*
		Event to start world game.
	*/
	virtual void OnGameStart();
	/*
		Event to end world game.
	*/
	virtual void OnGameEnd();

private:

	void InitSpawnedActor2D(std::shared_ptr<ISpawnableObject2D> Object2D, const FSceneObject2DSpawnParams& SpwnParams);
	void InitSpawnedActor3D(std::shared_ptr<ISpawnableObject3D> Object3D, const FSceneObject3DSpawnParams& SpwnParams);




private:

	/*
		Spawned 2D objects.
	*/
	std::vector<std::shared_ptr<ISpawnableObject2D>> SceneObjects2D;
	/*
		Spawned 3D objects.
	*/
	std::vector<std::shared_ptr<ISpawnableObject3D>> SceneObjects3D;


	/*
		Marks that the world has started the game.
	*/
	bool WorldGameWasStarted = false;
};





template<class T, typename... args>
FORCEINLINE std::shared_ptr<T> GWorld::SpawnActor2D(const FSceneObject2DSpawnParams& SpwnParams, args... Params)
{
	std::shared_ptr<T> LNewActor = std::make_shared<T>(Params...);

	if( LNewActor == nullptr ) return nullptr;
	SceneObjects2D.push_back(LNewActor);


	LNewActor->PreConstruct();
	InitSpawnedActor2D(LNewActor, SpwnParams);
	LNewActor->OnConstruct();

	if( WorldGameWasStarted )
	{
		LNewActor->BeginPlay();
	}

	return LNewActor;
}

template<class T, typename... args>
FORCEINLINE std::shared_ptr<T> GWorld::SpawnActor3D(const FSceneObject3DSpawnParams& SpwnParams, args... Params)
{
	std::shared_ptr<T> LNewActor = std::make_shared<T>(Params...);

	if( LNewActor == nullptr ) return nullptr;
	SceneObjects3D.push_back(LNewActor);


	LNewActor->PreConstruct();
	InitSpawnedActor3D(LNewActor, SpwnParams);
	LNewActor->OnConstruct();

	if( WorldGameWasStarted )
	{
		LNewActor->BeginPlay();
	}

	return LNewActor;
}


template<typename LAMBDA>
FORCEINLINE void GWorld::IterateOverActors(LAMBDA L)
{
	for( std::shared_ptr<ISpawnableObject2D> L2DObject : SceneObjects2D )
	{
		L(L2DObject);
	}

	for( std::shared_ptr<ISpawnableObject3D> L3DObject : SceneObjects3D )
	{
		L(L3DObject);
	}
}
