
#pragma once

#include "GenericPlatform.h"
#include "Vector2D.h"


struct ENGINE_API FBox2D
{
public:

	/** Holds the box's minimum point. */
	FVector2D Min;

	/** Holds the box's maximum point. */
	FVector2D Max;

	/** Holds a flag indicating whether this box is valid. */
	uint8 IsValid;
};