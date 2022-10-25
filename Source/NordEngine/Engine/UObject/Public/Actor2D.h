
#pragma once

#include "CoreMinimal.h"

#include "World/SpawnableObject.h"




class ENGINE_API AActor2D : public ISpawnableObject2D
{
	GENERATED_BODY(AActor2D)
	NONCOPYABLE(AActor2D)

public:

	AActor2D();
	~AActor2D();



public:

	//....................ISpawnableObject2D interface..........................//

	virtual void PreConstruct() override;
	virtual void OnConstruct() override;
	virtual void BeginPlay() override;

	virtual void Tick(double DeltaTime) override;
	virtual bool GetIsTickEnabled() const override { return CanEverTick; }
	virtual void SetTickEnabled(bool Enabled) override { CanEverTick = Enabled; };

	virtual bool GetIsPendingToKill() const override { return IsPendingToKill; }
	virtual void Destroy() override;


	virtual FVector2D GetWorldLocation() const override { return ActorWorldLocation; }
	virtual void SetWorldLocation(const FVector2D& NewLocation) override { ActorWorldLocation = NewLocation; }

	virtual float GetWorldRotation() const override { return ActorWorldRotation; }
	virtual void SetWorldRotation(float NewAngle) override { ActorWorldRotation = NewAngle; }

	virtual FVector2D GetWorldScale() const override { return ActorWorldScale; }
	virtual void SetWorldScale(const FVector2D& NewScale) override { ActorWorldScale = NewScale; }

	virtual FVector2D GetOrigin() const override { return ActorOrigin; }
	virtual void SetOrigin(const FVector2D& NewOrigin) override { ActorOrigin = NewOrigin; }

	virtual int GetLayer() const override { return LayerIndex; }
	virtual void SetLayer(int NewLayer) override { LayerIndex = NewLayer; }


	virtual bool GetIsVisible() const override { return IsVisible; }
	virtual void SetIsVisible(bool NewVisible) override { IsVisible = NewVisible; }

	virtual std::shared_ptr<UScene2DView> GetSceneView() override { return nullptr; }

	//.......................................................................//




private:

	bool CanEverTick = true;
	bool IsPendingToKill = false;
	bool IsVisible = true;

	FVector2D ActorWorldLocation = FVector2D::ZeroVector;
	float ActorWorldRotation = 0.0f;
	FVector2D ActorWorldScale = FVector2D::UnitVector;
	FVector2D ActorOrigin = FVector2D::ZeroVector;
	int LayerIndex = 0;
};