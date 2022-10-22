
#pragma once

#include "CoreMinimal.h"

#include "KeyCodes.h"




/*
	Class for implementing input settings.
	You can inherit it and set it in the game settings.
	@see GGameSettings.
*/
class ENGINE_API UGameInputSettings
{
	GENERATED_BODY(UGameInputSettings)
	NONCOPYABLE(UGameInputSettings)

public:

	UGameInputSettings() { }
	virtual ~UGameInputSettings() { }



public:

	/*
		Load input settings map from Game input config file.
		For example you can use FPath::GetGameInputConfigPath().
	*/
	virtual void LoadSettings();
	/*
		Save input settings map.
		For example you can use FPath::GetGameInputConfigPath().
	*/
	virtual void SaveInputSettings();

	/*
		Check that ActionName has input key.
		@param ActionName - Action name to check.
	*/
	bool HasAction(const std::string& ActionName) const;
	/*
		@return input key of action.
	*/
	FKey GetActionKey(const std::string& ActionName) const;
	/*
		Add/set pair of ActionName <-> input key.
		@param ActionName - Name of input action.
		@param Key - input key of action.
	*/
	void SetActionKey(const std::string& ActionName, const FKey& Key);




protected:

	/*
		Key - Action name.
		Value - Input key.
	*/
	std::map<std::string, FKey> ActionsMap;
};
