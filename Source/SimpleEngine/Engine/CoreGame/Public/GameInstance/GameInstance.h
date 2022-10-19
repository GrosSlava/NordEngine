
#pragma once

#include "CoreMinimal.h"




/*
	This class will be always available from GCoreGame.
	You can inherit it and set it in the game settings.

	Created by game settings and cached in GCoreGame.
*/
class ENGINE_API GGameInstance
{
	GENERATED_BODY(GGameInstance)
	NONCOPYABLE(GGameInstance)

public:

	GGameInstance() { }
	virtual ~GGameInstance() { }
};

