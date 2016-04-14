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

#include "GameEvents.h"
#include "ShieldFXComponent.h"

#include <World.h>
#include <Application.h>
#include <AudioEmitterComponent.h>
#include <LocalTimerManager.h>

NATIVE_COMPONENT_DEFINITION( PlayerShieldFX );

using namespace ursine;
using namespace ecs;
using namespace resources;

PlayerShieldFX::PlayerShieldFX(void)
    : BaseComponent( )
    , m_destroyed( false )
    , m_duration( 0.25f )
    , m_timer( 0.0f )
    , m_initTimer( 0.1f )
    , m_hurtSfxTimer( 0.0f )
    , m_hurtSfxMinDelay( 1.0f )
    , m_hurtSfxMaxDelay( 2.0f )
    , m_playingHeartBeat( false ) { }

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

    GetOwner( )->Listener( this )
        .Off( game::BOSS_SHIELD_DOWN, &PlayerShieldFX::onBossShieldDown );
}

void PlayerShieldFX::OnSceneReady(ursine::Scene *scene)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &PlayerShieldFX::onUpdate );

    GetOwner( )->Listener( this )
        .On( game::BOSS_SHIELD_DOWN, &PlayerShieldFX::onBossShieldDown );
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

float PlayerShieldFX::GetHurtSfxMinDelay(void) const
{
    return m_hurtSfxMinDelay;
}

void PlayerShieldFX::SetHurtSfxMinDelay(float delay)
{
    m_hurtSfxMinDelay = delay;

    NOTIFY_COMPONENT_CHANGED( "hurtSfxMinDelay", m_hurtSfxMinDelay );
}

float PlayerShieldFX::GetHurtSfxMaxDelay(void) const
{
    return m_hurtSfxMaxDelay;
}

void PlayerShieldFX::SetHurtSfxMaxDelay(float delay)
{
    m_hurtSfxMaxDelay = delay;

    NOTIFY_COMPONENT_CHANGED( "hurtSfxMaxDelay", m_hurtSfxMaxDelay );
}

const ResourceReference &PlayerShieldFX::GetHeartBeatPlaySfx(void) const
{
    return m_heartBeatPlaySfx;
}

void PlayerShieldFX::SetHeartBeatPlaySfx(const ResourceReference &sfx)
{
    m_heartBeatPlaySfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "heartBeatPlaySfx", m_heartBeatPlaySfx );
}

const ResourceReference &PlayerShieldFX::GetHeartBeatStopSfx(void) const
{
    return m_heartBeatStopSfx;
}

void PlayerShieldFX::SetHeartBeatStopSfx(const ResourceReference &sfx)
{
    m_heartBeatStopSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "heartBeatStopSfx", m_heartBeatStopSfx );
}

const ResourceReference &PlayerShieldFX::GetBossShieldDownSfx(void) const
{
    return m_bossShieldDownSfx;
}

void PlayerShieldFX::SetBossShieldDownSfx(const ResourceReference &sfx)
{
    m_bossShieldDownSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "bossShieldDownSfx", m_bossShieldDownSfx );
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

    // Update the heart beat sound effect
    auto audioEmitter = GetOwner( )->GetComponent<AudioEmitter>( );

    if (m_playingHeartBeat)
    {
        if (health->GetHealth( ) <= 0.0f)
        {
            audioEmitter->PushEvent( m_heartBeatStopSfx );
            m_playingHeartBeat = false;
        }
    }
    else
    {
        if (health->GetHealth( ) > 0.0f)
        {
            audioEmitter->PushEvent( m_heartBeatPlaySfx );
            m_playingHeartBeat = true;
        }
    }

    if (m_playingHeartBeat)
    {
        auto event = std::make_shared<AudioRTPC>( );

        event->parameter = "Health";
        event->val = 100.0f * (health->GetHealth( ) / health->GetMaxHealth( ));

        audioEmitter->PushEvent( event );
    }

    if (m_hurtSfxTimer > 0.0f)
        m_hurtSfxTimer -= dt;
}

void PlayerShieldFX::onHealthDamaged(EVENT_HANDLER(Health))
{
    auto health = GetOwner( )->GetComponent<Health>( );

    if (health->GetShieldHealth( ) > 0.0f)
    {
        m_timer = m_duration;
    }
    else
    {
        if (m_hurtSfxTimer > 0.0f)
            return;

        // play hurt sound effect
        GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_hurtSfx );

        m_hurtSfxTimer = math::Rand( m_hurtSfxMinDelay, m_hurtSfxMaxDelay );
    }

}

void PlayerShieldFX::onBossShieldDown(EVENT_HANDLER(Entity))
{
    // delay it for a little bit
    GetOwner( )->GetTimers( ).Create( TimeSpan::FromSeconds( math::Rand( 1.0f, 4.0f ) ) ).Completed( [=] {
        GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_bossShieldDownSfx );
    } );
}
