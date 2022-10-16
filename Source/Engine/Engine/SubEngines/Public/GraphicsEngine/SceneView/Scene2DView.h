
#pragma once

#include "CoreMinimal.h"

#include <SFML/Graphics/Texture.hpp>//TODO replace with own texture




/*
	Drawable information about 2d object.
*/
struct F2DDrawableView
{
	const sf::Texture* Texture = nullptr; //TODO optimize by texture id(asset manager)
	//FVector2D ViewRectangle = FVector2D::ZeroVector; //TODO rectangle container
};

/*
	View of 2d object for deawing.
*/
struct F2DView
{
	F2DDrawableView DrawableView;

	FVector2D Origin = FVector2D::ZeroVector;
	FVector2D WorldLocation = FVector2D::ZeroVector;
	float Rotation = 0.0f;
	FVector2D WorldScale = FVector2D::UnitVector;
	int LayerIndex = 0;
};



/*
	Object for generation 2D view.
*/
class ENGINE_API UScene2DView
{
public:

	virtual ~UScene2DView() {};



public:

	virtual F2DDrawableView GetView() { return F2DDrawableView(); }
};
