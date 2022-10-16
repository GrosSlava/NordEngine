
#pragma once

#include "CoreMinimal.h"




/*
	Class for implementing user settings.
	You can inherit it and set it in the game settings.
*/
class ENGINE_API UGameUserSettings
{
	GENERATED_BODY(UGameUserSettings)
	NONCOPYABLE(UGameUserSettings)

public:

	UGameUserSettings() { }
	virtual ~UGameUserSettings() { }



public:

	virtual void LoadSettings() { }
	virtual void ApplyGameSettings() { }
	virtual void SaveGameSettings() { }
};
