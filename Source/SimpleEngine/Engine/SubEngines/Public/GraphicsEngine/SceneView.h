
#pragma once

#include "CoreMinimal.h"

#include "World/SceneView/Scene2DView.h"
#include "World/SceneView/Scene3DView.h"




/*
	Main struct for containing information for rendering.
*/
struct FSceneView
{
	/*
		2D view from all 2D objects.
	*/
	std::vector<F2DView> View2D;
	/*
		3D view from all 3D objects.
	*/
	std::vector<F3DView> View3D;
};
