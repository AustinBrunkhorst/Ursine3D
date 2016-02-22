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

InitializeSegmentState::InitializeSegmentState(const ursine::Array<std::string> &loadInArchetypes, LevelSegments unloadSegment)
    : SegmentLogicState( "Initialize Segment State" )
    , m_loadInArchetypes( loadInArchetypes )
    , m_unloadSegment( unloadSegment )
{
}

void InitializeSegmentState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto world = segmentManager->GetOwner( )->GetWorld( );

    for (auto &archetype : m_loadInArchetypes)
    {
        auto e = world->CreateEntityFromArchetype(
            WORLD_ARCHETYPE_PATH + archetype + ".uatype", archetype
        );

        e->AddComponent<UnloadResource>( segmentManager, m_unloadSegment );
    }
}
