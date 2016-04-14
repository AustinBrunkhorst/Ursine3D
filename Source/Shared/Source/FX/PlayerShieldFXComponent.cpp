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
#include <Application.h>
#include <AudioEmitterComponent.h>

NATIVE_COMPONENT_DEFINITION( PlayerShieldFX );

using namespace ursine;
using namespace ecs;
using namespace resources;

PlayerShieldFX::PlayerShieldFX(void)
    : BaseComponent( )
    , m_destroyed( false )
    , m_duration( 0.25f )
    , m_timer( 0.0f )
    , m_initTimer( 0.1f ) { }

PlayerShieldFX::~PlayerShieldFX(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (world)
    {
        world->Listener( this )
            .Off( WORLD_UPDATE, &PlayerShieldFX::onUpdate );
    }

    auto health = GetOwner( )->GetComponent<Health>( );
    
    if (health)
        health->Listener( this )
            .Off( HEALTH_DAMAGED, &PlayerShieldFX::onHealthDamaged );
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

const ResourceReference &PlayerShieldFX::GetHurtSfx(void) const
{
    return m_hurtSfx;
}

void PlayerShieldFX::SetHurtSfx(const ResourceReference &sfx)
{
    m_hurtSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "hurtSfx", m_hurtSfx );
}

const Color &PlayerShieldFX::GetFlashColor(void) const
{
    return m_flashColor;
}

void PlayerShieldFX::SetFlashColor(const Color &color)
{
    m_flashColor = color;

    NOTIFY_COMPONENT_CHANGED( "flashColor", m_flashColor );
}

float PlayerShieldFX::GetFlashDuration(void) const
{
    return m_duration;
}

void PlayerShieldFX::SetFlashDuration(float duration)
{
    m_duration = duration;

    NOTIFY_COMPONENT_CHANGED( "flashDuration", m_duration );
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

    // This is for initializing the arrays of fragmenters and default colors.
    // The short delay is because on the first update of InventorySystem, the
    // player's weapon is created.  So we wait until it's created so we can get it's
    // fragmenter.
    auto dt = Application::Instance->GetDeltaTime( );

    if (m_initTimer > 0.0f)
    {
        m_initTimer -= dt;

        if (m_initTimer <= 0.0f)
        {
            auto fragmenters = GetOwner( )->GetComponentsInChildren<ModelFragmenter>( );

            for (auto &fragmenter : fragmenters)
            {
                m_fragmenters.push_back( fragmenter );
                m_defaultColors.push_back( fragmenter->GetColor( ) );
            }

            // subscribe to the health's events
            GetOwner( )->GetComponent<Health>( )->Listener( this )
                .On( HEALTH_DAMAGED, &PlayerShieldFX::onHealthDamaged );
        }
    }

    // Update the color transition of the fragmenters
    if (m_timer > 0.0f)
    {
        m_timer -= dt;

        if (m_timer < 0.0f)
            m_timer = 0.0f;

        // We're going form 0 to 1
        float t = (m_duration - m_timer) / m_duration;

        for (size_t i = 0, n = m_fragmenters.size( ); i < n; ++i)
        {
            // if we're going from default to flash
            if (t < 0.5f)
            {
                float trans = t / 0.5f;

                m_fragmenters[ i ]->SetColor(
                    (m_flashColor.ToVector4( ) * trans) + 
                    ((1.0f - trans) * m_defaultColors[ i ].ToVector4( ))
                );
            }
            // going back from flash to default
            else
            {
                float trans = (t - 0.5f) / 0.5f;

                m_fragmenters[ i ]->SetColor(
                    (m_defaultColors[ i ].ToVector4( ) * trans) + 
                    ((1.0f - trans) * m_flashColor.ToVector4( ))
                );
            }
        }
    }
}

void PlayerShieldFX::onHealthDamaged(EVENT_HANDLER(Health))
{
    auto health = GetOwner( )->GetComponent<Health>( );

    if (health->GetShieldHealth( ) > 0.0f)
    {
        m_timer = m_duration;
    }
    else
        // play hurt sound effect
        GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_hurtSfx );
}
