/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerWinState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerWinState.h"

#include "SegmentLogicStateMachine.h"
#include "LevelSegmentManagerComponent.h"
#include "PlayerAnimationControllerComponent.h"

PlayerWinState::PlayerWinState(bool toggle)
    : SegmentLogicState( "Player Win State" )
    , m_toggle( toggle ) { }

void PlayerWinState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );

    segmentManager->GetPlayer1( )->GetComponentInChildren<PlayerAnimationController>( )->SetWonFlag( m_toggle );
    segmentManager->GetPlayer2( )->GetComponentInChildren<PlayerAnimationController>( )->SetWonFlag( m_toggle );
}
