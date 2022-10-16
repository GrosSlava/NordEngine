
#pragma once

#include "CoreMinimal.h"




class GWorld;
class GBaseWindow;
class GGameInstance;
class GCameraManager;
class UGameUserSettings;
class UGameInputSettings;

class IDeviceResourcesAdapter;
class GGraphicsEngine;



//.................................................Builders....................................................//

class ENGINE_API FWorldBuilder
{
	GENERATED_BODY(FWorldBuilder)

public:

	FWorldBuilder() { }
	virtual ~FWorldBuilder() { }

public:

	virtual GWorld* ConstructWorld();
};

class ENGINE_API FWindowBuilder
{
	GENERATED_BODY(FWindowBuilder)

public:

	FWindowBuilder() { }
	virtual ~FWindowBuilder() { }

public:

	virtual GBaseWindow* ConstructWindow(uint16 Width, uint16 Height, int WindowStyle);
};

class ENGINE_API FGameInstanceBuilder
{
	GENERATED_BODY(FGameInstanceBuilder)

public:

	FGameInstanceBuilder() { }
	virtual ~FGameInstanceBuilder() { }

public:

	virtual GGameInstance* ConstructGameInstance();
};

class ENGINE_API FCameraManagerBuilder
{
	GENERATED_BODY(FCameraManagerBuilder)

public:

	FCameraManagerBuilder() { }
	virtual ~FCameraManagerBuilder() { }

public:

	virtual GCameraManager* ConstructCameraManager();
};

class ENGINE_API FGameUserSettingsBuilder
{
	GENERATED_BODY(FGameUserSettingsBuilder)

public:

	FGameUserSettingsBuilder() { }
	virtual ~FGameUserSettingsBuilder() { }

public:

	virtual UGameUserSettings* ConstructGameUserSettings();
};

class ENGINE_API FGameInputSettingsBuilder
{
	GENERATED_BODY(FGameInputSettingsBuilder)

public:

	FGameInputSettingsBuilder() { }
	virtual ~FGameInputSettingsBuilder() { }

public:

	virtual UGameInputSettings* ConstructGameInputSettings();
};

class ENGINE_API FDeviceResourcesAdapterBuilder
{
	GENERATED_BODY(FDeviceResourcesAdapterBuilder)

public:

	FDeviceResourcesAdapterBuilder() { }
	virtual ~FDeviceResourcesAdapterBuilder() { }

public:

	virtual IDeviceResourcesAdapter* ConstructDeviceResourcesAdapter();
};

class ENGINE_API FGraphicsEngineBuilder
{
	GENERATED_BODY(FGraphicsEngineBuilder)

public:

	FGraphicsEngineBuilder() { }
	virtual ~FGraphicsEngineBuilder() { }

public:

	virtual GGraphicsEngine* ConstructGraphicsEngine();
};

//.............................................................................................................//



template<class T>
struct TFactoryContainer
{
public:

	FORCEINLINE void RegisterBuilderClass(const std::string& ClassName, T* ClassBuilder) 
	{ 
		ClassesMap.insert_or_assign(ClassName, ClassBuilder); 
	}

	FORCEINLINE void UnregisterBuilderClass(const std::string& ClassName)
	{
		auto LPair = ClassesMap.find(ClassName);
		if( LPair != ClassesMap.end() )
		{
			if( LPair->second != nullptr )
			{
				delete LPair->second;
			}
			ClassesMap.erase(ClassName);
		}
	}

	FORCEINLINE void Clear() 
	{
		for( auto LPair : ClassesMap )
		{
			if( LPair.second != nullptr )
			{
				delete LPair.second;
			}
		}
		ClassesMap.clear();
	}

	FORCEINLINE T* GetClassBuilder(const std::string& ClassName) const
	{
		auto LPair = ClassesMap.find(ClassName);
		if( LPair == ClassesMap.end() ) return nullptr;

		return LPair->second;
	}


private:

	std::unordered_map<std::string, T*> ClassesMap;
};




/*
	Factory for registering user classes for core objects.
	@see GameSettings
*/
class ENGINE_API GCoreObjectsFactory final
{
	SINGLETON(GCoreObjectsFactory)

public:

	~GCoreObjectsFactory();



public:

	/*
		Clear all registered classes.
	*/
	void Clear();


	void RegisterWorldClass(const std::string& ClassName, FWorldBuilder* WorldBuilder);
	void UnregisterWorldClass(const std::string& ClassName);
	GWorld* ConstructWorld(const std::string& ClassName);

	void RegisterWindowClass(const std::string& ClassName, FWindowBuilder* WindowBuilder);
	void UnregisterWindowClass(const std::string& ClassName);
	GBaseWindow* ConstructWindow(const std::string& ClassName, uint16 Width, uint16 Height, int WindowStyle);

	void RegisterGameInstanceClass(const std::string& ClassName, FGameInstanceBuilder* GameInstanceBuilder);
	void UnregisterGameInstanceClass(const std::string& ClassName);
	GGameInstance* ConstructGameInstance(const std::string& ClassName);

	void RegisterCameraManagerClass(const std::string& ClassName, FCameraManagerBuilder* CameraManagerBuilder);
	void UnregisterCameraManagerClass(const std::string& ClassName);
	GCameraManager* ConstructCameraManager(const std::string& ClassName);

	void RegisterGameUserSettingsClass(const std::string& ClassName, FGameUserSettingsBuilder* GameUserSettingsBuilder);
	void UnregisterGameUserSettingsClass(const std::string& ClassName);
	UGameUserSettings* ConstructGameUserSettings(const std::string& ClassName);

	void RegisterGameInputSettingsClass(const std::string& ClassName, FGameInputSettingsBuilder* GameInputSettingsBuilder);
	void UnregisterGameInputSettingsClass(const std::string& ClassName);
	UGameInputSettings* ConstructGameInputSettings(const std::string& ClassName);

	void RegisterDeviceResourcesAdapterClass(const std::string& ClassName, FDeviceResourcesAdapterBuilder* DeviceResourcesAdapterBuilder);
	void UnregistervClass(const std::string& ClassName);
	IDeviceResourcesAdapter* ConstructDeviceResourcesAdapter(const std::string& ClassName);

	void RegisterGraphicsEngineClass(const std::string& ClassName, FGraphicsEngineBuilder* GraphicsEngineBuilder);
	void UnregisterGraphicsEngineClass(const std::string& ClassName);
	GGraphicsEngine* ConstructGraphicsEngine(const std::string& ClassName);

	


private:

	TFactoryContainer<FWorldBuilder> WorldClassesMap;
	TFactoryContainer<FWindowBuilder> WindowClassesMap;
	TFactoryContainer<FGameInstanceBuilder> GameInstanceClassesMap;
	TFactoryContainer<FCameraManagerBuilder> CameraManagerClassesMap;
	TFactoryContainer<FGameUserSettingsBuilder> GameUserSettingsClassesMap;
	TFactoryContainer<FGameInputSettingsBuilder> GameInputSettingsClassesMap;
	TFactoryContainer<FDeviceResourcesAdapterBuilder> DeviceResourcesAdapterClassesMap;
	TFactoryContainer<FGraphicsEngineBuilder> GraphicsEngineClassesMap;
};



FORCEINLINE GCoreObjectsFactory* GCoreObjectsFactory::Get() noexcept
{
	static GCoreObjectsFactory CoreObjectsFactory;
	return &CoreObjectsFactory;
}