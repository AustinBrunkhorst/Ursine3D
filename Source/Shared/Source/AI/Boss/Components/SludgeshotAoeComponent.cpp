/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SludgeshotAoeComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SludgeshotAoeComponent.h"

#include "PlayerIDComponent.h"
#include "HealthComponent.h"

NATIVE_COMPONENT_DEFINITION( SludgeshotAoe );

using namespace ursine;
using namespace ecs;

SludgeshotAoe::SludgeshotAoe(void)
    : BaseComponent( )
    , m_lifeTime( 1.0f )
    , m_damage( 10.0f )
    , m_interval( 1.0f )
    , m_fadeOutTimer( 0.0f )
    , m_originalScale( 0.0f ) { }

SludgeshotAoe::~SludgeshotAoe(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (world)
        world->Listener( this )
            .Off( WORLD_UPDATE, &SludgeshotAoe::onUpdate );
}

void SludgeshotAoe::OnSceneReady(Scene *scene)
{
    // subscribe to update
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &SludgeshotAoe::onUpdate );
}

float SludgeshotAoe::GetLifeTime(void) const
{
    return m_lifeTime;
}

void SludgeshotAoe::SetLifeTime(float lifeTime)
{
    m_lifeTime = lifeTime;

    NOTIFY_COMPONENT_CHANGED( "lifeTime", m_lifeTime );
}

float SludgeshotAoe::GetDamage(void) const
{
    return m_damage;
}

void SludgeshotAoe::SetDamage(float damage)
{
    m_damage = damage;

    NOTIFY_COMPONENT_CHANGED( "damage", m_damage );
}

float SludgeshotAoe::GetInterval(void) const
{
    return m_interval;
}

void SludgeshotAoe::SetInterval(float interval)
{
    m_interval = interval;

    NOTIFY_COMPONENT_CHANGED( "interval", m_interval );
}

void SludgeshotAoe::onUpdate(EVENT_HANDLER(World))
{
    auto dt = Application::Instance->GetDeltaTime( );

    // if our lifetime is greater than zero
    if (m_lifeTime > 0.0f)
    {
        m_lifeTime -= dt;

        // If it's time to die
        if (m_lifeTime <= 0.0f)
        {
            auto emitter = GetOwner( )->GetComponent<ParticleEmitter>( );

            m_fadeOutTimer = emitter->GetLifetime( ) + emitter->GetLifetimeRange( );
            m_fadeOutTime = m_fadeOutTimer;

            emitter->SetEmitting( false );

            m_originalScale = GetOwner( )->GetTransform( )->GetWorldScale( ).X( );

            m_damageMap.clear( );

            return;
        }

        // look to see if players are in range
        auto world = GetOwner( )->GetWorld( );
        auto trans = GetOwner( )->GetTransform( );
        auto position = trans->GetWorldPosition( );
        auto scale = trans->GetWorldScale( ).X( );
        auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

        for (auto &player : players)
        {
            auto playerPos = player->GetTransform( )->GetWorldPosition( );

            if (SVec3::Distance( playerPos, position ) <= scale)
            {
                auto itr = m_damageMap.find( player );

                if (itr == m_damageMap.end( ) || m_damageMap[ player ] <= 0.0f)
                {
                    m_damageMap[ player ] = m_interval;

                    player->GetComponent<Health>( )->DealDamage(
                        GetOwner( ), m_damage
                    );
                }
            }
        }
    }
    else
    {
        m_fadeOutTimer -= dt;

        if (m_fadeOutTimer <= 0.0f)
        {
            GetOwner( )->Delete( );
            return;
        }

        // Set the new scale
        GetOwner( )->GetTransform( )->SetWorldScale(
            SVec3( 1.0f, 0.0f, 1.0f ) * m_originalScale * (m_fadeOutTimer / m_fadeOutTime)
        );
    }

    for (auto &pair : m_damageMap)
    {
        if (pair.second > 0.0f)
            pair.second -= dt;
    }
}
