
#pragma once

#include "CoreMinimal.h"




/*
	Interface for all subengines.
*/
class ENGINE_API ISubEngine
{
public:

	/*
		Give chance for subengine to populate internal events.
	*/
	virtual void BroadcastEvents() = 0;

	/*
		Tell engine about game logic critial section begin.
	*/
	virtual void BeginGameLogicSection() = 0;
	/*
		Tell engine about game logic critial section end.
	*/
	virtual void EndGameLogicSection() = 0;
};