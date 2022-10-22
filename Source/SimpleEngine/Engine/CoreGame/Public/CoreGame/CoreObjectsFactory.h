
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

/*
	Builder for World.
	@see GCoreObjectsFactory.
*/
class ENGINE_API FWorldBuilder
{
	GENERATED_BODY(FWorldBuilder)

public:

	FWorldBuilder() { }
	virtual ~FWorldBuilder() { }

public:

	virtual GWorld* ConstructWorld();
};

/*
	Builder for Window.
	@see GCoreObjectsFactory.
*/
class ENGINE_API FWindowBuilder
{
	GENERATED_BODY(FWindowBuilder)

public:

	FWindowBuilder() { }
	virtual ~FWindowBuilder() { }

public:

	virtual GBaseWindow* ConstructWindow(int WindowStyle);
};

/*
	Builder for GameInstance.
	@see GCoreObjectsFactory.
*/
class ENGINE_API FGameInstanceBuilder
{
	GENERATED_BODY(FGameInstanceBuilder)

public:

	FGameInstanceBuilder() { }
	virtual ~FGameInstanceBuilder() { }

public:

	virtual GGameInstance* ConstructGameInstance();
};

/*
	Builder for CameraManager.
	@see GCoreObjectsFactory.
*/
class ENGINE_API FCameraManagerBuilder
{
	GENERATED_BODY(FCameraManagerBuilder)

public:

	FCameraManagerBuilder() { }
	virtual ~FCameraManagerBuilder() { }

public:

	virtual GCameraManager* ConstructCameraManager();
};

/*
	Builder for GameUserSettings.
	@see GCoreObjectsFactory.
*/
class ENGINE_API FGameUserSettingsBuilder
{
	GENERATED_BODY(FGameUserSettingsBuilder)

public:

	FGameUserSettingsBuilder() { }
	virtual ~FGameUserSettingsBuilder() { }

public:

	virtual UGameUserSettings* ConstructGameUserSettings();
};

/*
	Builder for GameInputSettings.
	@see GCoreObjectsFactory.
*/
class ENGINE_API FGameInputSettingsBuilder
{
	GENERATED_BODY(FGameInputSettingsBuilder)

public:

	FGameInputSettingsBuilder() { }
	virtual ~FGameInputSettingsBuilder() { }

public:

	virtual UGameInputSettings* ConstructGameInputSettings();
};

/*
	Builder for DeviceResourcesAdapter.
	@see GCoreObjectsFactory, GGraphicsEngine.
*/
class ENGINE_API FDeviceResourcesAdapterBuilder
{
	GENERATED_BODY(FDeviceResourcesAdapterBuilder)

public:

	FDeviceResourcesAdapterBuilder() { }
	virtual ~FDeviceResourcesAdapterBuilder() { }

public:

	virtual IDeviceResourcesAdapter* ConstructDeviceResourcesAdapter();
};

/*
	Builder for GraphicsEngine.
	@see GCoreObjectsFactory, IDeviceResourcesAdapter.
*/
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


/*
	Container for builders of objects of the same class.
	@see GCoreObjectsFactory.
*/
template<class T>
struct TFactoryContainer
{
public:

	/*
		Register object builder under unique name.
		@param ClassName - name of class that will be used as key to construct object.
		@param ClassBuilder - Builder of object.
	*/
	FORCEINLINE void RegisterBuilderClass(const std::string& ClassName, T* ClassBuilder) { ClassesMap.insert_or_assign(ClassName, ClassBuilder); }

	/*
		Unregister object builder if it exists.
		@param ClassName - name of class to unregister.
	*/
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

	/*
		Clear all builder and forget all classes.
	*/
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

	/*
		Find builder of class by ClassName.
		@param ClassName - name of class as key for builder searching.
		@return class builder or nullptr if not found.
	*/
	FORCEINLINE T* GetClassBuilder(const std::string& ClassName) const
	{
		auto LPair = ClassesMap.find(ClassName);
		if( LPair == ClassesMap.end() ) return nullptr;

		return LPair->second;
	}


private:

	/*
		Key - Class name.
		Value - Builder of concrete class.
	*/
	std::unordered_map<std::string, T*> ClassesMap;
};




/*
	Factory for registering user classes for core objects.
	@see GameSettings
*/
class ENGINE_API GCoreObjectsFactory final
{
	SINGLETON(GCoreObjectsFactory)
	GENERATED_BODY(GCoreObjectsFactory)

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
	GBaseWindow* ConstructWindow(const std::string& ClassName, int WindowStyle);

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