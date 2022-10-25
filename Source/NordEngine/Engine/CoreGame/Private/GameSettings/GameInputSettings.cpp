
#include "GameSettings/GameInputSettings.h"

#include "Path.h"

#include "INI/INI.h"





void UGameInputSettings::LoadSettings()
{
	ActionsMap.clear();

	FINIFile LIniFile(FPath::GetGameInputConfigPath());

	for( const std::string& LAction : LIniFile.GetKeys("Actions") )
	{
		const std::string LKey = LIniFile.Get<std::string>("Actions", LAction, "");

		ActionsMap.insert(std::pair(LAction, FKey(LKey)));
	}
}

void UGameInputSettings::SaveInputSettings()
{
	FINIFile LIniFile;

	for( const auto LAction : ActionsMap )
	{
		LIniFile.Set("Actions", LAction.first, LAction.second.ToStr());
	}

	FINIWriter::Write(FPath::GetGameInputConfigPath(), LIniFile);
}




bool UGameInputSettings::HasAction(const std::string& ActionName) const
{
	const auto LResult = ActionsMap.find(ActionName);
	if( LResult == ActionsMap.end() ) return false;

	return true;
}

FKey UGameInputSettings::GetActionKey(const std::string& ActionName) const
{
	const auto LResult = ActionsMap.find(ActionName);
	if( LResult == ActionsMap.end() ) return EKey::KeyCount;

	return LResult->second;
}

void UGameInputSettings::SetActionKey(const std::string& ActionName, const FKey& Key)
{
	ActionsMap.insert_or_assign(ActionName, Key);
}
