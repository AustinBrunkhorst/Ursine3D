/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InitializeSegmentState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "InitializeSegmentState.h"

#include "SegmentLogicStateIncludes.h"
#include "UnloadResourceComponent.h"

#include "RafflesiaMain.h"

#include <World.h>
#include <Scene.h>
#include <AudioSystem.h>
#include <SystemManager.h>

using namespace ursine;
using namespace ecs;

InitializeSegmentState::InitializeSegmentState(
    const resources::ResourceReference &loadInWorld,
    LevelSegments unloadSegment, bool stopAllSounds, bool removePrevious
)
    : SegmentLogicState( "Initialize Segment State" )
    , m_loadInWorld( loadInWorld )
    , m_unloadSegment( unloadSegment )
    , m_segmentManager( nullptr )
    , m_stopAllSounds( stopAllSounds )
    , m_removePrevious( removePrevious )
{
}

void InitializeSegmentState::Enter(SegmentLogicStateMachine *machine)
{
    m_segmentManager = machine->GetSegmentManager( );

    auto world = m_segmentManager->GetOwner( )->GetWorld( );

    if (m_removePrevious)
    {
        auto *gameManager = static_cast<RafflesiaMain*>( world->GetOwner( )->GetGameContext( )->GetManager( ) );

        if (gameManager)
            gameManager->SpawnEndingCredits( );

        return;
    }

    // subscribe to entity added events so we can add the unload resource component
    world->Listener( this )
        .On( WORLD_ENTITY_ADDED, &InitializeSegmentState::onEntityAdded );

    world->ImportWorld( world->GetOwner( )->GetResourceManager( ), m_loadInWorld );

    world->Listener( this )
        .Off( WORLD_ENTITY_ADDED, &InitializeSegmentState::onEntityAdded );

    if (m_stopAllSounds)
    {
        auto audio = world->GetEntitySystem<AudioSystem>( );

        if (audio)
        {
            audio->StopAllSounds( );
        }
    }
}

void InitializeSegmentState::onEntityAdded(EVENT_HANDLER(World))
{
    EVENT_ATTRS(World, EntityEventArgs);

    if (!args->entity->HasComponent<UnloadResource>( ))
        args->entity->AddComponent<UnloadResource>( m_segmentManager, m_unloadSegment );
}
