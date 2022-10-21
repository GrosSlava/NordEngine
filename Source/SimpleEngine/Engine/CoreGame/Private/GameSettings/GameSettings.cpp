
#include "GameSettings/GameSettings.h"
#include "GameSettings/GameUserSettings.h"
#include "GameSettings/GameInputSettings.h"

#include "CoreGame.h"
#include "CoreObjectsFactory.h"
#include "Path.h"

#include "Window/BaseWindow.h"

#include "INI/INI.h"






GGameSettings::GGameSettings()
{
}

GGameSettings::~GGameSettings()
{
	if( GameUserSettings != nullptr )
	{
		delete GameUserSettings;
	}

	if( GameInputSettings != nullptr )
	{
		delete GameInputSettings;
	}
}




void GGameSettings::LoadSettings()
{
	FINIFile LIniFile(FPath::GetEngineConfigPath());

	WorldClassName = LIniFile.Get<std::string>("Classes", "WorldClassName", "");
	WindowClassName = LIniFile.Get<std::string>("Classes", "WindowClassName", "");
	GameInstanceClassName = LIniFile.Get<std::string>("Classes", "GameInstanceClassName", "");
	CameraManagerClassName = LIniFile.Get<std::string>("Classes", "CameraManagerClassName", "");
	GameUserSettingsClassName = LIniFile.Get<std::string>("Classes", "GameUserSettingsClassName", "");
	GameInputSettingsClassName = LIniFile.Get<std::string>("Classes", "GameInputSettingsClassName", "");
	DeviceResourcesAdapterClassName = LIniFile.Get<std::string>("Classes", "DeviceResourcesAdapterClassName", "");
	GraphicsEngineClassName = LIniFile.Get<std::string>("Classes", "GraphicsEngineClassName", "");

	MainWindowWithTitle = LIniFile.Get<bool>("Window", "EnableTitle", true);
	MainWindowResizable = LIniFile.Get<bool>("Window", "EnableResizing", true);
	MainWindowCloseable = LIniFile.Get<bool>("Window", "EnableClosing", true);
	MainWindowFullscreen = LIniFile.Get<bool>("Window", "EnableFullscreen", false);
	WindowBaseWidth = LIniFile.Get<uint16>("Window", "WindowBaseWidth", 900);
	WindowBaseHeight = LIniFile.Get<uint16>("Window", "WindowBaseHeight", 900);
	VSyncEnabled = LIniFile.Get<bool>("Window", "VSyncEnabled", false);
	FrameRateLimit = LIniFile.Get<uint16>("Window", "FrameRateLimit", 60);

	GameName = LIniFile.Get<std::string>("Game", "GameName", "");
	GameIconPath = LIniFile.Get<std::string>("Game", "GameIconPath", "");
	GameCursorPath = LIniFile.Get<std::string>("Game", "GameCursorPath", "");


	if( GameUserSettings != nullptr )
	{
		delete GameUserSettings;
	}
	GameUserSettings = ConstructGameUserSettings();
	if( GameUserSettings != nullptr )
	{
		GameUserSettings->LoadSettings();
	}

	if( GameInputSettings != nullptr )
	{
		delete GameInputSettings;
	}
	GameInputSettings = ConstructGameInputSettings();
	if( GameInputSettings != nullptr )
	{
		GameInputSettings->LoadSettings();
	}
}

void GGameSettings::ApplyGameSettings(GBaseWindow* Window)
{
	if( Window != nullptr )
	{
		Window->SetWindowTitle(GameName);
		Window->SetWindowSize(WindowBaseWidth, WindowBaseHeight);
		Window->SetWindowIcon(GameIconPath);
		Window->SetWindowCursor(GameCursorPath);
		Window->SetWindowFullScreen(MainWindowFullscreen);
	}

	GCoreGame::Get()->SetFPSLock(FrameRateLimit);
	GCoreGame::Get()->SetVSyncEnabled(VSyncEnabled);
	

	if( GameUserSettings != nullptr )
	{
		GameUserSettings->ApplyGameSettings();
	}
}

void GGameSettings::SaveGameSettings()
{
	FINIFile LIniFile;

	LIniFile.Set("Classes", "WorldClassName", WorldClassName);
	LIniFile.Set("Classes", "WindowClassName", WindowClassName);
	LIniFile.Set("Classes", "GameInstanceClassName", GameInstanceClassName);
	LIniFile.Set("Classes", "CameraManagerClassName", CameraManagerClassName);
	LIniFile.Set("Classes", "GameUserSettingsClassName", GameUserSettingsClassName);
	LIniFile.Set("Classes", "GameInputSettingsClassName", GameInputSettingsClassName);
	LIniFile.Set("Classes", "DeviceResourcesAdapterClassName", DeviceResourcesAdapterClassName);
	LIniFile.Set("Classes", "GraphicsEngineClassName", GraphicsEngineClassName);

	LIniFile.Set("Window", "EnableTitle", MainWindowWithTitle);
	LIniFile.Set("Window", "EnableResizing", MainWindowResizable);
	LIniFile.Set("Window", "EnableClosing", MainWindowCloseable);
	LIniFile.Set("Window", "EnableFullscreen", MainWindowFullscreen);
	LIniFile.Set("Window", "WindowBaseWidth", WindowBaseWidth);
	LIniFile.Set("Window", "WindowBaseHeight", WindowBaseHeight);
	LIniFile.Set("Window", "VSyncEnabled", VSyncEnabled);
	LIniFile.Set("Window", "FrameRateLimit", FrameRateLimit);

	LIniFile.Set("Game", "GameName", GameName);
	LIniFile.Set("Game", "GameIconPath", GameIconPath);
	LIniFile.Set("Game", "GameCursorPath", GameCursorPath);

	FINIWriter::Write(FPath::GetEngineConfigPath(), LIniFile);


	if( GameUserSettings != nullptr )
	{
		GameUserSettings->SaveGameSettings();
	}
	if( GameInputSettings != nullptr )
	{
		GameInputSettings->SaveInputSettings();
	}
}





GWorld* GGameSettings::ConstructWorld()
{
	return GCoreObjectsFactory::Get()->ConstructWorld(WorldClassName);
}

GBaseWindow* GGameSettings::ConstructWindow()
{
	int LWindowStyle = 0;
	if( MainWindowWithTitle ) LWindowStyle |= EWindowStyle::Titlebar;
	if( MainWindowResizable ) LWindowStyle |= EWindowStyle::Resize;
	if( MainWindowCloseable ) LWindowStyle |= EWindowStyle::Close;

	return GCoreObjectsFactory::Get()->ConstructWindow(WindowClassName, WindowBaseWidth, WindowBaseHeight, LWindowStyle);
}

GGameInstance* GGameSettings::ConstructGameInstance()
{
	return GCoreObjectsFactory::Get()->ConstructGameInstance(GameInstanceClassName);
}

GCameraManager* GGameSettings::ConstructCameraManager()
{
	return GCoreObjectsFactory::Get()->ConstructCameraManager(CameraManagerClassName);
}

UGameUserSettings* GGameSettings::ConstructGameUserSettings()
{
	return GCoreObjectsFactory::Get()->ConstructGameUserSettings(GameUserSettingsClassName);
}

UGameInputSettings* GGameSettings::ConstructGameInputSettings()
{
	return GCoreObjectsFactory::Get()->ConstructGameInputSettings(GameInputSettingsClassName);
}

IDeviceResourcesAdapter* GGameSettings::ConstructDeviceResourcesAdapter()
{
	return GCoreObjectsFactory::Get()->ConstructDeviceResourcesAdapter(DeviceResourcesAdapterClassName);
}

GGraphicsEngine* GGameSettings::ConstructGraphicsEngine()
{
	return GCoreObjectsFactory::Get()->ConstructGraphicsEngine(GraphicsEngineClassName);
}
