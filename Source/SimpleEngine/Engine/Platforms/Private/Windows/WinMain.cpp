
#include "Windows/WinMain.h"

#include "CoreGame.h"
#include "CoreObjectsFactory.h"

#include "Windows/WinWindow.h"
#include "GraphicsEngine/DeviceAdapters/SFMLAdapter.h"





int WINAPI wWinMain_Internal(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	GCoreObjectsFactory::Get()->RegisterDeviceResourcesAdapterClass("USFMLAdapter2D", new FSFMLAdapter2DBuilder());
	GCoreObjectsFactory::Get()->RegisterWindowClass("WinWindow", new FWinWindowBuilder());


	GCoreGame::Get()->Start();

	MSG msg = {};
	while( WM_QUIT != msg.message )
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


	return (int)msg.wParam;
}

int WINAPI wWinMain_Internal()
{
	return wWinMain_Internal(nullptr, nullptr, nullptr, 0);
}