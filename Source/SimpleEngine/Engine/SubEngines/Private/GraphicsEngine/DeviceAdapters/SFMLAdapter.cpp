
#include "GraphicsEngine/DeviceAdapters/SFMLAdapter.h"

#include "GameInput/GameInput.h"
#include "CoreGame.h"

#include "Window/BaseWindow.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>
#include "SFML/System/Vector2.hpp"
#include <SFML/Window/Event.hpp>





IDeviceResourcesAdapter* FSFMLAdapter2DBuilder::ConstructDeviceResourcesAdapter()
{
	return new USFMLAdapter2D();
}





USFMLAdapter2D::~USFMLAdapter2D()
{
	if( SFMLWindow.isOpen() )
	{
		SFMLWindow.close();
	}
}

void USFMLAdapter2D::InitAdapter(GBaseWindow* Window)
{
	if( Window == nullptr || MyWindow == Window ) return;

	MyWindow = Window;

	SFMLWindow.create(Window->GetWindowHandle());
}





void USFMLAdapter2D::BeginGameLogicSection()
{
}

void USFMLAdapter2D::EndGameLogicSection()
{
}



void USFMLAdapter2D::BroadcastEvents()
{
	if( !SFMLWindow.isOpen() ) return;
	
	/*
		Give SFML chance to do cache operations.
	*/
	sf::Event LEvent;
	SFMLWindow.pollEvent(LEvent);
}

void USFMLAdapter2D::Render(const FSceneView& SceneView)
{
	if( !SFMLWindow.isOpen() ) return;

	SFMLWindow.clear(sf::Color::Black);

	//TODO optimize
	for( const F2DView& LSceneView : SceneView.View2D )
	{
		if( LSceneView.DrawableView.Texture == nullptr ) continue;

		sf::Sprite LSprite(*LSceneView.DrawableView.Texture);

		LSprite.setOrigin(sf::Vector2f(LSceneView.Origin.X, LSceneView.Origin.Y));
		LSprite.setPosition(sf::Vector2f(LSceneView.WorldLocation.X, LSceneView.WorldLocation.Y));
		LSprite.setRotation(LSceneView.Rotation);
		LSprite.setScale(sf::Vector2f(LSceneView.WorldScale.X, LSceneView.WorldScale.Y));

		SFMLWindow.draw(LSprite);
	}

	SFMLWindow.display();
}



void USFMLAdapter2D::SetVSyncEnabled(bool Enable)
{
	if( !SFMLWindow.isOpen() ) return;

	SFMLWindow.setVerticalSyncEnabled(Enable);
}

void USFMLAdapter2D::SetFrameRateLimit(uint16 Limit)
{
	if( !SFMLWindow.isOpen() ) return;

	SFMLWindow.setFramerateLimit(Limit);
}

void USFMLAdapter2D::SetCameraFOV(float VerticalFovInRadians)
{
	//TODO
}

void USFMLAdapter2D::SetCameraLocation(const FVector2D& NewLocation)
{
	//TODO
}

void USFMLAdapter2D::SetCameraRotation(float NewRotation)
{
	//TODO
}



void USFMLAdapter2D::OnDeviceLost()
{
}

void USFMLAdapter2D::OnDeviceRestored()
{
}

void USFMLAdapter2D::OnGameStart()
{
}

void USFMLAdapter2D::PreGameDestroy()
{
	if( !SFMLWindow.isOpen() ) return;

	SFMLWindow.close();
}

void USFMLAdapter2D::OnActivated()
{
}

void USFMLAdapter2D::OnDeactivated()
{
}

void USFMLAdapter2D::OnSuspending()
{
}

void USFMLAdapter2D::OnResuming()
{
}

void USFMLAdapter2D::OnWindowMoved()
{
}

void USFMLAdapter2D::OnWindowSizeChanged()
{
	if( !SFMLWindow.isOpen() || !GCoreGame::Get()->GetWindow() ) return;

	sf::View LView = SFMLWindow.getDefaultView();

	uint16 LWidth;
	uint16 LHeight;
	GCoreGame::Get()->GetWindow()->GetWindowSize(LWidth, LHeight);

	LView.setSize(sf::Vector2f(LWidth, LHeight));
	LView.setCenter(sf::Vector2f(LWidth / 2, LHeight / 2));

	SFMLWindow.setView(LView);
}

void USFMLAdapter2D::OnWindowTitleChanged()
{
}

void USFMLAdapter2D::OnWindowIconChanged()
{
}

void USFMLAdapter2D::OnWindowCursorChanged()
{
}

void USFMLAdapter2D::OnWindowMouseCursorVisibilityChanged()
{
}

void USFMLAdapter2D::OnWindowMouseCursorGrabbingChanged()
{
}
