
#pragma once

#include "CoreMinimal.h"




/*
	Interface of object that can be rendered.
*/
class ENGINE_API IRenderableObject
{
public:

	virtual ~IRenderableObject() {};



public:

	virtual bool GetIsVisible() const = 0;
	virtual void SetIsVisible(bool NewVisible) = 0;
};

/*
	Interface of renderable 2D objects.
*/
class ENGINE_API IRenderableObject2D : public IRenderableObject
{
public:

	virtual std::shared_ptr<class UScene2DView> GetSceneView() = 0;
};
