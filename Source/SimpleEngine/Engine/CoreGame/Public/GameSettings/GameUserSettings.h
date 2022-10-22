
#pragma once

#include "CoreMinimal.h"




/*
	Class for implementing user settings.
	You can inherit it and set it in the game settings.
	@see GGameSettings.
*/
class ENGINE_API UGameUserSettings
{
	GENERATED_BODY(UGameUserSettings)
	NONCOPYABLE(UGameUserSettings)

public:

	UGameUserSettings() { }
	virtual ~UGameUserSettings() { }



public:

	/*
		Load settings from Game config file.
		For example you can use FPath::GetGameConfigPath().
	*/
	virtual void LoadSettings() { }
	/*
		Apply loaded settings.
	*/
	virtual void ApplyGameSettings() { }
	/*
		Save current settings.
		For example you can use FPath::GetGameConfigPath().
	*/
	virtual void SaveGameSettings() { }
};
