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
#include "DamageEvent.h"
#include "AudioEmitterComponent.h"
#include "PlayerIdComponent.h"
#include "CollisionEventArgs.h"
#include "DamageOnCollideComponent.h"
#include "AbstractHitscanWeapon.h"

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
    , m_dead( false )
    , m_invulnerable( false )
    , m_type( ENEMY_HEALTH ) { }

Health::~Health(void)
{
    GetOwner( )->Listener(this)
        .Off( ursine::ecs::ENTITY_REMOVED, &Health::OnDeath );
}

HealthType Health::GetHealthType(void) const
{
    return m_type;
}

void Health::SetHealthType(HealthType type)
{
    m_type = type;
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

void Health::SetSpawnOnDeath(bool state)
{
    m_spawnOnDeath = state;
}

bool Health::GetInvulnerable(void) const
{
    return m_invulnerable;
}

void Health::SetInvulnerable(bool invulnerable)
{
    m_invulnerable = invulnerable;
}

void Health::DealDamage(float damage)
{
    if (m_dead || m_invulnerable)
        return;

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

        m_dead = true;
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
}


void Health::DealDamage(const ursine::SVec3& contactPoint, float damage, bool crit)
{
    DealDamage(damage);

    sendDamageTextEvent(contactPoint, damage, crit);
}

bool Health::CanDamage(DamageOnCollide *damage) const
{
    auto type = damage->GetDamageType( );

    return type == DAMAGE_ALL || type == m_type;
}

bool Health::CanDamage(AbstractHitscanWeapon *weapon) const
{
    auto type = weapon->GetDamageType( );

    return type == DAMAGE_ALL || type == m_type;
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

    game::DamageEventArgs dEvent(contact, owner, damage, crit, m_invulnerable);

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
