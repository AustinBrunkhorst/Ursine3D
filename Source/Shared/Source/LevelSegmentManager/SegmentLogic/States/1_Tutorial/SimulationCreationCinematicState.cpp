/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SimulationCreationCinematicState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SimulationCreationCinematicState.h"

#include "SegmentLogicStateIncludes.h"

using namespace ursine;
using namespace ecs;

void SimulationCreationCinematicState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto owner = segmentManager->GetOwner( );
    auto world = owner->GetWorld( );
    auto &timers = owner->GetTimers( );

    timers.Create( TimeSpan::FromSeconds( 1.0f ) )
        .Completed( [=] {

            

        } );
}
