
#include "World/World.h"





void GWorld::OnGameStart()
{
	if( WorldGameWasStarted ) return;
	WorldGameWasStarted = true;


	// clang-format off
	IterateOverActors([this](std::shared_ptr<ISpawnableObject> LObject) 
	{
		if( !IsValid(LObject) ) return;

		LObject->BeginPlay();
	});
	// clang-format on
}

void GWorld::OnGameEnd()
{
	if( !WorldGameWasStarted ) return;

	DestroyAllObjects();
	
	WorldGameWasStarted = false;
}



void GWorld::Tick(double DeltaTime)
{
	if( WorldGameWasStarted ) return;


	for( int64 i = SceneObjects2D.size() - 1; i >= 0; --i )
	{
		if( !IsValid(SceneObjects2D[i]) )
		{
			SceneObjects2D.erase(SceneObjects2D.begin() + i);
			continue;
		}

		if( SceneObjects2D[i]->GetIsTickEnabled() )
		{
			SceneObjects2D[i]->Tick(DeltaTime);
		}
	}

	for( int64 i = SceneObjects3D.size() - 1; i >= 0; --i )
	{
		if( !IsValid(SceneObjects3D[i]) )
		{
			SceneObjects3D.erase(SceneObjects3D.begin() + i);
			continue;
		}

		if( SceneObjects3D[i]->GetIsTickEnabled() )
		{
			SceneObjects3D[i]->Tick(DeltaTime);
		}
	}
}





void GWorld::DestroyAllObjects()
{
	// clang-format off
	IterateOverActors([this](std::shared_ptr<ISpawnableObject> LObject) 
	{
		if( !IsValid(LObject) ) return;

		LObject->Destroy();
	});
	// clang-format on

	SceneObjects2D.clear();
	SceneObjects3D.clear();
}



void GWorld::InitSpawnedActor2D(std::shared_ptr<ISpawnableObject2D> Object2D, const FSceneObject2DSpawnParams& SpwnParams)
{
	Object2D->SetWorldLocation(SpwnParams.Location);
	Object2D->SetWorldRotation(SpwnParams.Rotation);
	Object2D->SetWorldScale(SpwnParams.Scale);
	Object2D->SetLayer(SpwnParams.LayerIndex);
}

void GWorld::InitSpawnedActor3D(std::shared_ptr<ISpawnableObject3D> Object3D, const FSceneObject3DSpawnParams& SpwnParams)
{
	//TODO
}