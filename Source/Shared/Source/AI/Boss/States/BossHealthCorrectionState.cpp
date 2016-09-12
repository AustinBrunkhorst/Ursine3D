/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossHealthCorrectionState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossHealthCorrectionState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"
#include "HealthComponent.h"

BossHealthCorrectionState::BossHealthCorrectionState(float fraction)
    : BossAIState( "Boss Health Correction" )
    , m_fraction( fraction ) { }

void BossHealthCorrectionState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );

    auto health = boss->GetOwner( )->GetComponent<Health>( );

    auto curr = health->GetHealth( );
    auto desired = health->GetMaxHealth( ) * m_fraction;

    if (curr > desired)
        health->SetHealth( desired );
}
