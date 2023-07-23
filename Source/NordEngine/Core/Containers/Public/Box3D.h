
#pragma once

#include "GenericPlatform.h"
#include "Vector3D.h"


struct ENGINE_API FBox3D
{
public:

	/** Holds the box's minimum point. */
	FVector3D Min;

	/** Holds the box's maximum point. */
	FVector3D Max;

	/** Holds a flag indicating whether this box is valid. */
	uint8 IsValid;
};