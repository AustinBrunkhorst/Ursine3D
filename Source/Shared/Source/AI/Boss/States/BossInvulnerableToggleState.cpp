/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossInvulnerableToggleState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossInvulnerableToggleState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"
#include "HealthComponent.h"

#include <ParticleEmitterComponent.h>

using namespace ursine;
using namespace ecs;

BossInvulnerableToggleState::BossInvulnerableToggleState(bool toggle)
    : BossAIState( toggle ? "Toggle Invulnerable True" : "Toggle Invulnerable False" )
    , m_toggle( toggle ) { }

void BossInvulnerableToggleState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );

    auto health = boss->GetOwner( )->GetComponent<Health>( );

    if (health)
        health->SetInvulnerable( m_toggle );

    auto emitter = boss->GetInvulnerableEmitterEntity( );

    if (emitter)
        emitter->GetComponent<ParticleEmitter>( )->SetEmitting( m_toggle );

    if (m_toggle)
        boss->Dispatch( BossAIEvents::ShieldUp, EventArgs::Empty );
    else
        boss->Dispatch( BossAIEvents::ShieldDown, EventArgs::Empty );
}
