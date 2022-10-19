
#pragma once

#include "CoreMinimal.h"

#include "KeyCodes.h"




/*
	Class for implementing input settings.
	You can inherit it and set it in the game settings.
*/
class ENGINE_API UGameInputSettings
{
	GENERATED_BODY(UGameInputSettings)
	NONCOPYABLE(UGameInputSettings)

public:

	UGameInputSettings() { }
	virtual ~UGameInputSettings() { }



public:

	virtual void LoadSettings();
	virtual void SaveInputSettings();


	bool HasAction(const std::string& ActionName) const;
	FKey GetActionKey(const std::string& ActionName) const;
	void SetActionKey(const std::string& ActionName, const FKey& Key);




protected:

	std::map<std::string, FKey> ActionsMap;
};
