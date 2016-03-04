/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialMovementTutorialState.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "TutorialMovementTutorialState.h"

#include "SegmentLogicStateIncludes.h"

#include "EntityAnimatorComponent.h"

using namespace ursine;
using namespace ecs;

TutorialMovementTutorialState::TutorialMovementTutorialState(void)
	: SegmentLogicState( "Tutorial Movement" )
	, m_complete( false )
{
}

void TutorialMovementTutorialState::Enter(SegmentLogicStateMachine *machine)
{
	auto segmentManager = machine->GetSegmentManager( );
	auto world = segmentManager->GetOwner( )->GetWorld( );
	
}

void TutorialMovementTutorialState::onCameraAnimFinish(EVENT_HANDLER(CameraAnimator))
{
	EVENT_ATTRS( EntityAnimator, EventArgs );

	m_complete = true;

	// Unsuscribe TODO: Talk to austin about why this breaks
	/*sender->Listener( this )
	.Off( EntityAnimatorEvent::FinishedAnimating, &CombatBowl1IntroCinematicState::onCameraAnimFinish );*/
}
