/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MultiplayerPlayScreen.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "MultiplayerPlayScreen.h"

#include <WorldSerializer.h>
#include <SystemManager.h>
#include <Timer.h>

#include <WorldConfigComponent.h>

#include "Retrospect.h"

using namespace ursine;

namespace
{
    const auto kWorldFile = "Assets/Worlds/Demo/DemoWorld.uworld";

    const auto kPauseScreenName = "PauseScreen";
}

MultiplayerPlayScreen::MultiplayerPlayScreen(ScreenManager *manager)
    : GameplayScreen( manager )
{
    URSINE_TODO( "this is to fix the transition flicker" );
    Timer::Create( 0 ).Completed( [&] {
        world = ecs::World::Handle( ecs::WorldSerializer( ).Deserialize( kWorldFile ) );
        
        world->SetOwner( this );

        URSINE_TODO( "This guy should be saved in an editor specific file eventually" );
        auto editorCam = world->GetEntityFromName( "Editor Camera" );

		if (editorCam)
			editorCam->Delete( );

        loadSystems( );
    } );

    auto *appHandler = GetCoreSystem( Retrospect );

    appHandler->GetMainWindowHandle( )->Listener( this )
        .On( WINDOW_FOCUS_CHANGED, &MultiplayerPlayScreen::onMainWindowFocusChanged );
}

MultiplayerPlayScreen::~MultiplayerPlayScreen(void)
{
    auto *appHandler = GetCoreSystem( Retrospect );

    appHandler->GetMainWindowHandle( )->Listener( this )
        .Off( WINDOW_FOCUS_CHANGED, &MultiplayerPlayScreen::onMainWindowFocusChanged );
}

void MultiplayerPlayScreen::onMainWindowFocusChanged(EVENT_HANDLER(Window))
{
    EVENT_ATTRS(Window, WindowFocusArgs);

    if (args->focused)
        return;

    auto *appHandler = GetCoreSystem( Retrospect );
    auto *screenManager = appHandler->GetScreenManager( );
    auto *screen = screenManager->GetScreen( kPauseScreenName );

    // add the pause screen if it doesn't already exist
    if (!screen) 
        screenManager->AddOverlay( kPauseScreenName );
}

void MultiplayerPlayScreen::loadSystems(void)
{
    auto *config = world->GetSettings( )->GetComponent<ecs::WorldConfig>( );

    auto *systemManager = world->GetSystemManager( );

    for (auto &system : config->systems)
    {
        auto type = meta::Type::GetFromName( system.type );

        UAssert( type.IsValid( ), 
            "Invalid system type '%s'.", 
            system.type.c_str( ) 
        );

        UAssert( !systemManager->HasSystem( type ), 
            "System '%s' already exists.", 
            system.type.c_str( ) 
        );

        systemManager->AddSystem( type );
    }
}