
#pragma once

#include "CoreMinimal.h"




class ENGINE_API ISpawnableObject
{
public:

	virtual ~ISpawnableObject() {};



public:

	virtual void PreConstruct() = 0;
	virtual void OnConstruct() = 0;
	virtual void BeginPlay() = 0;

	virtual void Tick(double DeltaTime) = 0;
	virtual bool GetIsTickEnabled() const = 0;
	virtual void SetTickEnabled(bool Enabled) = 0;

	virtual bool GetIsPendingToKill() const = 0;
	virtual void Destroy() = 0;
};

FORCEINLINE bool IsValid(const ISpawnableObject* SceneObject)
{
	return SceneObject != nullptr && !SceneObject->GetIsPendingToKill();
}

FORCEINLINE bool IsValid(const std::shared_ptr<ISpawnableObject> SceneObject)
{
	return IsValid(SceneObject.get());
}




class ENGINE_API ISpawnableObject2D : public ISpawnableObject
{
public:

	virtual FVector2D GetWorldLocation() const = 0;
	virtual void SetWorldLocation(const FVector2D& NewLocation) = 0;

	virtual float GetWorldRotation() const = 0;
	virtual void SetWorldRotation(float NewAngle) = 0;

	virtual FVector2D GetWorldScale() const = 0;
	virtual void SetWorldScale(const FVector2D& NewScale) = 0;

	virtual FVector2D GetOrigin() const = 0;
	virtual void SetOrigin(const FVector2D& NewOrigin) = 0;

	virtual int GetLayer() const = 0;
	virtual void SetLayer(int NewLayer) = 0;
};

struct FSceneObject2DSpawnParams
{
	FVector2D Location = FVector2D::ZeroVector;
	float Rotation = 0;
	FVector2D Scale = FVector2D::UnitVector;
	int LayerIndex = 0;
};
