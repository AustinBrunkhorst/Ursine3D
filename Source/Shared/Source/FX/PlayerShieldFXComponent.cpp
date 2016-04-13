/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerShieldFXComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerShieldFXComponent.h"

#include "ShieldFXComponent.h"

#include <World.h>
#include <AudioEmitterComponent.h>

NATIVE_COMPONENT_DEFINITION( PlayerShieldFX );

using namespace ursine;
using namespace ecs;
using namespace resources;

PlayerShieldFX::PlayerShieldFX(void)
    : BaseComponent( )
    , m_destroyed( false ) { }

PlayerShieldFX::~PlayerShieldFX(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (world)
    {
        world->Listener( this )
            .Off( WORLD_UPDATE, &PlayerShieldFX::onUpdate );
    }
}

void PlayerShieldFX::OnSceneReady(ursine::Scene *scene)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &PlayerShieldFX::onUpdate );
}

const ResourceReference &PlayerShieldFX::GetShieldBreakSfx(void) const
{
    return m_shieldBreakSfx;
}

void PlayerShieldFX::SetShieldBreakSfx(const ResourceReference &shieldBreakSfx)
{
    m_shieldBreakSfx = shieldBreakSfx;

    NOTIFY_COMPONENT_CHANGED( "shieldBreakSfx", m_shieldBreakSfx );
}

const ResourceReference &PlayerShieldFX::GetShieldRebuildSfx(void) const
{
    return m_shieldRebuildSfx;
}

void PlayerShieldFX::SetShieldRebuildSfx(const ResourceReference &shieldRebuildSfx)
{
    m_shieldRebuildSfx = shieldRebuildSfx;

    NOTIFY_COMPONENT_CHANGED( "shieldRebuildSfx", m_shieldRebuildSfx );
}

void PlayerShieldFX::onUpdate(EVENT_HANDLER(World))
{
    auto health = GetOwner( )->GetComponent<Health>( );

    auto shield = health->GetShieldHealth( );

    if (shield > 0.0f && m_destroyed)
    {
        auto fxs = GetOwner( )->GetComponentsInChildren<ShieldFX>( );

        for (auto &fx : fxs)
        {
            fx->RebuildShield( );
        }

        auto emitter = GetOwner( )->GetComponentInChildren<AudioEmitter>( );

        if (emitter)
        {
            emitter->PushEvent( m_shieldRebuildSfx );
        }

        m_destroyed = false;
    }
    else if (shield == 0.0f && !m_destroyed)
    {
        auto fxs = GetOwner( )->GetComponentsInChildren<ShieldFX>( );

        for (auto &fx : fxs)
        {
            fx->DestroyShield( );
        }

        auto emitter = GetOwner( )->GetComponentInChildren<AudioEmitter>( );

        if (emitter)
        {
            emitter->PushEvent( m_shieldBreakSfx );
        }

        m_destroyed = true;
    }
}
