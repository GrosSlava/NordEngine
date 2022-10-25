
#pragma once

#include "CoreMinimal.h"




/*
	Drawable information about 3D object.
*/
struct F3DDrawableView
{
	//TODO
};

/*
	View of 3D object for rendering.
*/
struct F3DView
{
	/*
		Drawable information for rendering.
	*/
	F3DDrawableView DrawableView;

	//TODO
};




/*
	Object for generation 3D view.
*/
class ENGINE_API UScene3DView
{
public:

	virtual ~UScene3DView() {};



public:

	/*
		Construct 3D view of owned object.
	*/
	virtual F3DDrawableView GetView() { return F3DDrawableView(); }
};
