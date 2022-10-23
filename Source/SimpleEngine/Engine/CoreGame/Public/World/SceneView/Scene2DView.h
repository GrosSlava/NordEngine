
#pragma once

#include "CoreMinimal.h"

#include <SFML/Graphics/Texture.hpp>//TODO replace with own texture




/*
	Drawable information about 2D object.
*/
struct F2DDrawableView
{
	const sf::Texture* Texture = nullptr; //TODO optimize by texture id(asset manager)
	//FVector2D ViewRectangle = FVector2D::ZeroVector; //TODO rectangle container
};

/*
	View of 2D object for rendering.
*/
struct F2DView
{
	/*
		Drawable information for rendering.
	*/
	F2DDrawableView DrawableView;

	/*
		Render origin 2D.
	*/
	FVector2D Origin = FVector2D::ZeroVector;
	/*
		Render location 2D.
	*/
	FVector2D WorldLocation = FVector2D::ZeroVector;
	/*
		Render rotation angle in [TODO FAngle].
	*/
	float Rotation = 0.0f;
	/*
		Render scale 2D.
	*/
	FVector2D WorldScale = FVector2D::UnitVector;
	/*
		Render layer 2D.
	*/
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

	/*
		Construct 2D view of owned object.
	*/
	virtual F2DDrawableView GetView() { return F2DDrawableView(); }
};
