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

#include <World.h>

using namespace ursine;
using namespace ecs;

InitializeSegmentState::InitializeSegmentState(const std::string &loadInWorld, LevelSegments unloadSegment)
    : SegmentLogicState( "Initialize Segment State" )
    , m_loadInWorld( loadInWorld )
    , m_unloadSegment( unloadSegment )
    , m_segmentManager( nullptr )
{
}

void InitializeSegmentState::Enter(SegmentLogicStateMachine *machine)
{
    m_segmentManager = machine->GetSegmentManager( );
    auto world = m_segmentManager->GetOwner( )->GetWorld( );

    // subscribe to entity added events so we can add the unload resource component
    world->Listener( this )
        .On( WORLD_ENTITY_ADDED, &InitializeSegmentState::onEntityAdded );

    world->MergeWorld( URSINE_PROJECTS_DIRECTORY + m_loadInWorld );

    world->Listener( this )
        .Off( WORLD_ENTITY_ADDED, &InitializeSegmentState::onEntityAdded );
}

void InitializeSegmentState::onEntityAdded(EVENT_HANDLER(World))
{
    EVENT_ATTRS(World, EntityEventArgs);

    if (!args->entity->HasComponent<UnloadResource>( ))
        args->entity->AddComponent<UnloadResource>( m_segmentManager, m_unloadSegment );
}
