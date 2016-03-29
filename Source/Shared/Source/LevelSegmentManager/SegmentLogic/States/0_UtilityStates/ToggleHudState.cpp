/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ToggleHudState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ToggleHudState.h"

#include "SegmentLogicStateIncludes.h"

ToggleHudState::ToggleHudState(bool toggle)
    : SegmentLogicState( toggle ? "Turn Hud On" : "Turn Hud Off" )
    , m_toggle( toggle ) { }

void ToggleHudState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto world = segmentManager->GetOwner( )->GetWorld( );

    //if (m_toggle)
        // send message here
    // else
        // send message here
}
