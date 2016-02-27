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

InitializeSegmentState::InitializeSegmentState(const std::string &loadInWorld, LevelSegments unloadSegment)
    : SegmentLogicState( "Initialize Segment State" )
    , m_loadInWorld( loadInWorld )
    , m_unloadSegment( unloadSegment )
{
}

void InitializeSegmentState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto world = segmentManager->GetOwner( )->GetWorld( );

    world->MergeWorld( URSINE_PROJECTS_DIRECTORY + m_loadInWorld );
}
