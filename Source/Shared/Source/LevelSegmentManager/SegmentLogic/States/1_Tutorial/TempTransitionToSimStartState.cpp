/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TransitionToCB1State.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "TempTransitionToSimStartState.h"

#include "SegmentLogicStateIncludes.h"

using namespace ursine;
using namespace ecs;

void TempTransitionToSimStartState::Enter(SegmentLogicStateMachine *machine)
{
    static float transitionTime = 5.0f;

    auto segmentManager = machine->GetSegmentManager( );

    segmentManager->GetOwner( )->GetTimers( )
        .Create( TimeSpan::FromSeconds( transitionTime ) )
        .Completed( [=] {

            segmentManager->SetCurrentSegment( LevelSegments::Tut_SimulationCreationCinematic );

        } );
}
