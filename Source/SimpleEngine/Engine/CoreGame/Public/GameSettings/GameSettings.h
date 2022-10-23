
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




/*
	Main object of all settings in game.
	@see UGameInputSettings, UGameUserSettings.
*/
class ENGINE_API GGameSettings final
{
	friend class GCoreObjectsFacade;

	SINGLETON(GGameSettings)
	GENERATED_BODY(GGameSettings)

public:

	~GGameSettings();



public:

	/*
		Load all settings.
	*/
	void LoadSettings();
	/*
		Apply all settings.
	*/
	void ApplyGameSettings();
	/*
		Save all settings.
	*/
	void SaveGameSettings();

protected:

	GWorld* ConstructWorld();
	GBaseWindow* ConstructWindow();
	GGameInstance* ConstructGameInstance();
	GCameraManager* ConstructCameraManager();
	UGameUserSettings* ConstructGameUserSettings();
	UGameInputSettings* ConstructGameInputSettings();

	IDeviceResourcesAdapter* ConstructDeviceResourcesAdapter();
	GGraphicsEngine* ConstructGraphicsEngine();




public:

	FORCEINLINE std::string GetGameName() const noexcept { return GameName; }
	FORCEINLINE void SetGameName(const std::string& NewGameName) { GameName = NewGameName; }


	FORCEINLINE bool GetIsWindowWithTitle() const noexcept { return MainWindowWithTitle; }
	FORCEINLINE void SetWindowWithTitle(bool HasTitle) noexcept { MainWindowWithTitle = HasTitle; }

	FORCEINLINE bool GetIsWindowResizable() const noexcept { return MainWindowResizable; }
	FORCEINLINE void SetWindowIsResizable(bool Resizable) noexcept { MainWindowResizable = Resizable; }

	FORCEINLINE bool GetIsWindowCloseable() const noexcept { return MainWindowCloseable; }
	FORCEINLINE void SetWindowIsCloseable(bool Closeable) noexcept { MainWindowCloseable = Closeable; }

	FORCEINLINE bool GetIsWindowFullscreen() const noexcept { return MainWindowFullscreen; }
	FORCEINLINE void SetWindowIsFullscreen(bool IsFullscreen) noexcept { MainWindowFullscreen = IsFullscreen; }

	FORCEINLINE bool GetVSyncEnabled() const noexcept { return VSyncEnabled; }
	FORCEINLINE void SetVSyncEnabled(bool Enable) noexcept { VSyncEnabled = Enable; }


	FORCEINLINE uint16 GetWindowBaseWidth() const noexcept { return WindowBaseWidth; }
	FORCEINLINE void SetWindowBaseWidth(uint16 NewWidth) noexcept { WindowBaseWidth = NewWidth; }

	FORCEINLINE uint16 GetWindowBaseHeight() const noexcept { return WindowBaseWidth; }
	FORCEINLINE void SetWindowBaseHeight(uint16 NewHeight) noexcept { WindowBaseHeight = NewHeight; }

	FORCEINLINE uint16 GetFrameRateLimit() const noexcept { return FrameRateLimit; }
	FORCEINLINE void SetFrameRateLimit(uint16 Limit) noexcept { FrameRateLimit = Limit; }


	FORCEINLINE UGameUserSettings* GetGameUserSettings() const noexcept { return GameUserSettings; }
	FORCEINLINE UGameInputSettings* GetGameInputSettings() const noexcept { return GameInputSettings; }

private:

	//.......................User configurable objects...............................//

	std::string WorldClassName = "";
	std::string WindowClassName = "";
	std::string GameInstanceClassName = "";
	std::string CameraManagerClassName = "";
	std::string GameUserSettingsClassName = "";
	std::string GameInputSettingsClassName = "";

	std::string DeviceResourcesAdapterClassName = "";
	std::string GraphicsEngineClassName = "";

	//..............................................................................//

	//............................Active after reload...............................//

	bool MainWindowWithTitle = true;
	bool MainWindowResizable = true;
	bool MainWindowCloseable = true;
	bool MainWindowFullscreen = false;

	//..............................................................................//


	std::string GameName = "";
	std::string GameIconPath = "";
	std::string GameCursorPath = "";

	uint16 WindowBaseWidth = 900;
	uint16 WindowBaseHeight = 900;

	bool VSyncEnabled = false;
	uint16 FrameRateLimit = 60;


	/*
		Cached user settings.
	*/
	UGameUserSettings* GameUserSettings = nullptr;
	/*
		Cached input settings.
	*/
	UGameInputSettings* GameInputSettings = nullptr;
};



FORCEINLINE GGameSettings* GGameSettings::Get() noexcept
{
	static GGameSettings GameSettings;
	return &GameSettings;
}