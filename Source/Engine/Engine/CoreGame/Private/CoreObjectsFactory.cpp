
#include "CoreObjectsFactory.h"

#include "World/World.h"
#include "Window/BaseWindow.h"
#include "GameInstance/GameInstance.h"
#include "CameraManager/CameraManager.h"
#include "GameSettings/GameUserSettings.h"
#include "GameSettings/GameInputSettings.h"

#include "GraphicsEngine/DeviceResourcesAdapter.h"
#include "GraphicsEngine/GraphicsEngine.h"





//....................................................Builders...........................................................................//
//.......................................................................................................................................//

GWorld* FWorldBuilder::ConstructWorld()
{
	return new GWorld();
}

GBaseWindow* FWindowBuilder::ConstructWindow(uint16 Width, uint16 Height, int WindowStyle)
{
	return new GBaseWindow(Width, Height, WindowStyle);
}

GGameInstance* FGameInstanceBuilder::ConstructGameInstance()
{
	return new GGameInstance();
}

GCameraManager* FCameraManagerBuilder::ConstructCameraManager()
{
	return new GCameraManager();
}

UGameUserSettings* FGameUserSettingsBuilder::ConstructGameUserSettings()
{
	return new UGameUserSettings();
}

UGameInputSettings* FGameInputSettingsBuilder::ConstructGameInputSettings()
{
	return new UGameInputSettings();
}

GGraphicsEngine* FGraphicsEngineBuilder::ConstructGraphicsEngine()
{
	return new GGraphicsEngine();
}

IDeviceResourcesAdapter* FDeviceResourcesAdapterBuilder::ConstructDeviceResourcesAdapter()
{
	return nullptr;
}

//.......................................................................................................................................//
//.......................................................................................................................................//





GCoreObjectsFactory::GCoreObjectsFactory()
{
}

GCoreObjectsFactory::~GCoreObjectsFactory()
{
	Clear();
}

void GCoreObjectsFactory::Clear()
{
	WorldClassesMap.Clear();
	WindowClassesMap.Clear();
	GameInstanceClassesMap.Clear();
	CameraManagerClassesMap.Clear();
	GameUserSettingsClassesMap.Clear();
	GameInputSettingsClassesMap.Clear();
	DeviceResourcesAdapterClassesMap.Clear();
	GraphicsEngineClassesMap.Clear();
}




void GCoreObjectsFactory::RegisterWorldClass(const std::string& ClassName, FWorldBuilder* WorldBuilder)
{
	WorldClassesMap.RegisterBuilderClass(ClassName, WorldBuilder);
}

void GCoreObjectsFactory::UnregisterWorldClass(const std::string& ClassName)
{
	WorldClassesMap.UnregisterBuilderClass(ClassName);
}

GWorld* GCoreObjectsFactory::ConstructWorld(const std::string& ClassName)
{
	if( ClassName.empty() )
	{
		FWorldBuilder LWorldBuilder;
		return LWorldBuilder.ConstructWorld();
	}

	FWorldBuilder* LWorldBuilder = WorldClassesMap.GetClassBuilder(ClassName);
	if( LWorldBuilder == nullptr ) return nullptr;

	return LWorldBuilder->ConstructWorld();
}



void GCoreObjectsFactory::RegisterWindowClass(const std::string& ClassName, FWindowBuilder* WindowBuilder)
{
	WindowClassesMap.RegisterBuilderClass(ClassName, WindowBuilder);
}

void GCoreObjectsFactory::UnregisterWindowClass(const std::string& ClassName)
{
	WindowClassesMap.UnregisterBuilderClass(ClassName);
}

GBaseWindow* GCoreObjectsFactory::ConstructWindow(const std::string& ClassName, uint16 Width, uint16 Height, int WindowStyle)
{
	if( ClassName.empty() )
	{
		FWindowBuilder LWindowBuilder;
		return LWindowBuilder.ConstructWindow(Width, Height, WindowStyle);
	}

	FWindowBuilder* LWindowBuilder = WindowClassesMap.GetClassBuilder(ClassName);
	if( LWindowBuilder == nullptr ) return nullptr;

	return LWindowBuilder->ConstructWindow(Width, Height, WindowStyle);
}



void GCoreObjectsFactory::RegisterGameInstanceClass(const std::string& ClassName, FGameInstanceBuilder* GameInstanceBuilder)
{
	GameInstanceClassesMap.RegisterBuilderClass(ClassName, GameInstanceBuilder);
}

void GCoreObjectsFactory::UnregisterGameInstanceClass(const std::string& ClassName)
{
	GameInstanceClassesMap.UnregisterBuilderClass(ClassName);
}

GGameInstance* GCoreObjectsFactory::ConstructGameInstance(const std::string& ClassName)
{
	if( ClassName.empty() )
	{
		FGameInstanceBuilder LGameInstanceBuilder;
		return LGameInstanceBuilder.ConstructGameInstance();
	}

	FGameInstanceBuilder* LGameInstanceBuilder = GameInstanceClassesMap.GetClassBuilder(ClassName);
	if( LGameInstanceBuilder == nullptr ) return nullptr;

	return LGameInstanceBuilder->ConstructGameInstance();
}



void GCoreObjectsFactory::RegisterCameraManagerClass(const std::string& ClassName, FCameraManagerBuilder* CameraManagerBuilder)
{
	CameraManagerClassesMap.RegisterBuilderClass(ClassName, CameraManagerBuilder);
}

void GCoreObjectsFactory::UnregisterCameraManagerClass(const std::string& ClassName)
{
	CameraManagerClassesMap.UnregisterBuilderClass(ClassName);
}

GCameraManager* GCoreObjectsFactory::ConstructCameraManager(const std::string& ClassName)
{
	if( ClassName.empty() )
	{
		FCameraManagerBuilder LCameraManagerBuilder;
		return LCameraManagerBuilder.ConstructCameraManager();
	}

	FCameraManagerBuilder* LCameraManagerBuilder = CameraManagerClassesMap.GetClassBuilder(ClassName);
	if( LCameraManagerBuilder == nullptr ) return nullptr;

	return LCameraManagerBuilder->ConstructCameraManager();
}



void GCoreObjectsFactory::RegisterGameUserSettingsClass(const std::string& ClassName, FGameUserSettingsBuilder* GameUserSettingsBuilder)
{
	GameUserSettingsClassesMap.RegisterBuilderClass(ClassName, GameUserSettingsBuilder);
}

void GCoreObjectsFactory::UnregisterGameUserSettingsClass(const std::string& ClassName)
{
	GameUserSettingsClassesMap.UnregisterBuilderClass(ClassName);
}

UGameUserSettings* GCoreObjectsFactory::ConstructGameUserSettings(const std::string& ClassName)
{
	if( ClassName.empty() )
	{
		FGameUserSettingsBuilder LGameUserSettingsBuilder;
		return LGameUserSettingsBuilder.ConstructGameUserSettings();
	}

	FGameUserSettingsBuilder* LGameUserSettingsBuilder = GameUserSettingsClassesMap.GetClassBuilder(ClassName);
	if( LGameUserSettingsBuilder == nullptr ) return nullptr;

	return LGameUserSettingsBuilder->ConstructGameUserSettings();
}



void GCoreObjectsFactory::RegisterGameInputSettingsClass(const std::string& ClassName, FGameInputSettingsBuilder* GameInputSettingsBuilder)
{
	GameInputSettingsClassesMap.RegisterBuilderClass(ClassName, GameInputSettingsBuilder);
}

void GCoreObjectsFactory::UnregisterGameInputSettingsClass(const std::string& ClassName)
{
	GameInputSettingsClassesMap.UnregisterBuilderClass(ClassName);
}

UGameInputSettings* GCoreObjectsFactory::ConstructGameInputSettings(const std::string& ClassName)
{
	if( ClassName.empty() )
	{
		FGameInputSettingsBuilder LGameInputSettingsBuilder;
		return LGameInputSettingsBuilder.ConstructGameInputSettings();
	}

	FGameInputSettingsBuilder* LGameInputSettingsBuilder = GameInputSettingsClassesMap.GetClassBuilder(ClassName);
	if( LGameInputSettingsBuilder == nullptr ) return nullptr;

	return LGameInputSettingsBuilder->ConstructGameInputSettings();
}



void GCoreObjectsFactory::RegisterDeviceResourcesAdapterClass(const std::string& ClassName, FDeviceResourcesAdapterBuilder* DeviceResourcesAdapterBuilder)
{
	DeviceResourcesAdapterClassesMap.RegisterBuilderClass(ClassName, DeviceResourcesAdapterBuilder);
}

void GCoreObjectsFactory::UnregistervClass(const std::string& ClassName)
{
	DeviceResourcesAdapterClassesMap.UnregisterBuilderClass(ClassName);
}

IDeviceResourcesAdapter* GCoreObjectsFactory::ConstructDeviceResourcesAdapter(const std::string& ClassName)
{
	if( ClassName.empty() )
	{
		FDeviceResourcesAdapterBuilder LDeviceResourcesAdapterBuilder;
		return LDeviceResourcesAdapterBuilder.ConstructDeviceResourcesAdapter();
	}

	FDeviceResourcesAdapterBuilder* LDeviceResourcesAdapterBuilder = DeviceResourcesAdapterClassesMap.GetClassBuilder(ClassName);
	if( LDeviceResourcesAdapterBuilder == nullptr ) return nullptr;

	return LDeviceResourcesAdapterBuilder->ConstructDeviceResourcesAdapter();
}



void GCoreObjectsFactory::RegisterGraphicsEngineClass(const std::string& ClassName, FGraphicsEngineBuilder* GraphicsEngineBuilder)
{
	GraphicsEngineClassesMap.RegisterBuilderClass(ClassName, GraphicsEngineBuilder);
}

void GCoreObjectsFactory::UnregisterGraphicsEngineClass(const std::string& ClassName)
{
	GraphicsEngineClassesMap.UnregisterBuilderClass(ClassName);
}

GGraphicsEngine* GCoreObjectsFactory::ConstructGraphicsEngine(const std::string& ClassName)
{
	if( ClassName.empty() )
	{
		FGraphicsEngineBuilder LGraphicsEngineBuilder;
		return LGraphicsEngineBuilder.ConstructGraphicsEngine();
	}

	FGraphicsEngineBuilder* LGraphicsEngineBuilder = GraphicsEngineClassesMap.GetClassBuilder(ClassName);
	if( LGraphicsEngineBuilder == nullptr ) return nullptr;

	return LGraphicsEngineBuilder->ConstructGraphicsEngine();
}
