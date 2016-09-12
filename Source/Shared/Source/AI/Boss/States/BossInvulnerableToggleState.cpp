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
#include "ShieldFxComponent.h"
#include "PlayerIDComponent.h"
#include "GameEvents.h"

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

    auto fx = boss->GetOwner( )->GetComponentInChildren<ShieldFX>( );

    if (fx)
    {
        if (m_toggle)
            fx->RebuildShield( );
        else
            fx->DestroyShield( );
    }

    if (m_toggle)
        boss->Dispatch( BossAIEvents::ShieldUp, EventArgs::Empty );
    else
        boss->Dispatch( BossAIEvents::ShieldDown, EventArgs::Empty );

    auto emitter = boss->GetOwner( )->GetComponent<AudioEmitter>( );

    if (emitter)
    {
        if (m_toggle)
            emitter->PushEvent( boss->GetShieldRebuildSfx( ) );
        else
            emitter->PushEvent( boss->GetShieldBreakSfx( ) );
    }

    if (!m_toggle)
    {
        auto world = boss->GetOwner( )->GetWorld( );

        auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

        for (auto &player : players)
        {
            player->Dispatch( game::BOSS_SHIELD_DOWN, EventArgs::Empty );
        }
    }
}
