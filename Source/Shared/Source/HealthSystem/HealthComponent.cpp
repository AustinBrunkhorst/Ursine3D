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
#include "UIEvents.h"
#include "UIScreensConfigComponent.h"
#include "GameEvents.h"

#include <Application.h>

NATIVE_COMPONENT_DEFINITION( Health );

using namespace ursine;
using namespace ecs;

Health::Health(void)
    : BaseComponent( )
    , EventDispatcher( this )
    , m_type( ENEMY_HEALTH )
    , m_health( 100 )
    , m_maxHealth( 100 )
    , m_shield( 100 )
    , m_maxShield( 100 )
    , m_deleteOnZero( false )
    , m_spawnOnDeath( false )
    , m_invulnerable( false )
    , m_hasShield( false )
{ }

Health::~Health(void)
{
    GetOwner( )->Listener(this)
        .Off( game::REVIVE_PLAYER, &Health::onRevive )
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
    m_health = math::Max(health, 0.0f);

    NOTIFY_COMPONENT_CHANGED( "EntityHealth", m_health );
}

float Health::GetMaxHealth(void) const
{
    return m_maxHealth;
}

void Health::SetMaxHealth(float health)
{
    m_maxHealth = health;

    SetHealth( math::Min( m_health, m_maxHealth ) );
    NOTIFY_COMPONENT_CHANGED( "MaxHealth", m_maxHealth );
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
    m_shield = math::Max(shield, 0.0f);

    NOTIFY_COMPONENT_CHANGED( "ShieldHealth", m_shield );
}

float Health::GetMaxShieldHealth(void) const
{
    return m_maxShield;
}

void Health::SetMaxShieldHealth(float shield)
{
    m_maxShield = shield;

    SetShieldHealth( math::Min( shield, m_shield ) );

    NOTIFY_COMPONENT_CHANGED( "MaxShieldHealth", m_maxShield );
}

void Health::AddHealth(float healthToAdd)
{
    // make sure not to exceed max health
    SetHealth( math::Min( m_health + healthToAdd, m_maxHealth ) );
}

void Health::AddShieldHealth(float shieldToAdd)
{
    // make sure not to exceed max shield
    SetShieldHealth( math::Min( m_shield + shieldToAdd, m_maxShield ) );
}

void Health::DealDamage(float damage)
{
    if (m_health <= 0 || m_invulnerable)
        return;

    auto owner = GetOwner( );
    auto *world = owner->GetWorld( );

    owner->Dispatch( HEALTH_DAMAGED, ursine::ecs::EntityEventArgs::Empty );

    // Check to see if we have a shield
    if (m_hasShield && m_shield > 0.0f)
    {
        // deal the damage to the shield
        SetShieldHealth( m_shield - damage );

        // early out while shield is still up
        if ( m_shield > 0.0f )
            return;

        // carry over the damage to the health
        damage = -m_shield;

        SetShieldHealth( 0.0f );
    }

    SetHealth( GetHealth( ) - damage );

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

        auto *player = owner->GetComponent<PlayerID>( );

        // dispacth to ui if player
        if (player)
        {
            ui_event::PlayerHealthUpdated healthEvent;

            healthEvent.playerID = player->GetID( );
            healthEvent.percent = percentage;

            auto *ui = world->GetSettings( )->GetComponent<UIScreensConfig>( );

            if (ui)
                ui->TriggerPlayerHUDEvent( healthEvent );
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

    GetOwner( )->Listener( this )
        .On( game::REVIVE_PLAYER, &Health::onRevive )
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

}

void Health::onRevive(EVENT_HANDLER(ursine::ecs::Entity))
{
    AddHealth( m_maxHealth );
}
