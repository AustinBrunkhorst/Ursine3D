/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ChangeSegmentState.cpp
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ChangeSegmentState.h"

#include "SegmentLogicStateMachine.h"
#include "LevelSegmentManagerComponent.h"

ChangeSegmentState::ChangeSegmentState(LevelSegments changeToSegment)
    : SegmentLogicState( "Change Segment State" )
    , m_segment( changeToSegment )
{
    
}

void ChangeSegmentState::Enter(SegmentLogicStateMachine *machine)
{
    auto manager = machine->GetSegmentManager( );

    if (manager->GetCurrentSegment( ) < m_segment)
        manager->SetCurrentSegment( m_segment );
}
