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

#include <Application.h>

NATIVE_COMPONENT_DEFINITION( Health );

using namespace ursine;
using namespace ecs;

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
    , EventDispatcher( this )
    , m_type( ENEMY_HEALTH )
    , m_health( 100 )
    , m_maxHealth( 100 )
    , m_shield( 100 )
    , m_maxShield( 100 )
    , m_shieldRechargeDelay( 10 )
    , m_shieldRechargeTimer( 0 )
    , m_shieldRechargeRate( 10 )
    , m_deleteOnZero( false )
    , m_spawnOnDeath( false )
    , m_dead( false )
    , m_invulnerable( false )
    , m_hasShield( false ) { }

Health::~Health(void)
{
    GetOwner( )->Listener(this)
        .Off( ursine::ecs::ENTITY_REMOVED, &Health::onDeath );

    auto world = GetOwner( )->GetWorld( );

    if (world)
        world->Listener( this )
            .Off( WORLD_UPDATE, &Health::onUpdate );
}

void Health::OnSceneReady(Scene *scene)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &Health::onUpdate );
}

HealthType Health::GetHealthType(void) const
{
    return m_type;
}

void Health::SetHealthType(HealthType type)
{
    m_type = type;

    NOTIFY_COMPONENT_CHANGED( "healthType", m_type );
}

float Health::GetHealth(void) const
{
    return m_health;
}

void Health::SetHealth(const float health)
{
    m_health = health;

    NOTIFY_COMPONENT_CHANGED( "EntityHealth", m_health );
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

    NOTIFY_COMPONENT_CHANGED( "archetypeToSpawnOnDeath", m_objToSpawn );
}

bool Health::GetDeleteOnZeroHealth(void) const
{
    return m_deleteOnZero;
}

void Health::SetDeleteOnZeroHealth(bool flag)
{
    m_deleteOnZero = flag;

    NOTIFY_COMPONENT_CHANGED( "deleteOnZeroHealth", m_deleteOnZero );
}

bool Health::GetSpawnOnDeath(void) const
{
    return m_spawnOnDeath;
}

void Health::SetSpawnOnDeath(bool state)
{
    m_spawnOnDeath = state;

    NOTIFY_COMPONENT_CHANGED( "SpawnOnDeath", m_spawnOnDeath );
}

bool Health::GetInvulnerable(void) const
{
    return m_invulnerable;
}

void Health::SetInvulnerable(bool invulnerable)
{
    m_invulnerable = invulnerable;

    NOTIFY_COMPONENT_CHANGED( "invulnerable", m_invulnerable );
}

bool Health::GetHasShield(void) const
{
    return m_hasShield;
}

void Health::SetHasShield(bool toggle)
{
    m_hasShield = toggle;

    NOTIFY_COMPONENT_CHANGED( "hasShield", m_hasShield );
}

float Health::GetShieldHealth(void) const
{
    return m_shield;
}

void Health::SetShieldHealth(float shield)
{
    m_shield = shield;

    NOTIFY_COMPONENT_CHANGED( "shieldHealth", m_shield );
}

float Health::GetMaxShieldHealth(void) const
{
    return m_maxShield;
}

float Health::GetShieldRechargeDelay(void) const
{
    return m_shieldRechargeDelay;
}

void Health::SetShieldRechargeDelay(float delay)
{
    m_shieldRechargeDelay = delay;

    NOTIFY_COMPONENT_CHANGED( "shieldRechargeDelay", m_shieldRechargeDelay );
}

float Health::GetShieldRechargeRate(void) const
{
    return m_shieldRechargeRate;
}

void Health::SetShieldRechargeRate(float rate)
{
    m_shieldRechargeRate = rate;

    NOTIFY_COMPONENT_CHANGED( "shieldRechargeRate", m_shieldRechargeRate );
}

void Health::DealDamage(float damage)
{
    if (m_dead || m_invulnerable)
        return;

    auto owner = GetOwner( );

    // early out if we've already "died"
    if (m_health < 0)
        return;

    // Check to see if we have a shield
    if (m_hasShield && m_shield > 0.0f)
    {
        // reset the recharge timer
        m_shieldRechargeTimer = m_shieldRechargeDelay;

        // deal the damage to the shield
        SetShieldHealth( m_shield - damage );

        // Early out so we don't deal any health damage
        if (m_shield > 0.0f)
            return;

        // carry over the damage to the health
        damage = -m_shield;

        m_shield = 0.0f;
    }

    SetHealth( GetHealth( ) - damage );

    if (m_health <= 0)
    {
        Dispatch( HEALTH_ZERO, ursine::EventArgs::Empty );

        if (m_deleteOnZero)
            GetOwner( )->Delete( );

        m_dead = true;

        m_hasShield = false;
    }
    else
    {
        float percentage = m_health / m_maxHealth;

        // dispatch damage taken event
        HealthEventArgs args( damage, m_health / m_maxHealth );
        Dispatch( HEALTH_DAMAGE_TAKEN, &args );

        // dispacth to ui if player
        if (owner->HasComponent<PlayerID>( ) )
        {
            URSINE_TODO( "Create health event" );
            /*ursine::Json message = ursine::Json::object {
                { "playerID", owner->GetComponent< PlayerID >( )->GetID( ) },
                { "healthPercent", percentage }
            };

            GetOwner( )->GetWorld( )->MessageUI( gameUIEvents::UI_HealthComponentStats, message );*/
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

    m_maxShield = m_shield;

    GetOwner( )->Listener(this)
        .On( ENTITY_REMOVED, &Health::onDeath );
}

void Health::sendDamageTextEvent(const SVec3& contact, float damage, bool crit)
{
    EntityHandle owner = GetOwner( );

    game::DamageEventArgs dEvent( contact, owner, damage, crit, m_invulnerable );

    owner->GetWorld( )->Dispatch( game::DAMAGE_TEXT_EVENT, &dEvent );
}

void Health::onDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED))
{
    if (m_spawnOnDeath)
    {
        auto obj = GetOwner( )->GetWorld( )->CreateEntityFromArchetype( m_objToSpawn );

        if (obj)
            obj->GetTransform( )->SetWorldPosition( GetOwner( )->GetTransform( )->GetWorldPosition( ) );
    }
}

void Health::onUpdate(EVENT_HANDLER(World))
{
    // update the recharging of the shield
    if (m_hasShield && m_shield < m_maxShield)
    {
        auto dt = Application::Instance->GetDeltaTime( );

        if (m_shieldRechargeTimer <= 0.0f)
        {
            SetShieldHealth( math::Min( m_shield + dt * m_shieldRechargeRate, m_maxShield ) );
        }
        else
        {
            m_shieldRechargeTimer -= dt;
        }
    }
}
