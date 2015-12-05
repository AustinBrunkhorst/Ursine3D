#include "Precompiled.h"

#include "MultiplayerPlayScreen.h"

#include <WorldSerializer.h>
#include <SystemManager.h>
#include <Timer.h>

#include <RoundSystem.h>

using namespace ursine;

namespace
{
    const auto kWorldFile = "Assets/Worlds/BlankWorld.uworld";
}

MultiplayerPlayScreen::MultiplayerPlayScreen(ScreenManager *manager)
    : GameplayScreen( manager )
{
    URSINE_TODO( "this is to fix the transition flicker" );
    Timer::Create( 250 ).Completed( [&] {
        world = ecs::WorldSerializer( ).Deserialize( kWorldFile );
        
        world->SetOwner( this );

        world->GetSystemManager( )->AddSystem<RoundSystem>( );

        URSINE_TODO( "This guy should be saved in an editor specific file eventually" );
        auto editorCam = world->GetEntityFromName( "Editor Camera" );

		if (editorCam)
			editorCam->Delete( );
    } );
}