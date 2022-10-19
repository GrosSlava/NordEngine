
#pragma once

#include "GenericPlatform.h"

#include <string>




struct ENGINE_API FPath
{
public:

	static FORCEINLINE std::string GetEngineResourcesFolderRelativePath()
	{ 
		return "Content\\Engine";
	}

	static FORCEINLINE std::string GetGameResourcesFolderRelativePath()
	{ 
		return "Content\\Game";
	}

	static FORCEINLINE std::string GetEngineConfigPath()
	{ 
		return GetEngineResourcesFolderRelativePath() + "\\" + "EngineConfig.ini"; 
	}

	static FORCEINLINE std::string GetGameConfigPath()
	{ 
		return GetGameResourcesFolderRelativePath() + "\\" + "GameConfig.ini"; 
	}

	static FORCEINLINE std::string GetGameInputConfigPath()
	{ 
		return GetGameResourcesFolderRelativePath() + "\\" + "InputConfig.ini"; 
	}
};