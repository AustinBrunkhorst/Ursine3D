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

#include "EntityAnimatorComponent.h"

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

    auto elevator = world->GetEntityFromName( "CombatBowl1/CombatBowl1ElevatorLift" );

    UAssert( elevator, "Error: Where's the elevator entity?" );

    // only start the animation if we're doing the cinematic
    if (cinematicRunning)
    {
        auto cameraAnimator = elevator->GetComponentInChildren<EntityAnimator>( );

        // find the camera animator
        UAssert( cameraAnimator, "Error: Where's the camera animator for this cinematic?" );

        // subscribe to the finished event
        cameraAnimator->Listener( this )
            .On( EntityAnimatorEvent::FinishedAnimating, &CombatBowl1IntroCinematicState::onCameraAnimFinish );

        // Start the animation
        cameraAnimator->JumpToStart( );
        cameraAnimator->Play( );
    }

    // Start the elevator
    auto elevatorAnim = elevator->GetComponent<EntityAnimator>( );

    UAssert( elevatorAnim, "Error: Where's the animator on the elevator?" );

    if (cinematicRunning)
    {
        elevatorAnim->JumpToStart( );
        elevatorAnim->Play( );
    }
    else
    {
        elevatorAnim->JumpToEnd( );
        m_complete = true;
    }
}

void CombatBowl1IntroCinematicState::onCameraAnimFinish(EVENT_HANDLER(CameraAnimator))
{
    EVENT_ATTRS(EntityAnimator, EventArgs);

    m_complete = true;

    // Unsuscribe TODO: Talk to austin about why this breaks
    /*sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &CombatBowl1IntroCinematicState::onCameraAnimFinish );*/
}
