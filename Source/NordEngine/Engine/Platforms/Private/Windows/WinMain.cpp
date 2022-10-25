
#include "GenericPlatform.h"
#if PLATFORM_WINDOWS


#include "Windows/WinMain.h"

#include "CoreGame/CoreGame.h"
#include "CoreGame/CoreObjectsFactory.h"

#include "Windows/WinWindow.h"
#include "GraphicsEngine/DeviceAdapters/SFMLAdapter.h"





int WINAPI wWinMain_Internal()
{
	//.................Register platform specific objects.................//
	
	GCoreObjectsFactory::Get()->RegisterDeviceResourcesAdapterClass("USFMLAdapter2D", new FSFMLAdapter2DBuilder());
	GCoreObjectsFactory::Get()->RegisterWindowClass("WinWindow", new FWinWindowBuilder());

	//....................................................................//


	GCoreGame::Get()->Start();

	MSG msg = {};
	while( msg.message != WM_QUIT )
	{
		if( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GCoreGame::Get()->Tick();
		}
	}


	return static_cast<int>(msg.wParam);
}

#endif // PLATFORM_WINDOWS