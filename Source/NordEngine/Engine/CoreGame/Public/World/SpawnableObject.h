
#pragma once

#include "CoreMinimal.h"




/*
	Interface of all objects that can be spawned in the world.
	Can't be implemented directly.
	@see ISpawnableObject2D, ISpawnableObject3D.
*/
class ENGINE_API ISpawnableObject
{
public:

	virtual ~ISpawnableObject() {}



public:

	/*
		Event befor construct initialization.
	*/
	virtual void PreConstruct() = 0;
	/*
		Event after construct initialization.
	*/
	virtual void OnConstruct() = 0;
	/*
		Event when world starts the game or after spawn when the game is already running.
	*/
	virtual void BeginPlay() = 0;

	/*
		Every tick update.
		@param DeltaTime - time in seconds between last update.
	*/
	virtual void Tick(double DeltaTime) = 0;
	/*
		@return true if object should tick.
	*/
	virtual bool GetIsTickEnabled() const = 0;
	/*
		Enable/disable object tick.
	*/
	virtual void SetTickEnabled(bool Enabled) = 0;

	/*
		@return true if object market to be destroyed.
	*/
	virtual bool GetIsPendingToKill() const = 0;
	/*
		Notify the object of the next destruction.
		GetIsPendingToKill should return true if we want to completely delete the object from memory.
	*/
	virtual void Destroy() = 0;

	/*
		@return true if object should be rendered.
	*/
	virtual bool GetIsVisible() const = 0;
	/*
		Enable/disable object rendering.
	*/
	virtual void SetIsVisible(bool NewVisible) = 0;
};

FORCEINLINE bool IsValid(const ISpawnableObject* SceneObject)
{
	return SceneObject != nullptr && !SceneObject->GetIsPendingToKill();
}

FORCEINLINE bool IsValid(const std::shared_ptr<ISpawnableObject> SceneObject)
{
	return IsValid(SceneObject.get());
}



/*
	Interface for 2D scene objects.
	@see ISpawnableObject, UScene2DView.
*/
class ENGINE_API ISpawnableObject2D : public ISpawnableObject
{
public:

	/*
		@return current world 2D location.
	*/
	virtual FVector2D GetWorldLocation() const = 0;
	/*
		Set current world 2D location.
	*/
	virtual void SetWorldLocation(const FVector2D& NewLocation) = 0;

	/*
		@return current world 2D rotation.
	*/
	virtual float GetWorldRotation() const = 0;
	/*
		Set current world 2D rotation.
		@param NewAngle - angle in [TODO FAngle].
	*/
	virtual void SetWorldRotation(float NewAngle) = 0;

	/*
		@return current world 2D scale.
	*/
	virtual FVector2D GetWorldScale() const = 0;
	/*
		Set current world 2D scale.
	*/
	virtual void SetWorldScale(const FVector2D& NewScale) = 0;

	/*
		@return current world 2D origin.
	*/
	virtual FVector2D GetOrigin() const = 0;
	/*
		Set current world 2D origin.
	*/
	virtual void SetOrigin(const FVector2D& NewOrigin) = 0;

	/*
		@return current object layer.
		A higher index means a deeper layer.
	*/
	virtual int GetLayer() const = 0;
	/*
		Set current object layer.
		A higher index means a deeper layer.
	*/
	virtual void SetLayer(int NewLayer) = 0;



	/*
		@return Scene2DView for building 2D view of object for rendering.
	*/
	virtual std::shared_ptr<class UScene2DView> GetSceneView() = 0;
};

/*
	Helper struct for 2D object spawning.
*/
struct FSceneObject2DSpawnParams
{
	/*
		Location to spawn.
	*/
	FVector2D Location = FVector2D::ZeroVector;
	/*
		Rotation to spawn.
	*/
	float Rotation = 0;
	/*
		Scale to spawn.
	*/
	FVector2D Scale = FVector2D::UnitVector;
	/*
		2D object layer to spawn.
		A higher index means a deeper layer.
	*/
	int LayerIndex = 0;
};




/*
	Interface for 3D scene objects.
	@see ISpawnableObject, UScene3DView.
*/
class ENGINE_API ISpawnableObject3D : public ISpawnableObject
{
public:

	//TODO
};

/*
	Helper struct for 3D object spawning.
*/
struct FSceneObject3DSpawnParams
{
	// TODO
};
