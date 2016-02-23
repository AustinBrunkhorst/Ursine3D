/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** CombatBowl1IntroCinematic.cpp
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CombatBowl1IntroCinematicState.h"

#include "SegmentLogicStateIncludes.h"

#include "ElevatorLiftMoverComponent.h"

using namespace ursine;
using namespace ecs;

CombatBowl1IntroCinematicState::CombatBowl1IntroCinematicState(void)
    : SegmentLogicState( "Combat Bowl 1 Intro Cinematic" )
    , m_complete( false )
{
}

void CombatBowl1IntroCinematicState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto world = segmentManager->GetOwner( )->GetWorld( );
    auto cinematicRunning = segmentManager->GetCurrentSegment( ) == LevelSegments::CB1_SimulationStartCinematic;

    // only start the animation if we're doing the cinematic
    if (cinematicRunning)
    {
        auto camAnimators = world->GetEntitiesFromFilter( Filter( ).All<CameraAnimator>( ) );

        // find the camera animator
        UAssert( camAnimators.size( ) != 0, "Error: Where's the camera animator for this cinematic?" );

        auto camAnimator = camAnimators[ 0 ]->GetComponent<CameraAnimator>( );

        // subscribe to the finished event
        camAnimator->Listener( this )
            .On( CameraAnimatorEvent::FinishedAnimating, &CombatBowl1IntroCinematicState::onCameraAnimFinish );

        // Start the animation
        camAnimator->Play( );
    }

    // find the elevator lift
    auto elevatorMovers = world->GetEntitiesFromFilter( Filter( ).All<ElevatorLiftMover>( ) );

    UAssert( elevatorMovers.size( ) != 0, "Error: Where's the elevator lift mover?" );

    auto elevator = elevatorMovers[ 0 ]->GetComponent<ElevatorLiftMover>( );

    if (cinematicRunning)
        elevator->StartMoving( );
    else
    {
        elevator->GoToEndPosition( );
        m_complete = true;
    }
}

void CombatBowl1IntroCinematicState::onCameraAnimFinish(EVENT_HANDLER(CameraAnimator))
{
    EVENT_ATTRS(CameraAnimator, EventArgs);

    m_complete = true;

    // Unsuscribe TODO: Talk to austin about why this breaks
    /*sender->Listener( this )
        .Off( CameraAnimatorEvent::FinishedAnimating, &CombatBowl1IntroCinematicState::onCameraAnimFinish );*/
}
