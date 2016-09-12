/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossJumpToHomeLocation.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossJumpToHomeLocationState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

BossJumpToHomeLocationState::BossJumpToHomeLocationState(void)
    : BossAIState( "Jump To Home Location" ) { }

void BossJumpToHomeLocationState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );
    auto trans = boss->GetOwner( )->GetTransform( );

    trans->SetWorldPosition(
        boss->GetHomeLocation( )
    );

    trans->SetWorldRotation(
        boss->GetSpawnOrientation( )
    );

    machine->SetBool( BossAIStateMachine::GoHome, false );
}
