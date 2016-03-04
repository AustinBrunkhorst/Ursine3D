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

#include "LockPlayerCharacterControllerState.h"

#include "SegmentLogicStateIncludes.h"

#include "CharacterControllerComponent.h"

LockPlayerCharacterControllerState::LockPlayerCharacterControllerState(bool lockP1Move, bool lockP1Look,
                                                                       bool lockP2Move, bool lockP2Look)
    : SegmentLogicState( "Lock Player Character Controller")
    , m_lockP1Move( lockP1Move )
    , m_lockP1Look( lockP1Look )
    , m_lockP2Move( lockP2Move )
    , m_lockP2Look( lockP2Look )
{
}

void LockPlayerCharacterControllerState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto &p1 = segmentManager->GetPlayer1( );
    auto &p2 = segmentManager->GetPlayer2( );

    UAssert( p1 && p2, "Error: The players haven't been spawned." );

    auto *c1 = p1->GetComponent<CharacterController>( );
    auto *c2 = p2->GetComponent<CharacterController>( );

    c1->SetLockMovement( m_lockP1Move );
    c1->SetLockLooking( m_lockP1Look );

    c2->SetLockMovement( m_lockP2Move );
    c2->SetLockLooking( m_lockP2Look );
}

