
#include "World/World.h"





void GWorld::OnGameStart()
{
	WorldGameWasStarted = true;

	for( std::shared_ptr<ISpawnableObject> LSceneObject : SceneObjects )
	{
		if( !IsValid(LSceneObject) ) continue;

		LSceneObject->BeginPlay();
	}
}

void GWorld::OnGameEnd()
{
	for( std::shared_ptr<ISpawnableObject> LSceneObject : SceneObjects )
	{
		if( !IsValid(LSceneObject) ) continue;

		LSceneObject->Destroy();
	}

	SceneObjects.clear();
	WorldGameWasStarted = false;
}



void GWorld::Tick(double DeltaTime)
{
	for( int64 i = SceneObjects.size() - 1; i >= 0; --i )
	{
		if( !IsValid(SceneObjects[i]) )
		{
			SceneObjects.erase(SceneObjects.begin() + i);
			continue;
		}

		if( SceneObjects[i]->GetIsTickEnabled() )
		{
			SceneObjects[i]->Tick(DeltaTime);
		}
	}
}