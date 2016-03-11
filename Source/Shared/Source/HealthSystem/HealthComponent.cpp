/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HealthComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "HealthComponent.h"
#include "GameEvents.h"
#include "AudioEmitterComponent.h"
#include "PlayerIdComponent.h"
#include "CollisionEventArgs.h"

NATIVE_COMPONENT_DEFINITION( Health );

namespace gameUIEvents
{
    const auto UI_HealthComponentStats = "UI_HealthComponentStats";
}

namespace
{
    const std::string kTakeDamage = "PLAYER_TAKE_DAMAGE";
}

Health::Health(void)
    : BaseComponent( )
    , m_health( 100 )
    , m_deleteOnZero( false )
    , m_spawnOnDeath( false )
{
}

Health::~Health(void)
{
    GetOwner( )->Listener(this)
        .Off( ursine::ecs::ENTITY_REMOVED, &Health::OnDeath );
}

float Health::GetHealth(void) const
{
    return m_health;
}

void Health::SetHealth(const float health)
{
    m_health = health;
}

float Health::GetMaxHealth(void) const
{
    return m_maxHealth;
}

const ursine::resources::ResourceReference &Health::GetArchetypeOnDeath(void) const
{
    return m_objToSpawn;
}

void Health::SetArchetypeOnDeath(const ursine::resources::ResourceReference &objToSpawn)
{
    m_objToSpawn = objToSpawn;
}

bool Health::GetDeleteOnZeroHealth(void) const
{
    return m_deleteOnZero;
}

void Health::SetDeleteOnZeroHealth(bool flag)
{
    m_deleteOnZero = flag;
}

bool Health::GetSpawnOnDeath(void) const
{
    return m_spawnOnDeath;
}
void Health::SetSpawnOnDeath(const bool state)
{
    m_spawnOnDeath = state;
}

void Health::DealDamage(const float damage)
{
    auto owner = GetOwner( );

    // early out if we've already "died"
    if (m_health < 0)
        return;

    m_health -= damage;

    if (m_health <= 0)
    {
        Dispatch( HEALTH_ZERO, ursine::EventArgs::Empty );

        if (m_deleteOnZero)
        GetOwner( )->Delete( );
    }
    else
    {
        float percentage = m_health / m_maxHealth;

        // dispatch damage taken event
        HealthEventArgs args( damage, m_health / m_maxHealth );
        Dispatch( HEALTH_DAMAGE_TAKEN, &args );

        // dispacth to ui if player
        if ( owner->HasComponent< PlayerID >( ) )
        {
            ursine::Json message = ursine::Json::object {
                { "playerID", owner->GetComponent< PlayerID >( )->GetID( ) },
                { "healthPercent", percentage }
            };

            GetOwner( )->GetWorld( )->MessageUI( gameUIEvents::UI_HealthComponentStats, message );
        }
    }

    URSINE_TODO("Fix sound hack for health");
    ursine::ecs::AudioEmitter* emitter = owner->GetComponent<ursine::ecs::AudioEmitter>( );

    //if ( emitter )
       // emitter->AddSoundToPlayQueue(kTakeDamage);
}


void Health::DealDamage(const ursine::SVec3& contactPoint, float damage, bool crit)
{
    DealDamage(damage);

    sendDamageTextEvent(contactPoint, damage, crit);
}

void Health::OnInitialize(void)
{
    m_maxHealth = m_health;

    GetOwner( )->Listener(this)
        .On(ursine::ecs::ENTITY_REMOVED, &Health::OnDeath);
}

void Health::sendDamageTextEvent(const ursine::SVec3& contact, float damage, bool crit)
{
    ursine::ecs::EntityHandle owner = GetOwner( );

    game::DamageEventArgs dEvent(contact, owner, damage, crit);

    owner->GetWorld( )->Dispatch(game::DAMAGE_TEXT_EVENT, &dEvent);
}

void Health::OnDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED))
{
    if (m_spawnOnDeath)
    {
        auto obj = GetOwner( )->GetWorld( )->CreateEntityFromArchetype( m_objToSpawn );

        if (obj)
            obj->GetTransform( )->SetWorldPosition( GetOwner( )->GetTransform( )->GetWorldPosition( ) );
    }
}
