/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialWaitForPlayerReviveState.cpp
**
** Author:
** - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "TutorialWaitForPlayerReviveState.h"

#include "SegmentLogicStateMachine.h"
#include "LevelSegmentManagerComponent.h"
#include "GameEvents.h"

using namespace ursine;
using namespace ecs;

TutorialWaitForPlayerReviveState::TutorialWaitForPlayerReviveState(void)
    : SegmentLogicState( "Wait For Player Revive" )
    , m_completed( false ) { }

void TutorialWaitForPlayerReviveState::Enter(SegmentLogicStateMachine *machine)
{
    m_machine = machine;

    auto sm = machine->GetSegmentManager( );

    sm->GetPlayer1( )->Listener( this )
        .On( game::REVIVE_PLAYER, &TutorialWaitForPlayerReviveState::onPlayerRevive );

    sm->GetPlayer2( )->Listener( this )
        .On( game::REVIVE_PLAYER, &TutorialWaitForPlayerReviveState::onPlayerRevive );
}

void TutorialWaitForPlayerReviveState::onPlayerRevive(EVENT_HANDLER(Entity))
{
    auto sm = m_machine->GetSegmentManager( );

    // change the segment
    sm->SetCurrentSegment( LevelSegments::Tut_ReviveTutorial );

    m_completed = true;

    sm->GetPlayer1( )->Listener( this )
        .Off( game::REVIVE_PLAYER, &TutorialWaitForPlayerReviveState::onPlayerRevive );

    sm->GetPlayer2( )->Listener( this )
        .Off( game::REVIVE_PLAYER, &TutorialWaitForPlayerReviveState::onPlayerRevive );
}
